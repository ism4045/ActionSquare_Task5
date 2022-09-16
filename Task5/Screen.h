#pragma once

#include <Windows.h>
#include <ctime>
#include <vector>
#include "BackGround.h"
#include "PlayGame.h"
#include "Block.h"
#include <string>
using namespace std;

class Screen {
public:
	Screen(PlayGame& P);
	void ScreenFlipping();
	void ScreenClear();

	void DrawBackGround();
	void DrawManual();

	void DrawPlayBoard();
	void DrawNextBlock();
	void DrawPlayerInfo();

	void Render();

	void TextColor(int foreGround, int backGround);
	void DecisionBlockColor(int blockType);
	void ResetColor();
private:
	BackGround BG;
	PlayGame* game;

	bool screenIndex;
	HANDLE doubleBuffer[2];

	pair<int, int> centerPos;
};