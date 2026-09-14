#pragma once

// AI layer: simple random legal shot producer.

#include "ai/IAIstrategy.h"

#include <random>

class RandomAI : public IAIStrategy {
public:
    Shot chooseShot(const AIContext& context) override;

private:
    std::mt19937 rng_{std::random_device{}()};
};
