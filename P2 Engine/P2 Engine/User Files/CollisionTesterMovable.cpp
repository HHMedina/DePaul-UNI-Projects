
#include "CollisionTesterMovable.h"

CollisionTesterMovable::CollisionTesterMovable(Vect& position,Vect& scl,Vect& rotation,float spd){
	
	CamOffset = Vect(0,4,-15);
	
	hit=false;
	speed = spd;
	rotSpeed =2;

	pos = position;
	scale = scl;
	rot =rotation;

	SetGraphicAndCollisionModel(GraphicsObjectMaker::CreateGraphicsObjectFlatTexture(AssetManager::GetModel("SpaceShip"),AssetManager::GetTexture("BombTexture")),AABBBoxVolume);
	SetGameObjectWorld(pos,scale,rot);
	RegisterCollision(this);
}

CollisionTesterMovable::~CollisionTesterMovable(){
	//Note: delete isnt called on collision model because asset manager owns model.
	//Note: Delete is not called on graphics object because we do not own graphics object.
	//Unless if a graphicsobject of any type is called using new and assigned into graphicsObject
	//member.
}


void CollisionTesterMovable::Update(){

	Matrix rotm = Matrix();
	rotm.set(ROT_XYZ,rot[x],rot[y],rot[z]);

	if(InputManager::IsKeyPressed(AZUL_KEY::KEY_W)){
		pos += Vect(0,0,1) *rotm*speed;
	}
	if(InputManager::IsKeyPressed(AZUL_KEY::KEY_S)){
		pos -= Vect(0,0,1) *rotm*speed;
	}

	if(InputManager::IsKeyPressed(AZUL_KEY::KEY_A)){
		rot += Vect(0,P2Math::DegToRad(rotSpeed),0);
	}

	if(InputManager::IsKeyPressed(AZUL_KEY::KEY_D)){
		rot -= Vect(0,P2Math::DegToRad(rotSpeed),0);
	}

	//pos += Vect(0,0,1) *rotm*speed;
	SetGameObjectWorld(pos,scale,rot);

	CameraMan::GetCurrent()->setOrientAndPosition( 
													Vect(0,1,0),
													pos, 
													CamOffset + pos );
	CameraMan::GetCurrent()->updateCamera();
}


void CollisionTesterMovable::Draw(){
	
	

	if(hit){
		hit=true;
		colVolume->DrawColVolume(Vect(1,0,0));
		//DebugVisualizer::ShowAABB(colVolume->GetCheapMinAABBPoint(),colVolume->GetCheapMaxAABBPoint(),Vect(1,0,0));
		//DebugVisualizer::ShowOBB(colVolume->GetCheapMinAABBPoint(),colVolume->GetCheapMaxAABBPoint(),colWorld ,Vect(1,0,0));
		//DebugVisualizer::ShowSphere(colVolume->GetCheapCenter(), colVolume->GetCheapSphereRadius(), Vect(1,0,0));
	}else{
		colVolume->DrawColVolume(Vect(0,0,1));
		//DebugVisualizer::ShowAABB(colVolume->GetCheapMinAABBPoint(),colVolume->GetCheapMaxAABBPoint(),Vect(0,0,1));
		//DebugVisualizer::ShowAABB(colVolume->GetCheapMinAABBPoint(),colVolume->GetCheapMaxAABBPoint(),Vect(0,0,1));
		//DebugVisualizer::ShowSphere(colVolume->GetCheapCenter(), colVolume->GetCheapSphereRadius(), Vect(0,0,1));
	}
	hit =false;
	RenderGraphics();


}