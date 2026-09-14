#include "core/config.h"
#include "game.h"

#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>

int main()
{
#if SFML_VERSION_MAJOR >= 3
    sf::RenderWindow window(
        sf::VideoMode({
            static_cast<unsigned int>(Config::windowWidth),
            static_cast<unsigned int>(Config::windowHeight)}),
        "Puck Soccer");
#else
    sf::RenderWindow window(
        sf::VideoMode(
            static_cast<unsigned int>(Config::windowWidth),
            static_cast<unsigned int>(Config::windowHeight)),
        "Puck Soccer");
#endif
    window.setFramerateLimit(120);

    Game game;
    sf::Clock clock;

    while (window.isOpen()) {
#if SFML_VERSION_MAJOR >= 3
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            game.handleEvent(*event);
        }
#else
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            game.handleEvent(event);
        }
#endif

        game.update(clock.restart().asSeconds());
        game.render(window);
    }

    return 0;
}
