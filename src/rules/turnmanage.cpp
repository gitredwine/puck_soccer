#include "rules/turnmanage.h"

TurnManager::TurnManager(EventBus& events) : events_(events) {}

void TurnManager::reset(Team firstTeam)
{
    activeTeam_ = firstTeam;
    events_.emit(TurnChanged{activeTeam_});
}

void TurnManager::nextTurn()
{
    activeTeam_ = oppositeTeam(activeTeam_);
    events_.emit(TurnChanged{activeTeam_});
}

Team TurnManager::activeTeam() const
{
    return activeTeam_;
}
