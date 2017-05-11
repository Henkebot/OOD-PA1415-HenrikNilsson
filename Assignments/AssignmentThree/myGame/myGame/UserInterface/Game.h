#ifndef GAME_H
#define GAME_H
#include "State.h"
#include "Menu.h"
#include "../Container\Map.h"
#include <iostream>

class Game : public State
{
public:
	Game(GameStateManager* gsm = nullptr);
	Game(const Game& other);
	virtual ~Game();

	void update(float dt);
	void handleEvents();

	Game& operator=(const Game& other);
private:
	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
private:
	Container::Map map;
	sf::RectangleShape shape;
};
#endif // !GAME_H