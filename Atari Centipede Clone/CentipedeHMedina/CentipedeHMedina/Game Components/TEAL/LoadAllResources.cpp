// LoadAllResources.cpp
// Andre Berthiaume, June 2012
//
// The user fills the method with all resources that need loading

#include "ResourceManager.h"

#include "../Level1.h"

void ResourceManager::LoadAllResources()
{
	SetStartScene( new Level1() );

	//
	AddTexture("Blaster", "Blaster.png");
	AddTexture("PlayerExplosion", "death.png");
	AddTexture("CentipedeHead", "CentipedeHead.png");
	AddTexture("CentipedeBody", "CentipedeBody.png");
	AddTexture("Mushroom", "mushroom.png");
	AddTexture("MushroomTransparency", "mushroomTransparency.png");


	AddTexture("Bullet", "Bullet.png");
	AddTexture("Flea", "Flea.png");
	AddTexture("Scorpion", "Scorpion.png");
	AddTexture("EnemyExplosion", "EnemyExplosion.png");
	AddTexture("Spider", "Spider.png");
	AddTexture("Score300", "Score300.png");
	AddTexture("Score600", "Score600.png");
	AddTexture("Score900", "Score900.png");
	AddTexture("CentFontImage","FONT.bmp");


	AddSound("BlasterShot", "BlasterShot.wav");
	AddSound("ScorpionSound", "ScorpionSound.wav");
	AddSound("EnemyDeathSound", "EnemyDeath.wav");
	AddSound("PlayerDeathSound", "death.wav");
	AddSound("SpiderSound", "SpiderSound.wav");
	AddSound("FleaSound", "FleaSound.wav");
	AddSound("CentipedeSound", "CentipedeSound.wav");



	AddFont("PointFont", "MotorwerkOblique.ttf" );
}

