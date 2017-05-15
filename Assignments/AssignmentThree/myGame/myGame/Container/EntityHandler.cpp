#include "EntityHandler.h"
using namespace Container;
#include "../Utility/Values.h"
#include <algorithm>
EntityHandler::EntityHandler()
{
	player = nullptr;
	extraCon();
	initLines();
	
}

EntityHandler::EntityHandler(Player* player)
{
	this->player = player;
	extraCon();
}
EntityHandler::EntityHandler(Player* player, Vector2f playerCoords)
{
	this->player = player;
	player->setCoords(playerCoords);
	extraCon();
}

void EntityHandler::extraCon()
{
	playersTurn = true;
	nrOfItems = 0;
	nrOfStructures = 0;
	inputTimer.restart().asSeconds();
	timeCollector = 0;

	statusTexture.loadFromFile(".\\textures\\hud.png", sf::IntRect(960, 293, 320, 427));
	statusSprite.setTexture(statusTexture);
	statusSprite.setPosition(960, 293);

	if (statusFont.loadFromFile(".\\font\\D3Stonism.ttf"))
		std::cout << "statusFont couldnt load from file! " << std::endl;
	
	armorText.setFont(statusFont);
	armorText.setString("10");
	armorText.setCharacterSize(20);
	armorText.setPosition(1005, 314);

	attackText.setFont(statusFont);
	attackText.setString("10");
	attackText.setCharacterSize(20);
	attackText.setPosition(1115, 314);

	healthText.setFont(statusFont);
	healthText.setString("10");
	healthText.setCharacterSize(20);
	healthText.setPosition(1212, 314);
	


}

int Container::EntityHandler::calculateDmg(Stats attackerStats, Stats defenderStats)
{
	int dmg = attackerStats.getAttack();
	float defenceModifier = 1 - defenderStats.getDefence();
	dmg *= defenceModifier;
	return dmg;
}

EntityHandler::~EntityHandler()
{
	for (int i = 0; i < nrOfEnemies; i++)
	{
		delete enemys[i];
	}
	delete enemys;

	for (int i = 0; i < nrOfStructures; i++)
	{
		delete structures[i];
	}
	delete structures;


	for (int i = 1; i < Val::ROOM_HEIGHT - 1; i++)
	{
		for (int j = 1; j < Val::ROOM_WIDTH - 1; j++)
		{
			delete floor[i][j];
		}
	}
}

void EntityHandler::update(float dt)
{
	player->update(dt);
	if (playersTurn == true)
	{
		handleInput(dt);
	}
	else
	{
		Vector2f playerCoords = player->getCoords();
		for (int i = 0; i < nrOfEnemies; i++)
		{
			enemys[i]->checkState(playerCoords);
			enemys[i]->update(dt);
			if (enemys[i]->getState() == attack)
			{
				std::cout << "attack" << std::endl;
				Stats enemyStat = *enemys[i]->getStats();
				Stats playerStat = *player->getStats();

				int dmg = calculateDmg(enemyStat, playerStat);

				player->getStats()->takeDMG(dmg);
			}
			else if (enemys[i]->getState() == move)
			{
				std::cout << "move" << std::endl;
				EnemyMove(i);
			}
			else
			{
				std::cout << "idle" << std::endl;
				int rng = rand() % 10;
				if (rng < 4)
				{
					EnemyMove(i);
				}
			}
		}
		playersTurn = true;
	}
	remoceDead();

	for (int i = 0; i < nrOfEnemies; i++)
	{
		enemys[i]->updateHealthbar();
	}
	player->updateHealthbar();
	updateStatusText();
}

void Container::EntityHandler::render(sf::RenderTarget & target) const
{
	target.draw(statusSprite);
	for (int y = 1; y < Val::ROOM_HEIGHT - 1; y++)
	{
		for (int x = 1; x < Val::ROOM_WIDTH - 1; x++)
		{
			target.draw(*floor[y][x]);
		}
	}
	
	if (structureProperty == 1)
	{
		target.draw(*structures[0]);

	}
	if (structureProperty == 2)
	{
		target.draw(*structures[0]);
	}
	target.draw(*player);
	for (int i = 0; i < Val::ROOM_WIDTH; i++)
		target.draw(lineX[i], 2, sf::Lines);	
	for (int i = 0; i < Val::ROOM_HEIGHT; i++)
		target.draw(lineY[i], 2, sf::Lines);

	for (int i = 0; i < nrOfEnemies; i++)
	{
		target.draw(*enemys[i]);	
	}

	if (structureProperty == 1)
		target.draw(*structures[1]);

	target.draw(armorText);
	target.draw(attackText);
	target.draw(healthText);
}

