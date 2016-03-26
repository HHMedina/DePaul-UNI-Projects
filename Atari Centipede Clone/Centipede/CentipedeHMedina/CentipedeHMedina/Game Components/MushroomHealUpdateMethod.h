#ifndef MUSHROOMHEALUPDATEMETHOD_H
#define MUSHROOMHEALUPDATEMETHOD_H
#include "Mushroomfield.h"
#include "RecyclingEnemyExplosionFactory.h"
#include "UpdateMethod.h"

class MushroomHealUpdateMethod:public UpdateMethod{
public:
	MushroomHealUpdateMethod(){}
	MushroomHealUpdateMethod(const MushroomHealUpdateMethod&){}
	MushroomHealUpdateMethod& operator =(const MushroomHealUpdateMethod&){}

	virtual void Update(){
		
		for(CurrentRowHealing; CurrentRowHealing<rows;){
		for(CurrentColumnHealing;CurrentColumnHealing<columns;){
			
			//if there is a mushroom in that spot
				if(Field[CurrentRowHealing][j]!=NULL&&Field[i][j]->HP<Mushroomfield::MaxMushroomHP){
					RecyclingEnemyExplosionFactory::CreateEnemyExplosion(Field[CurrentRowHealing][j]->WorldPosition);
					Field[CurrentRowHealing][j]->FullyHeal(); //heal the mushroom
							
				}
				CurrentColumnHealing++;
		}
		++CurrentRowHealing;
	}
		
	}


private:
	Mushroomfield* FatherField;
};




#endif MUSHROOMHEALUPDATEMETHOD_H