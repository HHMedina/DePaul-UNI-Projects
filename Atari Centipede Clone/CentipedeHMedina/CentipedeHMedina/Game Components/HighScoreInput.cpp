#include "HighScoreInput.h"
#include "WaveManager.h"
#include "ModeManager.h"

//define static members used in .h
HighScoreInput * HighScoreInput::instance =NULL;
const std::string HighScoreInput::NotificationText = "NEW HIGH SCORE";
const std::string HighScoreInput::PleaseEnterInitialsText = "PLEASE ENTER INITIALS:";

HighScoreInput::HighScoreInput(){

	
}

HighScoreInput::~HighScoreInput(){

	PlayerInputList.clear();//clear list (although already cleaned up by default since it is a list of objects not pointer..just in case)
	NotificationList.clear();//clear lists (although already cleaned up by default since it is a list of objects not pointer..just in case)
}

void HighScoreInput::StartInput(){

	Instance().NumberOfLettersEntered =0; //set number of letters entered to zero
	Instance().PlayerInitials ="";//reset player initial input
	Instance().RegisterInput(InputFlags::TextEntered);//register input

	 //reset positions of text
	Instance().NotificationPosition = sf::Vector2f(MUSHROOMSEGMENT*8,MUSHROOMSEGMENT*4);
	Instance().PlayerInputPosition = sf::Vector2f(MUSHROOMSEGMENT*14,MUSHROOMSEGMENT*8);
	Instance().PleaseEnterInitialsPosition = sf::Vector2f(MUSHROOMSEGMENT*4,MUSHROOMSEGMENT*6);


	//set notification text
	for(int i =0;i<NotificationText.size();++i){

		Instance().NotificationList.push_back( Instance().CentFont.GetGlyph( NotificationText.at(i), Instance().NotificationPosition ) );
		Instance().NotificationPosition.x+=Instance().CentFont.CellWidth();

	}

	for(int i =0;i<PleaseEnterInitialsText.size();++i){

		Instance().PleaseEnterInitialsList.push_back( Instance().CentFont.GetGlyph( PleaseEnterInitialsText.at(i), Instance().PleaseEnterInitialsPosition ) );
		Instance().PleaseEnterInitialsPosition.x+=Instance().CentFont.CellWidth();

	}

}

void HighScoreInput::KeyPressed(sf::Keyboard::Key k,bool altKey, bool ctrlKey, bool shiftKey, bool systemKey){
	
	
	//if backspace then remove last input
	if(k == sf::Keyboard::Back) 
		PlayerInputList.pop_back();
	
}

void HighScoreInput::Draw(){

	//draw all text
	std::list<Glyph>::iterator it;
	for(it=PlayerInputList.begin();it!=PlayerInputList.end();++it){
		it->Draw();
	}

	for(it=NotificationList.begin();it!=NotificationList.end();++it){
		it->Draw();
	}

	for(it=PleaseEnterInitialsList.begin();it!=PleaseEnterInitialsList.end();++it){
		it->Draw();
	}

}

void HighScoreInput::TextEntered(sf::Uint32 UnicodeValue){

	
	if(PlayerInputList.size()<MAXIMUMINPUTCHARS&&UnicodeValue!=8&&UnicodeValue!=13){// if the input list is less than max allowed and is not backspace or enter -Can be hardcoded because unicode value never changes
		// convert unicode to upper char value
		char input = (char) UnicodeValue;
		input=toupper(input);
		PlayerInitials += input;//add the inputed letter to the string containing the inputted text
		PlayerInputList.push_back( CentFont.GetGlyph( input, PlayerInputPosition ) );
		PlayerInputPosition.x+= CentFont.CellWidth();
	}


	if(UnicodeValue == 8){ // In unicode 32, 8 is the value for a backspace -Can be hardcoded because unicode value never changes
		if(PlayerInputList.size()>0){
			PlayerInputList.pop_back();//get rid of the last inputted key
			PlayerInputPosition.x-=CentFont.CellWidth(); // set the position of next spawn to the previous position (*2 due to taking into account the backspace entered)
			PlayerInitials.erase(PlayerInitials.size()-1); //erae last input in the initials string
		}
	}


	if(UnicodeValue == 13){ // In unicode 32, 13 is the value for a Enter Key
		

		//check for score and write
		WaveManager::CheckForNewHighScore(PlayerInitials);
		WaveManager::WriteHighScoresToFile();

		//do the corresponding actions based on which mode the player is in
		ModeManager::PressEnterOnHighScoreAction();

		//Stops Player Input
		Instance().StopInput();
	}


}

void HighScoreInput::Destroy(){
		

}

void HighScoreInput::StopInput(){

	//clear all text from Window
	Instance().PlayerInputList.clear();
	Instance().NotificationList.clear();
	Instance().PleaseEnterInitialsList.clear();

	Instance().DeregisterInput();//deregister input

}