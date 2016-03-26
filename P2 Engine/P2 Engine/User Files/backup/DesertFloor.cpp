#include "DesertFloor.h"

DesertFloor::DesertFloor(){

	//only graphics are needed. No collision
	SetGraphicsObject(GraphicsObjectMaker::CreateGraphicsObjectFlatTexture(AssetManager::GetModel("Plane"),AssetManager::GetTexture("DesertFloorTexture")   ));


	Matrix temp = Matrix();
	floorScale.set( SCALE, 10000.0f, 10000.0f, 10000.0f);
	floorRot.set( ROT_XYZ, 0, 0, 0);
	floorPos.set(0,0,0);
	temp = floorScale * floorRot * Matrix( TRANS, floorPos );
	SetGameObjectWorld(temp);

}
DesertFloor::~DesertFloor(){
}

void DesertFloor::Update(){
}

void DesertFloor::Draw(){
	RenderGraphics();


}