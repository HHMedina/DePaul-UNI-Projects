#include "HUD.h"
#include "RecyclingMushroomFactory.h"
#include "Mushroom.h"
#include "WaveManager.h"
#include "PlayerManager.h"
#include "Mushroomfield.h"
#include "Player.h"
#include "Player1.h"
#include "Player2.h"

HUD* HUD::instance =NULL;
const std::string HUD::HighScoresText = "HIGH SCORES";
const std::string HUD::CopyrightText = "[1980 ATARI";
const std::string HUD::CreditsText = "CREDITS 2";
const std::string HUD::BonusText = "BONUS EVERY 12000";
HUD::HUD(){

	DummyHighestScore=0;

	Player1ScorePosition = sf::Vector2f( (MUSHROOMSEGMENT*5) , 0 );
	Player2ScorePosition = sf::Vector2f( (MUSHROOMSEGMENT*29) , 0 );


	HighScorePosition = sf::Vector2f( (MUSHROOMSEGMENT*16) , 0 );
	Player1LivesRemainingPosition = sf::Vector2f( (MUSHROOMSEGMENT*6) , 0 );
	Player2LivesRemainingPosition = sf::Vector2f( (MUSHROOMSEGMENT*(29-6)) , 0 );

	TextSayingHighScoresPosition = sf::Vector2f((MUSHROOMSEGMENT*10),(MUSHROOMSEGMENT*2));
	CopyrightListPosition = sf::Vector2f((MUSHROOMSEGMENT*10),WindowManager::MainWindow.getView().getSize().y-MUSHROOMSEGMENT);
	CreditsTextListPosition = sf::Vector2f((MUSHROOMSEGMENT*10),MUSHROOMSEGMENT*13);
	BonusTextListPosition = sf::Vector2f((MUSHROOMSEGMENT*7),MUSHROOMSEGMENT*14);
	HighScoresListPosition = sf::Vector2f((MUSHROOMSEGMENT*15),(MUSHROOMSEGMENT*3));
	NamesListPosition = sf::Vector2f((MUSHROOMSEGMENT*17),(MUSHROOMSEGMENT*3));
	

}

void HUD::SetPlayer1Score(const int currentScore){

	sf::Vector2f InitialPlacement = Instance().Player1ScorePosition;

	Instance().Player1ScoreText.clear();//clears the list (old score)

	//sets the new score
	std::string Player1Score = Tools::ToString(currentScore);

	if(Player1Score.length()<2){
		
		Instance().Player1ScoreText.push_back(  Instance().CentFont.GetGlyph('0', InitialPlacement-sf::Vector2f(Instance().CentFont.CellWidth(),0 ) ) );
	}

	for (int i=Player1Score.length()-1; i >-1; i--)
	{
		Instance().Player1ScoreText.push_back(  Instance().CentFont.GetGlyph(Player1Score.at(i), InitialPlacement) );
		InitialPlacement.x -= Instance().CentFont.CellWidth();
	}
	

}

void HUD::SetPlayer2Score(const int currentScore){

	sf::Vector2f InitialPlacement = Instance().Player2ScorePosition;

	Instance().Player2ScoreText.clear();//clears the list (old score)

	//sets the new score
	std::string Player2Score = Tools::ToString(currentScore);

	if(Player2Score.length()<2){
		
		Instance().Player2ScoreText.push_back(  Instance().CentFont.GetGlyph('0', InitialPlacement-sf::Vector2f(Instance().CentFont.CellWidth(),0 ) ) );
	}

	for (int i=Player2Score.length()-1; i >-1; i--)
	{
		Instance().Player2ScoreText.push_back(  Instance().CentFont.GetGlyph(Player2Score.at(i), InitialPlacement) );
		InitialPlacement.x -= Instance().CentFont.CellWidth();
	}
	

}

