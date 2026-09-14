#include "render/drawentities.h"

#include "core/config.h"

namespace {
sf::Vector2f toPixels(Vec2f value)
{
    return {
        value.x * Config::pixelsPerMeter + Config::fieldMarginPixels,
        value.y * Config::pixelsPerMeter + Config::fieldMarginPixels
    };
}

sf::Color teamColor(Team team)
{
    return team == Team::Red ? sf::Color(205, 64, 64) : sf::Color(62, 105, 210);
}
}

void drawEntities(sf::RenderTarget& target, const WorldSnapshot& world)
{
    sf::CircleShape ball(world.ball.radius * Config::pixelsPerMeter);
    ball.setOrigin(ball.getRadius(), ball.getRadius());
    ball.setPosition(toPixels(world.ball.position));
    ball.setFillColor(sf::Color(245, 245, 235));
    ball.setOutlineThickness(2.0f);
    ball.setOutlineColor(sf::Color(40, 45, 45));
    target.draw(ball);

    for (const auto& puck : world.pucks) {
        sf::CircleShape shape(puck.radius * Config::pixelsPerMeter);
        shape.setOrigin(shape.getRadius(), shape.getRadius());
        shape.setPosition(toPixels(puck.position));
        shape.setFillColor(teamColor(puck.team));
        shape.setOutlineThickness(3.0f);
        shape.setOutlineColor(sf::Color(30, 35, 42));
        target.draw(shape);
    }
}

void drawObstacles(sf::RenderTarget& target, const WorldSnapshot& world)
{
    for (const auto& obstacle : world.obstacles) {
        sf::RectangleShape shape({
            obstacle.size.x * Config::pixelsPerMeter,
            obstacle.size.y * Config::pixelsPerMeter
        });
        shape.setOrigin(shape.getSize() * 0.5f);
        shape.setPosition(toPixels(obstacle.position));
        shape.setRotation(obstacle.rotationDegrees);
        shape.setFillColor(sf::Color(72, 78, 86));
        shape.setOutlineThickness(2.0f);
        shape.setOutlineColor(sf::Color(230, 235, 230));
        target.draw(shape);
    }
}

void drawAim(sf::RenderTarget& target, const AimView& aim)
{
    if (!aim.active) {
        return;
    }

    sf::Vertex line[] = {
        {sf::Vector2f(aim.startPixels.x, aim.startPixels.y), sf::Color(255, 240, 120)},
        {sf::Vector2f(aim.currentPixels.x, aim.currentPixels.y), sf::Color(255, 240, 120)}
    };
    target.draw(line, 2, sf::Lines);

    const Vec2f drag = aim.startPixels - aim.currentPixels;
    const float length = std::min(
        140.0f,
        std::sqrt(drag.x * drag.x + drag.y * drag.y));
    sf::RectangleShape power({length, 8.0f});
    power.setPosition(24.0f, Config::windowHeight - 28.0f);
    power.setFillColor(sf::Color(245, 205, 68));
    target.draw(power);
}
