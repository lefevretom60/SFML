#include "brick.h"

Brick::Brick(int x, int y, int width, int height, int health)
{
	position.x = x;
	position.y = y;
	size.x = width;
	size.y = height;
	this->health = health;
	
	shape.setSize(size);
	shape.setPosition(position);
	shape.setFillColor(getColorFromLife());
}

Brick::~Brick() {}

void Brick::draw(sf::RenderWindow& window)
{
	if (isAlive())
	{
		window.draw(shape);
	}
}

sf::Color Brick::getColorFromLife()
{
	if (health > 10)
	{
		return sf::Color::Green;
	}
	else if (health == 1)
	{
		return sf::Color::Yellow;
	}
	else
	{
		return sf::Color::Red;
	}
}

bool Brick::isAlive()
{
	return health > 0;
}

void Brick::hit()
{
	health--;
	shape.setFillColor(getColorFromLife());

	if (!isAlive())
	{
		destroy();
	}
}
void Brick::destroy()
{
	position.x = -100;
	position.y = -100;
	shape.setPosition(position);
}
sf::Vector2f Brick::getPosition()
{
	return position;
}

sf::Vector2f Brick::getSize()
{
	return size;
}
sf::RectangleShape Brick::getShape()
{
	return shape;
}