Player * Container::EntityHandler::getPlayer() const
{
	return player;
}

void Container::EntityHandler::setPlayer(Player* player)
{
	this->player = player;
}

void Container::EntityHandler::destroyPlayer()
{
	delete player;
}

void Container::EntityHandler::setDoors(bool * doors)
{
	for (int i = 0; i < 4; i++)
	{
		doorStatus[i] = doors[i];
	}
}

void Container::EntityHandler::setStructureProperty(int role, int spriteSheet)
{
	structureProperty = role;
	floorSheet = spriteSheet;
}

void Container::EntityHandler::createEntities(Identifier * inRoom, int size)
{
	MasterSpawner spawner(inRoom, size);
	nrOfEnemies = spawner.spawnEnemies(enemys);

	sf::Texture* tiles = new sf::Texture();
	if(floorSheet == 0)
		tiles->loadFromFile("./textures/sheet.png", sf::IntRect(Val::SPRITE_SIZE * 1, Val::SPRITE_SIZE * 1, Val::SPRITE_SIZE, Val::SPRITE_SIZE));
	else if(floorSheet == 1)
		tiles->loadFromFile("./textures/sheet2.png", sf::IntRect(Val::SPRITE_SIZE * 1, Val::SPRITE_SIZE * 1, Val::SPRITE_SIZE, Val::SPRITE_SIZE));
	else 
		tiles->loadFromFile("./textures/sheet3.png", sf::IntRect(Val::SPRITE_SIZE * 1, Val::SPRITE_SIZE * 1, Val::SPRITE_SIZE, Val::SPRITE_SIZE));

	for (int y = 1; y < Val::ROOM_HEIGHT - 1; y++)
	{
		for (int x = 1; x < Val::ROOM_WIDTH - 1; x++)
		{
			floor[y][x] = new Floor(tiles, sf::Vector2f(x * Val::FINAL_SIZE, y * Val::FINAL_SIZE));
		}
	}

	if (structureProperty == 1)
	{
		std::cout << "k�res!!!!!!!!";
		Texture* textureLadderLower = new sf::Texture();
		Texture* textureLadderUpper = new sf::Texture();
		textureLadderLower->loadFromFile("./textures/sheet.png", sf::IntRect(3 * Val::SPRITE_SIZE, 2 * Val::SPRITE_SIZE,
			Val::SPRITE_SIZE, Val::SPRITE_SIZE));
		textureLadderUpper->loadFromFile("./textures/sheet.png", sf::IntRect(3 * Val::SPRITE_SIZE, 1 * Val::SPRITE_SIZE,
			Val::SPRITE_SIZE, Val::SPRITE_SIZE));

		this->structures = new Structure*[2];
		structures[1] = new Structure(textureLadderUpper, sf::Vector2f(5*Val::FINAL_SIZE, 5*Val::FINAL_SIZE));
		structures[0] = new Structure(textureLadderLower, sf::Vector2f(5 * Val::FINAL_SIZE, 6 * Val::FINAL_SIZE));
		nrOfStructures = 1;
		
		
	}
	else if (structureProperty == 2)
	{
		Texture* textureHole = new sf::Texture();
		textureHole->loadFromFile("./textures/sheet.png", sf::IntRect(3 * Val::SPRITE_SIZE, 3 * Val::SPRITE_SIZE,
			Val::SPRITE_SIZE, Val::SPRITE_SIZE));
		this->structures = new Structure*[1];
		structures[0] = new Structure(textureHole, sf::Vector2f(5 * Val::FINAL_SIZE, 5 * Val::FINAL_SIZE));
		nrOfStructures = 1;
	}
	// L�gga till start och slut skiten n�r victor �r klar


}


