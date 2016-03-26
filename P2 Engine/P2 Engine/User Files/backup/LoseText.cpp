#include "LoseText.h"

LoseText::LoseText(){

	//only graphics are needed. No collision
	SetGraphicsObject(GraphicsObjectMaker::CreateGraphicsObjectFlatTexture(AssetManager::GetModel("LoseModel"),AssetManager::GetTexture("BucketTexture")   ));
	SetAlarm(AlarmID_0,10);

	Matrix temp = Matrix();
	winScale.set( SCALE, 5, 5, 5);
	winRot.set( ROT_XYZ, 0, 0, 0);
	winPos.set(0,0,0);
	temp = winScale * winRot * Matrix( TRANS, winPos );
	SetGameObjectWorld(temp);

}

LoseText::~LoseText(){
}

void LoseText::Draw(){
	RenderGraphics();


}