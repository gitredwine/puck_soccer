#pragma once

// AI layer: strategy interface and AI decision context.

#include "core/types.h"

#include <memory>

struct AIContext {
    Team team = Team::Blue;
    WorldSnapshot world;
    LevelConfig level;
};

class IAIStrategy {
public:
    virtual ~IAIStrategy() = default;
    virtual Shot chooseShot(const AIContext& context) = 0;
};

std::unique_ptr<IAIStrategy> makeAI(AIKind kind);
