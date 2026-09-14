#pragma once

// Rules layer: subscribes to BallEnteredGoal and owns the current score only.

#include "core/eventbus.h"
#include "core/types.h"

class ScoreManager {
public:
    ScoreManager(EventBus& events, int maxScore);

    void reset();
    void setMaxScore(int maxScore);
    ScoreChanged score() const;
    bool isGameOver() const;

private:
    void awardGoalIn(Team goalTeam);

    EventBus& events_;
    int red_ = 0;
    int blue_ = 0;
    int maxScore_ = 2;
    bool gameOver_ = false;
};
