#include "RecyclingBombFactory.h"
#include "Bomb.h"


//declare static members used in .h
RecyclingBombFactory * RecyclingBombFactory::factoryInstance =NULL;

RecyclingBombFactory::~RecyclingBombFactory(){
	

	factoryInstance=NULL;
}



Bomb& RecyclingBombFactory::CreateBomb(float xPos,float yPos, float zPos){
	
	//cout<<"RBF:Bombs in recycle bin "<<Instance().recycledItems.size()<<endl;
	Bomb* bomb;
		
	//if the recyclebin is empty create a Bomb
	if ( Instance().recycledItems.empty() )
	{
	
		 
		bomb = new Bomb(xPos,yPos,zPos);
			
		// Tell Teal to let us manually manage the EnemyExplosion and send to RecycleEnemyExplosion()
		bomb->SetExternalManagement( RecycleBomb);
	}
	else
	{
		//sets EnemyExplosion to the top of the recycle stack (grab a previously used EnemyExplosion)
		bomb = Instance().recycledItems.top();
		Instance().recycledItems.pop();//remove from stack (since it will be used)

	}
	//respawn the EnemyExplosion
	bomb->Initialize(xPos,yPos,zPos);
	
	Instance().activeInScene.push_back(bomb);
	return *bomb;
	
}

void RecyclingBombFactory::RecycleBomb(Terminable* bomb)
{
	//adds bomb to recycle stack after casting it to Bomb type
	Instance().recycledItems.push( (Bomb*) bomb);
	
	//if being recycled that means its not active anymore
	Instance().activeInScene.remove((Bomb*) bomb);
}

void RecyclingBombFactory::DeleteMe(){

	//deletes every bomb in recycling the stack
	while(!Instance().recycledItems.empty()){
		delete Instance().recycledItems.top();
		Instance().recycledItems.pop();
	}


	//deletes every bomb in the scene
	while(!Instance().activeInScene.empty()){
		delete Instance().activeInScene.front();
		Instance().activeInScene.pop_front();
	}

	delete factoryInstance;

}