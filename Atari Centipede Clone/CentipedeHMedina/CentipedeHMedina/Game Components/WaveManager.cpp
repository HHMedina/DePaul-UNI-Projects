#include "WaveManager.h"
#include "RecyclingSpiderFactory.h"
#include "RecyclingScorpionFactory.h"
#include "Scorpion.h"
#include "Spider.h"
#include "Blaster.h"
#include "Mushroomfield.h"
#include "RecyclingFleaFactory.h"
#include "Flea.h"
#include "HUD.h"
#include <fstream>
#include "WaveDefintion.h"
#include "CentipedeFactory.h"
#include "RecyclingCHeadSegmentFactory.h"
#include "CentipedeHeadSegment.h"
#include "SoundManager.h"

#include "Player.h"
#include "Player1.h"
#include "Player2.h"
#include "Player0.h"
#include "ModeManager.h"
#include "AttractionMode.h"
#include "PlayerOneMode.h"
#include "PlayerTwoMode.h"

#include "PlayerManager.h"

//declare static members used in .h
WaveManager * WaveManager::instance =NULL;
int WaveManager::RightStartPosition =0;
const float WaveManager::TopBorder=  408;



WaveManager::WaveManager(){

	//pointers of objects on scene
	FleaOnScene=NULL;
	ScorpionOnScene=NULL;
	SpiderOnScene=NULL;

	//current wave variables
	CanSpawnAflea=false;
	CanSpawnAScorpion=false;
	CanSpawnASpider =true;
	CentipedeSpeed=0;
	CentipedeLength=0;
	CentipedeHeadNumber=0;
	CentipedeHeadSpeed=0;
	SpiderSpeed=0;
	SpiderSpawnTime=0;
	ScorpionSpawnTime=false;
	FleaMushroomCountValue=false;
	CentipedeHeadSpawningActivated=false;

	FleaSpeed =4;

	CurrentWave=0;


	srand(time(NULL));//seed the rand based on time
	Player1Score=0;
	Player2Score=0;
	CurrentNumberOfLives=INITIALNUMBEROFLIVES;
	FleaIsInScene=false;
	ScorpionIsInScene=false;
	SPIDERSTARTY =WindowManager::MainWindow.getView().getSize().y-(MUSHROOMSEGMENT*11 + MUSHROOMSEGMENT/2);
	RightStartPosition=WindowManager::MainWindow.getView().getSize().x+(MUSHROOMSEGMENT+MUSHROOMSEGMENT/2);
}



void WaveManager::Update(){
}


void WaveManager::StartUp(){
	
	Instance().CanSpawnASpider =true;
	//sets up the first scene
	Instance().CanSpawnAflea=Instance().Waves[0]->CanSpawnFleas;
	Instance().CanSpawnAScorpion=Instance().Waves[0]->CanSpawnScorpions;
	Instance().CentipedeSpeed = Instance().Waves[0]->CentipedeSpeed;
	Instance().CentipedeLength = Instance().Waves[0]->CentipedeLength;
	Instance().CentipedeHeadNumber = Instance().Waves[0]->CentipedeHeadNumber;
	Instance().CentipedeHeadSpeed = Instance().Waves[0]->CentipedeHeadSpeed;
	Instance().SpiderSpeed = Instance().Waves[0]->SpiderSpeed;
	Instance().SpiderSpawnTime = Instance().Waves[0]->SpiderSpawnTime;
	Instance().ScorpionSpawnTime = Instance().Waves[0]->ScorpionSpawnTime;
	Instance().FleaMushroomCountValue = Instance().Waves[0]->FleaMushroomCountValue;

	Instance().CheckToSpawnFlea();
	
	
	//creates the main centipede
	CentipedeFactory::CreateCentipede( (sf::Vector2f((MUSHROOMSEGMENT*5)-MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2+MUSHROOMSEGMENT*2)),Instance().CentipedeSpeed,Instance().CentipedeLength,PlayerManager::GetPlayer()->GetPlayerField(),sf::Vector2f(-1,1));

	//creates the centipede Heads if any
	for(int i=0;i<Instance().CentipedeHeadNumber;++i){

		if(rand()%2==0){
			RecyclingCHeadSegmentFactory::CreateCentipedeHeadSegment(sf::Vector2f( ( MUSHROOMSEGMENT*(rand()%26) )+(MUSHROOMSEGMENT*2+MUSHROOMSEGMENT/2),MUSHROOMSEGMENT+MUSHROOMSEGMENT/2),Instance().CentipedeHeadSpeed,PlayerManager::GetPlayer()->GetPlayerField(),sf::Vector2f(-1,1));
		}else{
			RecyclingCHeadSegmentFactory::CreateCentipedeHeadSegment(sf::Vector2f( ( MUSHROOMSEGMENT*(rand()%26) )+(MUSHROOMSEGMENT*2+MUSHROOMSEGMENT/2),MUSHROOMSEGMENT+MUSHROOMSEGMENT/2),Instance().CentipedeHeadSpeed,PlayerManager::GetPlayer()->GetPlayerField(),sf::Vector2f(1,1));
		}
	}
	

	Instance().SetAlarm(0,Instance().SpiderSpawnTime);

	Instance().SetAlarm(1,Instance().ScorpionSpawnTime);
	
}

