#ifndef PLAYER0_H
#define PLAYER0_H

#include "Player.h"

class WaveManager;
class Player0: public Player{
public:

	Player0();
	Player0(const Player0&){};
	~Player0();

	virtual void AddToPlayerScore(int score);
	virtual int GetPlayerScore();
	virtual int& GetPlayerLives();
	virtual void UpdatePlayerLivesRemaining();

	virtual void LowerLivesByOne();

	virtual Mushroomfield& GetPlayerField();
	virtual void ClearPlayerField();
	virtual void RespawnPlayerField();

	virtual int GetCurrentWave();
	virtual void SetCurrentWave(int);
	virtual void IncrementPlayerCurrentWave();

	virtual void PlayerInitialize();//sets the players to full health, current wave =0... etc..

	virtual Blaster& SpawnNewPlayer();

};






#endif PLAYER0_H