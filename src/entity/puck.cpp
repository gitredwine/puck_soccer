#include "entity/puck.h"

#include <box2d/box2d.h>

Puck::Puck(std::string id, Team team, float radius, b2Body* body)
    : id_(std::move(id)), team_(team), radius_(radius), body_(body)
{
}

const std::string& Puck::id() const { return id_; }

Team Puck::team() const { return team_; }

float Puck::radius() const { return radius_; }

b2Body* Puck::body() const { return body_; }

Vec2f Puck::position() const
{
    const b2Vec2 pos = body_->GetPosition();
    return {pos.x, pos.y};
}
