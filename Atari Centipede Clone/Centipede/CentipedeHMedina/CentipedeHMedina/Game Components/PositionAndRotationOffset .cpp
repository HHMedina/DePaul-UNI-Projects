#include "PositionAndRotationOffset.h"

PositionAndRotationOffset::PositionAndRotationOffset(sf::Vector2f posOff,float rotOff,sf::Vector2f dir,bool turning){
			PositionOffset=posOff;
			RotationOffset=rotOff;
			Direction =dir;
			IsInATurn =turning;
		}
PositionAndRotationOffset::PositionAndRotationOffset(const PositionAndRotationOffset& rhs){
	PositionOffset = rhs.PositionOffset;
	RotationOffset = rhs.RotationOffset;
	Direction = rhs.Direction;
	IsInATurn = rhs.IsInATurn;
}

PositionAndRotationOffset& PositionAndRotationOffset::operator =(const PositionAndRotationOffset& rhs){
	if(&rhs==this)
		return *this;

	PositionOffset = rhs.PositionOffset;
	RotationOffset = rhs.RotationOffset;
	Direction = rhs.Direction;
	IsInATurn = rhs.IsInATurn;
	return *this;
}
PositionAndRotationOffset::~PositionAndRotationOffset(){

}