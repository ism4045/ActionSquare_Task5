#include "GameManager.h"

GameManager::GameManager(PlayGame& pg)
{	
	PG = &pg;
	endMenu = true;
	Initialize();
}

void GameManager::Initialize()
{
	gameState = GameState::Intro;
	endMenu = true;
}

void GameManager::EnterPlay()
{
	gameState = GameState::Play;
}

void GameManager::DecisionEnd()
{
	if (endMenu) {
		gameState = GameState::Play;
		PG->Initialize();
	}
	else
		gameState = GameState::End;
}

void GameManager::SelectROE_L()
{
	endMenu = true;
}

void GameManager::SelectROE_R()
{
	endMenu = false;
}

void GameManager::Update()
{
	if (gameState == Play) {
		PG->Update();
		if (PG->GetPlayGameState() == PlayGameState::GameOver)
			gameState = RestartOREnd;
	}
}
