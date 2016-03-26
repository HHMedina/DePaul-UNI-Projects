#ifndef PLAYER1_H
#define PLAYER1_H

#include "Player.h"

class WaveManager;
class Blaster;
class Player1: public Player{
public:

	Player1();
	Player1(const Player1&){};
	~Player1();

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






#endif PLAYER1_H