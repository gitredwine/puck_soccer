#include "physics/world.h"

#include "core/config.h"

#include <algorithm>
#include <cmath>

namespace {
bool isBodyStill(const b2Body* body)
{
    const b2Vec2 velocity = body->GetLinearVelocity();
    const float speedSquared = velocity.x * velocity.x + velocity.y * velocity.y;
    return speedSquared <= Config::sleepVelocityThreshold * Config::sleepVelocityThreshold
        && std::abs(body->GetAngularVelocity()) <= Config::sleepAngularThreshold;
}
}

PhysicsWorld::PhysicsWorld(EventBus& events, const LevelConfig& level)
    : world_(b2Vec2(0.0f, 0.0f)),
      level_(level),
      contactListener_(events)
{
    world_.SetContactListener(&contactListener_);
    events.subscribe<BallEnteredGoal>([this](const BallEnteredGoal&) {
        goalEntered_ = true;
    });
    buildScene();
}

void PhysicsWorld::loadLevel(const LevelConfig& level)
{
    stopSimulation();
    for (b2Body* body = world_.GetBodyList(); body != nullptr;) {
        b2Body* next = body->GetNext();
        world_.DestroyBody(body);
        body = next;
    }
    pucks_.clear();
    tags_.clear();
    level_ = level;
    ball_ = Ball();
    goalEntered_ = false;
    stillTime_ = 0.0f;
    simulationTime_ = 0.0f;
    buildScene();
}

void PhysicsWorld::buildScene()
{
    BodyFactory::createFieldWalls(world_, level_, addTag(BodyKind::Wall, Team::Red));
    BodyFactory::createGoal(world_, level_, Team::Red, addTag(BodyKind::Goal, Team::Red));
    BodyFactory::createGoal(world_, level_, Team::Blue, addTag(BodyKind::Goal, Team::Blue));
    for (const auto& puck : level_.pucks) {
        pucks_.emplace_back(
            puck.id, puck.team, puck.radius,
            BodyFactory::createPuck(world_, puck, addTag(BodyKind::Puck, puck.team)));
    }
    for (const auto& obstacle : level_.obstacles) {
        BodyFactory::createObstacle(world_, obstacle, addTag(BodyKind::Wall, Team::Red));
    }
    ball_ = Ball(
        level_.ballRadius,
        BodyFactory::createBall(world_, level_, addTag(BodyKind::Ball, Team::Red)));
}

void PhysicsWorld::step(float dt)
{
    if (!simulationActive_) {
        return;
    }

    accumulator_ += dt;
    while (accumulator_ >= Config::fixedTimeStep) {
        world_.Step(
            Config::fixedTimeStep,
            Config::velocityIterations,
            Config::positionIterations);
        accumulator_ -= Config::fixedTimeStep;
        simulationTime_ += Config::fixedTimeStep;

        if (goalEntered_) {
            stopSimulation();
            break;
        }

        bool allStill = isBodyStill(ball_.body());
        for (const auto& puck : pucks_) {
            allStill = allStill && isBodyStill(puck.body());
        }
        if (allStill) {
            stillTime_ += Config::fixedTimeStep;
        } else {
            stillTime_ = 0.0f;
        }

        if (stillTime_ >= Config::sleepConfirmSeconds
            || simulationTime_ >= Config::maxSimulationSeconds) {
            stopSimulation();
            break;
        }
    }
}

void PhysicsWorld::applyShot(const Shot& shot)
{
    if (simulationActive_ || shot.power <= 0.0f) {
        return;
    }

    auto puck = std::find_if(pucks_.begin(), pucks_.end(), [&](const Puck& value) {
        return value.id() == shot.puckId;
    });
    if (puck == pucks_.end()) {
        return;
    }

    simulationActive_ = true;
    stillTime_ = 0.0f;
    simulationTime_ = 0.0f;
    accumulator_ = 0.0f;
    goalEntered_ = false;

    b2Body* body = puck->body();
    body->SetAwake(true);
    body->ApplyLinearImpulseToCenter(
        {shot.dir.x * shot.power * Config::shotImpulseScale,
         shot.dir.y * shot.power * Config::shotImpulseScale},
        true);
}

void PhysicsWorld::stopSimulation()
{
    simulationActive_ = false;
    accumulator_ = 0.0f;
    ball_.body()->SetLinearVelocity({0.0f, 0.0f});
    ball_.body()->SetAngularVelocity(0.0f);
    for (auto& puck : pucks_) {
        puck.body()->SetLinearVelocity({0.0f, 0.0f});
        puck.body()->SetAngularVelocity(0.0f);
    }
}

void PhysicsWorld::resetBall()
{
    b2Body* body = ball_.body();
    body->SetTransform({level_.ballPos.x, level_.ballPos.y}, 0.0f);
    body->SetLinearVelocity({0.0f, 0.0f});
    body->SetAngularVelocity(0.0f);
    body->SetAwake(true);
    goalEntered_ = false;
}

void PhysicsWorld::resetScene()
{
    stopSimulation();
    resetBall();
    for (std::size_t index = 0; index < pucks_.size(); ++index) {
        b2Body* body = pucks_[index].body();
        const Vec2f position = level_.pucks[index].position;
        body->SetTransform({position.x, position.y}, 0.0f);
        body->SetLinearVelocity({0.0f, 0.0f});
        body->SetAngularVelocity(0.0f);
        body->SetAwake(true);
    }
    stillTime_ = 0.0f;
    simulationTime_ = 0.0f;
}

bool PhysicsWorld::isSimulationActive() const { return simulationActive_; }

float PhysicsWorld::stillTime() const { return stillTime_; }

float PhysicsWorld::simulationTime() const { return simulationTime_; }

WorldSnapshot PhysicsWorld::snapshot() const
{
    WorldSnapshot world;
    world.ball = {ball_.position(), ball_.radius()};
    for (const auto& puck : pucks_) {
        world.pucks.push_back({puck.id(), puck.team(), puck.position(), puck.radius()});
    }
    for (const auto& obstacle : level_.obstacles) {
        world.obstacles.push_back({
            obstacle.position, obstacle.size, obstacle.rotationDegrees});
    }
    return world;
}

const LevelConfig& PhysicsWorld::level() const
{
    return level_;
}

BodyTag* PhysicsWorld::addTag(BodyKind kind, Team team)
{
    auto tag = std::make_unique<BodyTag>();
    tag->kind = kind;
    tag->team = team;
    BodyTag* rawTag = tag.get();
    tags_.push_back(std::move(tag));
    return rawTag;
}