void HUD::SetHighScore(const int highScore){

	sf::Vector2f InitialPlacement = Instance().HighScorePosition;

	Instance().HighScoreText.clear();//clears the list (old score)

	//sets the new score
	std::string HighScore = Tools::ToString(highScore);

	if(HighScore.length()<2){
		
		Instance().HighScoreText.push_back(  Instance().CentFont.GetGlyph('0', InitialPlacement-sf::Vector2f(Instance().CentFont.CellWidth(),0 ) ) );
	}

	for (int i=HighScore.length()-1; i >-1; i--)
	{
		Instance().HighScoreText.push_back(  Instance().CentFont.GetGlyph(HighScore.at(i), InitialPlacement) );
		InitialPlacement.x -= Instance().CentFont.CellWidth();
	}

}

void HUD::SetPlayer1LivesRemaining(const int lives){

	
	sf::Vector2f InitialPlacement = Instance().Player1LivesRemainingPosition;

	Instance().Player1LivesRemainingText.clear();//clears the list (old lives)

	//sets the new lives remaining
	for (int i=0; i <lives; i++)
	{
		Instance().Player1LivesRemainingText.push_back(  Instance().CentFont.GetGlyph('#', InitialPlacement) );
		InitialPlacement.x += Instance().CentFont.CellWidth();
	}
}

void HUD::SetPlayer2LivesRemaining(const int lives){

	
	sf::Vector2f InitialPlacement = Instance().Player2LivesRemainingPosition;

	Instance().Player2LivesRemainingText.clear();//clears the list (old lives)

	//sets the new lives remaining
	for (int i=0; i <lives; i++)
	{
		Instance().Player2LivesRemainingText.push_back(  Instance().CentFont.GetGlyph('#', InitialPlacement) );
		InitialPlacement.x += Instance().CentFont.CellWidth();
	}
}

void HUD::Draw(){

	
	
	

	std::list<Glyph>::iterator it;
	//Draws the Player1 Score
	for (it = Instance().Player1ScoreText.begin(); it != Instance().Player1ScoreText.end(); it++)
		it->Draw();

	//Draws the Player2 Score
	for (it = Instance().Player2ScoreText.begin(); it != Instance().Player2ScoreText.end(); it++)
		it->Draw();

	//Draws the HighScore
	for (it = Instance().HighScoreText.begin(); it != Instance().HighScoreText.end(); it++)
		it->Draw();

	//Draws the RemainingLives for p1
	for (it = Instance().Player1LivesRemainingText.begin(); it != Instance().Player1LivesRemainingText.end(); it++)
		it->Draw();

	//Draws the RemainingLives for p2
	for (it = Instance().Player2LivesRemainingText.begin(); it != Instance().Player2LivesRemainingText.end(); it++)
		it->Draw();

	//Draw Text saying "High Scores"
	for (it = Instance().TextSayingHighScores.begin(); it != Instance().TextSayingHighScores.end(); it++)
		it->Draw();

	//Draw HighScoresList
	for (it = Instance().HighScoresList.begin(); it != Instance().HighScoresList.end(); it++)
		it->Draw();

	//Draw Copyright text
	for (it = Instance().CopyrightList.begin(); it != Instance().CopyrightList.end(); it++)
		it->Draw();

	//Draw Credits text
	for (it = Instance().CreditsTextList.begin(); it != Instance().CreditsTextList.end(); it++)
		it->Draw();

	//Draw Bonus text
	for (it = Instance().BonusTextList.begin(); it != Instance().BonusTextList.end(); it++)
		it->Draw();

}

