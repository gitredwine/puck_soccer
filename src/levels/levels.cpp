#include "levels/levels.h"

namespace {
std::vector<PuckConfig> standardPucks()
{
    return {
        {"red-1", Team::Red, {2.2f, 2.1f}},
        {"red-2", Team::Red, {2.2f, 4.5f}},
        {"red-3", Team::Red, {2.2f, 6.9f}},
        {"red-4", Team::Red, {3.8f, 3.2f}},
        {"red-5", Team::Red, {3.8f, 5.8f}},
        {"blue-1", Team::Blue, {13.8f, 2.1f}},
        {"blue-2", Team::Blue, {13.8f, 4.5f}},
        {"blue-3", Team::Blue, {13.8f, 6.9f}},
        {"blue-4", Team::Blue, {12.2f, 3.2f}},
        {"blue-5", Team::Blue, {12.2f, 5.8f}},
    };
}
}

LevelConfig makeLevelOne()
{
    LevelConfig level;
    level.id = "L1";
    level.name = "Warm Up";
    level.fieldWidth = 16.0f;
    level.fieldHeight = 9.0f;
    level.goalWidth = 3.0f;
    level.goalDepth = 0.7f;
    level.ballPos = {8.0f, 4.5f};
    level.maxScore = 2;
    level.ai = AIKind::Random;
    level.maxShots = -1;
    level.pucks = standardPucks();
    return level;
}

const std::vector<LevelConfig>& allLevels()
{
    static const std::vector<LevelConfig> levels = [] {
        std::vector<LevelConfig> result;
        result.push_back(makeLevelOne());

        LevelConfig narrow = makeLevelOne();
        narrow.id = "L2";
        narrow.name = "Narrow Goal";
        narrow.goalWidth = 2.5f;
        result.push_back(narrow);

        LevelConfig obstacle = makeLevelOne();
        obstacle.id = "L3";
        obstacle.name = "Around The Block";
        obstacle.obstacles.push_back({"center-block", {8.0f, 4.5f}, {0.55f, 2.2f}, 0.0f});
        obstacle.ballPos = {8.0f, 2.0f};
        result.push_back(obstacle);

        LevelConfig greedy = makeLevelOne();
        greedy.id = "L4";
        greedy.name = "Pressure";
        greedy.ai = AIKind::Greedy;
        result.push_back(greedy);

        LevelConfig comeback = makeLevelOne();
        comeback.id = "L5";
        comeback.name = "Comeback";
        comeback.ballDensity = 0.65f;
        comeback.pucks.erase(comeback.pucks.begin() + 4);
        result.push_back(comeback);

        return result;
    }();
    return levels;
}

const std::vector<LevelConfig>& localTwoPlayerLevels()
{
    static const std::vector<LevelConfig> levels = [] {
        LevelConfig openField = makeLevelOne();
        openField.id = "P1";
        openField.name = "Open Field";

        LevelConfig obstacleArena = openField;
        obstacleArena.id = "P2";
        obstacleArena.name = "Obstacle Arena";
        obstacleArena.obstacles.push_back(
            {"center-block", {8.0f, 4.5f}, {0.55f, 2.2f}, 0.0f});

        return std::vector<LevelConfig>{openField, obstacleArena};
    }();
    return levels;
}
