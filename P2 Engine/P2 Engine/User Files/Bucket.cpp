#include "Bucket.h"
#include "Bomb.h"
#include "GameController.h"
#include "RecyclingBombFactory.h"

const float Bucket::bucketMovementSpeed = .5f;
const float Bucket::bucketTurningSpeed = .05f;

Bucket::Bucket(float xPos , float yPos, float zPos){

	SetGraphicAndCollisionModel(GraphicsObjectMaker::CreateGraphicsObjectFlatTexture(AssetManager::GetModel("BucketModel"),AssetManager::GetTexture("BucketTexture")   ));
	RegisterForInput(AZUL_KEY::KEY_SPACE,KeyEvent::KeyDown);
	DeregisterForInput(AZUL_KEY::KEY_SPACE,KeyEvent::KeyDown);
	Matrix temp = Matrix();
	bucketScale.set( SCALE, .25f, .25f, .25f);
	bucketRot.set( ROT_XYZ, 0, 0, 0);
	bucketPos = Vect(xPos,yPos,zPos);
	temp = bucketScale * bucketRot * Matrix( TRANS, bucketPos );
	SetGameObjectWorld(temp);
	RegisterCollision(this);
}

Bucket::~Bucket(){
}

void Bucket::Update(){

	
	//bucket Translation movement
	if (  InputManager::IsKeyPressed(AZUL_KEY::KEY_W))
	{
		bucketPos += Vect(0,0,1) * bucketRot * bucketMovementSpeed;
	}
	else if ( InputManager::IsKeyPressed(AZUL_KEY::KEY_S) )
	{
		bucketPos += Vect(0,0,1) * bucketRot * - bucketMovementSpeed;
	}

	// Ship Rotation movement (not using time-based values for simplicity)
	if ( InputManager::IsKeyPressed(AZUL_KEY::KEY_A) )
	{
		bucketRot *= Matrix( ROT_Y, this->bucketTurningSpeed );
	}
	else if ( InputManager::IsKeyPressed(AZUL_KEY::KEY_D))
	{

		bucketRot *= Matrix( ROT_Y, -this->bucketTurningSpeed );
	}

	// adjust the position of the gameobject
	Matrix tempWorld = Matrix();
	tempWorld =  bucketScale * bucketRot * Matrix( TRANS, bucketPos );
	SetGameObjectWorld(tempWorld);// updates the visual mesh and collision mesh

}

void Bucket::Collision(Bomb*){

	GameController::IncrementBombsDiffused();
	GameController::SpawnRandomBomb();
}

void Bucket::OnTerminate(){
	
	DeregisterForInput(AZUL_KEY::KEY_SPACE,KeyEvent::KeyDown);
	DeregisterCollision(this);

}

void Bucket::Draw(){
	RenderGraphics();
}

void Bucket::KeyDown(AZUL_KEY key){

	if(key==AZUL_KEY::KEY_SPACE){
		RecyclingBombFactory::CreateBomb(50,0,50);
	}

	//if(key==AZUL_KEY::KEY_L){
	//	new Bomb(50,0,50);
	//}

}