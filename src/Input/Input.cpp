#include "Input.hpp"
#include <Game/Game.hpp>

void Input::HandleEvent(Event& event)
{
    switch (event.type)
    {
        case Event::KeyPressed:
        {
            switch (event.key.code)
            {
                case Keyboard::Escape:
                    Game::Get().Exit();
                    break;
            }
        } break;

        case Event::Closed:
            Game::Get().Exit();
            break;

        case Event::Resized:
            Game::Get().GetWindow().setView(
                sf::View(sf::FloatRect(0, 0, (float) event.size.width, (float) event.size.height))
            );
            break;
    }
}

bool Input::GetKey(Keyboard::Key key)
{
    return Keyboard::isKeyPressed(key);
}

bool Input::GetMouseButton(Mouse::Button button)
{
    return Mouse::isButtonPressed(button);
}

sf::Vector2i Input::GetMousePosition()
{
    return Mouse::getPosition(Game::Get().GetWindow());
}
