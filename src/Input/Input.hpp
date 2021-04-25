#pragma once

#include <SFML/Window.hpp>
#include "../Game/Game.hpp"

namespace Input
{
    void Setup(Game& game);
    void HandleEvent(sf::Event& event);

    bool GetKey(sf::Keyboard::Key key);
    bool GetMouseButton(sf::Mouse::Button button);
    sf::Vector2i GetMousePosition();
}