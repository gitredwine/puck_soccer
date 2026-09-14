#pragma once

// Core layer: publish/subscribe bus; payloads include ShotExecuted, BallEnteredGoal, TurnChanged, ScoreChanged, and GameOver.

#include <functional>
#include <typeindex>
#include <unordered_map>
#include <vector>

class EventBus {
public:
    template <typename Event>
    using Handler = std::function<void(const Event&)>;

    template <typename Event>
    void subscribe(Handler<Event> handler)
    {
        auto wrapper = [handler = std::move(handler)](const void* event) {
            handler(*static_cast<const Event*>(event));
        };
        handlers_[std::type_index(typeid(Event))].push_back(std::move(wrapper));
    }

    template <typename Event>
    void emit(const Event& event) const
    {
        const auto found = handlers_.find(std::type_index(typeid(Event)));
        if (found == handlers_.end()) {
            return;
        }

        for (const auto& handler : found->second) {
            handler(&event);
        }
    }

private:
    using ErasedHandler = std::function<void(const void*)>;
    std::unordered_map<std::type_index, std::vector<ErasedHandler>> handlers_;
};