void WaveManager::Alarm0(){
	//gets a random int to figure out if the spider will start on the left or right when called for creation
	if(Instance().CanSpawnASpider){
		if(rand()%2==0){
			SpiderIsInScene=true;
			SpiderOnScene=&RecyclingSpiderFactory::CreateSpider(sf::Vector2f(LeftStartPosition,SPIDERSTARTY),Instance().SpiderSpeed,sf::Vector2i(1,1),PlayerManager::GetPlayer()->GetPlayerField());
		}else{
			SpiderIsInScene=true;
			SpiderOnScene=&RecyclingSpiderFactory::CreateSpider(sf::Vector2f(RightStartPosition,SPIDERSTARTY),Instance().SpiderSpeed,sf::Vector2i(-1,1),PlayerManager::GetPlayer()->GetPlayerField());
		}
	}

}

void WaveManager::Alarm1(){
	if(!ScorpionIsInScene&&Instance().CanSpawnAScorpion){
		//gets a random int to figure out if the scorpion will start on the left or right when called for creation
		if(rand()%2==0){
			ScorpionIsInScene =true;
			ScorpionOnScene=&RecyclingScorpionFactory::CreateScorpion(sf::Vector2f(LeftStartPosition,WindowManager::MainWindow.getView().getSize().y - (MUSHROOMSEGMENT/2+MUSHROOMSEGMENT*(rand()%10+16))),1,PlayerManager::GetPlayer()->GetPlayerField());
		}else{
			ScorpionIsInScene =true;
			ScorpionOnScene=&RecyclingScorpionFactory::CreateScorpion(sf::Vector2f(RightStartPosition,WindowManager::MainWindow.getView().getSize().y - (MUSHROOMSEGMENT/2+MUSHROOMSEGMENT*(rand()%10+16))),-1,PlayerManager::GetPlayer()->GetPlayerField());
		}
	
 	}
	
}

void WaveManager::Alarm2(){

	if(rand()%2==0){//decide between spawning on the left or right
		RecyclingCHeadSegmentFactory::CreateCentipedeHeadSegment(sf::Vector2f(WindowManager::MainWindow.getView().getSize().x+MUSHROOMSEGMENT/2,TopBorder),BottomSpawnedCentipedeHeadSpeed,PlayerManager::GetPlayer()->GetPlayerField(),sf::Vector2f(-1,1));//spawn on the right going left
		
	}else{
		RecyclingCHeadSegmentFactory::CreateCentipedeHeadSegment(sf::Vector2f(-MUSHROOMSEGMENT/2,TopBorder),BottomSpawnedCentipedeHeadSpeed,PlayerManager::GetPlayer()->GetPlayerField(),sf::Vector2f(1,1)); //spawn on the left going right
	}
	this->SetAlarm(2,CentipedeHeadSpawnTime);//restart alarm (loop)
}


void WaveManager::SetSpiderToDead(){

	Instance().SpiderIsInScene =false;
}

void WaveManager::SetScorpionToDead(){

	Instance().ScorpionOnScene=NULL;
	Instance().ScorpionIsInScene =false;

}

void WaveManager::SetFleaToDead(){
	Instance().FleaOnScene=NULL;
	Instance().FleaIsInScene =false;
}


void WaveManager::SetSpiderAlarm(float time ){

	Instance().SetAlarm(0,time);
}

void WaveManager::SetScorpionAlarm(float time ){

	Instance().SetAlarm(1,time);
}


