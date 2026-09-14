#include "entity/goal.h"

Goal::Goal(std::string id, Team team, b2Body* body)
    : id_(std::move(id)), team_(team), body_(body)
{
}

const std::string& Goal::id() const { return id_; }

Team Goal::team() const { return team_; }

b2Body* Goal::body() const { return body_; }
