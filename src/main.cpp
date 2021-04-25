#include <iostream>
#include <SFML/Main.hpp>

#include "Game/Game.hpp"

int main()
{
	auto rw = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600), "SFML window");

	Game game(std::move(rw));
	while (game.IsRunning())
	{
		game.PollEvents();
		game.Update();
		game.Render();
	}

    return EXIT_SUCCESS;
}
