#ifndef GAME_H
#define GAME_H

#include "Azul.h"
#include "P2Engine\P2Engine.h"
#include "DebugVisualizer.h"

class Game : public Engine
{

public:
	Game( const char * const windowName, int widthScreen, int heightScreen );

	void Initialize() override;
	void LoadContent() override;
	void Update() override;
	void Draw() override;
	void UnLoadContent() override;

	virtual void InitializeContext();
	virtual void LoadGameContent();
	virtual void UnloadGameContent();
	virtual void TerminateContext();

private:

};



#endif 