// WidgetGenerator
// Andre Berthiaume, March 2013

#include "WidgetGenerator.h"
#include "Widget.h"

WidgetGenerator::WidgetGenerator()
{
	SetAlarm(0,2);
	GENLINE =  WindowManager::MainWindow.getSize().x;
	TOP = 100;
	BOTTOM = WindowManager::MainWindow.getSize().y - 30;
}

void WidgetGenerator::Alarm0()
{
	new Widget( sf::Vector2f( (float) GENLINE, (float) ( TOP + rand() % (BOTTOM - TOP))) );

	SetAlarm(0, 2.0f + rand() % 5);
}