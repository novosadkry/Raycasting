#pragma once
#include <Core.hpp>

using sf::Keyboard;
using sf::Mouse;
using sf::Event;

namespace Input
{
    inline std::unordered_map<Keyboard::Key, bool> KeysHeld{};

    void HandleEvent(Event& event);

    bool GetKey(Keyboard::Key key);
    bool GetKeyDown(Keyboard::Key key);

    bool GetMouseButton(Mouse::Button button);
    sf::Vector2i GetMousePosition();
}