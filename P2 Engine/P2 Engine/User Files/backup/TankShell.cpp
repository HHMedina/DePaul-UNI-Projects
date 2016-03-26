
#include "TankShell.h"
#include "MathEngine.h"
#include "SoundSystem.h"

TankShell::TankShell(Vect& position,Matrix& rotation){

	speed =4.0f;
	
	SoundSystem::PlayExample();
	//graphicsObject = GraphicsObjectHandler::CreateGraphicalObject(AssetManager::GetModel("Cottage"));
	SetGraphicAndCollisionModel(GraphicsObjectMaker::CreateGraphicsObjectFlatTexture(AssetManager::GetModel("TankShellModel"),AssetManager::GetTexture("BombTexture")) );
	SetAlarm(AlarmID_0,10);
	colVolume = new AABBBoundingBox(this);

	Matrix temp = Matrix();
	tankShellScale.set( SCALE, 10.0f, 10.0f, 10.0f);
	tankShellRot = rotation;
	tankShellPos = position;
	temp = tankShellScale * tankShellRot * Matrix( TRANS, tankShellPos );
	SetGameObjectWorld(temp);
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
	tankShellScale.set( SCALE, 10.0f, 10.0f, 10.0f);
	//tankShellRot.set( ROT_XYZ, rotation[x],rotation[y] , rotation[z]);
	tankShellPos = Vect(x,y,z);
	temp = tankShellScale * tankShellRot * Matrix( TRANS, tankShellPos );
	SetGameObjectWorld(temp);
	RegisterCollision(this);
}

void TankShell::Update(){

	tankShellPos += Vect(0,0,1) *tankShellRot* speed;
	Matrix temp = Matrix();
	temp = tankShellScale * tankShellRot * Matrix( TRANS, tankShellPos );
	SetGameObjectWorld(temp);
	//cout<<GetTimeRemainingOfAlarm(AlarmID_0)<<endl;
}

void TankShell::OnTerminate(){
	DeregisterCollision(this);
}

void TankShell::Draw(){
	RenderGraphics();
#ifdef Debug
	colVolume->debugWireFrame->Render();
#endif
}

