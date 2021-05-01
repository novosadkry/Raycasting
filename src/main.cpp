#include <SFML/Main.hpp>
#include <Game/Game.hpp>

int main()
{
	auto& game = Game::Init(sf::VideoMode(1024, 720), "Raycaster");

	game.LoadLevel(std::make_shared<Level>(500, 500,
		Grid({10, 10}, {
			Wall, Wall , Wall , Wall , Wall , Wall , Wall , Wall , Wall , Wall,
			Wall, Empty, Empty, Empty, Wall , Empty, Empty, Empty, Empty, Wall,
			Wall, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Wall,
			Wall, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Wall,
			Wall, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Wall,
			Wall, Empty, Empty, Empty, Wall , Wall , Wall , Empty, Empty, Wall,
			Wall, Empty, Empty, Empty, Wall , Empty, Empty, Empty, Empty, Wall,
			Wall, Empty, Empty, Empty, Wall , Empty, Empty, Empty, Empty, Wall,
			Wall, Empty, Empty, Empty, Wall , Empty, Empty, Empty, Empty, Wall,
			Wall, Wall , Wall , Wall , Wall , Wall , Wall , Wall , Wall , Wall,
		})
	));

	while (game.IsRunning())
		game.Tick();

    return EXIT_SUCCESS;
}
