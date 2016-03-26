#ifndef PLAYER_H
#define PLAYER_H

class Mushroomfield;
class Blaster;
class Player{
public:

	Player(){CurrentScore=0;NumberOfLives=3; CurrentWave=0;};
	Player(const Player&){};
	~Player();

	virtual void AddToPlayerScore(int score)=0;
	virtual int GetPlayerScore()=0;
	virtual int& GetPlayerLives()=0;
	virtual void UpdatePlayerLivesRemaining()=0;

	virtual void LowerLivesByOne()=0;
	virtual Mushroomfield& GetPlayerField()=0;
	virtual void ClearPlayerField() =0;
	virtual void RespawnPlayerField()=0;

	virtual int GetCurrentWave()=0;
	virtual void SetCurrentWave(int)=0;
	virtual void IncrementPlayerCurrentWave()=0;

	virtual Blaster& SpawnNewPlayer()=0;

	virtual void PlayerInitialize()=0;//sets the players to full health, current wave =0... etc..

protected:
	static const int StartNumberOfMushrooms =44;
	int CurrentScore;
	int NumberOfLives;
	int CurrentWave;
	Mushroomfield * PlayerField;
};






#endif PLAYER_H