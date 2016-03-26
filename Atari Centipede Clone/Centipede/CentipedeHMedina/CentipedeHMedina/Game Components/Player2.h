#ifndef PLAYER2_H
#define PLAYER2_H

class Blaster;
#include "Player.h"
class Player2: public Player{
public:

	Player2();
	Player2(const Player2&){};
	~Player2();

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

	virtual Blaster& SpawnNewPlayer();

	virtual void PlayerInitialize();//sets the players to full health, current wave =0... etc..

};






#endif PLAYER2_H