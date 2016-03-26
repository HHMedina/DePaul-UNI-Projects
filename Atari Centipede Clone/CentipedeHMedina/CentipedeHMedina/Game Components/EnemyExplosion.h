#ifndef ENEMYEXPLOSION_H
#define ENEMYEXPLOSION_H

//#include "BlasterBullet.h"
#include "TEAL\CommonElements.h"
#include "Mushroomfield.h"

class EnemyExplosion:public GameObject{

	friend class RecyclingEnemyExplosionFactory;

public:

	EnemyExplosion(const sf::Vector2f SpawnLocation);
	EnemyExplosion(const EnemyExplosion&){}
	~EnemyExplosion(){/*no Dynamically allocated object members to delete*/};

	virtual void Draw();
	virtual void Update();
	virtual void Destroy();
	virtual void Collision( GameObject *other ){};


	
private:

	AnimatedSprite Sprite;//visual image
	void Initialize(sf::Vector2f SpawnLocation); //used to respawn via factory
};





#endif ENEMYEXPLOSION_H