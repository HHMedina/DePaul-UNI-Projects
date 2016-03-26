// Widget
// Andre Berthiaume, March 2013

#include "Widget.h"

Widget::Widget( sf::Vector2f p ) : Pos(p)
{
	RADIUS = 20.0f;
	ROTSPEED = 180.0f;
	SPEED = 50.0f + rand() % 100;

	pcount = 3 + rand() % 6;
	MainShape = sf::CircleShape( RADIUS, pcount);
	MainShape.setOrigin( RADIUS, RADIUS);

	sf::Color col = sf::Color::Color( 128 + rand() % 128, 128 + rand() % 128, 128 + rand() % 128  );
	MainShape.setOutlineColor( col );
	MainShape.setOutlineThickness(2);
	MainShape.setFillColor( sf::Color::Transparent );

	Value = pcount * 10;

	// creating a label
	LabelFont = ResourceManager::GetFont("PointFont");
	int pointsize = 20;
	PointLabel = sf::Text( Tools::ToString(Value), LabelFont, pointsize);

	// Centering label
	int linespacing = LabelFont.getLineSpacing(pointsize);
	PointLabel.setOrigin( .5f * PointLabel.getLocalBounds().width, linespacing - .5f * PointLabel.getLocalBounds().height ); 
	PointLabel.setColor(col);

	SetCollider(MainShape, true);
	RegisterCollision<Widget>( *this );
}

void Widget::Update()
{
	Pos.x -= Game::FrameTime() * SPEED;
	if (Pos.x < - RADIUS ) MarkForDestroy();  // kill when offscreen

	MainShape.setRotation( Game::TotalTime() * - ROTSPEED );
	MainShape.setPosition(Pos);

	//PointLabel.setRotation( Game::TotalTime() * ROTSPEED );
	PointLabel.setPosition(Pos );
}

void Widget::Destroy()
{
	DeregisterCollision<Widget>( *this );
}

void Widget::Draw()
{
	WindowManager::MainWindow.draw(MainShape);
	WindowManager::MainWindow.draw(PointLabel);
}

int Widget::GetValue()
{
	return Value;
}