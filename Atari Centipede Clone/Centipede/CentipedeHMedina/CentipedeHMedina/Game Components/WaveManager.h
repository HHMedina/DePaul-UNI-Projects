
#ifndef WAVEMANAGER_H
#define WAVEMANAGER_H

#include <stack>
#include <list>
#include <stdlib.h>
#include <time.h>
#include "TEAL\CommonElements.h"
#include <vector>

class Mushroomfield;
class Blaster;
class WaveDefinition;
class Scorpion;
class Flea;
class Spider;

class Player;
class Player1;
class Player2;

struct HighScoreCombination;
//create struct class
struct HighScoreCombination{

public:
	int Score;
	std::string Name;

	HighScoreCombination(int a, std::string b){
		Score = a;
		Name =b;
	}

	HighScoreCombination(const HighScoreCombination& other){
		Score = other.Score;
		Name =other.Name;
	}

	HighScoreCombination& operator =(const HighScoreCombination& other){
		Score = other.Score;
		Name =other.Name;
	}

	~HighScoreCombination(){}//only primative datatypes
};


class WaveManager: public GameObject{

	

private:

	static WaveManager * instance; //set up as singleton
	WaveManager(); //prevent public constructor
	WaveManager(WaveManager&){} //prevent copy-construction
	WaveManager& operator = (const WaveManager&){} //prevent assignment
	~WaveManager();

	virtual void Update();
	virtual void Alarm0();//alarm for spider
	virtual void Alarm1();//alarm for Scorpion
	virtual void Alarm2();//alarm for Spawning Heads after touching bottom

	static WaveManager& Instance()
	{
		if(!instance){
			instance = new WaveManager();
		}
		return *instance;
		
	}

	


	//int to hold Currentscore
	int Player1Score;
	int Player2Score;

	int CurrentNumberOfLives;
	
	//Bools to hold if they can spawn certain things (example.. fleas only appear on level 2 and on.. i believe..)
	bool CanSpawnAflea;
	bool CanSpawnASpider;
	bool CanSpawnAScorpion;
	int CentipedeSpeed;
	int CentipedeLength ;
	int CentipedeHeadNumber;
	float SpiderSpeed;
	float SpiderSpawnTime;
	float ScorpionSpawnTime;
	int FleaMushroomCountValue;
	int CentipedeHeadSpeed;

	//variables used to hold wave conditions
	bool FleaIsInScene;
	bool SpiderIsInScene;
	bool ScorpionIsInScene;
	bool CentipedeHeadSpawningActivated;

	//pointers to entities on the field (used for destroying all on player death)
	Flea * FleaOnScene;
	Scorpion * ScorpionOnScene;
	Spider * SpiderOnScene;



	float SPIDERSTARTY;
	static const int MUSHROOMSEGMENT =16;
	static const int LeftStartPosition=-(MUSHROOMSEGMENT+MUSHROOMSEGMENT/2);
	static int RightStartPosition;
	static const int NumberOfMushroomsLeftBeforeFlea=4;
	static const int FLEASPAWNBORDERLEFT=8;
	static const int INITIALNUMBEROFLIVES=3;
	
	//Vector Holding the definition of the waves
	static const int NumberOfWaves =10;
	std::vector<WaveDefinition*> Waves;
	int CurrentWave;

	float FleaSpeed;

	//Variables & data types concerning HighScores
	std::list<HighScoreCombination> HighScores;



public:

	
	//consts
	static const int TimeInbetweenSpiderOnDeath =4;
	static const int TimeInbetweenScorpions =6;
	static const int CentipedeHeadSpawnTime=4;
	static const int ScorpionSpeed =2;
	static const int BottomSpawnedCentipedeHeadSpeed =4;

	static void CheckToSpawnFlea();

	
	static void DisableSpiderSpawn();
	static void EndOfWaveDecision();
	
	static void StartUp();//use to initialize the wave manager
	static void SetSpiderToDead();
	static void SetFleaToDead();
	static void SetScorpionToDead();
	static void SetScorpionAlarm(float time);
	static void SetSpiderAlarm(float time);
	

	//called when player explosion animation is done (causes healing of mushrooms)
	static void PlayerDied();

	static void LoadWaveInformation();
	static void LoadPlayerWave();

	//methods for dealing with scoring System
	static void LoadHighScores();
	static void WriteHighScoresToFile();
	static void CheckForNewHighScore(std::string);
	static bool IsScoreANewHighScore();

	static void DestroyAllWaveEntities();

	//getters
	static float GetScorpionSpawnTime();
	static float GetSpiderSpawnTime();
	static std::list<HighScoreCombination>& GetHighScoresList();

	//method used to actiavte centipede head spawning
	static void ActivateHeadSpawning();
	static void StopSpawningCentipedeHeads();

	

	static const float TopBorder;

	
};



#endif WAVEMANAGER_H
