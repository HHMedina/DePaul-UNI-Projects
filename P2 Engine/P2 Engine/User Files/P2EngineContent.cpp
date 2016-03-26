#include <../Game.h>
#include "Level1.h"
#include "TutorialLevel1.h"
#include "WinScene.h"
#include "LoseScene.h"
#include "GameController.h"
#include "RecyclingBombFactory.h"

void Game::InitializeContext(){


}

void Game::LoadGameContent(){
	// Model loading ---------------------------------------

		// Load external models -------------------------------
		//AssetManager::LoadModel("space_frigate.fbx","SpaceShip");
		//AssetManager::LoadModel("WeirdShape.fbx","WeirdShape");
		AssetManager::LoadModel("bucket.fbx","BucketModel");
		AssetManager::LoadModel("bomb.fbx","BombModel");
		//AssetManager::LoadModel("Axis.fbx","Axis");
		AssetManager::LoadModel("Plane.fbx","Plane");
		AssetManager::LoadModel("Win.fbx","WinModel");
		AssetManager::LoadModel("Lose.fbx","LoseModel");
		//AssetManager::LoadModel("Cottage.fbx","Cottage");
	// Texture Loading  --------------------------------

		// Space Ship texture
		AssetManager::LoadTexture("bombTexture.tga","BombTexture");
		AssetManager::LoadTexture("bucketTexture.tga","BucketTexture");
		//AssetManager::LoadTexture("Test.tga","ShipTex");
		//AssetManager::LoadTexture("space_frigate2.tga","ShipTex2");
		AssetManager::LoadTexture("grid.tga","Grid");

		SceneManager::SetStartScene(new TutorialLevel1());
	
}

void Game::UnloadGameContent(){

	RecyclingBombFactory::DeleteMe();
	GameController::DeleteMe();
}

void Game::TerminateContext(){


}