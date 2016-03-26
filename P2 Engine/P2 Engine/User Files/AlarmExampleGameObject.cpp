#include "AlarmExampleGameObject.h"
#include "SceneManager.h"
#include "LoseScene.h"

AlarmExampleGameObject::AlarmExampleGameObject(){

	//register for all Keys used
	RegisterForInput(AZUL_KEY::KEY_A);
	RegisterForInput(AZUL_KEY::KEY_S);
	RegisterForInput(AZUL_KEY::KEY_C);
	RegisterForInput(AZUL_KEY::KEY_ENTER);
	SetAlarm(AlarmID_0,10);



}

void AlarmExampleGameObject::Update(){
	if(GetTimeRemainingOfAlarm(AlarmID_0)>=0){
		cout<<GetTimeRemainingOfAlarm(AlarmID_0)<<endl;
	}
}

void AlarmExampleGameObject::Alarm0(){

	//MarkForDestroy();

	//SceneManager::ChangeScene(new LoseScene() );
}

void AlarmExampleGameObject::KeyDown(AZUL_KEY key){

	if(key == AZUL_KEY::KEY_A){
		AddTimeToAlarm(AlarmID_0,5);
	}

	if(key == AZUL_KEY::KEY_S){
		SubtractTimeFromAlarm(AlarmID_0,5);
	}

	if(key == AZUL_KEY::KEY_C){
		CancelAlarm(AlarmID_0);
	}

	if(key == AZUL_KEY::KEY_ENTER){
		SetAlarm(AlarmID_0,20);
	}
		
}

void AlarmExampleGameObject::KeyUp(AZUL_KEY key){
	if(key == AZUL_KEY::KEY_1){
		cout<<"1 Key Up"<<endl;
	}

}

void AlarmExampleGameObject::OnTerminate(){

	
	DeregisterForInput(AZUL_KEY::KEY_1);
	
	DeregisterAllInputKeys();

}

AlarmExampleGameObject::~AlarmExampleGameObject(){


}