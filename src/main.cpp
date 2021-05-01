#include <SFML/Main.hpp>
#include <Game/Game.hpp>

int main(int argc, char** argv)
{
	auto& game = Game::Init(sf::VideoMode(1024, 720), "Raycaster");

	if (argc > 1)
	{
		auto level = Level::From(argv[1]);
		game.LoadLevel(level);
	}

	while (game.IsRunning())
		game.Tick();

    return EXIT_SUCCESS;
}
