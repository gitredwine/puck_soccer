#pragma once

// Physics layer: constructs demo bodies, static boundary, and goal sensors.

#include "core/types.h"

#include <box2d/box2d.h>

enum class BodyKind { Puck, Ball, Goal, Wall };

struct BodyTag {
    BodyKind kind = BodyKind::Wall;
    Team team = Team::Red;
};

class BodyFactory {
public:
    static b2Body* createPuck(b2World& world, const PuckConfig& config, BodyTag* tag);
    static b2Body* createBall(b2World& world, const LevelConfig& config, BodyTag* tag);
    static b2Body* createGoal(b2World& world, const LevelConfig& config, Team team, BodyTag* tag);
    static void createFieldWalls(b2World& world, const LevelConfig& config, BodyTag* tag);
    static void createObstacle(b2World& world, const ObstacleConfig& config, BodyTag* tag);
};
