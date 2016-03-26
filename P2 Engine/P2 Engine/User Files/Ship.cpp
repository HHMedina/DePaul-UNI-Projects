#include "Ship.h"
#include <iostream>
#include <Level2.h>

#include "WeirdShape.h"
// Spaceship vars

const float ShipTransSpeed = 1;
const float ShipRotAng = .05f;
bool BsphereToggle = false;

Ship::Ship(){
	
	s = new GraphicsObjectWireFrame(GpuModel::Create("BoundingSphere.azul"));

	SetCollider(AssetManager::GetModel("SpaceShip"));
	DeregisterForUpdate();
	//RegisterForAlarm(AlarmID_0,5);
	//set up gameobject here
	SetGraphicAndCollisionModel( new GraphicsObjectFlatTexture(AssetManager::GetModel("SpaceShip"),AssetManager::GetTexture("ShipTex")));
	//graphicsObject->setWorld(world);
	//SetCollider(AssetManager::GetModel("SpaceShip"));
	//collisionModel = graphicsObject->getModel();

	//RegisterForInput(AZUL_KEY::KEY_H,KeyEvent::KeyDown);
	//RegisterForInput(AZUL_KEY::KEY_H,KeyEvent::KeyUp);
	//SceneManager::RegisterForInput(this,AZUL_KEY::KEY_H,KeyEvent::KeyUp);
	//SceneManager::RegisterForInput(this,AZUL_KEY::KEY_A,KeyEvent::KeyDown);
	//SceneManager::DeregisterForInput(this,AZUL_KEY::KEY_A,KeyEvent::KeyDown);
	//this->RegisterToDraw();
	//SceneManager::RegisterForUpdating(this);
	
	SetAlarm(AlarmID_0,10);
	/*
	RegisterForAlarm(AlarmID_1,3);
	RegisterForAlarm(AlarmID_1,10);
	RegisterForAlarm(AlarmID_1,7);
	RegisterForAlarm(AlarmID_1,3);
	RegisterForAlarm(AlarmID_2,7);
	
	DeregisterForAlarm(AlarmID_1);	
	DeregisterForAlarm(AlarmID_1);
	DeregisterForAlarm(AlarmID_1);	
	DeregisterForAlarm(AlarmID_1);
	DeregisterForAlarm(AlarmID_1);	
	DeregisterForAlarm(AlarmID_1);
	*/

	// Spaceship
		ShipScale.set( SCALE, 0.5f, 0.5f, 0.5f);
		ShipRot.set( ROT_XYZ, 0, 0, 0);
		ShipPos.set(0, 0, -40);
		world.set(IDENTITY);
		world *= ShipScale * ShipRot * Matrix( TRANS, ShipPos );
		
		SetGameObjectWorld(world);


	RegisterCollision(this);
}

