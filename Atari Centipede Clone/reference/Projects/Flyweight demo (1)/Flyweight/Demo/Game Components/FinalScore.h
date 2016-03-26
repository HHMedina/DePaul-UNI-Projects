// FinalScore
// Andre Berthiaume, March 2013

#ifndef _FinalScore
#define _FinalScore

#include "TEAL\CommonElements.h"

class FinalScore : public GameObject
{
public:
	FinalScore();
	virtual ~FinalScore();
	virtual void Draw();
	virtual void Alarm0();
	virtual void KeyPressed (sf::Keyboard::Key k, bool altKey, bool ctrlKey, bool shiftKey, bool systemKey);

private:
	sf::Font ScoreFont;
	int pointsize;
	std::string Label;
	sf::Color ScoreColor;
	sf::Text ScoreDisplay;

	float fadeInterval;

	void Center( sf::Text& T, sf::Vector2f Pos );

};

#endif _FinalScore