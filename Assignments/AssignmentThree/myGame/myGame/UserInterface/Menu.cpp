#include "Menu.h"

Menu::Menu(GameStateManager * gsm) :
	State(gsm)
{
	std::cout << "Menu state pushed!\n";
	
	this->initiateVars();
}


Menu::Menu(const Menu & other):
	State(other)
{

}

Menu::~Menu()
{

}

void Menu::update(float dt)
{
	this->handleEvents();

	//Stuff goind down	


}

void Menu::handleEvents()
{
	if (InputManager::keyPressed(sf::Keyboard::Return))
	{
		this->gsm->pushState(new Game(this->gsm));
	}
}

Menu & Menu::operator=(const Menu & other)
{
	if (this != &other)
	{
		State::operator=(other);
		//more stuff
	}
	
	return *this;

}

void Menu::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(this->wallpaper);
	target.draw(this->endGame);
	target.draw(this->nethack);
	target.draw(this->startGame);
}

void Menu::initiateVars()
{
	if (!this->texture.loadFromFile("textures/menu_wallpaper.jpg"))
	{
		std::cout << "Failed to load menu texture!" << std::endl;
	}
	this->wallpaper.setTexture(this->texture);
	this->wallpaper.setPosition(0.0f, 0.0f);

	if (!this->aFont.loadFromFile("font/D3Stonism.ttf"))
	{
		std::cout << "Failed to load fonts" << std::endl;
	}

	this->startGame.setFont(aFont);
	this->endGame.setFont(aFont);
	this->nethack.setFont(aFont);

	this->startGame.setString("Press Enter To Start Game");
	this->endGame.setString("Press ESC To Quit Game");
	this->nethack.setString("TwitterNethack");

	this->startGame.setCharacterSize(32);
	this->endGame.setCharacterSize(16);
	this->nethack.setCharacterSize(128);

	this->startGame.setPosition(sf::Vector2f(Application::SCREEN_WIDTH / 2 - (15 * 32)/2, Application::SCREEN_HEIGHT / 2 + 120));
	this->endGame.setPosition(sf::Vector2f(Application::SCREEN_WIDTH / 2 - (15 * 16)/2, Application::SCREEN_HEIGHT / 2 + 250));
	this->nethack.setPosition(sf::Vector2f(Application::SCREEN_WIDTH / 2 - (17 * 64)/2, Application::SCREEN_HEIGHT / 2 - 150));
}
