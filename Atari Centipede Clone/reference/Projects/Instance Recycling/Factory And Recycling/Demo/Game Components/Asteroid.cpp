// Asteroid
// // Andre Berthiaume, March 2013

#include "Asteroid.h"
#include "Explosion.h"
#include <math.h>

Asteroid::Asteroid( sf::Vector2f p ) : Pos(p)
{
	SPEED = 200.0f + rand() % 100;	// Randomize the speed


	bitmap = &ResourceManager::GetTextureBitmap("Asteroids");
	MainSprite = AnimatedSprite(  ResourceManager::GetTexture("Asteroids"), 8, 8, 30);

	int selector = rand() % 2; // The selector allows to change which series to use for animation (see texture file)
	MainSprite.SetAnimation(32 * selector, 32 * selector + 31); 
	
	MAXHEALTH = 1 + rand() % 5;	// between 1 and 5 inclusively;
	health = MAXHEALTH;

	scale = 1.0f + 2 * MAXHEALTH / 10.0f;
	MainSprite.scale( scale, scale );
	MainSprite.setOrigin(MainSprite.getTextureRect().width / 2.0f, MainSprite.getTextureRect().height / 2.0f);

	SetCollider(MainSprite, *bitmap);
	RegisterCollision<Asteroid>( *this );
}

void Asteroid::Update()
{
	MainSprite.Update();

	Pos.x -= Game::FrameTime() * SPEED;
	if (Pos.x < - MainSprite.getTextureRect().width ) 
		MarkForDestroy();  // kill when offscreen

	MainSprite.setPosition(Pos);
}

void Asteroid::Draw()
{
	WindowManager::MainWindow.draw(MainSprite);
}

void Asteroid::Collision( Bullet *other )
{
	health -= 1;

	MainSprite.setColor( sf::Color( 255, (int) ( (float) health/MAXHEALTH ) * 255, (int) ((float) health/MAXHEALTH ) * 255) );

	if (health <= 0)
		Explode();
}

void Asteroid::Explode()
{
	new Explosion(Pos, .7f * SPEED );
	MarkForDestroy();
}

void Asteroid::Destroy()
{
	DeregisterCollision<Asteroid>( *this );
}