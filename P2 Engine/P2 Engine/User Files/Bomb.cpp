#include "Bomb.h"
#include "GameController.h"

Bomb::Bomb(float xPos , float yPos, float zPos){

	//graphicsObject = GraphicsObjectHandler::CreateGraphicalObject(AssetManager::GetModel("Cottage"));
	SetGraphicAndCollisionModel(GraphicsObjectMaker::CreateGraphicsObjectFlatTexture(AssetManager::GetModel("BombModel"),AssetManager::GetTexture("BombTexture")) );
	SetAlarm(AlarmID_0,10);
	colVolume = new BoundingSphereVolume(this);

	Matrix temp = Matrix();
	bombScale.set( SCALE, 10.0f, 10.0f, 10.0f);
	bombRot.set( ROT_XYZ, 0, 0, 0);
	bombPos = Vect(xPos,yPos,zPos);
	temp = bombScale * bombRot * Matrix( TRANS, bombPos );
//	SetGameObjectWorld(temp);
	RegisterCollision(this);
}

Bomb::~Bomb(){
	//Note: delete isnt called on collision model because asset manager owns model.
	//Note: Delete is not called on graphics object because we do not own graphics object.
	//Unless if a graphicsobject of any type is called using new and assigned into graphicsObject
	//member.
}


void Bomb::Initialize(float x, float y, float z){
	RegisterForAutomated();
	SetAlarm(AlarmID_0,10);

	Matrix temp = Matrix();
	bombScale.set( SCALE, 10.0f, 10.0f, 10.0f);
	bombRot.set( ROT_XYZ, 0, 0, 0);
	bombPos = Vect(x,y,z);
	temp = bombScale * bombRot * Matrix( TRANS, bombPos );
//	SetGameObjectWorld(temp);
	RegisterCollision(this);
}

void Bomb::Update(){
	//cout<<GetTimeRemainingOfAlarm(AlarmID_0)<<endl;
}

void Bomb::Collision(Bucket* other){

	other;

	//if bucket has diffused bomb (by touching it) cancel the alarm. (cancel the explosion)
	CancelAlarm(AlarmID_0);

	MarkForDestroy();//destroy self.
}

void Bomb::OnTerminate(){
	this->CancelAlarm(AlarmID_0);
	DeregisterCollision(this);
}

void Bomb::Draw(){
	RenderGraphics();
}

void Bomb::Alarm0(){

	//bomb has activated. You lose.
	GameController::GoToLoseScreen();

}
