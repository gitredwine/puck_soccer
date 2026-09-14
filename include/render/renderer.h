#pragma once

// Render layer: draws the physics demo from engine-independent snapshots.

#include "core/types.h"

#include <SFML/Graphics.hpp>
#include <cstddef>
#include <string>
#include <vector>

struct AimView {
    bool active = false;
    Vec2f startPixels;
    Vec2f currentPixels;
};

class Renderer {
public:
    Renderer();

    void drawLevelMenu(
        sf::RenderWindow& window,
        const std::vector<LevelConfig>& levels,
        std::size_t unlockedLevels,
        PlayMode mode);
    void draw(
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
        const std::string& statusText);

private:
    sf::Font font_;
    bool fontLoaded_ = false;
};
