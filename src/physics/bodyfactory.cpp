#include "physics/bodyfactory.h"

#include "core/config.h"

#include <cstdint>

namespace {
void setTag(b2Body* body, BodyTag* tag)
{
    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(tag);
}

b2Body* createDynamicCircle(
    b2World& world,
    float x,
    float y,
    float radius,
    float density,
    float friction,
    float restitution,
    BodyTag* tag)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y);
    bodyDef.linearDamping = Config::demoLinearDamping;

    b2Body* body = world.CreateBody(&bodyDef);
    setTag(body, tag);

    b2CircleShape shape;
    shape.m_radius = radius;
    b2FixtureDef fixture;
    fixture.shape = &shape;
    fixture.density = density;
    fixture.friction = friction;
    fixture.restitution = restitution;
    body->CreateFixture(&fixture);
    return body;
}
}

b2Body* BodyFactory::createPuck(b2World& world, const PuckConfig& config, BodyTag* tag)
{
    return createDynamicCircle(world, config.position.x, config.position.y, config.radius,
                               config.density, config.friction, config.restitution, tag);
}

b2Body* BodyFactory::createBall(b2World& world, const LevelConfig& config, BodyTag* tag)
{
    return createDynamicCircle(world, config.ballPos.x, config.ballPos.y, config.ballRadius,
                               config.ballDensity, config.ballFriction, config.ballRestitution, tag);
}

b2Body* BodyFactory::createGoal(b2World& world, const LevelConfig& config, Team team, BodyTag* tag)
{
    b2BodyDef bodyDef;
    bodyDef.position.Set(
        team == Team::Red ? config.goalDepth * 0.5f
                          : config.fieldWidth - config.goalDepth * 0.5f,
        config.fieldHeight * 0.5f);
    b2Body* body = world.CreateBody(&bodyDef);
    setTag(body, tag);

    b2PolygonShape shape;
    shape.SetAsBox(config.goalDepth * 0.5f, config.goalWidth * 0.5f);
    b2FixtureDef fixture;
    fixture.shape = &shape;
    fixture.isSensor = true;
    body->CreateFixture(&fixture);
    return body;
}

void BodyFactory::createFieldWalls(b2World& world, const LevelConfig& config, BodyTag* tag)
{
    b2BodyDef bodyDef;
    b2Body* body = world.CreateBody(&bodyDef);
    setTag(body, tag);

    auto addWall = [&body](float centerX, float centerY, float halfWidth, float halfHeight) {
        b2PolygonShape shape;
        shape.SetAsBox(halfWidth, halfHeight, {centerX, centerY}, 0.0f);

        b2FixtureDef fixture;
        fixture.shape = &shape;
        fixture.friction = Config::demoFriction;
        fixture.restitution = Config::demoRestitution;
        body->CreateFixture(&fixture);
    };

    constexpr float thickness = 0.16f;
    const float halfWidth = config.fieldWidth * 0.5f;
    const float halfHeight = config.fieldHeight * 0.5f;
    addWall(halfWidth, -thickness * 0.5f, halfWidth, thickness * 0.5f);
    addWall(halfWidth, config.fieldHeight + thickness * 0.5f,
            halfWidth, thickness * 0.5f);
    addWall(-thickness * 0.5f, halfHeight,
            thickness * 0.5f, halfHeight);
    addWall(config.fieldWidth + thickness * 0.5f, halfHeight,
            thickness * 0.5f, halfHeight);
}

void BodyFactory::createObstacle(b2World& world, const ObstacleConfig& config, BodyTag* tag)
{
    b2BodyDef bodyDef;
    bodyDef.position.Set(config.position.x, config.position.y);
    b2Body* body = world.CreateBody(&bodyDef);
    setTag(body, tag);

    b2PolygonShape shape;
    shape.SetAsBox(config.size.x * 0.5f, config.size.y * 0.5f,
                   {0.0f, 0.0f}, config.rotationDegrees * b2_pi / 180.0f);
    b2FixtureDef fixture;
    fixture.shape = &shape;
    fixture.friction = config.friction;
    fixture.restitution = config.restitution;
    body->CreateFixture(&fixture);
}
