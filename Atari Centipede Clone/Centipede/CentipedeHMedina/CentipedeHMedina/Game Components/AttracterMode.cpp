#include "AttracterMode.h"
#include "Blaster.h"
#include "BlasterBullet.h"
#include "RecyclingBulletFactory.h"

//used to test runtime sound system change
#include "SoundManager.h"
#include "NoSoundSystem.h"
#include <stdlib.h>
#include <time.h>

const float AttracterMode::TimeInbetweenMovingAndPausing = 2.5f;

AttracterMode::AttracterMode(Blaster& controller){
	srand(time(NULL));
	blaster = &controller;

	Direction = sf::Vector2f(1,-1);
	IsOnBorder =false;
	CanMove=true;

	this->SetUpdateOrder(1);

	this->Alarm0();

	ConsoleMsg::WriteLine(Tools::ToString(blaster->Speed));
	//Set up The precomputed Downward motion with initial speed
	for(int i =0;i<MUSHROOMSEGMENT/blaster->Speed;i++){
		StopQueue.push(PositionAndRotationOffset(sf::Vector2f(0,0),0,sf::Vector2f(0,0),false));
	}

	//Set up The precomputed Diagonal motion with initial speed
	for(int i =0;i<NumberOfSpeedOffsetsToPutInDiagonalQueue;i++){
		DiagonalQueue.push(PositionAndRotationOffset(sf::Vector2f(AttractModeSpeed,AttractModeSpeed),0,sf::Vector2f(0,1),false));
	}

}

AttracterMode::AttracterMode(AttracterMode& other){
	this->blaster = other.blaster;
}

void AttracterMode::move(){

	if(blaster->BulletFired==NULL){ //if blaster has no buller shoot a bullet
		blaster->BulletFired =&RecyclingBulletFactory::CreateBullet(blaster->Position,*blaster);
	}


	if(CanMove){ //if can move
		blaster->LastPosition = blaster->Position;

		if(CurrentQueue.empty()){//if the cycle queue is empty then decide what the next cycle is (random)
	
			int YDirectionchoice = rand()%2;

			///block of code used to decide which y rando direction to go in
		
			if(YDirectionchoice == 0){// if not at the borders then choose a random direction to go in
				Direction.y =1;
			}else{
				Direction.y =-1;
			}
		
		
			if(blaster->Position.y<=blaster->TopBorder||blaster->Position.y>=blaster->BottomBorder){//if on one of the borders, then change the direction to up or down accordingly
			
				if(blaster->Position.y<=blaster->TopBorder)//if at the top border
					Direction.y=1;

				if(blaster->Position.y>=blaster->BottomBorder)// if at the bottom border
					Direction.y=-1;

			} 

			srand(time(NULL));
			int XDirectionchoice = rand()%2;

			///block of code used to decide which y random direction to go in
		
			if(XDirectionchoice == 0){// if not at the borders then choose a random direction to go in
				Direction.x =1;
			}else{
				Direction.x =-1;
			}
		
			if(blaster->Position.y<=blaster->TopBorder||blaster->Position.y>=blaster->BottomBorder){//if on one of the borders, then change the direction to up or down accordingly
			
				if(blaster->Position.x<=blaster->Sprite.getTextureRect().width/2)//if at the left border
					Direction.x=1;

				if(blaster->Position.x>=WindowManager::MainWindow.getView().getSize().x-blaster->Sprite.getTextureRect().width/2)// if at the Right border
					Direction.x=-1;

			} 



			
			int numberOfMoves = rand()%2+2;//addition of one to prevent int from being 0. This randomly chooses how many diagonals 
										   //or vertical moves to do in the next cycle, with a minimum and a maximum number
									   
						
			for(numberOfMoves;numberOfMoves>0;--numberOfMoves){
				CurrentQueue.push(DiagonalQueue);
			}
			
		}

	
		//If the current cycle queue is not empty then work on the next offset queue 
		if(!CurrentQueue.empty()){

		
		
			//If The Spider is on the top or bottom Border, then empty the Current Cycle
			//Change the vertical direction and make a new decision 
			if((blaster->Position.y<=blaster->TopBorder||blaster->Position.y>=blaster->BottomBorder)&&!IsOnBorder)
			{
				while(!CurrentQueue.empty()){
					CurrentQueue.pop();
				}
			
				//prevents a loop of decision making based on if it is on the border. (will only make one decision on border instead of infinite.)
				IsOnBorder=true;

			} else if(!CurrentQueue.front().empty()){//if the current queue of the cycle is not empty
				//process the next offset in the queue
				blaster->Position+=sf::Vector2f(CurrentQueue.front().front().PositionOffset.x*Direction.x,CurrentQueue.front().front().PositionOffset.y*Direction.y);
				//clamp position to make sure blaster doesnt go past borders
				Tools::Clamp<float>(blaster->Position.x,(float) blaster->Sprite.getTextureRect().width/2,WindowManager::MainWindow.getView().getSize().x-blaster->Sprite.getTextureRect().width/2);
				Tools::Clamp<float>(blaster->Position.y,blaster->TopBorder,blaster->BottomBorder);
				blaster->Sprite.setPosition(blaster->Position);
				//Remove the queue element that was processed
				CurrentQueue.front().pop();

			}else{ //if current queue is empty
				//sets if on border to false since it has moves away from border (next cell away and any other cell)
				IsOnBorder=false;
				CurrentQueue.pop();// pops the top of the queue because it is empty and moves on to the next queue in the cycle
			}
		
		}
	}
}

//does nothing for 2.5 seconds
 void AttracterMode::Alarm0(){

	 //Allows the AI to move
	 CanMove=true;
	
	 //sets alarm that allows doing things for 3 seconds
	 this->SetAlarm(1,TimeInbetweenMovingAndPausing);

 }

 void AttracterMode::Alarm1(){

	 //disables the decision process
	 CanMove =false;
	  //Empty out the current queue
	 while(!CurrentQueue.empty()){

		 while(!CurrentQueue.front().empty()){
			 CurrentQueue.front().pop();
		 }
		 CurrentQueue.pop();
	 }

	 this->SetAlarm(0,TimeInbetweenMovingAndPausing);
 }

 AttracterMode::~AttracterMode(){
	 this->DeregisterInput();//remove from input list

 }