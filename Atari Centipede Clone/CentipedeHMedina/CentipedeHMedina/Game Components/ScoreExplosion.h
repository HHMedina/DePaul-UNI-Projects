#ifndef SCOREEXPLOSION_H
#define SCOREEXPLOSION_H

//#include "BlasterBullet.h"
#include "TEAL\CommonElements.h"


class ScoreExplosion:public GameObject{

	friend class RecyclingScoreExplosionFactory;

public:

	ScoreExplosion(const int WhatScore,const sf::Vector2f SpawnLocation);
	ScoreExplosion(const ScoreExplosion&){}
	~ScoreExplosion(){/*no Dynamically allocated object members to delete*/};

	virtual void Draw();
	virtual void Update();
	virtual void Destroy();
	virtual void Collision( GameObject *other ){};
	virtual void Alarm0();

	
private:
	static const int LifeTime = 1; //seconds
	AnimatedSprite Sprite;//visual image
	void Initialize(const int WhichScore,sf::Vector2f SpawnLocation); //used to respawn via factory
};





#endif SCOREEXPLOSION_H