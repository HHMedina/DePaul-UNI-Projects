/*
#ifndef MUSHROOMHEALEXPLOSION_H
#define MUSHROOMHEALEXPLOSION_H

//#include "BlasterBullet.h"
#include "TEAL\CommonElements.h"
#include "Mushroomfield.h"

class MushroomHealExplosion:public GameObject{

	friend class RecyclingMushroomHealExplosionFactory;

public:

	MushroomHealExplosion(const sf::Vector2f SpawnLocation, Mushroomfield&);
	MushroomHealExplosion(const MushroomHealExplosion&){}
	~MushroomHealExplosion(){}///no Dynamically allocated object members to delete

	virtual void Draw();
	virtual void Update();
	virtual void Destroy();
	virtual void Collision( GameObject *other ){};


	
private:

	AnimatedSprite Sprite;//visual image
	void Initialize(sf::Vector2f SpawnLocation); //used to respawn via factory
};





#endif MUSHROOMHEALEXPLOSION_H

*/