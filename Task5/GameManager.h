#pragma once
#ifndef _GAMEMANAGER_H

#define _GAMEMANAGER_H

#include "Tetris.h"
#include "TetrisAI.h"
#include "Input.h"
#include "Screen.h"
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
	Tetris* playerTetris;
	TetrisAI* aiTetris;

	GameManager();
	~GameManager();
	void Init();

	void EnterPlay();
	void SelectROE_L();
	void SelectROE_R();
	void DecisionEnd();

	void GameUpdate();
	void Input();
	void Render();

	GameState GetGameState() { return gameState; }
	bool GetEndMenu() { return endMenu; }
private:
	GameState gameState;
	InputManager* inputManager;
	Screen* screen;
	bool endMenu;
};

#endif // !_GAMEMANAGER_H