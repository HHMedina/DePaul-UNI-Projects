#include "WinScene.h"
#include "Floor.h"
#include "WinText.h"

void WinScene::SetUp(){

	new Floor();
	new WinText();
}