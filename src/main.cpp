#include <SFML/Main.hpp>
#include <Game/Game.hpp>

int main()
{
	auto& game = Game::Init();
	game.LoadLevel(std::make_shared<Level>(500, 500, Grid(5, 5)));

	while (game.IsRunning())
		game.Tick();

    return EXIT_SUCCESS;
}
