#ifndef WAVEDEFINTION_H
#define WAVEDEFINITON_H

class WaveDefinition{

public:
	WaveDefinition();
	WaveDefinition(const WaveDefinition&); 
	WaveDefinition& operator = (const WaveDefinition& Other){} //prevent assignment
	~WaveDefinition(){}//only primatives used
	
	int CentipedeSpeed;
	int CentipedeLength;
	int CentipedeHeadNumber;
	int CentipedeHeadSpeed;
	float SpiderSpeed;
	float SpiderSpawnTime;
	float ScorpionSpawnTime;
	int FleaMushroomCountValue;
	bool CanSpawnFleas;
	bool CanSpawnScorpions;
};




#endif WAVEDEFINITON_H