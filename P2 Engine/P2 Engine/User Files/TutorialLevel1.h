#ifndef TUTORIALLEVEL1_H
#define TUTORIALLEVEL1_H

#include <P2Engine.h>
using namespace std;

class Floor;
class Bucket;
class TutorialLevel1: public Scene{


public:

	TutorialLevel1(){}
	TutorialLevel1( const TutorialLevel1 & rhs){ rhs;}
	TutorialLevel1& operator = (const TutorialLevel1& rhs){ rhs;return *this;}
	~TutorialLevel1(){
	}

	virtual void SetUp();
	virtual void OnSceneEnd();

	//pointer members here to allow for calling mark on destroy explicitly
	Floor* floor;
	Bucket* bucket;
};
#endif