#pragma once

// Render layer: moving objects and aiming overlays.

#include "core/types.h"

#include <SFML/Graphics.hpp>
#include <optional>

struct AimView {
    bool active = false;
    Vec2f startPixels;
    Vec2f currentPixels;
};

void drawEntities(sf::RenderTarget& target, const WorldSnapshot& world);
void drawObstacles(sf::RenderTarget& target, const WorldSnapshot& world);
void drawAim(sf::RenderTarget& target, const AimView& aim);
