#ifndef STRUCTURE_H 
#define STRUCTURE_H 
#include "NonMoveable.h"
class Structure : public NonMoveable
{
private:

public:
	Structure(sf::Texture* texture = nullptr, sf::Vector2f coords = sf::Vector2f(0, 0), int health = 0);
	~Structure(); 
	void create(); 
	std::string getInteractionEffect(); 
	void update(float dt); 
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif 
