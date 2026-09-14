#pragma once

// Render layer: field and static geometry drawing helpers.

#include "core/types.h"

#include <SFML/Graphics.hpp>

void drawField(sf::RenderTarget& target, const LevelConfig& level);
