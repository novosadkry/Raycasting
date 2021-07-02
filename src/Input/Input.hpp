#pragma once

#include <SFML/Window.hpp>

using sf::Keyboard;
using sf::Mouse;
using sf::Event;

namespace Input
{
    void HandleEvent(Event& event);

    bool GetKey(Keyboard::Key key);
    bool GetMouseButton(Mouse::Button button);
    sf::Vector2i GetMousePosition();
}