#ifndef MOB_H 
#define MOB_H
#include "Entity.h"
#include "../Utility//enums.h"
#include "Stats..h"
class Mob : public Entity
{
private:

	directions m_direction;
	Stats* stats;
	sf::RectangleShape entireHealthBar;
	sf::RectangleShape healthLeftBar;
public:
	Mob(sf::Texture* texture = nullptr, sf::Vector2f coords = sf::Vector2f(0,0));
	~Mob(); 
	sf::Vector2f attack(); 
	sf::Vector2f moveRequest(); 

	void move(int xDir, int yDir);

	Stats* getStats() const; 
	void setPos(sf::Vector2f coords); 

	virtual sf::Vector2f interact() = 0; 

	void updateHealthbar();

	directions getDirection(); 
	void setDirection(directions direction);
	int getDirX() const; 
	int getDirY() const;
	bool isDead() const; 
	void update(float dt);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif 

