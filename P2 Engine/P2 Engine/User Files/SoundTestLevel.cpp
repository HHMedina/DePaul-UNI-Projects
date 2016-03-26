#include "SoundTestLevel.h"
#include "SoundTester.h"
#include "TankBottom.h"
#include "Tank.h"

#include "SoundListenerTest.h"

void SoundTestLevel::SetUp(){
	//new Tank(Vect(0,0,0),Vect(.2f,.2f,.2f),Vect(0,0,0),0);

	//new SoundTester(0);
	new SoundTester(0);
	new SoundLisenerTest();
	new TankBottom(Vect(0,0,-6),Vect(.1f,.1f,.1f),Vect(0,0,0));
	new TankBottom(Vect(-6,0,0),Vect(.1f,.1f,.1f),Vect(0,0,0));
	
}

void SoundTestLevel::OnSceneEnd(){
	
}