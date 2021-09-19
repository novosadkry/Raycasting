#include "Input.hpp"

#include <Game/Game.hpp>
#include <Game/Render/Layers/DebugMenu.hpp>

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

                case Keyboard::F1:
                    Game::Get().GetLayers().Get<DebugMenu>()->Toggle();
                    break;

                case Keyboard::F5:
                    Level::Save(Game::Get().GetCurrentLevel(), "SAVE.lvl");
                    break;

                case Keyboard::F6:
                    Game::Get().LoadLevel(Level::From("SAVE.lvl"));
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
