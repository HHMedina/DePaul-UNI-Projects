#ifndef MUSHROOM_H
#define MUSHROOM_H

//#include "BlasterBullet.h"
#include "TEAL\CommonElements.h"
class BlasterBullet;
class Mushroomfield;
class RecyclingMushroomFactory;

class Mushroom:public GameObject{

	friend class RecyclingMushroomFactory;
	friend class Mushroomfield;
	friend class Scorpion;
	friend class HUD;
public:
	Mushroom();
	Mushroom(const float,const float,const sf::Vector2i,Mushroomfield&);
	Mushroom(const Mushroom&);
	~Mushroom(){/*no Dynamically allocated object members to delete*/};

	virtual void Draw();
	virtual void Update();
	virtual void Destroy();
	virtual void Collision( GameObject *other ){};
	void Collision( BlasterBullet *other);
	//void Collision( Scorpion *other);

	void SetImmuneToDamage();
	void SetImmuneToPoison();

	void Poison();
	void FullyHeal();
private:
	int HP;
	AnimatedSprite Sprite;
	CollisionTools::TextureBitmap bitmap;
	sf::Vector2f WorldPosition;
	sf::Vector2i GridPosition;
	Mushroomfield * FatherMushroomfield;
	void Initialize(sf::Vector2f,sf::Vector2i, Mushroomfield&);

	//controls the state of the mushroom (poison state)
	bool IsPoisoned;


	bool IsImmuneToDamage;
	bool IsImmuneToPoison;
	//consts
	static const int MaxHP =4;
	static const int PoisonStateFirstFrame = 4;
};





#endif MUSHROOM_H