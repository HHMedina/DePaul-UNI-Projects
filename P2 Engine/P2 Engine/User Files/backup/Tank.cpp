#include "Tank.h"
#include "TankTop.h"
#include "TankBottom.h"
#include "TankShell.h"
#include "SoundSystem.h"

Tank::Tank(Vect& position,Vect& scale,Vect& rotation,int mode){
	
	hp = maxHP;
	topVerticalOffset = Vect(0,13.0f,0);
	shellSpawnOffsetFront = 75.0f;//75.0f;
	//prevent automation
	DeregisterFromAutomated();
	if(mode==0){
		RegisterForUpdate();
	}
	RegisterForInput(AZUL_KEY::KEY_SPACE, KeyEvent::KeyDown);
	//set initial transform for bottom
	tankScale.set( SCALE, scale[x], scale[y], scale[z]);
	tankRot.set( ROT_XYZ, rotation[x],rotation[y] , rotation[z]);
	tankPos = position;
	topTankPartAdjustedPosition = tankPos+topVerticalOffset;
	topTankPartAdjustedRotation.set( ROT_XYZ, rotation[x],rotation[y] , rotation[z]);
	//set initial transform for top
	tankTopRot.set(ROT_XYZ, rotation[x],rotation[y] , rotation[z]);

	//set tankTop and bottom
	top = new TankTop(topTankPartAdjustedPosition,scale,rotation,this);
	bottom = new TankBottom(tankPos,scale,rotation,this);

	CamOffset = Vect(0, 50, -200);
	
	bottomSpeed =1.5f;
	bottomTurningSpeed = .015f;
	topTurningSpeed = .015f;
	
	shellSpawnPosition = topVerticalOffset + (Vect(0,0,shellSpawnOffsetFront)*tankTopRot);
}

Tank::~Tank(){
	//Note: delete isnt called on collision model because asset manager owns model.
	//Note: Delete is not called on graphics object because we do not own graphics object.
	//Unless if a graphicsobject of any type is called using new and assigned into graphicsObject
	//member.
}


void Tank::Initialize(float x, float y, float z){
	DeregisterFromAutomated();

}

void Tank::Update(){
	
	BottomMovement();//must go first in order for top to move synced
	TopMovement();

	// Place the camera relative to the spaceship
	
	CameraMan::GetCurrent()->setOrientAndPosition( 
													Vect(0,1,0),
													Vect(0,50,0) * tankRot *tankTopRot * Matrix( TRANS, topTankPartAdjustedPosition  ), 
													CamOffset * tankRot *tankTopRot* Matrix( TRANS, topTankPartAdjustedPosition  ) );
	CameraMan::GetCurrent()->updateCamera();
	
	


}

void Tank::OnTerminate(){
	
	DeregisterForInput(AZUL_KEY::KEY_SPACE, KeyEvent::KeyDown);
}

void Tank::KeyDown(AZUL_KEY key){
	if(key == AZUL_KEY::KEY_SPACE){
		shellSpawnPosition =topTankPartAdjustedPosition + (Vect(0,0,shellSpawnOffsetFront)*topTankPartAdjustedRotation);
		new TankShell(shellSpawnPosition,topTankPartAdjustedRotation);
	}

}

void Tank::TopMovement(){
	//Upper controls
	// TankTop Rotation movement (not using time-based values for simplicity)
	if ( InputManager::IsKeyPressed(AZUL_KEY::KEY_LEFT) )
	{
		tankTopRot *=  Matrix( ROT_Y, this->topTurningSpeed );
	}
	else if ( InputManager::IsKeyPressed(AZUL_KEY::KEY_RIGHT))
	{

		tankTopRot *= Matrix( ROT_Y, -this->topTurningSpeed );
	}

	//calculated adjusted top tank tranforms
	topTankPartAdjustedPosition = tankPos +topVerticalOffset;
	topTankPartAdjustedRotation = tankRot * tankTopRot;

	// adjust the position of the gameobject	
	top->SetGameObjectWorld(tankScale*topTankPartAdjustedRotation * Matrix(TRANS,topTankPartAdjustedPosition));
}

void Tank::BottomMovement(){
	///lower controls
	//Tank Translation movement
	if (  InputManager::IsKeyPressed(AZUL_KEY::KEY_W))
	{
		tankPos += Vect(0,0,1) * tankRot * bottomSpeed;
	}
	else if ( InputManager::IsKeyPressed(AZUL_KEY::KEY_S) )
	{
		tankPos += Vect(0,0,1) *  tankRot * - bottomSpeed;
	}

	// Ship Rotation movement (not using time-based values for simplicity)
	if ( InputManager::IsKeyPressed(AZUL_KEY::KEY_A) )
	{
		tankRot *= Matrix( ROT_Y, this->bottomTurningSpeed );
	}
	else if ( InputManager::IsKeyPressed(AZUL_KEY::KEY_D))
	{

		tankRot *= Matrix( ROT_Y, -this->bottomTurningSpeed);
	}

	// adjust the position of the gameobject
	bottom->SetGameObjectWorld(tankScale*tankRot* Matrix(TRANS,tankPos));

}

void Tank::AdjustHP(int amount){
	hp+=amount;
	if(hp<=0){
		DestroyParts();
		this->MarkForDestroy();
	}
}

void Tank::DestroyParts(){
	top->MarkForDestroy();
	top=NULL;
	bottom->MarkForDestroy();
	bottom=NULL;

}