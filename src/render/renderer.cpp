#include "render/renderer.h"

#include "core/config.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>

namespace {
const char* stateName(GameState state)
{
    switch (state) {
    case GameState::Aiming:
        return "Aiming";
    case GameState::Simulating:
        return "Simulating";
    case GameState::Resolving:
        return "Resolving";
    case GameState::CheckEnd:
        return "Check End";
    case GameState::AiTurn:
        return "AI Turn";
    case GameState::GameOver:
        return "Game Over";
    }
    return "Unknown";
}

sf::Vector2f toPixels(Vec2f position)
{
    return {
        Config::fieldMarginPixels + position.x * Config::pixelsPerMeter,
        Config::fieldMarginPixels + position.y * Config::pixelsPerMeter};
}

sf::FloatRect levelCardBounds(std::size_t index)
{
    return {
        Config::levelCardX + index * (Config::levelCardWidth + Config::levelCardGap),
        Config::levelCardY,
        Config::levelCardWidth,
        Config::levelCardHeight};
}

void drawText(
    sf::RenderTarget& target,
    const sf::Font& font,
    bool fontLoaded,
    const std::string& value,
    unsigned int size,
    sf::Vector2f position,
    sf::Color color)
{
    if (!fontLoaded) {
        return;
    }
    sf::Text text(value, font, size);
    text.setPosition(position);
    text.setFillColor(color);
    target.draw(text);
}
}

Renderer::Renderer()
{
    const char* fontPaths[] = {
        "C:/Windows/Fonts/segoeui.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/System/Library/Fonts/Supplemental/Arial.ttf"};
    for (const char* path : fontPaths) {
        if (font_.loadFromFile(path)) {
            fontLoaded_ = true;
            break;
        }
    }
}

void Renderer::drawLevelMenu(
    sf::RenderWindow& window,
    const std::vector<LevelConfig>& levels,
    std::size_t unlockedLevels,
    PlayMode mode)
{
    window.clear(sf::Color(18, 24, 28));
    drawText(window, font_, fontLoaded_, "PUCK SOCCER", 42, {44.0f, 54.0f}, sf::Color(242, 245, 238));
    const char* modeNames[] = {"VS AI", "LOCAL 2 PLAYER"};
    for (std::size_t index = 0; index < 2; ++index) {
        const bool selected = index == static_cast<std::size_t>(mode);
        sf::RectangleShape button({Config::modeButtonWidth, Config::modeButtonHeight});
        button.setPosition(
            Config::modeButtonX + index * (Config::modeButtonWidth + Config::modeButtonGap),
            Config::modeButtonY);
        button.setFillColor(selected ? sf::Color(61, 113, 83) : sf::Color(37, 47, 51));
        button.setOutlineThickness(2.0f);
        button.setOutlineColor(selected ? sf::Color(126, 205, 150) : sf::Color(77, 91, 94));
        window.draw(button);
        drawText(window, font_, fontLoaded_, modeNames[index], 16,
                 {Config::modeButtonX + 16.0f + index * (Config::modeButtonWidth + Config::modeButtonGap),
                  Config::modeButtonY + 10.0f},
                 sf::Color(242, 245, 238));
    }

    for (std::size_t index = 0; index < levels.size(); ++index) {
        const bool unlocked = index < unlockedLevels;
        const sf::FloatRect bounds = levelCardBounds(index);
        sf::RectangleShape card({bounds.width, bounds.height});
        card.setPosition(bounds.left, bounds.top);
        card.setFillColor(unlocked ? sf::Color(37, 57, 63) : sf::Color(31, 36, 40));
        card.setOutlineThickness(2.0f);
        card.setOutlineColor(unlocked ? sf::Color(104, 184, 132) : sf::Color(74, 81, 84));
        window.draw(card);

        const sf::Color accent = index % 2 == 0 ? sf::Color(205, 64, 64) : sf::Color(64, 112, 205);
        sf::CircleShape puck(28.0f);
        puck.setOrigin(28.0f, 28.0f);
        puck.setPosition(bounds.left + bounds.width * 0.5f, bounds.top + 78.0f);
        puck.setFillColor(unlocked ? accent : sf::Color(76, 81, 84));
        puck.setOutlineThickness(3.0f);
        puck.setOutlineColor(sf::Color(17, 22, 24));
        window.draw(puck);

        drawText(window, font_, fontLoaded_, levels[index].id, 26,
                 {bounds.left + 16.0f, bounds.top + 128.0f},
                 unlocked ? sf::Color(242, 245, 238) : sf::Color(142, 148, 150));
        drawText(window, font_, fontLoaded_, levels[index].name, 16,
                 {bounds.left + 16.0f, bounds.top + 168.0f},
                 unlocked ? sf::Color(190, 208, 198) : sf::Color(110, 116, 119));

        if (!unlocked) {
            sf::RectangleShape lock({26.0f, 22.0f});
            lock.setPosition(bounds.left + 73.0f, bounds.top + 220.0f);
            lock.setFillColor(sf::Color(116, 125, 128));
            window.draw(lock);
            sf::CircleShape shackle(14.0f);
            shackle.setPosition(bounds.left + 72.0f, bounds.top + 204.0f);
            shackle.setFillColor(sf::Color::Transparent);
            shackle.setOutlineThickness(5.0f);
            shackle.setOutlineColor(sf::Color(116, 125, 128));
            window.draw(shackle);
        }
    }

    window.setTitle("Puck Soccer | Level Select");
    window.display();
}

