// HUD
// Andre Berthiaume, March 2013

#include "HUD.h"
#include "GameController.h"

sf::Text HUD::ScoreDisplay;
std::string HUD::Label;
sf::Color HUD::ScoreColor;
sf::Sound HUD::ScoreSnd;

HUD::HUD()
{
	ScoreFont = ResourceManager::GetFont("PointFont");
	pointsize = 80;
	Label = "Score: ";
	ScoreColor = sf::Color(255,255,0,128);

	ScoreDisplay = sf::Text(Label, ScoreFont, pointsize );
	ScoreDisplay.setColor(ScoreColor);
	ScoreDisplay.setPosition( 10, -30 );
	
	ScoreSnd.setBuffer( ResourceManager::GetSound("Ding")  );
	ScoreSnd.setPitch(3);
	ScoreSnd.setVolume(50);

	GameController::Instance().ResetScore();
}

void HUD::ScoreUpdate(int v)
{
	ScoreDisplay.setString( Label + Tools::ToString( GameController::Instance().GetScore() ) );
	ScoreColor.a = 255;
	ScoreSnd.play();
}

void HUD::Update()
{
	if ( ScoreColor.a >= 128)
	{
		ScoreColor.a -= 8;
		ScoreDisplay.setColor(ScoreColor);
	}
}

void HUD::Draw()
{
	WindowManager::MainWindow.draw( ScoreDisplay );
}