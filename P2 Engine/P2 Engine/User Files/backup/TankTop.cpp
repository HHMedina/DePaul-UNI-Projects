#include "TankTop.h"
#include "MathEngine.h"
#include "SoundSystem.h"
#include "Tank.h"
#include "TankShell.h"

TankTop::TankTop(Vect& position,Vect& scale,Vect& rotation,Tank* owner){
	//DeregisterForUpdate();
	myOwner = owner;
	//graphicsObject = GraphicsObjectHandler::CreateGraphicalObject(AssetManager::GetModel("Cottage"));
	SetGraphicAndCollisionModel(GraphicsObjectMaker::CreateGraphicsObjectFlatTexture(AssetManager::GetModel("TankTopModel"),AssetManager::GetTexture("TankTopTexture")) );
	colVolume = new AABBBoundingBox(this);//BoundingSphereVolume(this);
	//debugWireFrame = GraphicsObjectMaker::CreateGraphicsObjectWireFrame(GpuModel::Create("BoundingBox.azul"));

	Matrix temp = Matrix();
	tankTopScale.set( SCALE, scale[x], scale[y], scale[z]);
	tankTopRot.set( ROT_XYZ, rotation[x],rotation[y] , rotation[z]);
	tankTopPos = position;
	temp = tankTopScale * tankTopRot * Matrix( TRANS, tankTopPos );
	SetGameObjectWorld(temp);
	RegisterCollision(this);
}

TankTop::~TankTop(){
	//Note: delete isnt called on collision model because asset manager owns model.
	//Note: Delete is not called on graphics object because we do not own graphics object.
	//Unless if a graphicsobject of any type is called using new and assigned into graphicsObject
	//member.
}

void TankTop::Update(){

	//tankTopScale.set( SCALE, scale[x], scale[y], scale[z]);
	//tankTopRot.set( ROT_XYZ, rotation[x],rotation[y] , rotation[z]);
	//tankTopPos = position;
	//debugAABB->setWorld();
	//Matrix boxScale;
	//boxScale.set( SCALE, 50.0f, 50.0f, 50.0f);
	//Matrix temp = boxScale * tankTopRot * Matrix( TRANS, tankTopPos );
	//debugWireFrame->setWorld(temp);
	
}

void TankTop::Initialize(float x, float y, float z){
	RegisterForAutomated();
	//DeregisterForUpdate();

	Matrix temp = Matrix();
	tankTopScale.set( SCALE, 10.0f, 10.0f, 10.0f);
	//tankShellRot.set( ROT_XYZ, rotation[x],rotation[y] , rotation[z]);
	tankTopPos = Vect(x,y,z);
	temp = tankTopScale * tankTopRot * Matrix( TRANS, tankTopPos );
	SetGameObjectWorld(temp);
	tankTopScale.set( SCALE, 5000.0f, 5000.0f, 5000.0f);
	
	temp = tankTopScale * tankTopRot * Matrix( TRANS, tankTopPos );

	RegisterCollision(this);
}

void TankTop::OnTerminate(){
	DeregisterCollision(this);
}

void TankTop::Draw(){
	RenderGraphics();
#ifdef Debug
	colVolume->debugWireFrame->Render();
#endif
}

void TankTop::Collision(TankShell* bullet){
	//bullet->MarkForDestroy();
	//myOwner->AdjustHP(-1);

}