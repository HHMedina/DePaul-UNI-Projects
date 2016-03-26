#ifndef POSITONANDROTATIONOFFSET
#define POSITONANDROTATIONOFFSET

#include "TEAL\CommonElements.h"
	//class for the storing of Offsets in centipedeQueues
	
	class PositionAndRotationOffset{
		friend class CentipedeHeadSegment;
		friend class CentipedeBodySegment;
	public:
		PositionAndRotationOffset(sf::Vector2f posOff,float rotOff,sf::Vector2f dir,bool);
		PositionAndRotationOffset(const PositionAndRotationOffset& rhs);
		PositionAndRotationOffset& operator =(const PositionAndRotationOffset& rhs);
		~PositionAndRotationOffset();
		sf::Vector2f PositionOffset;
		sf::Vector2f Direction;
		float RotationOffset;
		bool IsInATurn;
		

	};

#endif POSITONANDROTATIONOFFSET