#pragma once

// Entity layer: semantic wrapper for a goal sensor owned by Box2D.

#include "core/types.h"

class b2Body;

class Goal {
public:
    Goal(std::string id, Team team, b2Body* body);

    const std::string& id() const;
    Team team() const;
    b2Body* body() const;

private:
    std::string id_;
    Team team_ = Team::Red;
    b2Body* body_ = nullptr;
};
