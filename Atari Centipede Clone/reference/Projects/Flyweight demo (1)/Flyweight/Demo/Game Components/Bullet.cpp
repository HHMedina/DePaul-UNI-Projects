// Bullet
// Andre Berthiaume, Aug 2012

#include "Bullet.h"
	
Bullet::Bullet(sf::Vector2f p)
{
	SPEED = 800;

	bitmap = ResourceManager::GetTextureBitmap("Shots");
	MainSprite = AnimatedSprite(  ResourceManager::GetTexture("Shots"), 1, 4, 120);
	MainSprite.SetAnimation(0, 3);
	MainSprite.setOrigin(MainSprite.getTextureRect().width / 2.0f, MainSprite.getTextureRect().height / 2.0f);

	SetCollider(MainSprite, bitmap);

	Pos = p;
	RegisterCollision<Bullet>( *this );
}

void Bullet::Update()
{
	MainSprite.Update();

	Pos.x += Game::FrameTime() * SPEED;

	if (Pos.x > WindowManager::MainWindow.getSize().x ) MarkForDestroy();

	MainSprite.setPosition(Pos);
}

void Bullet::Draw()
{
	WindowManager::MainWindow.draw(MainSprite);
}

void Bullet::Destroy()
{
	DeregisterCollision<Bullet>( *this );
}

void Bullet::Collision( Asteroid *other )
{
	MarkForDestroy();
}