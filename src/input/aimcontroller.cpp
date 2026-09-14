#include "input/aimcontroller.h"

#include "core/config.h"

#include <algorithm>
#include <cmath>

namespace {
Vec2f eventPosition(const sf::Event& event)
{
    if (event.type == sf::Event::MouseMoved) {
        return {static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y)};
    }
    return {static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)};
}

Vec2f worldToPixels(Vec2f position)
{
    return {
        Config::fieldMarginPixels + position.x * Config::pixelsPerMeter,
        Config::fieldMarginPixels + position.y * Config::pixelsPerMeter};
}

float lengthSquared(Vec2f value)
{
    return value.x * value.x + value.y * value.y;
}

Vec2f normalize(Vec2f value)
{
    const float length = std::sqrt(lengthSquared(value));
    return length > 0.0f ? Vec2f{value.x / length, value.y / length} : Vec2f{};
}
}

AimController::AimController(EventBus& events) : events_(events) {}

void AimController::handleEvent(
    const sf::Event& event,
    const std::vector<BodyView>& pucks,
    Team activeTeam)
{
    if (event.type == sf::Event::MouseButtonPressed
        && event.mouseButton.button == sf::Mouse::Left) {
        const Vec2f mouse = eventPosition(event);
        for (const auto& puck : pucks) {
            if (puck.team != activeTeam) {
                continue;
            }

            const Vec2f offset = mouse - worldToPixels(puck.position);
            const float radius = puck.radius * Config::pixelsPerMeter;
            if (lengthSquared(offset) <= radius * radius) {
                aiming_ = true;
                selectedPuckId_ = puck.id;
                dragStartPixels_ = mouse;
                dragCurrentPixels_ = mouse;
                return;
            }
        }
    }

    if (event.type == sf::Event::MouseMoved && aiming_) {
        dragCurrentPixels_ = eventPosition(event);
        return;
    }

    if (event.type == sf::Event::MouseButtonReleased
        && event.mouseButton.button == sf::Mouse::Left
        && aiming_) {
        dragCurrentPixels_ = eventPosition(event);
        const Vec2f drag = dragStartPixels_ - dragCurrentPixels_;
        const float dragLength = std::sqrt(lengthSquared(drag));
        if (dragLength >= Config::minDragPixels) {
            events_.emit(ShotExecuted{
                selectedPuckId_,
                normalize(drag),
                std::min(Config::maxShotPower, dragLength / Config::shotPowerScale)});
        }
        aiming_ = false;
    }
}

bool AimController::isAiming() const { return aiming_; }

Vec2f AimController::dragStartPixels() const { return dragStartPixels_; }

Vec2f AimController::dragCurrentPixels() const { return dragCurrentPixels_; }
