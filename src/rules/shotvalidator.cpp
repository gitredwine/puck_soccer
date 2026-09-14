#include "rules/shotvalidator.h"

#include <algorithm>
#include <cmath>

bool ShotValidator::canShoot(
    const Shot& shot,
    Team activeTeam,
    const std::vector<BodyView>& pucks) const
{
    if (shot.power <= 0.0f) {
        return false;
    }

    const float len = std::sqrt(shot.dir.x * shot.dir.x + shot.dir.y * shot.dir.y);
    if (len < 0.95f || len > 1.05f) {
        return false;
    }

    const auto found = std::find_if(pucks.begin(), pucks.end(), [&](const BodyView& puck) {
        return puck.id == shot.puckId;
    });
    return found != pucks.end() && found->team == activeTeam;
}
