#pragma once

#include <Windows.h>
#include <ctime>
#include <vector>
#include "BackGround.h"
#include "GameManager.h"
#include "PlayGame.h"
#include "Block.h"
#include <string>
using namespace std;

class Screen {
public:
	Screen(GameManager& gm);
	void ScreenFlipping();
	void ScreenClear();

	void DrawIntro();

	void DrawBackGround();
	void DrawManual();
	void DrawPause();
	void DrawPlayBoard();
	void DrawNextBlock();
	void DrawPlayerInfo();

	void DrawROE();

	void Render();

	void TextColor(int foreGround, int backGround);
	void DecisionBlockColor(int blockType);
	void ResetColor();

private:
	BackGround BG;
	PlayGame* game;
	GameManager* gameManager;
	bool screenIndex;
	HANDLE doubleBuffer[2];

	pair<int, int> centerPos;

	clock_t introStart;
	clock_t introCurrent;
	bool blink;
};