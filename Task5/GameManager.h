#pragma once
#include "PlayGame.h"
#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>
#include <mmsystem.h>
#include <mciapi.h>
#pragma comment(lib,"winmm.lib")
enum GameState
{
	Intro,
	Play,
	RestartOrEnd,
	End
};

class GameManager {
public: 
	PlayGame* playGame;
	GameManager();
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