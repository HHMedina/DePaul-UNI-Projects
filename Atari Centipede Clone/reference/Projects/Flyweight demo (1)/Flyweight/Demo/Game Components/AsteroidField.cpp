// AsteroidField
// Andre Berthiaume, March 2013

#include "AsteroidField.h"
#include "Asteroid.h"

AsteroidField::AsteroidField()
{
	SetAlarm(0,2);

	GENLINE =  WindowManager::MainWindow.getSize().x;
	TOP = 100;
	BOTTOM = WindowManager::MainWindow.getSize().y - 30;
}

void AsteroidField::Alarm0()
{
	new Asteroid( sf::Vector2f( (float) GENLINE, (float) ( TOP + rand() % (BOTTOM - TOP))) );

	SetAlarm(0, .25f + rand() % 1);
}