void HUD::Initialize(){

	Instance().DummyHighestScore = WaveManager::GetHighScoresList().front().Score;
	Instance().SetHighScore(WaveManager::GetHighScoresList().front().Score);
	
	Instance().Player1ScoreText.clear();
	Instance().Player2ScoreText.clear();
	Instance().Player1LivesRemainingText.clear();
	Instance().Player2LivesRemainingText.clear();

	sf::Vector2f Player1InitialPlacement;

	//set initial "00" of score area.
	std::string InitialScore = "00";
	Player1InitialPlacement = Instance().Player1ScorePosition;

	for (int i=0; i <InitialScore.length(); i++)
	{
		Instance().Player1ScoreText.push_back(  Instance().CentFont.GetGlyph(InitialScore.at(i), Player1InitialPlacement) );
		Player1InitialPlacement.x -= Instance().CentFont.CellWidth();
	}

	sf::Vector2f Player2InitialPlacement = Instance().Player2ScorePosition;

	for (int i=0; i <InitialScore.length(); i++)
	{
		Instance().Player2ScoreText.push_back(  Instance().CentFont.GetGlyph(InitialScore.at(i), Player2InitialPlacement) );
		Player2InitialPlacement.x -= Instance().CentFont.CellWidth();
	}


	//set initial blaster lives visual for p1
	int livesCounter= INITIALNUMBEROFLIVES;
	sf::Vector2f InitialPlacement = Instance().Player1LivesRemainingPosition;

	for (int i=0; i <INITIALNUMBEROFLIVES-1; i++)
	{
		Instance().Player1LivesRemainingText.push_back(  Instance().CentFont.GetGlyph('#', InitialPlacement) );
		InitialPlacement.x += Instance().CentFont.CellWidth();
	}

	//set initial blaster lives visual
	sf::Vector2f InitialPlacement2 = Instance().Player2LivesRemainingPosition;

	for (int i=0; i <INITIALNUMBEROFLIVES; i++)
	{
		Instance().Player2LivesRemainingText.push_back(  Instance().CentFont.GetGlyph('#', InitialPlacement2) );
		InitialPlacement2.x += Instance().CentFont.CellWidth();
	}

}

