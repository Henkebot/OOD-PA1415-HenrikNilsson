#include "Player.h"
#include <iostream>
Player::Player(sf::Texture* texture, sf::Vector2f coords, int health) : Mob(texture,coords,health)
{
	direction = up;
}

Player::~Player()
{

}

void Player::setXP(long xp)
{
	this->xp = xp; 
}

long Player::getXP() const
{
	return this->xp;
}

void Player::create()
{

}

void Player::update(float dt)
{
	
}

sf::Vector2f Player::interact()
{
	return sf::Vector2f(0, 0); 
}

void Player::pickUpItem(Item item)
{

}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(this->getSprite(), states); 
}

void Player::setDirection(directions newDirection)
{
	direction = newDirection;
}

directions Player::getDirection() const
{
	return direction;
}

sf::Vector2f Player::moveRequest()
{
	sf::Vector2f request = getCoords();
	if (direction == up)
	{
		request.y--;
	}
	else if(direction == left)
	{
		request.x--;
	}
	else if (direction == down)
	{
		request.y++;
	}
	else if (direction == right)
	{
		request.x++;
	}
	return request;
}

void Player::copy(Player player)
{
	this->xp = player.xp;
	this->inventory = player.inventory;
	this->direction = player.direction;
	this->setHealth(player.getHealth());
	this->setCoords(player.getCoords());
	this->setSprite(player.getSprite());
}