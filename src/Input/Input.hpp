#pragma once

#include <SFML/Window.hpp>

namespace Input
{
    void HandleEvent(sf::Event& event);

    bool GetKey(sf::Keyboard::Key key);
    bool GetMouseButton(sf::Mouse::Button button);
    sf::Vector2i GetMousePosition();
}