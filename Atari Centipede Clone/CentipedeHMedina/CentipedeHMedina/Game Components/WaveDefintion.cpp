#include "WaveDefintion.h"


WaveDefinition::WaveDefinition(){
	//default Settings
	CentipedeSpeed =2;
	CentipedeLength =4;
	CentipedeHeadNumber=0;
	CentipedeHeadSpeed=2;
	SpiderSpeed=2;
	SpiderSpawnTime=3;
	ScorpionSpawnTime=3;
	FleaMushroomCountValue=4;
	CanSpawnFleas = false;
	CanSpawnScorpions =false;
}

WaveDefinition::WaveDefinition(const WaveDefinition& other){
	CentipedeSpeed = other.CentipedeSpeed;
	CentipedeLength = other.CentipedeLength;
	CentipedeHeadNumber=other.CentipedeHeadNumber;
	SpiderSpeed= other.SpiderSpeed;
	SpiderSpawnTime= other.SpiderSpawnTime;
	ScorpionSpawnTime= other.ScorpionSpawnTime;
	FleaMushroomCountValue= other.FleaMushroomCountValue;
	CanSpawnFleas = other.CanSpawnFleas;
	CanSpawnScorpions = other.CanSpawnScorpions;


}