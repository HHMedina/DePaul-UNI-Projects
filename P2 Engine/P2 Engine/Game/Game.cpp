#include "Game.h"
#include <iostream>
GraphicsObjectFlatTexture *pGObj_SpaceshipWithTexture;
GraphicsObjectColorNoTexture *pGObj_Axis;
GraphicsObjectFlatTexture *pGObj_Plane;

GameObject* cool;
// Camera vars
Vect CamPos(0,170,420);	
Matrix CamRot( IDENTITY );	// No rotation initially
Vect CamUp(0,1,0);			// Using local Y axis as 'Up'
Vect CamDir(0,0,1);			// Using the local Z axis as 'forward'
float CamTranSpeed = 2;
float CamRotSpeed = .02f;

//-----------------------------------------------------------------------------
//  Game::Game()
//		Game Engine Constructor
//-----------------------------------------------------------------------------
Game::Game( const char * const windowName, int widthScreen, int heightScreen )
: Engine( windowName, widthScreen, heightScreen)
{
	printf("Game(): started\n");
	glfwSetWindowPos(this->window, 20, 30);
}

//-----------------------------------------------------------------------------
// Game::Initialize()
//		Allows the engine to perform any initialization it needs to before 
//      starting to run.  This is where it can query for any required services 
//      and load any non-graphic related content. 
//-----------------------------------------------------------------------------
void Game::Initialize()
{
	SoundSystem::StartSoundEngine();
	GraphicsObjectMaker::Initialize();
	InitializeContext();
}

//-----------------------------------------------------------------------------
// Game::LoadContent()
//		Allows you to load all content needed for your engine,
//	    such as objects, graphics, etc.
//-----------------------------------------------------------------------------
void Game::LoadContent()
{
	
	DebugVisualizer::Initialize();
	LoadGameContent();

	// Camera Setup ---------------------------------------

		// Initially setup the camera
		CameraMan::GetCurrent()->setViewport( 0, 0, this->screenWidth, this->screenHeight);
		CameraMan::GetCurrent()->setPerspective( 45, float(this->screenWidth)/float(this->screenHeight), 1, 5000);
				
	//changed initial stop watch to here because loading can take a while
	//and time to start as soon as the game does.
	TimeManager::InitializeStopWatches();
}

//-----------------------------------------------------------------------------
// Game::Update()
//      Called once per frame, update data, tranformations, etc
//      Use this function to control process order
//      Input, AI, Physics, Animation, and Graphics
//-----------------------------------------------------------------------------
void Game::Update()
{
	SoundSystem::ProcessSoundDrops();
	TimeManager::Update();
	SceneManager::UpdateCurrentScene();
	/*
	// Camera translation movement (not using time-based values for simplicity)
	if ( InputMan::GetKeyboard()->GetKeyState(AZUL_KEY::KEY_1 ) )
	{
		CamPos += Vect(0,0,1) * CamRot * CamTranSpeed;
	}
	else if ( InputMan::GetKeyboard()->GetKeyState(AZUL_KEY::KEY_2 ) )
	{
		CamPos += Vect(0,0,1) * CamRot * - CamTranSpeed;
	}

	if ( InputMan::GetKeyboard()->GetKeyState(AZUL_KEY::KEY_3 ) )
	{
		CamPos += Vect(1,0,0) * CamRot * CamTranSpeed;
	}
	else if ( InputMan::GetKeyboard()->GetKeyState(AZUL_KEY::KEY_4 ) )
	{
		CamPos += Vect(1,0,0) * CamRot * -CamTranSpeed;
	}

	// Camera Rotation movement (not using time-based values for simplicity)
	if ( InputMan::GetKeyboard()->GetKeyState(AZUL_KEY::KEY_5 ) )
	{
		CamRot *= Matrix( ROT_Y, CamRotSpeed );
	}
	else if ( InputMan::GetKeyboard()->GetKeyState(AZUL_KEY::KEY_6) )
	{
		CamRot *= Matrix( ROT_Y, -CamRotSpeed );
	}

	if ( InputMan::GetKeyboard()->GetKeyState(AZUL_KEY::KEY_7 ) )
	{
		CamRot *= Matrix( Quat( ROT_AXIS_ANGLE, Vect(1,0,0) * CamRot, -CamRotSpeed) );
	}
	else if ( InputMan::GetKeyboard()->GetKeyState(AZUL_KEY::KEY_8 ) )
	{
		CamRot *= Matrix( Quat( ROT_AXIS_ANGLE, Vect(1,0,0) * CamRot, CamRotSpeed) );
	}
	// Update the camera settings
	CameraMan::GetCurrent()->setOrientAndPosition( CamUp * CamRot, CamPos + CamDir * CamRot, CamPos);
	CameraMan::GetCurrent()->updateCamera();
	*/
}

//-----------------------------------------------------------------------------
// Game::Draw()
//		This function is called once per frame
//	    Use this for draw graphics to the screen.
//      Only do rendering here
//-----------------------------------------------------------------------------
void Game::Draw()
{ 
	
	SceneManager::DrawCurrentScene();
	DebugVisualizer::RenderVisuals();
}

//-----------------------------------------------------------------------------
// Game::UnLoadContent()
//       unload content (resources loaded above)
//       unload all content that was loaded before the Engine Loop started
//       (also used to clean up whatever was created in Game::Initialize()  )
//-----------------------------------------------------------------------------
void Game::UnLoadContent()
{
	
	UnloadGameContent();
	SceneManager::DeleteMe();
	TimeManager::DeleteMe();
	GraphicsObjectMaker::DeleteMe();
	SoundSystem::DeleteMe();//must go before assetmanager since it has to clean up assets in the assetmanager
	AssetManager::DeleteMe();
	TerminateContext();
}

// End of Game.cpp
