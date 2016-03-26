#include "TankBottom.h"
#include "MathEngine.h"
#include "SoundSystem.h"
#include "Tank.h"

TankBottom::TankBottom(Vect& position,Vect& scale,Vect& rotation, Tank* owner){
	DeregisterForUpdate();
	myOwner =owner;
	CamOffset = Vect(0, 70, -150);
	
	speed =1.5f;
	turningSpeed = .015f;

	//graphicsObject = GraphicsObjectHandler::CreateGraphicalObject(AssetManager::GetModel("Cottage"));
	
	

	Matrix temp = Matrix();
	tankBottomScale.set( SCALE, scale[x], scale[y], scale[z]);
	tankBottomRot.set( ROT_XYZ, rotation[x],rotation[y] , rotation[z]);
	tankBottomPos = position;
	temp = tankBottomScale * tankBottomRot * Matrix( TRANS, tankBottomPos );
	
	SetGraphicAndCollisionModel(GraphicsObjectMaker::CreateGraphicsObjectFlatTexture(AssetManager::GetModel("TankBottomModel"),AssetManager::GetTexture("TankBottomTexture")),AABBBoxVolume );
	SetGameObjectWorld(position,scale,rotation);
	RegisterCollision(this);
}

TankBottom::TankBottom(Vect& position,Vect& scale,Vect& rotation){
	DeregisterForUpdate();
	CamOffset = Vect(0, 70, -150);
	
	speed =1.5f;
	turningSpeed = .015f;

	//graphicsObject = GraphicsObjectHandler::CreateGraphicalObject(AssetManager::GetModel("Cottage"));
	
	

	Matrix temp = Matrix();
	tankBottomScale.set( SCALE, scale[x], scale[y], scale[z]);
	tankBottomRot.set( ROT_XYZ, rotation[x],rotation[y] , rotation[z]);
	tankBottomPos = position;
	temp = tankBottomScale * tankBottomRot * Matrix( TRANS, tankBottomPos );
//	SetGameObjectWorld(temp);
	SetGraphicAndCollisionModel(GraphicsObjectMaker::CreateGraphicsObjectFlatTexture(AssetManager::GetModel("TankBottomModel"),AssetManager::GetTexture("TankBottomTexture")),AABBBoxVolume );
	RegisterCollision(this);
	SetGameObjectWorld(position,scale,rotation);


}
 
TankBottom::~TankBottom(){
	//Note: delete isnt called on collision model because asset manager owns model.
	//Note: Delete is not called on graphics object because we do not own graphics object.
	//Unless if a graphicsobject of any type is called using new and assigned into graphicsObject
	//member.
}


void TankBottom::Initialize(float x, float y, float z){
	RegisterForAutomated();
	DeregisterForUpdate();

	SetAlarm(AlarmID_0,10);

	Matrix temp = Matrix();
	tankBottomScale.set( SCALE, 1.0f, 1.0f, 1.0f);
	//tankShellRot.set( ROT_XYZ, rotation[x],rotation[y] , rotation[z]);
	tankBottomPos = Vect(x,y,z);
	temp = tankBottomScale * tankBottomRot * Matrix( TRANS, tankBottomPos );
//	SetGameObjectWorld(temp);
	RegisterCollision(this);
}


void TankBottom::OnTerminate(){
	DeregisterCollision(this);
}

void TankBottom::Draw(){
	RenderGraphics();
#ifdef Debug
	colVolume->debugWireFrame->Render();
#endif
}