Ship::Ship(int x){
	
	SetAlarm(AlarmID_0,10);
	this->DeregisterForInput(AZUL_KEY::KEY_W,KeyEvent::KeyDown);
	s = new GraphicsObjectWireFrame(GpuModel::Create("BoundingSphere.azul"));
	x;
	//set up gameobject here
	SetGraphicAndCollisionModel( new GraphicsObjectFlatTexture(AssetManager::GetModel("SpaceShip"),AssetManager::GetTexture("ShipTex")));
	//graphicsObject = new GraphicsObjectFlatTexture(AssetManager::GetModel("SpaceShip"),AssetManager::GetTexture("ShipTex"));
	//graphicsObject->setWorld(world);
	
	//SetCollider(graphicsObject->getModel());

	//SceneManager::RegisterForInput(this,AZUL_KEY::KEY_H,KeyEvent::KeyDown);
	//RegisterForUpdate();
	//SceneManager::RegisterForUpdating(this);
	//SceneManager::RegisterForDrawing(this);
	
	//RegisterForAlarm(AlarmID_0,1000);
	/*
	RegisterForAlarm(AlarmID_1,3);
	RegisterForAlarm(AlarmID_1,10);
	RegisterForAlarm(AlarmID_1,7);
	RegisterForAlarm(AlarmID_1,3);
	RegisterForAlarm(AlarmID_2,7);
	
	DeregisterForAlarm(AlarmID_1);	
	DeregisterForAlarm(AlarmID_1);
	DeregisterForAlarm(AlarmID_1);	
	DeregisterForAlarm(AlarmID_1);
	DeregisterForAlarm(AlarmID_1);	
	DeregisterForAlarm(AlarmID_1);
	*/
	// Spaceship
		ShipScale.set( SCALE, 0.5f, 0.5f, 0.5f);
		ShipRot.set( ROT_XYZ, 0, 0, 0);
		ShipPos.set(0, 0, 40);
		Matrix world = ShipScale * ShipRot * Matrix( TRANS, ShipPos );
		//graphicsObject->setWorld(world);
		//colWorld = world
		SetGameObjectWorld(world);

		this->RegisterToDraw();

	RegisterCollision(this);
}


 void Ship::Draw(){

	 graphicsObject->Render();

	
	Matrix m_base = Matrix(SCALE,graphicsObject->getModel()->radius,graphicsObject->getModel()->radius,graphicsObject->getModel()->radius) *
		Matrix(TRANS,graphicsObject->getModel()->center);
	Matrix M = m_base * graphicsObject->getWorld();
	
	s->setWorld( M );

	s->Render();
}

 void Ship::Alarm0(){
	 //DeregisterForAlarm(AlarmID_1);
	cout<<"SHIP: ALARM 0 triggered (1 seconds)"<<endl;
 }

 void Ship::Alarm1(){

	 cout<<"SHIP: ALARM 1 triggered (3 seconds)"<<endl;
 }

 

 void Ship::Update(){

	//Ship Translation movement
	if (  InputManager::IsKeyPressed(AZUL_KEY::KEY_W))
	{
		ShipPos += Vect(0,0,1) * ShipRot * ShipTransSpeed;
	}
	else if ( InputManager::IsKeyPressed(AZUL_KEY::KEY_S) )
	{
		ShipPos += Vect(0,0,1) * ShipRot * - ShipTransSpeed;
	}

	// Ship Rotation movement (not using time-based values for simplicity)
	if ( InputManager::IsKeyPressed(AZUL_KEY::KEY_A) )
	{
		ShipRot *= Matrix( ROT_Y, ShipRotAng );
	}
	else if ( InputManager::IsKeyPressed(AZUL_KEY::KEY_D))
	{
		ShipRot *= Matrix( ROT_Y, -ShipRotAng );
	}

	// Spaceship adjust matrix
	world =  ShipScale * ShipRot * Matrix( TRANS, ShipPos );
	SetGameObjectWorld(world);

}

 void Ship::KeyDown(AZUL_KEY key){
	 if(key ==AZUL_KEY::KEY_W){
		
		cout<<"GAMEOBJECT "<<this<<" : "<<key<<" has been pressed Down"<<endl;
	}
	
 }

 void Ship::KeyUp(AZUL_KEY key){
	key;
	if(key ==AZUL_KEY::KEY_SPACE){
		Ship* cool = new Ship();
		cool;
		cout<<"GAMEOBJECT "<<this<<" : "<<key<<" has been Released"<<endl;
	}
 }

 Ship::~Ship(){
	 cout<<"~ship()";
 }

 void Ship::OnTerminate(){

	 //ship owns graphics Object
	 delete graphicsObject;
	 //this->DeregisterForUpdate();
	 DeregisterForInput(AZUL_KEY::KEY_H,KeyEvent::KeyDown);
	 DeregisterForInput(AZUL_KEY::KEY_H,KeyEvent::KeyUp);
	// DeregisterForUpdate();
	//SceneManager::DeregisterForUpdating(this);
	 //SceneManager::RegisterForInput(this,AZUL_KEY::KEY_H,KeyEvent::KeyDown);
	// SceneManager::DeregisterForInput(this);
	 //SceneManager::DeregisterForDrawing(this);
	 DeregisterCollision(this);
	// cout<<"Termination callback called";
	
 }

 void Ship::Collision(WeirdShape * p){
	 p;
	
	p->MarkForDestroy();
	cout<<"Ship collision with WeirdShape"<<endl;
	s->color = Vect(1,1,1);
	MarkForDestroy();
	SceneManager::ChangeScene(new Level2());

 }

  void Ship::Collision(Ship * p){

	p;
	cout<<"Ship collision with Ship"<<endl;
	p->MarkForDestroy();
	s->color = Vect(1,1,1);
	

 }