#include <iostream>
#include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow rw(sf::VideoMode(800, 600), "SFML window");

	while (rw.isOpen())
	{
		sf::Event e{};
		
		while (rw.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
				rw.close();
		}
	}
	
    return EXIT_SUCCESS;
}
