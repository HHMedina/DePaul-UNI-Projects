// SplashController
// Andre Berthiaume, March 2013

#include "SplashController.h"
#include "Level1.h"

SplashController::SplashController()
{
	SplashSprite = sf::Sprite(ResourceManager::GetTexture("Splashpage"));

	RegisterInput( InputFlags::KeyPressed );
}

SplashController::~SplashController()
{
	DeregisterInput(); 
}

void SplashController::Draw()
{
	WindowManager::MainWindow.draw(SplashSprite);
}

void SplashController::KeyPressed(sf::Keyboard::Key k, bool altKey, bool ctrlKey, bool shiftKey, bool systemKey)
{
	SceneManager::ChangeScene( new Level1 );
}