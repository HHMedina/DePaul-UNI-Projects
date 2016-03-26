#include "PlayerController.h"
#include "Blaster.h"
#include "BlasterBullet.h"
#include "RecyclingBulletFactory.h"
#include "WaveManager.h"

//used to test runtime sound system change
#include "SoundManager.h"
#include "NoSoundSystem.h"
Playercontroller::Playercontroller(Blaster& controlle){
	blaster = &controlle;
	CurrentlyInvincible =false;
	RegisterInput(InputFlags::KeyPressed);
}

Playercontroller::Playercontroller(Playercontroller& other){
	//this->blaster = other.blaster;
}

void Playercontroller::move(){

	//stores last position
	blaster->LastPosition = blaster->Position;

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
		blaster->Position += sf::Vector2f(0,-blaster->Speed);//move up
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
		blaster->Position += sf::Vector2f(-blaster->Speed,0);//move left
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
		blaster->Position += sf::Vector2f(0,blaster->Speed);//move down
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
		blaster->Position+= sf::Vector2f(blaster->Speed,0);//move right
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)&&blaster&&blaster->BulletFired==NULL){ //if there is a blaster without a bullet currently being fired
		
		blaster->BulletFired =&RecyclingBulletFactory::CreateBullet(blaster->Position,*blaster);//fire a bullet
		
	}

	

	Tools::Clamp<float>(blaster->Position.x,(float) blaster->Sprite.getTextureRect().width/2,WindowManager::MainWindow.getView().getSize().x-blaster->Sprite.getTextureRect().width/2);
	Tools::Clamp<float>(blaster->Position.y,blaster->TopBorder,blaster->BottomBorder);
	blaster->Sprite.setPosition(blaster->Position);

}

Playercontroller::~Playercontroller(){
	 this->DeregisterInput();

 }

void Playercontroller::KeyPressed (sf::Keyboard::Key k, bool altKey, bool ctrlKey, bool shiftKey, bool systemKey)
{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num0)){ //toggle collision on or off
		
		if(blaster){
			ConsoleMsg::WriteLine("Blaster Is Without Collider");
			blaster->DeregisterCollision(*blaster);
		}
	}
		
		
}