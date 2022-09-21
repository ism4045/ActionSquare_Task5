#pragma once
#ifndef _GAMEMANAGER_H

#define _GAMEMANAGER_H

#include "Tetris.h"
#include "Input.h"
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
	Tetris* playGame;
	GameManager();
	~GameManager();
	void Init();

	void EnterPlay();
	void SelectROE_L();
	void SelectROE_R();
	void DecisionEnd();

	void GameUpdate();
	void InputManage();

	GameState GetGameState() { return gameState; }
	bool GetEndMenu() { return endMenu; }
private:
	GameState gameState;
	Input* inputManager;
	bool endMenu;
};

#endif // !_GAMEMANAGER_H