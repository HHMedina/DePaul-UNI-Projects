#include "Bucket.h"
#include "Bomb.h"
#include "GameController.h"
#include "RecyclingBombFactory.h"
#include "BoundingSphereVolume.h"
#include "DebugVisualizer.h"
#include "LoseScene.h"

const float Bucket::bucketMovementSpeed = 1.0f;
const float Bucket::bucketTurningSpeed = .05f;

Bucket::Bucket(float xPos , float yPos, float zPos){



	RegisterForInput(AZUL_KEY::KEY_SPACE,KeyEvent::KeyDown);
	Matrix temp = Matrix();
	bucketScale.set( SCALE, 1, 1, 1);
	bucketRot.set( ROT_XYZ, 0, 0, 0);
	bucketPos = Vect(xPos,yPos,zPos);
	temp = bucketScale * bucketRot * Matrix( TRANS, bucketPos );
//	SetGameObjectWorld(temp);
	//SetGameObjectPosition(position);
	SetGraphicAndCollisionModel(GraphicsObjectMaker::CreateGraphicsObjectFlatTexture(AssetManager::GetModel("BucketModel"),AssetManager::GetTexture("BucketTexture")   ) , AABBBoxVolume);
	
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
		//SceneManager::ChangeScene(new LoseScene());
		bucketRot *= Matrix( ROT_Y, -this->bucketTurningSpeed );
	}
	else if ( InputManager::IsKeyPressed(AZUL_KEY::KEY_UP))
	{
		
		SceneManager::SetCurrentTerrain("TerTestTerrain2");
		bucketPos += Vect(0,1,0) * bucketRot * bucketMovementSpeed;
	}
	else if ( InputManager::IsKeyPressed(AZUL_KEY::KEY_DOWN))
	{
		SceneManager::SetCurrentTerrain("TerTestTerrain");
		bucketPos += Vect(0,-1,0) * bucketRot * bucketMovementSpeed;
	}

	CameraMan::GetCurrent()->setOrientAndPosition( 
													Vect(0,1,0),
													Vect(0,50,0) * bucketRot * Matrix( TRANS, bucketPos  ), 
													Vect(0, 50, -200) * bucketRot* Matrix( TRANS, bucketPos  ) );
	CameraMan::GetCurrent()->updateCamera();

	// adjust the position of the gameobject
	Matrix tempWorld = Matrix();
	tempWorld =  bucketScale * bucketRot * Matrix( TRANS, bucketPos );
//	SetGameObjectWorld(tempWorld);// updates the visual mesh and collision mesh
	//DebugVisualizer::ShowPoint(Vect(position[x],0,position[z]));
	//SetGameObjectPosition(bucketPos);
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

void Bucket::TerrainCollision(){
	cout<<this<<"BUCKET COLLISION WITH TERRAIN!"<<endl;
}

void Bucket::KeyDown(AZUL_KEY key){

	if(key==AZUL_KEY::KEY_SPACE){
		//SoundSystem::PlayExample();
		//RecyclingBombFactory::CreateBomb(50,0,50);
	}

	//if(key==AZUL_KEY::KEY_L){
	//	new Bomb(50,0,50);
	//}

}