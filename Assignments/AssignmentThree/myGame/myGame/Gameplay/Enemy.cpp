#include "Enemy.h"
#include <random>
#include <cmath>
#include "../Utility/Values.h"
#include <iostream>

Enemy::Enemy(sf::Texture* texture, sf::Vector2f coords)  : Mob(texture,coords)
{
	agro = 4;
	stupidity = 2;
	state = states::idle;
	getStats()->setStrength(2.0f);
}

Enemy::~Enemy()
{

}

void Enemy::create()
{

}

void Enemy::checkState(sf::Vector2f playerCoords)
{
	float distance = sqrt(pow((playerCoords.x - getCoords().x),2) + (pow((playerCoords.y - getCoords().y),2)));
	float tile = Val::SPRITE_SIZE;
	if (distance <= tile)
	{
		state = states::attack;
		if (playerCoords.x < getCoords().x)
		{
			this->setDirection(directions::left);
			this->setCurrentSpriteFrame(0, 3);
		}
		else if (playerCoords.x > getCoords().x)
		{
			this->setDirection(directions::right);
			this->setCurrentSpriteFrame(0, 1);
		}
		else if (playerCoords.y < getCoords().y)
		{
			this->setDirection(directions::up);
			this->setCurrentSpriteFrame(0, 0);
		}
		else if (playerCoords.y > getCoords().y)
		{
			this->setDirection(directions::down);
			this->setCurrentSpriteFrame(0, 2);
		}

	}
	else if (distance <= tile * agro)
	{
		state = states::move;
		int rng = rand() % 10;
		if (rng > stupidity)
		{
			rng = rand() % 2;
			if (rng = 0)
			{
				if (playerCoords.x < getCoords().x)
				{
					this->setDirection(directions::left);
					this->setCurrentSpriteFrame(0, 3);
				}
				else if (playerCoords.x > getCoords().x)
				{
					this->setDirection(directions::right);
					this->setCurrentSpriteFrame(0, 1);
				}
				else if (playerCoords.y < getCoords().y)
				{
					this->setDirection(directions::up);
					this->setCurrentSpriteFrame(0, 0);
				}
				else if (playerCoords.y > getCoords().y)
				{
					this->setDirection(directions::down);
					this->setCurrentSpriteFrame(0, 2);
				}
			}
			else
			{
				if (playerCoords.y < getCoords().y)
				{
					this->setDirection(directions::up);
					this->setCurrentSpriteFrame(0, 0);
				}
				else if (playerCoords.y > getCoords().y)
				{
					this->setDirection(directions::down);
					this->setCurrentSpriteFrame(0, 2);
				}
				else if (playerCoords.x < getCoords().x)
				{
					this->setDirection(directions::left);
					this->setCurrentSpriteFrame(0, 3);
				}
				else if (playerCoords.x > getCoords().x)
				{
					this->setDirection(directions::right);
					this->setCurrentSpriteFrame(0, 1);
				}
			}
		}
		else
		{
			state = states::idle;
			rng = (rand() % 15) + 1;

			if (rng < 3)
			{
				rng = (rand() % 4) + 1;
				if (rng == 1)
				{
					this->setDirection(directions::up);
					this->setCurrentSpriteFrame(0, 0);
				}
				else if (rng == 2)
				{
					this->setDirection(directions::right);
					this->setCurrentSpriteFrame(0, 1);
				}
				else if (rng == 3)
				{
					this->setDirection(directions::down);
					this->setCurrentSpriteFrame(0, 2);
				}
				else
				{
					this->setDirection(directions::left);
					this->setCurrentSpriteFrame(0, 3);
				}
			}
		}
	}
	else
	{
		state = states::idle;
		int rng = (rand() % 15) + 1;

		if (rng < 3)
		{
			rng = (rand() % 4) + 1;
			if (rng == 1)
			{
				this->setDirection(directions::up);
				this->setCurrentSpriteFrame(0, 0);
			}
			else if (rng == 2)
			{
				this->setDirection(directions::right);
				this->setCurrentSpriteFrame(0, 1);
			}
			else if (rng == 3)
			{
				this->setDirection(directions::down);
				this->setCurrentSpriteFrame(0, 2);
			}
			else
			{
				this->setDirection(directions::left);
				this->setCurrentSpriteFrame(0, 3);
			}
		}
	}
		
	
}

void Enemy::update(float dt)
{ 
}

sf::Vector2f Enemy::interact()
{
	return sf::Vector2f();
}

states Enemy::getState() const
{
	return state;
}

void Enemy::setState(states newState)
{
	state = newState;
}
