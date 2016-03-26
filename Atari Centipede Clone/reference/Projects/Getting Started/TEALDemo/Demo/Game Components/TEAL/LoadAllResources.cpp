// LoadAllResources.cpp
// Andre Berthiaume, June 2012
//
// The user fills the method with all resources that need loading

#include "ResourceManager.h"
#include "../Splash.h"

void ResourceManager::LoadAllResources()
{
	SetStartScene( new Splash() );

	AddTexture("Spaceship", "SpaceShip2.png");
	AddTexture("Shots", "ColoredShots.png");
	AddTexture("Mine", "Mine.png");
	AddTexture("Asteroids", "asteroids.png");
	AddTexture("Explosion", "Explosion.png");

	AddTexture("Splashpage", "DemoSplash.jpg", false);
	
	AddSound("Boom", "bazooka3.wav");
	AddSound("Zap", "scifiblast.wav");
	AddSound("Ding", "glass.wav");

	AddFont("PointFont", "MotorwerkOblique.ttf" );
}

