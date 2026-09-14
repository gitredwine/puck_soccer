#pragma once

// Levels layer: built-in level data access.

#include "core/types.h"

#include <vector>

const std::vector<LevelConfig>& allLevels();
const std::vector<LevelConfig>& localTwoPlayerLevels();
LevelConfig makeLevelOne();
