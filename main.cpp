#include <SFML/Graphics.hpp>
#include "Ball.h"
#include "player.h"
#include "Brick.h"

double mapValue(double x, double a, double b, double c, double d) {
	double y = (x - a) / (a - b)*(c - d) + c; 
	return y;
}
int main(int argc, char **argv)
{
	Player player(500, 100, 20);

	Ball ball(200, 250, 10,500);
	sf::RenderWindow window(sf::VideoMode(800, 600), "Jeu de casse-brique");

	sf::RectangleShape rdr2;
	rdr2.setSize(sf::Vector2f(window.getSize().x, 1));
	
	
	sf::Clock clock;
	sf::Vector3f xFactor(10, 20, 30);
	float ellapsedTime = 0;


	
	while (window.isOpen())
	{
		ellapsedTime = clock.restart().asSeconds();
		
		sf::Event event;
		while (window.pollEvent(event))
		{
			
			if (event.type == sf::Event::Closed)
				window.close();
			
		}
		
		ball.move(ellapsedTime);
		ball.manageCollisionWith(window);

		window.clear();

		for (int i = 0; i < window.getSize().y; i++)
		{
			double mappedValue = mapValue(i, 0, window.getSize().y, 0, 255);
			rdr2.setFillColor(sf::Color(mappedValue, mappedValue, 190));
			rdr2.setPosition(0, i);
			window.draw(rdr2);
		}

		//window.draw(rectangle);
		//window.draw(circle);
		ball.draw(window);
		player.draw(window);
		window.display();
	}

	return 0;
}