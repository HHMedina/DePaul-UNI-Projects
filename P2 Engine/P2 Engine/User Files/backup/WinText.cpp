#include "WinText.h"

WinText::WinText(){

	//only graphics are needed. No collision
	SetGraphicsObject(GraphicsObjectMaker::CreateGraphicsObjectFlatTexture(AssetManager::GetModel("WinModel"),AssetManager::GetTexture("BucketTexture")   ));
	SetAlarm(AlarmID_0,10);

	Matrix temp = Matrix();
	winScale.set( SCALE, 5, 5, 5);
	winRot.set( ROT_XYZ, 0, 0, 0);
	winPos.set(0,0,0);
	temp = winScale * winRot * Matrix( TRANS, winPos );
	SetGameObjectWorld(temp);

}

WinText::~WinText(){
}

void WinText::Draw(){
	RenderGraphics();


}