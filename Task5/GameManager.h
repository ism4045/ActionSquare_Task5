#pragma once
#include "PlayGame.h"

enum GameState
{
	Intro,
	Play,
	RestartOREnd,
	End
};

class GameManager {
public: 
	PlayGame* PG;
	GameManager(PlayGame& pg);
	void Initialize();

	void EnterPlay();

	void SelectROE_L();
	void SelectROE_R();
	void DecisionEnd();

	void Update();
	GameState GetGameState() { return gameState; }
	bool GetEndMenu() { return endMenu; }
private:
	GameState gameState;
	bool endMenu;
};