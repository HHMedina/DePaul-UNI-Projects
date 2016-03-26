#include "RecyclingBulletFactory.h"
#include "BlasterBullet.h"
#include "Blaster.h"

//declare static members used in .h
RecyclingBulletFactory * RecyclingBulletFactory::factoryInstance=NULL;


RecyclingBulletFactory::~RecyclingBulletFactory(){
	//deletes every bullet in the stack
	while(!recycledItems.empty()){
		delete recycledItems.top();
		recycledItems.pop();
	}
	
}



BlasterBullet& RecyclingBulletFactory::CreateBullet(const sf::Vector2f pos,Blaster& shooter){
	//checks to see if there are less than the maximum amount of bullets in existance
		BlasterBullet* bullet;
		
		//if the recyclebin is empty create a bullet
		if ( Instance().recycledItems.empty() )
		{
	
			
			bullet = new BlasterBullet(pos,shooter);
			
			// Tell Teal to let us manually manage the bullet and send to RecycleBullet()
			bullet->SetExternalManagement( RecycleBullet );
		}
		else
		{
			//sets bullet to the top of the recycle stack (grab a previously used bullet)
			bullet = Instance().recycledItems.top();
			Instance().recycledItems.pop();//remove from stack (since it will be used)

			// register bullet to scene
			bullet->RegisterToCurrentScene();  
		}

		//respawn the bullet
		bullet->Initialize(pos,shooter);
	
		return *bullet;
}

void RecyclingBulletFactory::RecycleBullet(GameObject* bullet)
{
	//adds bullet to recycle stack
	Instance().recycledItems.push( (BlasterBullet*) bullet);
	
}