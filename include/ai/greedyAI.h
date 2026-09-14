#pragma once

// AI layer: greedy strategy that shoots the nearest puck toward the ball.

#include "ai/IAIstrategy.h"

class GreedyAI : public IAIStrategy {
public:
    Shot chooseShot(const AIContext& context) override;
};
