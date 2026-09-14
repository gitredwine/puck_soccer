#pragma once

// Input layer: translates mouse drags into ShotExecuted event payloads without Box2D.

#include "core/eventbus.h"
#include "core/types.h"

#include <SFML/Graphics.hpp>

class AimController {
public:
    explicit AimController(EventBus& events);

    void handleEvent(const sf::Event& event, const std::vector<BodyView>& pucks, Team activeTeam);
    bool isAiming() const;
    Vec2f dragStartPixels() const;
    Vec2f dragCurrentPixels() const;

private:
    EventBus& events_;
    bool aiming_ = false;
    std::string selectedPuckId_;
    Vec2f dragStartPixels_;
    Vec2f dragCurrentPixels_;
};