void WaveManager::LoadWaveInformation(){

	std::string CurrentIdentifier;
	std::string Value;
	fstream WaveSettingsFile;
	WaveSettingsFile.open("WaveSettings.txt", fstream::in);
	if(WaveSettingsFile.is_open()){
		
		WaveDefinition* temp = new WaveDefinition();
		
		while(!WaveSettingsFile.eof()){

			
			//get the next two sections of the text file (everything is in pairs of twos)
			WaveSettingsFile>>CurrentIdentifier>>Value;
			//convert identifier to lowercase
			for(int i=0; i<CurrentIdentifier.size();++i)
				CurrentIdentifier[i] = tolower(CurrentIdentifier[i]);
			
		
			//Check whether line is identifier or a new Wave and check values of temp wave definition
			if(CurrentIdentifier == "centipedespeed"){

				temp->CentipedeSpeed = std::stoi(Value);

			}else if(CurrentIdentifier == "centipedelength"){

				temp->CentipedeLength = std::stoi(Value);

			}else if(CurrentIdentifier == "centipedeheadnumber"){

				temp->CentipedeHeadNumber = std::stoi(Value);

			}else if(CurrentIdentifier == "spiderspeed"){

				temp->SpiderSpeed = std::stof(Value);

			}else if(CurrentIdentifier == "spiderspawntime"){

				temp->SpiderSpawnTime = std::stof(Value);

			}else if(CurrentIdentifier == "scorpionspawntime"){

				temp->ScorpionSpawnTime = std::stof(Value);

			}else if(CurrentIdentifier == "centipedeheadspeed"){

				temp->CentipedeHeadSpeed = std::stoi(Value);

			}else if(CurrentIdentifier == "fleamushroomcountvalue"){

				temp->FleaMushroomCountValue = std::stoi(Value);
				
			}else if(CurrentIdentifier == "canspawnfleas"){
				//check if the corresponding value is true or false
				if(Value == "true"){
					temp->CanSpawnFleas = true;
				}else if(Value == "false"){
					temp->CanSpawnFleas = false;
				}
			}else if(CurrentIdentifier == "canspawnscorpions"){
				//check if the corresponding value is true or false
				if(Value == "true"){
					temp->CanSpawnScorpions = true;
				}else if(Value == "false"){
					temp->CanSpawnScorpions = false;
				}
			}else if(CurrentIdentifier == "//"){
				Instance().Waves.push_back(temp);
				temp= new WaveDefinition();
				
				
				
			}

		}
		
		
		//Since a new Wavedefinition is created at the end of each wave end signal.. delete the excess one at the end
		delete temp;
		temp=NULL;

		WaveSettingsFile.close();//close file

	}else{
		ConsoleMsg::WriteLine("Could not open File");
	}
	

}

WaveManager::~WaveManager(){
	

	//deallocate all members within the vector holding wave definitions
	while(!Waves.empty()){
		delete Waves.front(); //they are pointers of waves
		Waves.pop_back();
	}

	std::list<HighScoreCombination>::iterator it;

	for(it!=HighScores.begin(); it!= HighScores.end();){
		it = HighScores.erase(it);//remove the element from list and validate pointer
	}

	//delete this instance
	delete instance;
}

float WaveManager::GetScorpionSpawnTime(){
	return Instance().ScorpionSpawnTime;
}

float WaveManager::GetSpiderSpawnTime(){
	return Instance().SpiderSpawnTime;
}

