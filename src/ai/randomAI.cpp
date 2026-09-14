#include "ai/randomAI.h"

#include "core/config.h"

#include <algorithm>
#include <cmath>

namespace {
Vec2f normalize(Vec2f value)
{
    const float len = std::sqrt(value.x * value.x + value.y * value.y);
    if (len <= 0.0001f) {
        return {-1.0f, 0.0f};
    }
    return {value.x / len, value.y / len};
}
}

Shot RandomAI::chooseShot(const AIContext& context)
{
    std::vector<BodyView> ownPucks;
    for (const auto& puck : context.world.pucks) {
        if (puck.team == context.team) {
            ownPucks.push_back(puck);
        }
    }
    if (ownPucks.empty()) {
        return {};
    }

    std::uniform_int_distribution<std::size_t> puckDist(0, ownPucks.size() - 1);
    std::uniform_real_distribution<float> aimJitter(-0.8f, 0.8f);
    std::uniform_real_distribution<float> powerDist(0.45f, 0.85f);

    const BodyView& puck = ownPucks[puckDist(rng_)];
    Vec2f target = context.world.ball.position;
    target.y += aimJitter(rng_);

    return {
        puck.id,
        normalize(target - puck.position),
        Config::maxShotPower * powerDist(rng_)
    };
}
