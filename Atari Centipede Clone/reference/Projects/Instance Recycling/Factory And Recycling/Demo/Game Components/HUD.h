// HUD
// Andre Berthiaume, March 2013

#ifndef _HUD
#define _HUD

#include "TEAL\CommonElements.h"

class HUD : public GameObject
{
public:
	HUD();
	virtual void Update();
	virtual void Draw();

	static void ScoreUpdate(int v);	// Callback used when score changes

private:
	sf::Font ScoreFont;
	int pointsize;

	static std::string Label;
	static sf::Color ScoreColor;
	static sf::Text ScoreDisplay;

	static sf::Sound ScoreSnd;

};

#endif _HUD