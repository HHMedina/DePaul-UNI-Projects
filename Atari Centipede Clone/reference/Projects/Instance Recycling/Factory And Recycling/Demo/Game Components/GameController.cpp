// GameController
// Andre Berthiaume, March 2013

#include "GameController.h"
#include "HUD.h"
#include "FinalScore.h"

GameController* GameController::instance;

GameController::GameController()
{
	ResetScore();
}

void GameController::ResetScore()
{
	Score = 0;
}

void GameController::AddScore(int v)
{
	Score += v;
	HUD::ScoreUpdate(Score);
}

int GameController::GetScore()
{
	return Score;
}

void GameController::PlayerDies()
{
	new FinalScore();
}