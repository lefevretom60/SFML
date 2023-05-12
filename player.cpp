#include "player.h"



Player::Player(int y, int width, int height){
	this->y = y;
	position = sf::Vector2f(0, y);
	size = sf::Vector2f(width, height);
	shape.setSize(size);
	shape.setFillColor(sf::Color::White);
}

Player::~Player()
{
}

void Player::draw(sf::RenderWindow& window)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		position.x = sf::Mouse::getPosition(window).x - size.x / 2;

		if (position.x < 0)
		{
			position.x = 0;
		}
		if (position.x + size.x > window.getSize().x)
		{
			position.x = window.getSize().x - size.x;
		}
	}

	shape.setPosition(position);

	window.draw(shape);
}

sf::Vector2f Player::getPosition()
{
	return position;
}

sf::Vector2f Player::getSize()
{
	return size;
}

