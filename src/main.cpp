#include <Core.hpp>

#include <Game/Game.hpp>

int main(int argc, char** argv)
{
	auto& game = Game::Init(sf::VideoMode(1024, 720), "Raycaster");
	Unique<Level> level = Level::Empty();

	try
	{
		level = argc > 1
			? Level::From(argv[1])
			: Level::From("SAVE.lvl");
	}
	catch (std::exception& e)
	{
		LOG(e.what());
	}

	game.LoadLevel(std::move(level));

	while (game.IsRunning())
		game.Tick();

    return EXIT_SUCCESS;
}
