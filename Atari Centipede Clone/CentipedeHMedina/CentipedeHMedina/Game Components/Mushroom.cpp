#include "Mushroom.h"
#include "MushroomField.h"
#include "BlasterBullet.h"
#include "RecyclingMushroomFactory.h"
#include "PlayerManager.h"

Mushroom::Mushroom(){
	IsImmuneToPoison =false;
	IsImmuneToDamage =false;
	IsPoisoned=false;
	FatherMushroomfield = NULL;
	HP = 4;
	bitmap = ResourceManager::GetTextureBitmap("Mushroom");
	Sprite = AnimatedSprite(ResourceManager::GetTexture("Mushroom"),4,2,4,0,3,false,false);
	Sprite.setOrigin(Sprite.getTextureRect().width / 2.0f, Sprite.getTextureRect().height / 2.0f);
	Sprite.setPosition(0.0,0.0);
	SetCollider(Sprite,bitmap,true);
	RegisterCollision<Mushroom>(*this);
	SetDrawOrder(1000);
	this->SetUpdateOrder(9000);
}

Mushroom::Mushroom(const float x, const float y,const sf::Vector2i gridLocation,Mushroomfield& father){
	IsImmuneToPoison =false;
	IsImmuneToDamage =false;
	IsPoisoned=false;
	WorldPosition = sf::Vector2f(x,y);
	GridPosition = gridLocation;
	FatherMushroomfield = &father;
	HP = 4;
	bitmap = ResourceManager::GetTextureBitmap("Mushroom");
	Sprite = AnimatedSprite(ResourceManager::GetTexture("Mushroom"),4,2,4,0,3,false,false);
	Sprite.setOrigin(Sprite.getTextureRect().width / 2.0f, Sprite.getTextureRect().height / 2.0f);
	Sprite.setPosition(WorldPosition);
	SetCollider(Sprite,bitmap,true);
	RegisterCollision<Mushroom>(*this);
	SetDrawOrder(1000);
}

Mushroom::Mushroom(const Mushroom& other){
	HP = other.HP;
	Sprite = other.Sprite;
	bitmap = other.bitmap;
	WorldPosition=other.WorldPosition;
}



void Mushroom::Draw(){
	WindowManager::MainWindow.draw(Sprite);

}
void Mushroom::Update(){
	
}
void Mushroom::Destroy(){
	DeregisterCollision<Mushroom>(*this);
}

void Mushroom::Collision(BlasterBullet *other){
	
	if(!IsImmuneToDamage){
		// If the HP is 1 or less, then this collision destroys the mushroom	
		if(HP<2){
			PlayerManager::AddToCurrentScore(1);
		
				FatherMushroomfield->DestroyMushroom(GridPosition);
		
		}
		else{// if not then mushroom will not die this collision. Process sprite update and decrement hp
			--HP;
		
			if(IsPoisoned){//if is poisoned go the next frame of the poisoned state
			Sprite.SetFrame(4+(MaxHP-HP));
			}else{
				Sprite.SetFrame(MaxHP-HP);//else go the next frame of the Normal state
			}
		}
	}
}

void Mushroom::Initialize(sf::Vector2f w,sf::Vector2i g, Mushroomfield& m){
	IsImmuneToDamage =false;
	IsImmuneToPoison =false;
	WorldPosition = w;
	GridPosition = g;
	FatherMushroomfield = &m;
	Sprite.SetFrame(0);
	HP = 4;
	Sprite.setPosition(WorldPosition);
	Sprite.setTexture(ResourceManager::GetTexture("Mushroom"));
	RegisterCollision<Mushroom>( *this );
	IsPoisoned =false;
	

}


void Mushroom::Poison(){

	//if mushroom is not already poisoned
	if(!IsImmuneToPoison&&IsPoisoned ==false){
		//set to the poison visual state based on the HP it had on conversion
		Sprite.SetFrame(4+(MaxHP-HP));
		IsPoisoned =true;//set Mushroom to poisoned state
	}

}

void Mushroom::FullyHeal(){

	//sets mushroom back to initial state
	HP = MaxHP;
	IsPoisoned =false;
	Sprite.SetFrame(0);


}

void Mushroom::SetImmuneToDamage(){// used for attractor mode mushrooms behind text

	IsImmuneToDamage =true;


}

void Mushroom::SetImmuneToPoison(){// used for attractor mode mushrooms behind text

	IsImmuneToPoison =true;


}