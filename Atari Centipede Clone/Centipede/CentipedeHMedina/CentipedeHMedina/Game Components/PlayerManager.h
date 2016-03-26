#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

//class forward declarations
class Blaster;
class Player;
class Player0;
class Player1;
class Player2;
class PlayerExplosion;

class PlayerManager{

private:
	static PlayerManager* instance;

	PlayerManager();
	PlayerManager(const PlayerManager&){}
	PlayerManager& operator =(const PlayerManager&){}
	~PlayerManager();

	static PlayerManager& Instance(){
		if(!instance){
			instance = new PlayerManager();
		}
		return *instance;

	}

	Blaster* shooter;

	//players and player holder
	Player* CurrentPlayer;
	Player* player1;
	Player* player2;
	Player* player0;//attractor player

	PlayerExplosion* blasterExplosion;

public:

	static Player* GetPlayer();
	static Player* GetPlayer2Pointer();

	static void SwapPlayers();
	static void LowerLivesByOne(); 
	static void ChangePlayerToPlayer1();
	static void ChangePlayerToPlayer0();
	static void SpawnNewPlayer();
	static void AddToCurrentScore(int);
	static bool ThereIsABlaster();
	static void DestroyBlaster();
	static void DestroyBlasterBullet();
	static void SetPlayerExplosion(PlayerExplosion&);
	static void DestroyPlayerExplosion();
	
	static void Player1Initialize();
	static void Player2Initialize();
	static void Player0Initialize();

	static bool BothPlayersAreDead();
};

#endif PLAYERMANAGER_H