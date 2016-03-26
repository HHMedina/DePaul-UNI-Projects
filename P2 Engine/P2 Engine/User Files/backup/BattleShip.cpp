#include "BattleShip.h"

BattleShip::BattleShip(){

	RegisterCollision<BattleShip>(this);
}