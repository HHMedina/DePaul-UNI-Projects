#ifndef HIGHSCOREINPUT_H
#define HIGHSCOREINPUT_H

#include "TEAL\CommonElements.h"

#include "AwesomeFont.h"
#include <list>

class HighScoreInput: GameObject{


private:
	static HighScoreInput * instance;
	
	HighScoreInput();
	HighScoreInput(const HighScoreInput&){}
	HighScoreInput& operator =(const HighScoreInput&){}
	~HighScoreInput();

	static HighScoreInput& Instance()
	{
		if(!instance){
			instance = new HighScoreInput();
		}
		return *instance;
	}


	AwesomeFont CentFont;//font

	std::string PlayerInitials;
	int NumberOfLettersEntered;

	//lists holding glyphs
	std::list<Glyph> NotificationList;
	std::list<Glyph> PlayerInputList;
	std::list<Glyph> PleaseEnterInitialsList;


	//starting positions of glyphs
	sf::Vector2f NotificationPosition;
	sf::Vector2f PlayerInputPosition;
	sf::Vector2f PleaseEnterInitialsPosition;

	static const int MUSHROOMSEGMENT =16;
	static const int MAXIMUMINPUTCHARS =3;

	static const std::string NotificationText;
	static const std::string PleaseEnterInitialsText;

public:

	virtual void Draw();
	virtual void Destroy();
	virtual void Collision(GameObject* other){}
	virtual void KeyPressed(sf::Keyboard::Key k,bool altKey, bool ctrlKey, bool shiftKey, bool systemKey);
	virtual void TextEntered(sf::Uint32 UnicodeValue);

	static void StartInput();
	static void StopInput();
};


#endif HIGHSCOREINPUT_H