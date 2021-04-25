#include "Input.hpp"

static Game* s_Game;

void Input::Setup(Game& game)
{
    s_Game = &game;
}

void Input::HandleEvent(sf::Event& event)
{
    switch (event.type)
    {
        case sf::Event::Closed:
            s_Game->Exit();
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
    return sf::Mouse::getPosition(*s_Game->GetWindow());
}
