#include "Ball.h"

bool Ball::isRandInitialized = false;

double Ball::randomizeAngle()
{
	if (!isRandInitialized)
	{
		srand(time(NULL));
		isRandInitialized = true;

	}

	return (((double)rand() / (double)RAND_MAX)*60.0) - 30.0;
}

void Ball::setAngle(double angle)
{
	direction.x = sin(angle);
	direction.y = -cos(angle);
}

Ball::Ball(int x, int y, int radius, float speed)
{
	position.x = x;
	position.y = y;
	oldPosition = position;
	shape.setFillColor(sf::Color::White);
	this->radius = radius;
	shape.setRadius(radius);
	this->speed = speed;

	setAngle(randomizeAngle());
}

Ball::~Ball()
{
}

void Ball::move(float ellapsedTime)
{
	position += ellapsedTime * speed * direction;
}

void Ball::draw(sf::RenderWindow & window)
{
	shape.setPosition(position);
	window.draw(shape);
}

float Ball::getSpeed()
{
	 return speed;
}

void Ball::setSpeed(float newSpeed)
{
	speed = newSpeed;
}

void Ball::setPosition(sf::Vector2f newPosition)
{
	position = newPosition;
	oldPosition = position;
}

void Ball::setDirection(sf::Vector2f newDirection)
{
	direction = newDirection;
}

void Ball::manageCollisionWith(sf::RenderWindow& window)
{
	if (position.y <= 0 )
	{
		direction.y *= -1;
	}
	//partie basse a supprimer plus tard 
	if (position.y >= (window.getSize().y - 2 * radius))
	{
		direction.y *= -1;
	}
	
	if (position.x <= 0)
	{
		direction.x *= -1;
	}
	if (position.x >= (window.getSize().x - 2 * radius))
	{
		direction.x *= -1;
	}

}

void Ball::manageCollisionWith(Player & player)
{
	if (position.x + radius >= player.getPosition().x && position.x - radius <= player.getPosition().x + player.getSize().x && position.y + radius >= player.getPosition().y && position.y - radius <= player.getPosition().y + player.getSize().y)
	{
		double angle = ((position.x - player.getPosition().x) / player.getSize().x) * (5 * 30 / 12) - (5 * 30 / 12) / 2;
		setAngle(angle);
	}
}
