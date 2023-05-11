#include <SFML/Graphics.hpp>
#include "Ball.h"
#include "player.h"
#include "Brick.h"
#include <deque>
#include <memory>
#include <iostream>

double mapValue(double x, double a, double b, double c, double d) {
	double y = (x - a) / (a - b)*(c - d) + c;
	return y;
}
int main(int argc, char **argv)
{

	Player player(510, 70, 10);
	Ball ball(200, 250, 10, 500);
	Brick brick(100, 100, 50, 30, 3);
	sf::RenderWindow window(sf::VideoMode(800, 600), "Jeu de casse-brique");

	sf::RectangleShape rdr2;
	rdr2.setSize(sf::Vector2f(window.getSize().x, 1));


	sf::Clock clock;
	sf::Vector3f xFactor(10, 20, 30);
	float ellapsedTime = 0;

	std::deque<std::unique_ptr<Brick>> bricks;
	int numRows = 5;
	int numCols = 10;
	int brickWidth = 75;
	int brickHeight = 30;
	int brickSpacing = 5;
	int startX = (window.getSize().x - numCols * (brickWidth + brickSpacing)) / 2;
	int startY = 5;

	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			int x = startX + j * (brickWidth + brickSpacing);
			int y = startY + i * (brickHeight + brickSpacing);
			int health = numRows - i;
			std::unique_ptr<Brick> brick = std::make_unique<Brick>(x, y, brickWidth, brickHeight, health);
			bricks.push_back(std::move(brick));
		}
	}

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
		ball.manageCollisionWith(player, window);
		ball.manageCollisionWith(brick);
		window.clear();

		for (int i = 0; i < window.getSize().y; i++)
		{
			double mappedValue = mapValue(i, 0, window.getSize().y, 0, 255);
			rdr2.setFillColor(sf::Color(mappedValue, mappedValue, 190));
			rdr2.setPosition(0, i);
			window.draw(rdr2);
		}
		
		for (auto& brick : bricks)
		{
			brick->draw(window);
		}
		ball.draw(window);
		player.draw(window);
		window.display();
		window.clear(sf::Color::Black);
		}
	for (int i = 0; i < bricks.size(); i++)
	{
		ball.manageCollisionWith(*bricks[i]);
	}

	// ...

	// Dessiner les briques
	for (int i = 0; i < bricks.size(); i++)
	{
		bricks[i]->draw(window);
	}
	

	return 0;
}
