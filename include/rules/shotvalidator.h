#pragma once

// Rules layer: validates whether a shot is legal for the active turn.

#include "core/types.h"

#include <vector>

class ShotValidator {
public:
    bool canShoot(const Shot& shot, Team activeTeam, const std::vector<BodyView>& pucks) const;
};
