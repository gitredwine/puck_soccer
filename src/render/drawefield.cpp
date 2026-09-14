#include "render/drawfield.h"

#include "core/config.h"

namespace {
sf::Vector2f toPixels(Vec2f value)
{
    return {
        value.x * Config::pixelsPerMeter + Config::fieldMarginPixels,
        value.y * Config::pixelsPerMeter + Config::fieldMarginPixels
    };
}
}

void drawField(sf::RenderTarget& target, const LevelConfig& level)
{
    sf::RectangleShape field({
        level.fieldWidth * Config::pixelsPerMeter,
        level.fieldHeight * Config::pixelsPerMeter
    });
    field.setPosition(Config::fieldMarginPixels, Config::fieldMarginPixels);
    field.setFillColor(sf::Color(38, 130, 77));
    field.setOutlineThickness(4.0f);
    field.setOutlineColor(sf::Color(230, 236, 228));
    target.draw(field);

    sf::RectangleShape centerLine({3.0f, level.fieldHeight * Config::pixelsPerMeter});
    centerLine.setPosition(toPixels({level.fieldWidth * 0.5f, 0.0f}));
    centerLine.setFillColor(sf::Color(210, 225, 215, 150));
    target.draw(centerLine);

    sf::CircleShape centerCircle(1.2f * Config::pixelsPerMeter);
    centerCircle.setOrigin(centerCircle.getRadius(), centerCircle.getRadius());
    centerCircle.setPosition(toPixels({level.fieldWidth * 0.5f, level.fieldHeight * 0.5f}));
    centerCircle.setFillColor(sf::Color::Transparent);
    centerCircle.setOutlineThickness(3.0f);
    centerCircle.setOutlineColor(sf::Color(210, 225, 215, 150));
    target.draw(centerCircle);

    const float goalHeight = level.goalWidth * Config::pixelsPerMeter;
    const float goalDepth = level.goalDepth * Config::pixelsPerMeter;
    sf::RectangleShape redGoal({goalDepth, goalHeight});
    redGoal.setPosition(
        Config::fieldMarginPixels - goalDepth,
        Config::fieldMarginPixels + (level.fieldHeight * Config::pixelsPerMeter - goalHeight) * 0.5f);
    redGoal.setFillColor(sf::Color(188, 58, 58, 140));
    target.draw(redGoal);

    sf::RectangleShape blueGoal({goalDepth, goalHeight});
    blueGoal.setPosition(
        Config::fieldMarginPixels + level.fieldWidth * Config::pixelsPerMeter,
        Config::fieldMarginPixels + (level.fieldHeight * Config::pixelsPerMeter - goalHeight) * 0.5f);
    blueGoal.setFillColor(sf::Color(58, 98, 190, 140));
    target.draw(blueGoal);
}
