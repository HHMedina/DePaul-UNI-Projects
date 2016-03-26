#ifndef MUSHROOMFIELD_H
#define MUSHROOMFIELD_H

#include "TEAL\CommonElements.h"
class Mushroom;
class RecyclingMushroomFactory;
#include <stdlib.h>
#include <time.h>


class Mushroomfield{
	friend class Mushroom;
	friend class WaveManager;
	friend class Level1;
	//friend class Centipede;
public:
	Mushroomfield();
	Mushroomfield(Mushroomfield&);
	~Mushroomfield(){/*No Dynamically allocated objects to destroy Since Factory owns all mushrooms*/}

	void Initialize(int NumberOfMushrooms);
	sf::Vector2i WorldPositionToMushroomGrid(const sf::Vector2f) const;
	Mushroom& AccessMushroom(const sf::Vector2i) const;
	bool CheckMushroomForPoison(const sf::Vector2i) const;
	void CreateMushroomAtCoordinate(const sf::Vector2i&);
	bool CheckGridSpace(const sf::Vector2i) const;
	void DestroyMushroom(const sf::Vector2i);
	void ClearTheField();
	void RespawnField();

	void HealAllMushrooms();
protected:
	
	virtual void Alarm0();
	static const int MaxMushroomHP =4;
	sf::Vector2f GridStart;
	static const int rows=29;
	static const int columns=30;
	Mushroom* Field[rows][columns];
	bool StateField[rows][columns];
	int AmountOfMushrooms;
	int AmountOfPlayerSideMushrooms;

	static const int StartOfPlayerSideMushroomArea=24;
	static const int NumberOfMushroomsHealedBeforeGettingPoints =2;
	static const int ScoreRecievedPerEachBundleOfMushroomsHealed =5;
	
	static int NumberOfMushroomFields;
	int MushroomFieldNumber; //used to generate different field of reach player
	static const int MUSHROOMSEGMENT=16;//mushroom sprite's width or height (since the texture is square.)
	
};





#endif MUSHROOMFIELD_H