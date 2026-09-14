#pragma once

// Rules layer: tracks active team and turn transitions.

#include "core/eventbus.h"
#include "core/types.h"

class TurnManager {
public:
    explicit TurnManager(EventBus& events);

    void reset(Team firstTeam = Team::Red);
    void nextTurn();
    Team activeTeam() const;

private:
    EventBus& events_;
    Team activeTeam_ = Team::Red;
};