void WaveManager::LoadPlayerWave(){

	Instance().CanSpawnASpider = true;
	if(PlayerManager::GetPlayer()->GetCurrentWave()>=Instance().Waves.size()){ //if at end of waves just set wave to last one
		PlayerManager::GetPlayer()->SetCurrentWave(Instance().Waves.size()-1);	
	}
		//sets up the first scene
		Instance().CanSpawnAflea=Instance().Waves[PlayerManager::GetPlayer()->GetCurrentWave()]->CanSpawnFleas;
		Instance().CanSpawnAScorpion=Instance().Waves[PlayerManager::GetPlayer()->GetCurrentWave()]->CanSpawnScorpions;
		Instance().CentipedeSpeed = Instance().Waves[PlayerManager::GetPlayer()->GetCurrentWave()]->CentipedeSpeed;
		Instance().CentipedeLength = Instance().Waves[PlayerManager::GetPlayer()->GetCurrentWave()]->CentipedeLength;
		Instance().CentipedeHeadNumber = Instance().Waves[PlayerManager::GetPlayer()->GetCurrentWave()]->CentipedeHeadNumber;
		Instance().CentipedeHeadSpeed = Instance().Waves[PlayerManager::GetPlayer()->GetCurrentWave()]->CentipedeHeadSpeed;
		Instance().SpiderSpeed = Instance().Waves[PlayerManager::GetPlayer()->GetCurrentWave()]->SpiderSpeed;
		Instance().SpiderSpawnTime = Instance().Waves[PlayerManager::GetPlayer()->GetCurrentWave()]->SpiderSpawnTime;
		Instance().FleaMushroomCountValue = Instance().Waves[PlayerManager::GetPlayer()->GetCurrentWave()]->FleaMushroomCountValue;
		Instance().ScorpionSpawnTime = Instance().Waves[PlayerManager::GetPlayer()->GetCurrentWave()]->ScorpionSpawnTime;

		//if there is not a scorpion when wave changes and you can spawn a scorpion this wave
		//then start the scropion alarm (create a scorpion)
		if(!Instance().ScorpionIsInScene&&Instance().CanSpawnAScorpion){
			Instance().SetAlarm(1,Instance().ScorpionSpawnTime);
		}
		if(!Instance().SpiderIsInScene){
			//starts up the spider spawner alarm 
			Instance().SetAlarm(0,Instance().SpiderSpawnTime);
		}

		//creates the main centipede
		CentipedeFactory::CreateCentipede( (sf::Vector2f((MUSHROOMSEGMENT*5)-MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2+MUSHROOMSEGMENT*2)),Instance().CentipedeSpeed,Instance().CentipedeLength,PlayerManager::GetPlayer()->GetPlayerField(),sf::Vector2f(-1,1));
		
		//sees if it must spawn a flea at start of wave
		Instance().CheckToSpawnFlea();

		//creates the centipede Heads if any
		for(int i=0;i<Instance().CentipedeHeadNumber;++i){

			if(rand()%2==0){
				RecyclingCHeadSegmentFactory::CreateCentipedeHeadSegment(sf::Vector2f( ( MUSHROOMSEGMENT*(rand()%26) )+(MUSHROOMSEGMENT*2+MUSHROOMSEGMENT/2),MUSHROOMSEGMENT+MUSHROOMSEGMENT/2),Instance().CentipedeHeadSpeed,PlayerManager::GetPlayer()->GetPlayerField(),sf::Vector2f(-1,1));
			}else{
				RecyclingCHeadSegmentFactory::CreateCentipedeHeadSegment(sf::Vector2f( ( MUSHROOMSEGMENT*(rand()%26) )+(MUSHROOMSEGMENT*2+MUSHROOMSEGMENT/2),MUSHROOMSEGMENT+MUSHROOMSEGMENT/2),Instance().CentipedeHeadSpeed,PlayerManager::GetPlayer()->GetPlayerField(),sf::Vector2f(-1,1));
			}
		}


	

}

void WaveManager::CheckToSpawnFlea(){
	
	if(!Instance().FleaIsInScene&& PlayerManager::GetPlayer()->GetPlayerField().AmountOfPlayerSideMushrooms<Instance().FleaMushroomCountValue&&Instance().CanSpawnAflea){
		//gets a random int to figure out if the spider will start on the left or right when called for creation
		Instance().FleaOnScene=&RecyclingFleaFactory::CreateFlea(sf::Vector2f(MUSHROOMSEGMENT*((rand()%30))+MUSHROOMSEGMENT/2,-MUSHROOMSEGMENT/2),Instance().FleaSpeed,PlayerManager::GetPlayer()->GetPlayerField());
		Instance().FleaIsInScene =true;

	}
	
}

