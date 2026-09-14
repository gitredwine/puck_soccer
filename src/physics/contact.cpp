#include "physics/contact.h"

#include "physics/bodyfactory.h"

namespace {
BodyTag* tagFor(const b2Fixture* fixture)
{
    return reinterpret_cast<BodyTag*>(fixture->GetBody()->GetUserData().pointer);
}
}

GoalContactListener::GoalContactListener(EventBus& events) : events_(events) {}

void GoalContactListener::BeginContact(b2Contact* contact)
{
    const BodyTag* first = tagFor(contact->GetFixtureA());
    const BodyTag* second = tagFor(contact->GetFixtureB());
    if (first == nullptr || second == nullptr) {
        return;
    }

    if (first->kind == BodyKind::Ball && second->kind == BodyKind::Goal) {
        events_.emit(BallEnteredGoal{second->team});
    } else if (second->kind == BodyKind::Ball && first->kind == BodyKind::Goal) {
        events_.emit(BallEnteredGoal{first->team});
    }
}
