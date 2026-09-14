#include "game.h"

#include "core/config.h"

#include <algorithm>

Game::Game()
    : level_(makeLevelOne()),
      scores_(events_, level_.maxScore),
      physics_(events_, level_)
{
    events_.subscribe<ShotExecuted>([this](const ShotExecuted& shot) {
        executeShot(shot);
    });
    events_.subscribe<BallEnteredGoal>([this](const BallEnteredGoal& event) {
        pendingGoal_ = event.goalTeam;
        if (!gameOver_) {
            statusText_ = oppositeTeam(event.goalTeam) == Team::Red
                ? "Goal! Red scores"
                : "Goal! Blue scores";
            statusTime_ = 1.5f;
        }
    });
    events_.subscribe<ScoreChanged>([this](const ScoreChanged& score) {
        score_ = score;
    });
    events_.subscribe<GameOver>([this](const GameOver& event) {
        gameOver_ = event;
        state_ = GameState::GameOver;
        if (playMode_ == PlayMode::VsAI && event.winner == Team::Red) {
            unlockedLevels_ = std::max(
                unlockedLevels_, std::min(allLevels().size(), activeLevelIndex_ + 2));
            statusText_ = "Level cleared! Press R for level select";
        } else if (playMode_ == PlayMode::LocalTwoPlayer) {
            statusText_ = event.winner == Team::Red
                ? "Red wins! Press R for level select"
                : "Blue wins! Press R for level select";
        } else {
            statusText_ = "Blue wins! Press R for level select";
        }
        statusTime_ = 0.0f;
    });
    ai_ = makeAI(level_.ai);
    scores_.reset();
    turns_.reset(Team::Red);
}

void Game::handleEvent(const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
        if (state_ == GameState::GameOver) {
            returnToLevelMenu();
        } else if (!showingLevelMenu_) {
            restart();
        }
        return;
    }

    if (showingLevelMenu_) {
        if (event.type == sf::Event::MouseButtonReleased
            && event.mouseButton.button == sf::Mouse::Left) {
            const float mouseX = static_cast<float>(event.mouseButton.x);
            const float mouseY = static_cast<float>(event.mouseButton.y);
            if (mouseY >= Config::modeButtonY
                && mouseY <= Config::modeButtonY + Config::modeButtonHeight) {
                for (std::size_t index = 0; index < 2; ++index) {
                    const float x = Config::modeButtonX
                        + index * (Config::modeButtonWidth + Config::modeButtonGap);
                    if (mouseX >= x && mouseX <= x + Config::modeButtonWidth) {
                        playMode_ = index == 0 ? PlayMode::VsAI : PlayMode::LocalTwoPlayer;
                        return;
                    }
                }
            }
            const auto& levels = selectableLevels();
            const std::size_t unlockedLevels = unlockedSelectableLevels();
            for (std::size_t index = 0; index < levels.size(); ++index) {
                const float x = Config::levelCardX
                    + index * (Config::levelCardWidth + Config::levelCardGap);
                if (index < unlockedLevels
                    && mouseX >= x
                    && mouseX <= x + Config::levelCardWidth
                    && mouseY >= Config::levelCardY
                    && mouseY <= Config::levelCardY + Config::levelCardHeight) {
                    startLevel(index);
                    return;
                }
            }
        }
        return;
    }

    if (state_ == GameState::Aiming
        && (playMode_ == PlayMode::LocalTwoPlayer || turns_.activeTeam() == Team::Red)) {
        aim_.handleEvent(event, physics_.snapshot().pucks, turns_.activeTeam());
    }
}

