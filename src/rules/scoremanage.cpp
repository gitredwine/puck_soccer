#include "rules/scoremanage.h"

ScoreManager::ScoreManager(EventBus& events, int maxScore)
    : events_(events), maxScore_(maxScore)
{
    events_.subscribe<BallEnteredGoal>([this](const BallEnteredGoal& event) {
        awardGoalIn(event.goalTeam);
    });
}

void ScoreManager::reset()
{
    red_ = 0;
    blue_ = 0;
    gameOver_ = false;
    events_.emit(ScoreChanged{red_, blue_});
}

void ScoreManager::setMaxScore(int maxScore)
{
    maxScore_ = maxScore;
}

ScoreChanged ScoreManager::score() const
{
    return {red_, blue_};
}

bool ScoreManager::isGameOver() const
{
    return gameOver_;
}

void ScoreManager::awardGoalIn(Team goalTeam)
{
    if (oppositeTeam(goalTeam) == Team::Red) {
        ++red_;
    } else {
        ++blue_;
    }
    events_.emit(ScoreChanged{red_, blue_});

    if (!gameOver_ && (red_ >= maxScore_ || blue_ >= maxScore_)) {
        gameOver_ = true;
        events_.emit(GameOver{red_ > blue_ ? Team::Red : Team::Blue, false});
    }
}
