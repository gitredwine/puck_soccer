#pragma once

// Entity layer: semantic wrapper for the ball body owned by Box2D.

#include "core/types.h"

class b2Body;

class Ball {
public:
    Ball() = default;
    Ball(float radius, b2Body* body);

    float radius() const;
    b2Body* body() const;
    Vec2f position() const;

private:
    float radius_ = 0.0f;
    b2Body* body_ = nullptr;
};