void Game::update(float dt)
{
    if (showingLevelMenu_) {
        return;
    }

    if (statusTime_ > 0.0f) {
        statusTime_ = std::max(0.0f, statusTime_ - dt);
        if (statusTime_ == 0.0f) {
            statusText_.clear();
        }
    }

    switch (state_) {
    case GameState::Aiming:
        break;
    case GameState::Simulating:
        physics_.step(dt);
        if (pendingGoal_) {
            physics_.stopSimulation();
            physics_.resetBall();
            pendingGoal_.reset();
            if (!gameOver_) {
                state_ = GameState::Resolving;
            }
        } else if (!physics_.isSimulationActive()) {
            state_ = GameState::Resolving;
        }
        break;
    case GameState::Resolving:
        turns_.nextTurn();
        state_ = playMode_ == PlayMode::LocalTwoPlayer || turns_.activeTeam() == Team::Red
            ? GameState::Aiming
            : GameState::AiTurn;
        break;
    case GameState::AiTurn:
        startAiTurn();
        break;
    case GameState::CheckEnd:
    case GameState::GameOver:
        break;
    }
}

void Game::render(sf::RenderWindow& window)
{
    if (showingLevelMenu_) {
        renderer_.drawLevelMenu(
            window, selectableLevels(), unlockedSelectableLevels(), playMode_);
        return;
    }

    AimView aimView;
    aimView.active = state_ == GameState::Aiming && aim_.isAiming();
    aimView.startPixels = aim_.dragStartPixels();
    aimView.currentPixels = aim_.dragCurrentPixels();
    renderer_.draw(
        window,
        physics_.snapshot(),
        level_,
        playMode_,
        turns_.activeTeam(),
        state_,
        score_,
        physics_.isSimulationActive(),
        physics_.stillTime(),
        physics_.simulationTime(),
        aimView,
        statusText_);
}

void Game::executeShot(const ShotExecuted& shot)
{
    if (state_ != GameState::Aiming && state_ != GameState::AiTurn) {
        return;
    }

    physics_.applyShot({shot.puckId, shot.dir, shot.power});
    if (physics_.isSimulationActive()) {
        state_ = GameState::Simulating;
    }
}

void Game::startAiTurn()
{
    if (playMode_ != PlayMode::VsAI || !ai_ || gameOver_) {
        return;
    }

    AIContext context;
    context.team = turns_.activeTeam();
    context.world = physics_.snapshot();
    context.level = level_;
    const Shot shot = ai_->chooseShot(context);
    if (!shot.puckId.empty()) {
        events_.emit(ShotExecuted{shot.puckId, shot.dir, shot.power});
    }
}

void Game::restart()
{
    physics_.resetScene();
    pendingGoal_.reset();
    gameOver_.reset();
    statusText_.clear();
    statusTime_ = 0.0f;
    scores_.reset();
    turns_.reset(Team::Red);
    state_ = GameState::Aiming;
}

void Game::startLevel(std::size_t index)
{
    const auto& levels = selectableLevels();
    if (index >= unlockedSelectableLevels() || index >= levels.size()) {
        return;
    }

    activeLevelIndex_ = index;
    level_ = levels[index];
    physics_.loadLevel(level_);
    scores_.setMaxScore(level_.maxScore);
    scores_.reset();
    turns_.reset(Team::Red);
    ai_ = playMode_ == PlayMode::VsAI ? makeAI(level_.ai) : nullptr;
    pendingGoal_.reset();
    gameOver_.reset();
    statusText_.clear();
    statusTime_ = 0.0f;
    state_ = GameState::Aiming;
    showingLevelMenu_ = false;
}

void Game::returnToLevelMenu()
{
    physics_.stopSimulation();
    pendingGoal_.reset();
    gameOver_.reset();
    statusText_.clear();
    statusTime_ = 0.0f;
    showingLevelMenu_ = true;
}

const std::vector<LevelConfig>& Game::selectableLevels() const
{
    return playMode_ == PlayMode::LocalTwoPlayer ? localTwoPlayerLevels() : allLevels();
}

std::size_t Game::unlockedSelectableLevels() const
{
    return playMode_ == PlayMode::LocalTwoPlayer
        ? localTwoPlayerLevels().size()
        : unlockedLevels_;
}
