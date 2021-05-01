#include "Input.hpp"
#include <Game/Game.hpp>

void Input::HandleEvent(sf::Event& event)
{
    switch (event.type)
    {
        case sf::Event::KeyPressed:
        {
            switch (event.key.code)
            {
                case sf::Keyboard::Escape:
                    Game::Get().Exit();
                    break;
            }
        } break;

        case sf::Event::Closed:
            Game::Get().Exit();
            break;

        case sf::Event::Resized:
            Game::Get().GetWindow().setView(
                sf::View(sf::FloatRect(0, 0, (float) event.size.width, (float) event.size.height))
            );
            break;
    }
}

bool Input::GetKey(sf::Keyboard::Key key)
{
    return sf::Keyboard::isKeyPressed(key);
}

bool Input::GetMouseButton(sf::Mouse::Button button)
{
    return sf::Mouse::isButtonPressed(button);
}

sf::Vector2i Input::GetMousePosition()
{
    return sf::Mouse::getPosition(Game::Get().GetWindow());
}
