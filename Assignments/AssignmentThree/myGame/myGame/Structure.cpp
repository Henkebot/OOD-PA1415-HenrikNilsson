#include "Structure.h"

Structure::Structure(sf::Texture* texture, sf::Vector2f coords, int health) : NonMoveable(texture, coords, health)
{

}

Structure::~Structure()
{

}

void Structure::create()
{

}

std::string Structure::getInteractionEffect()
{
	return "InteractionEffect"; 
}

void Structure::update(float dt)
{

}

void Structure::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(this->getSprite(), states); 
}