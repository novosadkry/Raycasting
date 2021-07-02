#include <SFML/Main.hpp>
#include <Game/Game.hpp>

int main(int argc, char** argv)
{
	auto& game = Game::Init(sf::VideoMode(1024, 720), "Raycaster");

	auto level = argc > 1
		? Level::From(argv[1])
		: Level::Empty;

	game.LoadLevel(std::move(level));

	while (game.IsRunning())
		game.Tick();

    return EXIT_SUCCESS;
}
