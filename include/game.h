#pragma once

// Assembly layer: connects input events, physics simulation, and rendering.

#include "core/eventbus.h"
#include "ai/IAIstrategy.h"
#include "input/aimcontroller.h"
#include "levels/levels.h"
#include "physics/world.h"
#include "render/renderer.h"
#include "rules/scoremanage.h"
#include "rules/turnmanage.h"

#include <SFML/Graphics.hpp>
#include <cstddef>
#include <optional>
#include <memory>
#include <string>

class Game {
public:
    Game();

    void handleEvent(const sf::Event& event);
    void update(float dt);
    void render(sf::RenderWindow& window);

private:
    void executeShot(const ShotExecuted& shot);
    void startAiTurn();
    void restart();
    void startLevel(std::size_t index);
    void returnToLevelMenu();
    const std::vector<LevelConfig>& selectableLevels() const;
    std::size_t unlockedSelectableLevels() const;

    EventBus events_;
    LevelConfig level_;
    ScoreManager scores_;
    TurnManager turns_{events_};
    AimController aim_{events_};
    PhysicsWorld physics_;
    Renderer renderer_;
    std::unique_ptr<IAIStrategy> ai_;
    GameState state_ = GameState::Aiming;
    ScoreChanged score_;
    std::optional<Team> pendingGoal_;
    std::optional<GameOver> gameOver_;
    std::string statusText_;
    float statusTime_ = 0.0f;
    std::size_t activeLevelIndex_ = 0;
    std::size_t unlockedLevels_ = 1;
    bool showingLevelMenu_ = true;
    PlayMode playMode_ = PlayMode::VsAI;
};
