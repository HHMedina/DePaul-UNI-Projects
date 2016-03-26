
#include "CollisionTester.h"

CollisionTester::CollisionTester(Vect& position,Vect& scl,Vect& rotation,float spd){
	hit=false;
	speed = spd;

	pos = position;
	scale = scl;
	rot =rotation;

	SetGraphicAndCollisionModel(GraphicsObjectMaker::CreateGraphicsObjectFlatTexture(AssetManager::GetModel("TankShellModel"),AssetManager::GetTexture("BombTexture")),AABBBoxVolume);
	SetGameObjectWorld(pos,scale,rot);
	RegisterCollision(this);
}

CollisionTester::~CollisionTester(){
	//Note: delete isnt called on collision model because asset manager owns model.
	//Note: Delete is not called on graphics object because we do not own graphics object.
	//Unless if a graphicsobject of any type is called using new and assigned into graphicsObject
	//member.
}


void CollisionTester::Update(){

	Matrix rotm = Matrix();
	rotm.set(ROT_XYZ,rot[x],rot[y],rot[z]);

	pos += Vect(0,0,1) *rotm*speed;
	SetGameObjectWorld(pos,scale,rot);
}


void CollisionTester::Draw(){
	
	

	if(hit){
		hit=true;
		//colVolume->DrawColVolume(Vect(1,0,0));
		//DebugVisualizer::ShowAABB(colVolume->GetCheapMinAABBPoint(),colVolume->GetCheapMaxAABBPoint(),Vect(1,0,0));
		//DebugVisualizer::ShowOBB(colVolume->GetCheapMinAABBPoint(),colVolume->GetCheapMaxAABBPoint(),colWorld ,Vect(1,0,0));
		//DebugVisualizer::ShowSphere(colVolume->GetCheapCenter(), colVolume->GetCheapSphereRadius(), Vect(1,0,0));
	}else{
		//colVolume->DrawColVolume(Vect(0,0,1));
		//DebugVisualizer::ShowAABB(colVolume->GetCheapMinAABBPoint(),colVolume->GetCheapMaxAABBPoint(),Vect(0,0,1));
		//DebugVisualizer::ShowAABB(colVolume->GetCheapMinAABBPoint(),colVolume->GetCheapMaxAABBPoint(),Vect(0,0,1));
		//DebugVisualizer::ShowSphere(colVolume->GetCheapCenter(), colVolume->GetCheapSphereRadius(), Vect(0,0,1));
	}
	hit =false;
	RenderGraphics();


}