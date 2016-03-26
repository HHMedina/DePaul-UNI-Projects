
#include "TankShell.h"
#include "MathEngine.h"
#include "SoundSystem.h"
#include "Sound3D.h"

TankShell::TankShell(Vect& position,Matrix& rotation){
	hit=false;
	tankShot = AssetManager::GetSound3DInstance("TankShotSound");
	//tankShot->SetLooped(true);
	tankShot->Play();
	//SetAudibleSound("TankShotSound");
	//SetAudibleLooped(true);
	//PlayAudibleSound();//SOMETHING WRONG HERE
	//auseAudibleSound();
	//StopAudibleSound();//supicious
	/*
	shotFireSound = new Sound("GunShotSound");
	shotFire2Sound = new Sound("GunShotSound");
	shotFire2Sound->SetVolume(.1f);
	*/
	//SetAlarm(AlarmID_0,5);
	speed =.5f;
	static int hu =0;
	tankShellPos = Vect(0,0,0);
	/*
	++hu;
	if(hu%2==0){
		
		shotFireSound->PlayLooped();
	}else {
		shotFire2Sound->PlayLooped();
	}
	*/
	
	Matrix temp = Matrix();
	tankShellScale.set( SCALE, 1.0f, 1.0f, 1.0f);
	tankShellRot = rotation;
	tankShellPos = position - Vect(0,position[y],0);
	temp = tankShellScale * tankShellRot * Matrix( TRANS, tankShellPos );
//	SetGameObjectWorld(temp);
	SetGraphicAndCollisionModel(GraphicsObjectMaker::CreateGraphicsObjectFlatTexture(AssetManager::GetModel("TankShellModel"),AssetManager::GetTexture("BombTexture")),OOBBVolume);

	RegisterCollision(this);
}

TankShell::~TankShell(){
	//Note: delete isnt called on collision model because asset manager owns model.
	//Note: Delete is not called on graphics object because we do not own graphics object.
	//Unless if a graphicsobject of any type is called using new and assigned into graphicsObject
	//member.
}


void TankShell::Initialize(float x, float y, float z){
	RegisterForAutomated();
	SetAlarm(AlarmID_0,10);

	Matrix temp = Matrix();
	tankShellScale.set( SCALE, 1.0f, 1.0f, 1.0f);
	//tankShellRot.set( ROT_XYZ, rotation[x],rotation[y] , rotation[z]);
	tankShellPos = Vect(x,y,z);
	temp = tankShellScale * tankShellRot * Matrix( TRANS, tankShellPos );
	SetGameObjectWorld(tankShellPos,Vect(.1f,.1f,.1f),Vect(0,0,1)* tankShellRot);
	RegisterCollision(this);
}

void TankShell::Update(){

	tankShellPos += Vect(0,0,1) *tankShellRot* speed;
	Matrix temp = Matrix();
	temp = tankShellScale * tankShellRot * Matrix( TRANS, tankShellPos );
	SetGameObjectWorld(tankShellPos,Vect(10.0f,10.0f,10.0f),Vect(0,0,0));
	//tankShot->SetSoundPosition(tankShellPos);
	//	SetGameObjectWorld(temp);
	//cout<<GetTimeRemainingOfAlarm(AlarmID_0)<<endl;
}

void TankShell::OnTerminate(){
	
	DeregisterCollision(this);
}

void TankShell::Draw(){
	
	colVolume->DrawColVolume(Vect(0,0,1));

	if(hit){
		hit=true;
		
		//DebugVisualizer::ShowAABB(colVolume->GetCheapMinAABBPoint(),colVolume->GetCheapMaxAABBPoint(),Vect(1,0,0));
		//DebugVisualizer::ShowOBB(colVolume->GetCheapMinAABBPoint(),colVolume->GetCheapMaxAABBPoint(),colWorld ,Vect(1,0,0));
		//DebugVisualizer::ShowSphere(colVolume->GetCheapCenter(), colVolume->GetCheapSphereRadius(), Vect(1,0,0));
	}else{
		//DebugVisualizer::ShowAABB(colVolume->GetCheapMinAABBPoint(),colVolume->GetCheapMaxAABBPoint(),Vect(0,0,1));
		//DebugVisualizer::ShowAABB(colVolume->GetCheapMinAABBPoint(),colVolume->GetCheapMaxAABBPoint(),Vect(0,0,1));
		//DebugVisualizer::ShowSphere(colVolume->GetCheapCenter(), colVolume->GetCheapSphereRadius(), Vect(0,0,1));
	}
	hit =false;
	RenderGraphics();


}

