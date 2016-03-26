#include "TankTop.h"
#include "MathEngine.h"
#include "SoundSystem.h"
#include "Tank.h"
#include "TankShell.h"

TankTop::TankTop(Vect& position,Vect& scale,Vect& rotation,Tank* owner){
	hit =false;
	//SoundSystem::SetMainVolume(1.0f);
	//DeregisterFromAutomated();
	DeregisterToDraw();
	myOwner = owner;
	Matrix temp = Matrix();
	tankTopScale.set( SCALE, scale[x], scale[y], scale[z]);
	tankTopRot.set( ROT_XYZ, rotation[x],rotation[y] , rotation[z]);
	tankTopPos = position;
	temp = tankTopScale * tankTopRot * Matrix( TRANS, tankTopPos );
//	SetGameObjectWorld(temp);
	SetGraphicAndCollisionModel(GraphicsObjectMaker::CreateGraphicsObjectFlatTexture(AssetManager::GetModel("TankTopModel"),AssetManager::GetTexture("TankTopTexture")), OOBBVolume);


	RegisterCollision(this);
}

TankTop::~TankTop(){
	//Note: delete isnt called on collision model because asset manager owns model.
	//Note: Delete is not called on graphics object because we do not own graphics object.
	//Unless if a graphicsobject of any type is called using new and assigned into graphicsObject
	//member.
}

void TankTop::Update(){

	

	
}

void TankTop::Initialize(float x, float y, float z){
	//RegisterForAutomated();
	DeregisterForUpdate();

	Matrix temp = Matrix();
	tankTopScale.set( SCALE, 10.0f, 10.0f, 10.0f);
	//tankShellRot.set( ROT_XYZ, rotation[x],rotation[y] , rotation[z]);
	tankTopPos = Vect(x,y,z);
	temp = tankTopScale * tankTopRot * Matrix( TRANS, tankTopPos );
//	SetGameObjectWorld(temp);
	tankTopScale.set( SCALE, 5000.0f, 5000.0f, 5000.0f);
	
	temp = tankTopScale * tankTopRot * Matrix( TRANS, tankTopPos );

	RegisterCollision(this);
}

void TankTop::OnTerminate(){
	DeregisterCollision(this);
}

void TankTop::Draw(){
	
	
	RenderGraphics();
	if(hit){
		hit=true;
		colVolume->DrawColVolume(Vect(1,0,0));
		//DebugVisualizer::ShowAABB(colVolume->GetCheapMinAABBPoint(),colVolume->GetCheapMaxAABBPoint(),Vect(1,0,0));
		//DebugVisualizer::ShowOBB(colVolume->,colVolume->GetCheapMaxAABBPoint(),colWorld ,Vect(1,0,0));
		//DebugVisualizer::ShowSphere(colVolume->GetCheapCenter(), colVolume->GetCheapSphereRadius(), Vect(1,0,0));
	}else{
		colVolume->DrawColVolume(Vect(0,0,1));
		//DebugVisualizer::ShowAABB(colVolume->GetCheapMinAABBPoint(),colVolume->GetCheapMaxAABBPoint(),Vect(0,0,1));
		//DebugVisualizer::ShowAABB(colVolume->GetCheapMinAABBPoint(),colVolume->GetCheapMaxAABBPoint(),Vect(0,0,1));
		//DebugVisualizer::ShowSphere(colVolume->GetCheapCenter(), colVolume->GetCheapSphereRadius(), Vect(0,0,1));
	}
	hit =false;

}

void TankTop::Collision(TankShell* bullet){
	//SoundSystem::StopAllSounds();
 	hit =true;
	bullet;
	//bullet->MarkForDestroy();
	//myOwner->AdjustHP(-1);

}