void HUD::ShowHighScores(){

	Instance().SetHighScore(WaveManager::GetHighScoresList().front().Score);
	std::string CurrentHighScore="";
	std::string CurrentName="";


	//sets glyphs and textmushrooms for the word "2 credits"
	sf::Vector2f CreditsInitialPlacement = Instance().CreditsTextListPosition;
	for (int i=0; i<CreditsText.length(); ++i)
	{
		
		Instance().CreditsTextList.push_back(  Instance().CentFont.GetGlyph(CreditsText.at(i), CreditsInitialPlacement) );
		//creates a mushroom at the location of the text but doesnt check for duplicates (will be removed by either removal of
		//high scores display or by the mushroomfield itself in the end)
		if( &PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(CreditsInitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))) == NULL ){
			PlayerManager::GetPlayer()->GetPlayerField().CreateMushroomAtCoordinate(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(CreditsInitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2)));
			PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(CreditsInitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))).SetImmuneToDamage();
			PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(CreditsInitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))).SetImmuneToPoison();
			PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(CreditsInitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))).Sprite.setTexture(ResourceManager::GetTexture("MushroomTransparency"));
			Instance().TextMushrooms.push(&PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(CreditsInitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))));
		}else{

			PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(CreditsInitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))).SetImmuneToDamage();
			PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(CreditsInitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))).SetImmuneToPoison();
			PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(CreditsInitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))).Sprite.setTexture(ResourceManager::GetTexture("MushroomTransparency"));

		}



		CreditsInitialPlacement.x += Instance().CentFont.CellWidth();
	}
	

	//sets glyphs and textmushrooms for the word "Bonus Every 12000"
	sf::Vector2f BonusInitialPlacement = Instance().BonusTextListPosition;
	for (int i=0; i<BonusText.length(); ++i)
	{
		Instance().BonusTextList.push_back(  Instance().CentFont.GetGlyph(BonusText.at(i), BonusInitialPlacement) );
		
		//creates a mushroom at the location of the text but doesnt check for duplicates (will be removed by either removal of
		//high scores display or by the mushroomfield itself in the end)
		if( &PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(BonusInitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))) == NULL ){
			PlayerManager::GetPlayer()->GetPlayerField().CreateMushroomAtCoordinate(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(BonusInitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2)));
			PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(BonusInitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))).SetImmuneToDamage();
			PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(BonusInitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))).SetImmuneToPoison();
			PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(BonusInitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))).Sprite.setTexture(ResourceManager::GetTexture("MushroomTransparency"));
			Instance().TextMushrooms.push(&PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(BonusInitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))));
		}else{
			PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(BonusInitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))).SetImmuneToDamage();
			PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(BonusInitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))).SetImmuneToPoison();
			PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(BonusInitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))).Sprite.setTexture(ResourceManager::GetTexture("MushroomTransparency"));
		}



		BonusInitialPlacement.x += Instance().CentFont.CellWidth();
	
	}



	//sets glyphs and textmushrooms for the word "HIGH SCORES"
	sf::Vector2f InitialPlacement = Instance().TextSayingHighScoresPosition;
	for (int i=0; i<HighScoresText.length(); ++i)
	{
		Instance().TextSayingHighScores.push_back(  Instance().CentFont.GetGlyph(HighScoresText.at(i), InitialPlacement) );

		//creates a mushroom at the location of the text but doesnt check for duplicates (will be removed by either removal of
		//high scores display or by the mushroomfield itself in the end)
		if( &PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(InitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))) == NULL ){
			PlayerManager::GetPlayer()->GetPlayerField().CreateMushroomAtCoordinate(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(InitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2)));
			PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(InitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))).SetImmuneToDamage();
			PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(InitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))).SetImmuneToPoison();
			PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(InitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))).Sprite.setTexture(ResourceManager::GetTexture("MushroomTransparency"));
			Instance().TextMushrooms.push(&PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(InitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))));
		}else{
			PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(InitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))).SetImmuneToDamage();
			PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(InitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))).SetImmuneToPoison();
			PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(InitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))).Sprite.setTexture(ResourceManager::GetTexture("MushroomTransparency"));
		}
		
		InitialPlacement.x += Instance().CentFont.CellWidth();
	}

	//sets glyphs and textmushrooms for the word "[COPYRIGHT ATARI"
	sf::Vector2f CopyRightInitialPlacement = Instance().CopyrightListPosition;
	for (int i=0; i<CopyrightText.length(); ++i)
	{
		Instance().CopyrightList.push_back(  Instance().CentFont.GetGlyph(CopyrightText.at(i), CopyRightInitialPlacement) );
		CopyRightInitialPlacement.x += Instance().CentFont.CellWidth();
	}



	//sets glyphs and textmushrooms for the block of text that has the scores
	sf::Vector2f ScoreInitialPlacement = Instance().HighScoresListPosition;
	sf::Vector2f NameInitialPlacement = Instance().NamesListPosition;
	std::list<HighScoreCombination>::const_iterator it;

	//const std::list<HighScoreCombination>* TEST = &WaveManager::GetHighScoresList();
	for(it=WaveManager::GetHighScoresList().begin();it!=WaveManager::GetHighScoresList().end();++it){ // for each element in the list
		
		///////this section writes the high scores////
		ScoreInitialPlacement.x = Instance().HighScoresListPosition.x;
		CurrentHighScore=Tools::ToString(it->Score);
		for (int i=CurrentHighScore.length()-1; i >-1; i--)//for each character
		{
			Instance().HighScoresList.push_back(  Instance().CentFont.GetGlyph(CurrentHighScore.at(i), ScoreInitialPlacement) );

			//creates a mushroom at the location of the text but doesnt check for duplicates (will be removed by either removal of
			//high scores display or by the mushroomfield itself in the end)
			if( &PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(ScoreInitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))) == NULL ){
				PlayerManager::GetPlayer()->GetPlayerField().CreateMushroomAtCoordinate(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(ScoreInitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2)));
				PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(ScoreInitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))).SetImmuneToDamage();
				PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(ScoreInitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))).SetImmuneToPoison();
				PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(ScoreInitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))).Sprite.setTexture(ResourceManager::GetTexture("MushroomTransparency"));
				Instance().TextMushrooms.push(&PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(ScoreInitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))));
			
				
			}
			else{
				PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(ScoreInitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))).SetImmuneToDamage();
				PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(ScoreInitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))).SetImmuneToPoison();
				PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(ScoreInitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))).Sprite.setTexture(ResourceManager::GetTexture("MushroomTransparency"));

			}
			ScoreInitialPlacement.x -= Instance().CentFont.CellWidth();
		}
		//////end high score writing///

		///////this section writes the high scores NAMES////
		NameInitialPlacement.x = Instance().NamesListPosition.x;
		CurrentName=it->Name;
		for (int i=0; i<CurrentName.length(); ++i)//for each character
		{
			Instance().HighScoresList.push_back(  Instance().CentFont.GetGlyph(CurrentName.at(i), NameInitialPlacement) );

			//creates a mushroom at the location of the text but doesnt check for duplicates (will be removed by either removal of
			//high scores display or by the mushroomfield itself in the end)
			if( &PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(NameInitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))) == NULL ){
				PlayerManager::GetPlayer()->GetPlayerField().CreateMushroomAtCoordinate(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(NameInitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2)));
				PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(NameInitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))).SetImmuneToDamage();
				PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(NameInitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))).SetImmuneToPoison();
				PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(NameInitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))).Sprite.setTexture(ResourceManager::GetTexture("MushroomTransparency"));
				Instance().TextMushrooms.push(&PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(NameInitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))));
			}else{
				PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(NameInitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))).SetImmuneToDamage();
				PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(NameInitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))).SetImmuneToPoison();
				PlayerManager::GetPlayer()->GetPlayerField().AccessMushroom(PlayerManager::GetPlayer()->GetPlayerField().WorldPositionToMushroomGrid(NameInitialPlacement+sf::Vector2f(MUSHROOMSEGMENT/2,MUSHROOMSEGMENT/2))).Sprite.setTexture(ResourceManager::GetTexture("MushroomTransparency"));

			}

			NameInitialPlacement.x += Instance().CentFont.CellWidth();
		}
		//////end high score writing///


		ScoreInitialPlacement.y+= Instance().CentFont.CellHeight();
		NameInitialPlacement.y+= Instance().CentFont.CellHeight();
	}
}