void Renderer::draw(
    sf::RenderWindow& window,
    const WorldSnapshot& world,
    const LevelConfig& level,
    PlayMode mode,
    Team activeTeam,
    GameState state,
    ScoreChanged score,
    bool simulationActive,
    float stillTime,
    float simulationTime,
    const AimView& aim,
    const std::string& statusText)
{
    window.clear(sf::Color(18, 24, 28));

    sf::RectangleShape field({
        level.fieldWidth * Config::pixelsPerMeter,
        level.fieldHeight * Config::pixelsPerMeter});
    field.setPosition(Config::fieldMarginPixels, Config::fieldMarginPixels);
    field.setFillColor(sf::Color(42, 136, 76));
    field.setOutlineThickness(4.0f);
    field.setOutlineColor(sf::Color(226, 236, 226));
    window.draw(field);

    const float goalHeight = level.goalWidth * Config::pixelsPerMeter;
    const float goalDepth = level.goalDepth * Config::pixelsPerMeter;
    sf::RectangleShape redGoal({goalDepth, goalHeight});
    redGoal.setPosition(
        Config::fieldMarginPixels,
        Config::fieldMarginPixels + (level.fieldHeight * Config::pixelsPerMeter - goalHeight) * 0.5f);
    redGoal.setFillColor(sf::Color(190, 60, 60, 120));
    window.draw(redGoal);

    sf::RectangleShape blueGoal({goalDepth, goalHeight});
    blueGoal.setPosition(
        Config::fieldMarginPixels + (level.fieldWidth - level.goalDepth) * Config::pixelsPerMeter,
        Config::fieldMarginPixels + (level.fieldHeight * Config::pixelsPerMeter - goalHeight) * 0.5f);
    blueGoal.setFillColor(sf::Color(60, 100, 190, 120));
    window.draw(blueGoal);

    for (const auto& obstacle : world.obstacles) {
        sf::RectangleShape shape({
            obstacle.size.x * Config::pixelsPerMeter,
            obstacle.size.y * Config::pixelsPerMeter});
        shape.setOrigin(shape.getSize().x * 0.5f, shape.getSize().y * 0.5f);
        shape.setPosition(toPixels(obstacle.position));
        shape.setRotation(obstacle.rotationDegrees);
        shape.setFillColor(sf::Color(54, 63, 70));
        shape.setOutlineThickness(2.0f);
        shape.setOutlineColor(sf::Color(20, 25, 28));
        window.draw(shape);
    }

    sf::CircleShape ball(world.ball.radius * Config::pixelsPerMeter);
    ball.setOrigin(ball.getRadius(), ball.getRadius());
    ball.setPosition(toPixels(world.ball.position));
    ball.setFillColor(sf::Color(245, 245, 235));
    ball.setOutlineThickness(2.0f);
    ball.setOutlineColor(sf::Color(35, 40, 45));
    window.draw(ball);

    for (const auto& puck : world.pucks) {
        sf::CircleShape shape(puck.radius * Config::pixelsPerMeter);
        shape.setOrigin(shape.getRadius(), shape.getRadius());
        shape.setPosition(toPixels(puck.position));
        shape.setFillColor(
            puck.team == Team::Red ? sf::Color(205, 64, 64) : sf::Color(64, 112, 205));
        shape.setOutlineThickness(3.0f);
        shape.setOutlineColor(sf::Color(35, 40, 45));
        window.draw(shape);
    }

    if (aim.active) {
        const sf::Vertex line[] = {
            {sf::Vector2f(aim.startPixels.x, aim.startPixels.y), sf::Color(255, 240, 120)},
            {sf::Vector2f(aim.currentPixels.x, aim.currentPixels.y), sf::Color(255, 240, 120)}};
        window.draw(line, 2, sf::Lines);

        const Vec2f drag = aim.startPixels - aim.currentPixels;
        const float power = std::min(
            Config::maxShotPower,
            std::sqrt(drag.x * drag.x + drag.y * drag.y) / Config::shotPowerScale);
        sf::RectangleShape powerBar({power * 4.0f, 8.0f});
        powerBar.setPosition(24.0f, Config::windowHeight - 28.0f);
        powerBar.setFillColor(sf::Color(245, 205, 68));
        window.draw(powerBar);
    }

    std::ostringstream title;
    title << "Puck Soccer " << level.id << " - " << level.name << " | "
          << (mode == PlayMode::LocalTwoPlayer ? "Local 2 Player" : "VS AI")
          << " | Red "
          << score.red << " : " << score.blue << " Blue | "
          << stateName(state) << " | ";
    if (!statusText.empty()) {
        title << statusText;
    } else if (simulationActive) {
        title << "still " << std::fixed << std::setprecision(2)
              << stillTime << "/" << Config::sleepConfirmSeconds << "s | total "
              << simulationTime << "/" << Config::maxSimulationSeconds << "s";
    } else if (state == GameState::Resolving) {
        title << "simulation complete";
    } else if (mode == PlayMode::LocalTwoPlayer) {
        title << (activeTeam == Team::Red ? "Red player's turn" : "Blue player's turn");
    } else {
        title << "Drag the red puck to aim";
    }
    window.setTitle(title.str());
    window.display();
}