void EntityHandler::handleInput(float dt)
{
	timeCollector += dt;
	if (InputManager::keyPressed(sf::Keyboard::Q))
	{
		playerAttack();
		playersTurn = false;
	}
	else if (InputManager::keyPressed(sf::Keyboard::E))
	{
		playerInteract();
		playersTurn = false;
	}
	else if (timeCollector > 150 * dt)
	{
		
		if (InputManager::keyPress(sf::Keyboard::W) || InputManager::keyPress(sf::Keyboard::Up))
		{
			playerTurnUp();
			timeCollector = 0;
		}
		else if (InputManager::keyPress(sf::Keyboard::A) || InputManager::keyPress(sf::Keyboard::Left))
		{
			playerTurnLeft();
			timeCollector = 0;
		}
		else if (InputManager::keyPress(sf::Keyboard::S) || InputManager::keyPress(sf::Keyboard::Down))
		{
			playerTurnDown();
			timeCollector = 0;
		}
		else if (InputManager::keyPress(sf::Keyboard::D) || InputManager::keyPress(sf::Keyboard::Right))
		{
			playerTurnRight();
			timeCollector = 0;			
		}

	}
}

void EntityHandler::playerAttack()
{
	Vector2f attackCoord = player->attack();
	bool col = false;
	for (int i = 0; i < nrOfEnemies && col == false; i++)
	{
		Vector2f enemyCoord = enemys[i]->getCoords();

		col = isCol(attackCoord, enemyCoord);
		if (col == true)
		{
			Stats enemyStat = *enemys[i]->getStats();
			Stats playerStat = *player->getStats();

			float dmg = calculateDmg(playerStat, enemyStat);

			
			enemys[i]->getStats()->takeDMG(dmg);
		}
	}
	for (int i = 0; i < nrOfStructures && col == false; i++)
	{
		Vector2f structureCoord = structures[i]->getCoords();

		//col = isCol(attackCoord, enemyCoord);
		/*if (col == true)
		{
			Stats structureStat = structures[i]->getStats();
			Stats playerStat = player->getStats();

			float dmg = calculateDmg(playerStat, enemyStat);

			Vector2f enemyCoord = structures[i]->getCoords();
			structures[i]->setHealth(player->getHealth() - dmg);
		}*/
	}
}

void EntityHandler::playerInteract()
{
	Vector2f interactCoord = player->interact();
	bool col = false;
	for (int i = 0; i < nrOfStructures && col == false; i++)
	{
		Vector2f structureCoord = structures[i]->getCoords();

		//col =isCol(attackCoord, enemyCoord);
		if (col == true)
		{
		}
	}
}

bool EntityHandler::playerMove()
{
	playersTurn = false;
	Vector2f requestedCoords = player->moveRequest();
	bool col = isFloor(requestedCoords);
	for (int i = 0; i < nrOfEnemies && col == false; i++)
	{
		Vector2f otherCoords = enemys[i]->getCoords();
		col = isCol(requestedCoords, otherCoords);
	}
	for (int i = 0; i < nrOfStructures && col == false; i++)
	{
		Vector2f otherCoords = structures[i]->getCoords();
		if (requestedCoords == otherCoords)
		{
			col = true;
		}
	}
	for (int i = 0; i < nrOfItems; i++)
	{
		Vector2f otherCoords = items[i]->getCoords();
		col = isCol(requestedCoords, otherCoords);
		player->pickUpItem(*items[i]);
		removeItem(i);
	}
	
	float reqIndexX = requestedCoords.x / (Val::FINAL_SIZE);
	float reqIndexY = requestedCoords.y / (Val::FINAL_SIZE);

	// D�RRAR 
	// V�NSTER
	if (reqIndexX == 0 && reqIndexY == 5 && doorStatus[3])
		col = false;
	else if (reqIndexX == 10 && reqIndexY == 0 && doorStatus[0]) // UP
		col = false;
	else if (reqIndexX == 19 && reqIndexY == 5 && doorStatus[1]) // H�GER
		col = false;
	else if (reqIndexX == 10 && reqIndexY == 10 && doorStatus[2])
		col = false;
	return !col;
}

void EntityHandler::playerTurnUp()
{
	if (player->getDirection() == up)
	{
		if (playerMove())
		{
			player->move(0, -1);
		}
		
	}
	else
	{
		player->setDirection(up);
		player->setCurrentSpriteFrame(0, 0);
	}
}

void EntityHandler::playerTurnLeft()
{
	if (player->getDirection() == left)
	{
		if (playerMove())
		{
			player->move(-1, 0);
		}
	}
	else
	{
		player->setDirection(left);
		player->setCurrentSpriteFrame(0, 3);
	}
}

