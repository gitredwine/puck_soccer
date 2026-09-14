#pragma once

// Entity layer: semantic wrapper for a puck body owned by Box2D.

#include "core/types.h"

class b2Body;

class Puck {
public:
    Puck(std::string id, Team team, float radius, b2Body* body);

    const std::string& id() const;
    Team team() const;
    float radius() const;
    b2Body* body() const;
    Vec2f position() const;

private:
    std::string id_;
    Team team_ = Team::Red;
    float radius_ = 0.0f;
    b2Body* body_ = nullptr;
};
