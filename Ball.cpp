#include "Ball.h"
#include <cmath>
#include <cstdlib>

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
	if (position.y <= 0)
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

void Ball::manageCollisionWith(Player& player, sf::RenderWindow& window)
{
	float MAX_ANGLE = 45;
	const int MARGE = 2;

	if (position.y + 2 * radius >= player.getPosition().y &&
		position.y + 2 * radius <= player.getPosition().y + player.getSize().y &&
		position.x + 2 * radius >= player.getPosition().x &&
		position.x <= player.getPosition().x + player.getSize().x)
	{
		double relativeIntersectX = position.x + radius - player.getPosition().x - player.getSize().x / 2.0;

		double normalizedRelativeIntersectionX = relativeIntersectX / (player.getSize().x / 2.0);

		double angle = normalizedRelativeIntersectionX * MAX_ANGLE;

		setAngle(angle);

		position.y = player.getPosition().y - 2 * radius - 0.1f;
		direction.y = -std::abs(direction.y);

		double randomAngle = (((double)rand() / (double)RAND_MAX) * 30.0) - 15.0;
		double newAngle = angle + randomAngle;
		if (newAngle > MAX_ANGLE) {
			newAngle = MAX_ANGLE;
		}
		if (newAngle < -MAX_ANGLE) {
			newAngle = -MAX_ANGLE;
		}
		setAngle(newAngle);
	}
	if (position.y <= 0){
		position.y = 0;
		direction.y = std::abs(direction.y);
	}
 
	if (position.y + 2 * radius >= window.getSize().y){
		position.y = window.getSize().y - 2 * radius;
		direction.y = -std::abs(direction.y);
	}

	if (position.x <= 0){
		position.x = 0;
		direction.x = std::abs(direction.x);
	}

	if (position.x + 2 * radius >= window.getSize().x){
		position.x = window.getSize().x - 2 * radius;
		direction.x = -std::abs(direction.x);
	}
}
/*
void Ball::manageCollisionWith(Brick& brick)
{
	sf::FloatRect ballBounds = shape.getGlobalBounds();
	sf::FloatRect brickBounds = brick.getShape().getGlobalBounds();

	// Augmenter légèrement la taille des rectangles de collision
	float expandedBallLeft = ballBounds.left - 1.0f;
	float expandedBallRight = ballBounds.left + ballBounds.width + 1.0f;
	float expandedBallTop = ballBounds.top - 1.0f;
	float expandedBallBottom = ballBounds.top + ballBounds.height + 1.0f;

	float expandedBrickLeft = brickBounds.left - 1.0f;
	float expandedBrickRight = brickBounds.left + brickBounds.width + 1.0f;
	float expandedBrickTop = brickBounds.top - 1.0f;
	float expandedBrickBottom = brickBounds.top + brickBounds.height + 1.0f;

	// Vérifier les collisions sur les côtés
	if (expandedBallRight >= expandedBrickLeft && expandedBallLeft <= expandedBrickRight &&
		expandedBallTop <= expandedBrickBottom && expandedBallBottom >= expandedBrickTop)
	{
		// Collision détectée, gérer la collision avec la brique
		brick.hit();

		// Inverser la direction de la balle selon le côté de la collision
		if (expandedBallRight >= expandedBrickLeft && expandedBallLeft <= expandedBrickLeft)
		{
			// Collision côté gauche
			direction.x = -direction.x;
		}
		else if (expandedBallLeft <= expandedBrickRight && expandedBallRight >= expandedBrickRight)
		{
			// Collision côté droit
			direction.x = -direction.x;
		}
		else if (expandedBallBottom >= expandedBrickTop && expandedBallTop <= expandedBrickTop)
		{
			// Collision côté haut
			direction.y = -direction.y;
		}
		else if (expandedBallTop <= expandedBrickBottom && expandedBallBottom >= expandedBrickBottom)
		{
			// Collision côté bas
			direction.y = -direction.y;
		}
	}
}*/
void Ball::manageCollisionWith(Brick& brick)
{
	static bool hasCollided = false; // Variable pour suivre l'état de la collision précédente
	static const sf::Time collisionDelay = sf::seconds(0.1f); // Délai d'une seconde
	static sf::Clock collisionClock; // Horloge pour mesurer le temps écoulé depuis la dernière collision

	if (hasCollided && collisionClock.getElapsedTime() < collisionDelay)
	{
		// Attendre le délai entre les collisions
		return;
	}

	sf::FloatRect ballBounds = shape.getGlobalBounds();
	sf::FloatRect brickBounds = brick.getShape().getGlobalBounds();

	if (ballBounds.intersects(brickBounds))
	{
		if (brick.getColorFromLife() == sf::Color::Green)
		{
			// Brique verte, ne pas gérer la collision
			return;
		}

		brick.hit();

		float overlapX = std::min(ballBounds.left + ballBounds.width, brickBounds.left + brickBounds.width) - std::max(ballBounds.left, brickBounds.left);
		float overlapY = std::min(ballBounds.top + ballBounds.height, brickBounds.top + brickBounds.height) - std::max(ballBounds.top, brickBounds.top);

		if (overlapX > overlapY)
		{
			// Collision depuis le haut ou le bas
			direction.y = -direction.y;
		}
		else
		{
			// Collision depuis la gauche ou la droite
			direction.x = -direction.x;
		}

		hasCollided = true;
		collisionClock.restart(); // Redémarrer l'horloge après la collision
	}
	else
	{
		hasCollided = false;
	}
}