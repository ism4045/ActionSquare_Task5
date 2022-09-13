#pragma once

#include <Windows.h>
#include <ctime>
#include <vector>
#include "BackGround.h"
#include "Player.h"
#include "Block.h"
#include <string>
using namespace std;

class Screen {
public:
	Screen(Player& P);
	void ScreenFlipping();
	void ScreenClear();

	void DrawBackGround();
	void DrawPlayBoard();
	void DrawNextBlock();
	void DrawPlayerInfo();
	void DrawManual();
	void Render();

	void TextColor(int foreGround, int backGround);
	void DecisionBlockColor(int blockType);
	void ResetColor();
private:
	BackGround BG;
	Player* player;

	bool screenIndex;
	HANDLE doubleBuffer[2];

	pair<int, int> centerPos;
};