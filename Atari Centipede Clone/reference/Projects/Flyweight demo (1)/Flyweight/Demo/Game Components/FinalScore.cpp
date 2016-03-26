// FinalScore
// Andre Berthiaume, March 2013

#include "FinalScore.h"
#include "GameController.h"
#include "Level1.h"

FinalScore::FinalScore()
{
	Label = "Your Score ";
	ScoreColor = sf::Color(255,255,0,0);
	pointsize = 120;
	ScoreFont = ResourceManager::GetFont("PointFont");
	ScoreDisplay = sf::Text(Label + Tools::ToString( GameController::Instance().GetScore()  ) , ScoreFont, pointsize );

	SetDrawOrder(10000);

	fadeInterval = .01f;
	SetAlarm(0, fadeInterval );

	RegisterInput( InputFlags::KeyPressed );
}

FinalScore::~FinalScore()
{
	DeregisterInput();
}

void FinalScore::Alarm0()
{
	if ( ScoreColor.a < 250 )
	{
		ScoreColor.a += 4;
		SetAlarm(0, fadeInterval);
	}
}

void FinalScore::KeyPressed (sf::Keyboard::Key k, bool altKey, bool ctrlKey, bool shiftKey, bool systemKey)
{
	 SceneManager::ChangeScene( new Level1 );
}

void FinalScore::Draw()
{	
	ScoreDisplay.setColor( ScoreColor );
	sf::Vector2f p = sf::Vector2f(  WindowManager::MainWindow.getSize().x / 2.0f, 300);

	ScoreDisplay.setCharacterSize(pointsize);
	ScoreDisplay.setString(Label);
	Center(ScoreDisplay, p );
	WindowManager::MainWindow.draw( ScoreDisplay  );

	ScoreDisplay.setString( Tools::ToString( GameController::Instance().GetScore() ) );
	p.y += pointsize;
	Center(ScoreDisplay, p );
	WindowManager::MainWindow.draw( ScoreDisplay  );

	ScoreDisplay.setString( "Press any key to try again" );
	ScoreDisplay.setCharacterSize( pointsize / 3 );
	p.y += 1.5f * pointsize;
	Center(ScoreDisplay, p );
	WindowManager::MainWindow.draw( ScoreDisplay  );
}

void FinalScore::Center( sf::Text& T, sf::Vector2f pos )
{
	T.setPosition(pos);
	T.setOrigin(.5f * T.getLocalBounds().width, T.getFont().getLineSpacing(pointsize) - .5f * T.getLocalBounds().height );
}