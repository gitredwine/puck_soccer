#pragma once

// Physics layer: fixed-step world with ball, puck, field boundary, and goal sensors.

#include "core/eventbus.h"
#include "core/types.h"
#include "entity/ball.h"
#include "entity/puck.h"
#include "physics/bodyfactory.h"
#include "physics/contact.h"

#include <box2d/box2d.h>
#include <memory>
#include <vector>

class PhysicsWorld {
public:
    PhysicsWorld(EventBus& events, const LevelConfig& level);

    void loadLevel(const LevelConfig& level);
    void step(float dt);
    void applyShot(const Shot& shot);
    void stopSimulation();
    void resetBall();
    void resetScene();
    bool isSimulationActive() const;
    float stillTime() const;
    float simulationTime() const;
    WorldSnapshot snapshot() const;
    const LevelConfig& level() const;

private:
    void buildScene();
    BodyTag* addTag(BodyKind kind, Team team);

    b2World world_;
    LevelConfig level_;
    GoalContactListener contactListener_;
    Ball ball_;
    std::vector<Puck> pucks_;
    std::vector<std::unique_ptr<BodyTag>> tags_;
    float accumulator_ = 0.0f;
    float stillTime_ = 0.0f;
    float simulationTime_ = 0.0f;
    bool simulationActive_ = false;
    bool goalEntered_ = false;
};
