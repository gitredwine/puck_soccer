#pragma once

// Physics layer: turns ball-goal sensor contacts into BallEnteredGoal events.

#include "core/eventbus.h"

#include <box2d/box2d.h>

class GoalContactListener : public b2ContactListener {
public:
    explicit GoalContactListener(EventBus& events);

    void BeginContact(b2Contact* contact) override;

private:
    EventBus& events_;
};
