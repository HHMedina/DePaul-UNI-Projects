#ifndef HUD_H
#define HUD_H

#include "AwesomeFont.h"
#include <list>
#include <stack>
#include "TEAL\CommonElements.h"
class Mushroom;
class Mushroomfield;
class HUD:public GameObject{

private:
	static HUD * instance;
	HUD();//prevent constructor
	HUD(const HUD&){}//prevent copy construction
	HUD& operator =(const HUD&){}//prevent copy constructor
	~HUD();//cont forget to delete lists

	virtual void Draw();

	static HUD& Instance(){
		if(!instance){
			instance = new HUD();
		}
		return *instance;
	}

	AwesomeFont CentFont;

	//mushroomfield pointer to be able to access mushroom field and create text mushrooms in it
	Mushroomfield* FatherField;


	int DummyHighestScore;

	//each list is a seperateSection of the HUD
	std::list<Glyph> Player1ScoreText;
	std::list<Glyph> Player2ScoreText;
	std::list<Glyph> HighScoreText;
	
	std::list<Glyph> Player1LivesRemainingText;
	std::list<Glyph> Player2LivesRemainingText;

	std::list<Glyph> CreditsTextList;
	std::list<Glyph> BonusTextList;

	std::list<Glyph> TextSayingHighScores;
	std::list<Glyph> HighScoresList;
	std::list<Glyph> NamesList;
	std::list<Glyph> CopyrightList;

	//positions of text
	sf::Vector2f Player1ScorePosition;
	sf::Vector2f Player2ScorePosition;

	sf::Vector2f HighScorePosition;
	sf::Vector2f Player1LivesRemainingPosition;
	sf::Vector2f Player2LivesRemainingPosition;
	sf::Vector2f TextSayingHighScoresPosition;
	sf::Vector2f HighScoresListPosition;
	sf::Vector2f NamesListPosition;
	sf::Vector2f CopyrightListPosition;
	sf::Vector2f CreditsTextListPosition;
	sf::Vector2f BonusTextListPosition;

	//holds the mushrooms used for the text
	std::stack<Mushroom*> TextMushrooms; 

	//consts
	static const int MUSHROOMSEGMENT =16;
	static const int INITIALNUMBEROFLIVES =3;
	static const std::string HighScoresText;
	static const std::string CopyrightText;
	static const std::string BonusText;
	static const std::string CreditsText;

public:

	static const int& GetHighestScoreDisplayNumber();

	static void SetPlayer1Score(const int);
	static void SetPlayer2Score(const int);

	static void RemovePlayer1LivesDisplay();
	static void RemovePlayer2LivesDisplay();

	static void RemovePlayer2ScoreDisplay();

	static void SetPlayer1LivesRemaining(const int);
	static void SetPlayer2LivesRemaining(const int);

	static void SetHighScore(const int);
	
	static void ShowHighScores();
	static void RemoveHighScoresList();

	static void SetField(Mushroomfield&);

	static void Initialize();
};
#endif HUD_H