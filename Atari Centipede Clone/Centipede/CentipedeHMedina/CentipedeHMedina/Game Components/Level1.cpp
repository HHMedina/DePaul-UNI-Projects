// CentipedeLevel
// Andre Berthiaume, MArch 2013

#include "TEAL\CommonElements.h"

#include "Level1.h"
#include "GameController.h"

#include "Blaster.h"
#include "Mushroom.h"
#include "BlasterBullet.h"
#include "CentipedeHeadSegment.h"
#include "CentipedeBodySegment.h"
#include "CentipedeFactory.h"
#include "CentipedeSegment.h"
#include "Flea.h"
#include "Scorpion.h"
#include "Spider.h"

#include "WaveManager.h"
#include "ModeManager.h"


#include "BlasterFactory.h"

void Level1::Initialize()
{
	

	WindowManager::SetBackgroundColor( sf::Color(0,0,0,255) );
	WaveManager::LoadHighScores();
	WaveManager::LoadWaveInformation();
	ModeManager::StartUp();
	
	//RecyclingBlasterFactory::CreateBlaster(1);
	

	CollisionTestPair<Mushroom,BlasterBullet>();
	CollisionTestPair<CentipedeBodySegment,BlasterBullet>();
	CollisionTestPair<CentipedeHeadSegment,BlasterBullet>();
	CollisionTestPair<Flea,BlasterBullet>();
	
	CollisionTestPair<Spider,BlasterBullet>();
	CollisionTestPair<Scorpion,BlasterBullet>();
	CollisionTestPair<Mushroom,Blaster>();

	CollisionTestPair<Spider,Blaster>();
	CollisionTestPair<Blaster,Flea>();
	CollisionTestPair<Blaster, CentipedeHeadSegment>();
	CollisionTestPair<Blaster, CentipedeBodySegment>();

	
};