void EntityHandler::playerTurnDown()
{
	if (player->getDirection() == down)
	{
		if (playerMove())
		{
			player->move(0, 1);
		}
	}
	else
	{
		player->setCurrentSpriteFrame(0, 2);
		player->setDirection(down);
	}
}

void EntityHandler::playerTurnRight()
{
	if (player->getDirection() == right)
	{
		if (playerMove())
		{
			player->move(1, 0);
		}
	}
	else
	{
		player->setCurrentSpriteFrame(2, 1);
		player->setDirection(right);
	}
}

void Container::EntityHandler::EnemyMove(int index)
{
	Vector2f requestedCoords = enemys[index]->moveRequest();
	bool col = isFloor(requestedCoords);
	if (col == false)
	{
		col = isCol(requestedCoords, player->getCoords());
	}
	for (int i = 0; i < nrOfEnemies && col == false; i++)
	{
		Vector2f otherCoords = enemys[i]->getCoords();
		col = isCol(requestedCoords, otherCoords);
	}
	for (int i = 0; i < nrOfStructures && col == false; i++)
	{
		Vector2f otherCoords = structures[i]->getCoords();
		col = isCol(requestedCoords, otherCoords);
	}
	for (int i = 0; i < nrOfItems; i++)
	{
		Vector2f otherCoords = items[i]->getCoords();
		col = isCol(requestedCoords, otherCoords);
		player->pickUpItem(*items[i]);
		removeItem(i);
	}

	/*	int width = (20 - 1) * (64 * 0.75f);
		int height = (11 - 1) * (64 * 0.75f);
		if (requestedCoords.x == width ||
			requestedCoords.x == 0 ||
			requestedCoords.y == height ||
			requestedCoords.y == 0)
			col = true;*/
	if (!col)
	{
		this->enemys[index]->move(enemys[index]->getDirX(), enemys[index]->getDirY());
	}
}

bool EntityHandler::isFloor(Vector2f coords)
{

	bool isFloor = true;
	int x = coords.x / Val::FINAL_SIZE;
	int y = coords.y / Val::FINAL_SIZE;
	if (floor[y][x] != nullptr)
	{
		isFloor = false;
	}
	return isFloor;
}

bool Container::EntityHandler::isCol(Vector2f firstCoord, Vector2f SecondCoord)
{
	bool col = false;
	if (firstCoord == SecondCoord)
	{
		col = true;
	}
	return col;
}

void Container::EntityHandler::removeItem(int index)
{
}

void Container::EntityHandler::remoceDead()
{
	for (int i = 0; i < nrOfEnemies; i++)
	{
		if (enemys[i]->isDead() == true)
		{
			delete enemys[i];
			nrOfEnemies--;
			for (int j = i; j < nrOfEnemies; j++)
			{
				enemys[j] = enemys[j + 1];
			}
		}
	}
}

void Container::EntityHandler::updateStatusText()
{
	
	std::string armor = std::to_string(player->getStats()->getDefence());
	armor.erase(armor.find_first_not_of('0') + 2, std::string::npos);
	std::string attack = std::to_string(player->getStats()->getAttack());
	std::string health = std::to_string(player->getStats()->getHealth());
	health.erase(armor.find_first_not_of('0') + 2, std::string::npos);
	armorText.setString(armor);
	attackText.setString(attack);
	healthText.setString(health);
}

void Container::EntityHandler::initLines()
{
	for (int i = 0; i < Val::ROOM_WIDTH; i++)
	{
		lineX[i][0].position = sf::Vector2f(i * (Val::SPRITE_SIZE * Val::SCALE), 0);
		lineX[i][0].color = sf::Color::Red;
		lineX[i][1].position = sf::Vector2f(i * (Val::SPRITE_SIZE * Val::SCALE), (Val::SPRITE_SIZE * Val::SCALE) * 11);
		lineX[i][1].color = sf::Color::Red;
	}
	for (int i = 0; i < Val::ROOM_WIDTH; i++)
	{
		lineY[i][0].position = sf::Vector2f(0, i * (Val::SPRITE_SIZE * Val::SCALE));
		lineY[i][0].color = sf::Color::Red;
		lineY[i][1].position = sf::Vector2f(20 * (Val::SPRITE_SIZE * Val::SCALE), i* (Val::SPRITE_SIZE * Val::SCALE));
		lineY[i][1].color = sf::Color::Red;
	}
}
