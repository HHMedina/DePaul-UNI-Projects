#include <../Game.h>
#include "Level1.h"
#include "TutorialLevel1.h"
#include "DeathMatchLevel.h"
#include "WinScene.h"
#include "LoseScene.h"
#include "GameController.h"
#include "RecyclingBombFactory.h"
#include "AlarmTestLevel.h"
#include "SoundTestLevel.h"

void Game::InitializeContext(){

}

void Game::LoadGameContent(){
	// Model loading ---------------------------------------

		// Load external models -------------------------------
		AssetManager::LoadModel("space_frigate.fbx","SpaceShip");
		//AssetManager::LoadModel("WeirdShape.fbx","WeirdShape");
		AssetManager::LoadModel("bucket.fbx","BucketModel");
		AssetManager::LoadModel("Bomb.fbx","BombModel");
		//AssetManager::LoadModel("Axis.fbx","Axis");
		AssetManager::LoadModel("Plane.fbx","Plane");
		//AssetManager::LoadModel("Win.fbx","WinModel");
		//AssetManager::LoadModel("Lose.fbx","LoseModel");
		AssetManager::LoadModel("TankBottom.fbx","TankBottomModel");
		AssetManager::LoadModel("TankTop.fbx","TankTopModel");
		AssetManager::LoadModel("TankShellTest.fbx","TankShellModel");
		//AssetManager::LoadModel("Cottage.fbx","Cottage");
		AssetManager::LoadAzulModel("BoundingSphere.azul","BoundingSphereModel");

	// Texture Loading  --------------------------------

		AssetManager::LoadTexture("DesertFloorTex.tga","DesertFloorTexture");
		AssetManager::LoadTexture("TankBottom.tga","TankBottomTexture");
		AssetManager::LoadTexture("TankTop.tga","TankTopTexture");
		AssetManager::LoadTexture("bombTexture.tga","BombTexture");
		AssetManager::LoadTexture("bucketTexture.tga","BucketTexture");
		//AssetManager::LoadTexture("Test.tga","ShipTex");
		//AssetManager::LoadTexture("space_frigate2.tga","ShipTex2");
		//AssetManager::LoadTexture("grid.tga","Grid");

		AssetManager::LoadSound("TankShot.flac","TankShotSound");
		AssetManager::LoadSound("getout.ogg","GetOutBGM");
		//AssetManager::LoadSound("GunShot.wav","GunShotSound");
		//AssetManager::LoadTerrain("TerTest6.tga","TerTestTerrain","BombTexture",250,20,0,1,1);
		//SceneManager::SetCurrentTerrain("TerTestTerrain");
		
		AssetManager::LoadTerrain("TerTest6.tga","TerTestTerrain","BucketTexture",10,5.0f,0,1,1);
		AssetManager::LoadTerrain("TerrainTest.tga","TerTestTerrain2","BucketTexture",250,20,0,4,4);

		SceneManager::SetStartScene(new SoundTestLevel());

}

void Game::UnloadGameContent(){

	RecyclingBombFactory::DeleteMe();
	GameController::DeleteMe();
}

void Game::TerminateContext(){


}