#include <WeirdShape.h>
#include <iostream>
#include <Ship.h>

Matrix WeirdShapeScale;
Vect WeirdShapePos;
Matrix WeirdShapeRot;

WeirdShape::WeirdShape(){
	
	graphicsObject = new GraphicsObjectFlatTexture(AssetManager::GetModel("WeirdShape"),AssetManager::GetTexture("ShipTex"));
	collisionModel = AssetManager::GetModel("WeirdShape");

	WeirdShapeScale.set( SCALE, .25f, .25f, .25f);
	WeirdShapeRot.set( ROT_XYZ, 80, 22, 34);
	WeirdShapePos.set(0, 0, 0);
	Matrix world = WeirdShapeScale * WeirdShapeRot * Matrix( TRANS, WeirdShapePos );
	graphicsObject->setWorld(world);
	colWorld = world;

	//SceneManager::RegisterForInput(this,AZUL_KEY::KEY_H,KeyEvent::KeyDown);
	//SceneManager::RegisterForDrawing(this);
	SceneManager::RegisterForUpdating(this);
	
	//RegisterForAlarm(AlarmID_0,5);
	
	RegisterCollision<WeirdShape>(this);
}

 void WeirdShape::Draw(){

	graphicsObject->Render();

	GraphicsObjectWireFrame* s = new GraphicsObjectWireFrame(GpuModel::Create("BoundingSphere.azul"));
	Matrix m_base = Matrix(SCALE,graphicsObject->getModel()->radius,graphicsObject->getModel()->radius,graphicsObject->getModel()->radius) *
		Matrix(TRANS,graphicsObject->getModel()->center);
	Matrix M = m_base * graphicsObject->getWorld();
	
	s->setWorld( M );

	s->Render();
}

 void WeirdShape::Alarm0(){

	 cout<<"WeirdShape: Alarm0 has Activated"<<endl;
 }

 void WeirdShape::Update(){

	//cout<<"UPDATING!"<<endl;

}

 void WeirdShape::KeyDown(AZUL_KEY key){

	cout<<"GAMEOBJECT "<<this<<" : "<<key<<" has been pressed Down"<<endl;
	
 }

 void WeirdShape::KeyUp(AZUL_KEY key){
	key;
 }

  void WeirdShape::OnTerminate(){
	  
	 //ship owns graphics Object
	 delete graphicsObject;

	 SceneManager::DeregisterForUpdating(this);
	 SceneManager::DeregisterForDrawing(this);
	 DeregisterCollision<WeirdShape>(this);
	 
 }
