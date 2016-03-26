// Explosion
// Andre Berthiaume, March 2013

#include "Explosion.h"

Explosion::Explosion( sf::Vector2f pos, float speed ) : Speed(speed), Pos(pos)
{
	MainSprite = AnimatedSprite(  ResourceManager::GetTexture("Explosion"), 5, 5, 30); 
	MainSprite.SetAnimation(0, 24); 
	MainSprite.scale(2,2);
	MainSprite.setColor(  sf::Color::Color(255,180,180) );
	MainSprite.setOrigin(MainSprite.getTextureRect().width / 2.0f, MainSprite.getTextureRect().height / 2.0f);
	MainSprite.setPosition(Pos);

	kaboom.setBuffer( ResourceManager::GetSound("Boom"));
	kaboom.setVolume(25);
	kaboom.setPitch(.5f);
	kaboom.play();
}

void Explosion::Update()
{
	if ( MainSprite.IsLastAnimationFrame() )
		MarkForDestroy();

	Pos.x -= Game::FrameTime() * Speed;
	MainSprite.setPosition(Pos);
	MainSprite.Update();
}

void Explosion::Draw()
{
	WindowManager::MainWindow.draw(MainSprite);
}