#pragma once

// Core layer: shared value types and strongly typed events.

#include <string>
#include <vector>

enum class Team { Red, Blue };

inline Team oppositeTeam(Team team)
{
    return team == Team::Red ? Team::Blue : Team::Red;
}

struct Vec2f {
    float x = 0.0f;
    float y = 0.0f;
};

inline Vec2f operator+(Vec2f left, Vec2f right)
{
    return {left.x + right.x, left.y + right.y};
}

inline Vec2f operator-(Vec2f left, Vec2f right)
{
    return {left.x - right.x, left.y - right.y};
}

inline Vec2f operator*(Vec2f value, float scale)
{
    return {value.x * scale, value.y * scale};
}

enum class GameState {
    Aiming,
    Simulating,
    Resolving,
    CheckEnd,
    AiTurn,
    GameOver
};

enum class AIKind { Random, Greedy };

enum class PlayMode { VsAI, LocalTwoPlayer };

struct PuckConfig {
    std::string id;
    Team team = Team::Red;
    Vec2f position;
    float radius = 0.35f;
    float density = 1.0f;
    float friction = 0.25f;
    float restitution = 0.45f;
    float linearDamping = 0.8f;
    float angularDamping = 1.0f;
};

struct ObstacleConfig {
    std::string id;
    Vec2f position;
    Vec2f size;
    float rotationDegrees = 0.0f;
    float friction = 0.4f;
    float restitution = 0.25f;
};

struct LevelConfig {
    std::string id;
    std::string name;
    float fieldWidth = 16.0f;
    float fieldHeight = 9.0f;
    float goalWidth = 2.5f;
    float goalDepth = 0.6f;
    Vec2f ballPos;
    float ballRadius = 0.22f;
    float ballDensity = 0.45f;
    float ballFriction = 0.15f;
    float ballRestitution = 0.65f;
    float ballLinearDamping = 0.45f;
    float ballAngularDamping = 0.7f;
    std::vector<PuckConfig> pucks;
    std::vector<ObstacleConfig> obstacles;
    int maxScore = 2;
    AIKind ai = AIKind::Random;
    int maxShots = -1;
};

struct Shot {
    std::string puckId;
    Vec2f dir;
    float power = 0.0f;
};

struct BodyView {
    std::string id;
    Team team = Team::Red;
    Vec2f position;
    float radius = 0.0f;
};

struct BallView {
    Vec2f position;
    float radius = 0.0f;
};

struct ObstacleView {
    Vec2f position;
    Vec2f size;
    float rotationDegrees = 0.0f;
};

struct WorldSnapshot {
    std::vector<BodyView> pucks;
    BallView ball;
    std::vector<ObstacleView> obstacles;
};

struct BallEnteredGoal {
    Team goalTeam = Team::Red;
};

struct ShotExecuted {
    std::string puckId;
    Vec2f dir;
    float power = 0.0f;
};

struct TurnChanged {
    Team activeTeam = Team::Red;
};

struct ScoreChanged {
    int red = 0;
    int blue = 0;
};

struct GameOver {
    Team winner = Team::Red;
    bool draw = false;
};
