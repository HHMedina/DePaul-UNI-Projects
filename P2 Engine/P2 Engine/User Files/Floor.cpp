#include "Floor.h"

Floor::Floor(){

	//only graphics are needed. No collision
	SetGraphicsObject(GraphicsObjectMaker::CreateGraphicsObjectFlatTexture(AssetManager::GetModel("Plane"),AssetManager::GetTexture("Grid")   ));


	Matrix temp = Matrix();
	floorScale.set( SCALE, 500.0f, 500.0f, 500.0f);
	floorRot.set( ROT_XYZ, 0, 0, 0);
	floorPos.set(0,0,0);
	temp = floorScale * floorRot * Matrix( TRANS, floorPos );
	//SetGameObjectWorld(temp);

}
Floor::~Floor(){
}

void Floor::Update(){
}

void Floor::Draw(){
	RenderGraphics();


}