void HUD::RemoveHighScoresList(){
	
	//clear the highscores text
	Instance().TextSayingHighScores.clear();
	Instance().BonusTextList.clear();
	Instance().CreditsTextList.clear();
	Instance().NamesList.clear();
	Instance().HighScoresList.clear();
	Instance().CopyrightList.clear();
	//remove all text mushrooms
	while(!Instance().TextMushrooms.empty()){
		PlayerManager::GetPlayer()->GetPlayerField().DestroyMushroom(Instance().TextMushrooms.top()->GridPosition);
		Instance().TextMushrooms.pop();
	}


}

HUD::~HUD(){
	
	//Note:: no memoryleak caused by not destructing glyph lists since 
	//they are glyphs and not glyph * (new was never called)

	//deallocates all textmushrooms
	while(!TextMushrooms.empty()){
		delete TextMushrooms.top();
		TextMushrooms.pop();
	}
	
	
	delete instance;

}

void HUD::SetField(Mushroomfield& field){
	Instance().FatherField=&field;
}

void HUD::RemovePlayer2LivesDisplay(){

	Instance().Player2LivesRemainingText.clear();

}

void HUD::RemovePlayer1LivesDisplay(){

	Instance().Player1LivesRemainingText.clear();

}

void HUD::RemovePlayer2ScoreDisplay(){
	Instance().Player2ScoreText.clear();
}

const int& HUD::GetHighestScoreDisplayNumber(){
	return Instance().DummyHighestScore;
}
