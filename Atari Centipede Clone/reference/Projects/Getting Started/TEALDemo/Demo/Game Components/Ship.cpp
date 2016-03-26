// Ship
// Andre Berthiaume, Aug 2012

#include "Ship.h"
#include "Bullet.h"
#include "GameController.h"
#include "ShipExplosion.h"
#include "Widget.h"
#include "Asteroid.h"

Ship::Ship()
{
	SPEED = 20;
	HBORDER = 15;
	TBORDER = 600;

	GunOffset = sf::Vector2f(10,20);
	
	bitmap = ResourceManager::GetTextureBitmap("Spaceship");
	MainSprite = AnimatedSprite(  ResourceManager::GetTexture("Spaceship"), 1, 4);
	MainSprite.SetAnimation(0,3); 

	MainSprite.setOrigin( MainSprite.getTextureRect().width / 2.0f, MainSprite.getTextureRect().height / 2.0f);
	MainSprite.setScale(2,2);
	MainSprite.setPosition(Pos);
	
	Pos = sf::Vector2f(2.0f * MainSprite.getTextureRect().width ,  WindowManager::MainWindow.getView().getSize().y / 2.0f);
	Impulse = sf::Vector2f(0,0);
	friction = .97f;

	SetCollider(MainSprite, bitmap, true);
	RegisterCollision<Ship>(*this);

	FireSnd.setBuffer( ResourceManager::GetSound("Zap")  );
	FireSnd.setPitch(2);
	FireSnd.setVolume(25);
	
	SetDrawOrder(1000);

	RegisterInput( InputFlags::KeyPressed ); // Recieve single-presses events
}

void Ship::Destroy()
{
	DeregisterInput();
	DeregisterCollision<Ship>(*this);
}

void Ship::Update()
{

	MainSprite.Update();

	offset = SPEED * Game::FrameTime();
	// Continuous key-down tests
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::A)) Impulse += sf::Vector2f(-offset, 0);
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::D)) Impulse += sf::Vector2f( offset, 0);
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::W)) Impulse += sf::Vector2f(0, -offset);
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::S)) Impulse += sf::Vector2f(0, offset);

	Pos += Impulse;
	Tools::Clamp<float>(Pos.x, (float) 2*MainSprite.getTextureRect().width, WindowManager::MainWindow.getView().getSize().x );
	Tools::Clamp<float>(Pos.y, 100, WindowManager::MainWindow.getView().getSize().y - MainSprite.getTextureRect().height);	
	MainSprite.setPosition(Pos);

	Impulse *= friction;
}

void Ship::KeyPressed(sf::Keyboard::Key k, bool altKey, bool ctrlKey, bool shiftKey, bool systemKey)
{


	if ( k == sf::Keyboard::Return )
	{
		Bullet* bullet;

		// bottom bullet
		bullet = new Bullet(Pos + GunOffset); 
		GunOffset.y *= -1;

		// top bullet
		bullet = new Bullet(Pos + GunOffset); 
		GunOffset.y *= -1;

		FireSnd.play();
	}
}

void Ship::Collision(Widget *other)
{
	GameController::Instance().AddScore( other->GetValue() );
	other->MarkForDestroy();
}

void Ship::Collision(Asteroid *other)
{
	other->Explode();
	new ShipExplosion(Pos);
	MarkForDestroy();
}

void Ship::Draw()
{
	WindowManager::MainWindow.draw(MainSprite);
}

