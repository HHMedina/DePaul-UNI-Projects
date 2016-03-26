#ifndef PLAYEREXPLOSION_H
#define PLAYEREXPLOSION_H

#include "TEAL\CommonElements.h"
class PlayerExplosion : public GameObject{
public:
	PlayerExplosion(sf::Vector2f);
	PlayerExplosion (PlayerExplosion&);
	~PlayerExplosion(){/* nothing to destruct*/}

	void Update();
	void Draw();

	void Initialize(sf::Vector2f);

protected:
	AnimatedSprite PlayerExplosionSprite;
	CollisionTools::TextureBitmap PlayerExplosionBitmap;


};


#endif EXPLOSION_H