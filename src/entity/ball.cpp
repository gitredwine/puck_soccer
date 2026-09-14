#include "entity/ball.h"

#include <box2d/box2d.h>

Ball::Ball(float radius, b2Body* body) : radius_(radius), body_(body) {}

float Ball::radius() const { return radius_; }

b2Body* Ball::body() const { return body_; }

Vec2f Ball::position() const
{
    const b2Vec2 pos = body_->GetPosition();
    return {pos.x, pos.y};
}