void WaveManager::DestroyAllWaveEntities(){
	
	CentipedeHeadSegment::ResetNumberOfInstances();
	CentipedeFactory::RecallAllSegments();
	Instance().StopSpawningCentipedeHeads();


	//prevent enemies from spawning untill new wave is started.
	Instance().CanSpawnAScorpion =false;
	Instance().CanSpawnAflea =false;
	Instance().CanSpawnASpider =false;

	//Destroy Scorpion if there is one
	if(Instance().ScorpionOnScene!=NULL){
		Instance().ScorpionOnScene->MarkForDestroy();
		Instance().ScorpionIsInScene=NULL;
	}

	//Destroy Flea if there is one
	if(Instance().FleaOnScene!=NULL){
		Instance().FleaOnScene->MarkForDestroy();
		Instance().FleaOnScene=NULL;
	}

	//Destroy Spider if there is one
	if(Instance().SpiderOnScene!=NULL){
		Instance().SpiderOnScene->MarkForDestroy();
		Instance().SpiderOnScene=NULL;
	}

	

}

void WaveManager::LoadHighScores(){

	std::string CurrentLineScore;
	std::string CurrentName;

	fstream HighScoresFile;
	HighScoresFile.open("HighScores.txt", fstream::in);
	if(HighScoresFile.is_open()){

		while(!HighScoresFile.eof()){

			//extract the score and name from the line
			HighScoresFile>>CurrentLineScore>>CurrentName;

			//Load Score and name into the list
			Instance().HighScores.push_back(HighScoreCombination(std::stoi(CurrentLineScore),CurrentName));
			
		}
		
		HighScoresFile.close();
	}else{
		ConsoleMsg::WriteLine("Could Not Open HighScores.txt");
	}

}

void WaveManager::WriteHighScoresToFile(){

	int counter =0;

	ofstream HighScoresFile("HighScores.txt"); //Discards all data in txt file before opening

	if(HighScoresFile.is_open()){

		std::list<HighScoreCombination>::iterator it;

		for(it=Instance().HighScores.begin(); it!=Instance().HighScores.end();++it){
			
			//write the score then the name on one line
			HighScoresFile<<Tools::ToString(it->Score)+" " + it->Name;
			
			//if the current score is no the last score add a new line (prevents new line at end of file)
			if(counter<(Instance().HighScores.size()-1)){
				HighScoresFile<<"\n";
				++counter;
			}
		}
		HighScoresFile.close();//close file

	}else{
		ConsoleMsg::WriteLine("Could Not Open HighScores.txt");
	}

}


void WaveManager::CheckForNewHighScore(std::string name){

	bool SpotFound =false;
	std::list<HighScoreCombination>::iterator it;
	for(it = Instance().HighScores.begin();!SpotFound&&it!=Instance().HighScores.end();++it){//check list untill it has been found or at end
		
		if(PlayerManager::GetPlayer()->GetPlayerScore()>it->Score){//if player has beaten the highscore
			
			
			it=Instance().HighScores.insert(it,HighScoreCombination(PlayerManager::GetPlayer()->GetPlayerScore(),name));//add before the losing score
			Instance().HighScores.pop_back(); //remove the end
			SpotFound=true;
		
		}
	
	}

}	

std::list<HighScoreCombination>& WaveManager::GetHighScoresList(){
	return Instance().HighScores;
}

void WaveManager::PlayerDied(){
	Instance().DestroyAllWaveEntities();//destroy all wave entities if any are alive
	PlayerManager::GetPlayer()->GetPlayerField().HealAllMushrooms();//heal the mushroom field
}



void WaveManager::EndOfWaveDecision(){

	ModeManager::EndOfWaveDecision();//takes action depending on the mode
	
}

void WaveManager::DisableSpiderSpawn(){

	Instance().CanSpawnASpider=false();
}

bool WaveManager::IsScoreANewHighScore(){

	std::list<HighScoreCombination>::iterator it;
	for(it = Instance().HighScores.begin();it!=Instance().HighScores.end();++it){//check each high score
		
		if(PlayerManager::GetPlayer()->GetPlayerScore()>it->Score){//if player has beaten the highscore
			return true;		
		}
	}

	return false; //not a new high score
}

void WaveManager::ActivateHeadSpawning(){

	if(Instance().CentipedeHeadSpawningActivated ==false){// if not currently activated
		Instance().SetAlarm(2,CentipedeHeadSpawnTime);//activate spawning of centipede heads
	}

	Instance().CentipedeHeadSpawningActivated =true;//set activated to true

	

}


void WaveManager::StopSpawningCentipedeHeads(){

	Instance().AlarmCancel(2); //cancel alarm 2 causing the stopping of reactivating the alarm in a loop
	Instance().CentipedeHeadSpawningActivated =false;//set activated to false

	

}