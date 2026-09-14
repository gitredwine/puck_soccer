#include "ai/greedyAI.h"

#include "ai/randomAI.h"
#include "core/config.h"

#include <cmath>
#include <limits>
#include <memory>

namespace {
Vec2f normalize(Vec2f value)
{
    const float len = std::sqrt(value.x * value.x + value.y * value.y);
    if (len <= 0.0001f) {
        return {-1.0f, 0.0f};
    }
    return {value.x / len, value.y / len};
}

float distanceSquared(Vec2f a, Vec2f b)
{
    const Vec2f d = a - b;
    return d.x * d.x + d.y * d.y;
}
}

Shot GreedyAI::chooseShot(const AIContext& context)
{
    const BodyView* best = nullptr;
    float bestDistance = std::numeric_limits<float>::max();
    for (const auto& puck : context.world.pucks) {
        if (puck.team != context.team) {
            continue;
        }

        const float d = distanceSquared(puck.position, context.world.ball.position);
        if (d < bestDistance) {
            best = &puck;
            bestDistance = d;
        }
    }

    if (best == nullptr) {
        return {};
    }

    return {
        best->id,
        normalize(context.world.ball.position - best->position),
        Config::maxShotPower * 0.78f
    };
}

std::unique_ptr<IAIStrategy> makeAI(AIKind kind)
{
    if (kind == AIKind::Greedy) {
        return std::make_unique<GreedyAI>();
    }
    return std::make_unique<RandomAI>();
}
