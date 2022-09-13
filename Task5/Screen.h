#pragma once

#include <Windows.h>
#include <ctime>
#include <vector>
#include "BackGround.h"
#include "Player.h"

using namespace std;

class Screen {
public:
	BackGround BG;
	Player* player;
	Screen(Player& P);
	void ScreenFlipping();
	void ScreenClear();

	void DrawBackGround();
	void DrawPlayBoard();
	void DrawNextBlock();
	void Render();
private:
	bool screenIndex;
	HANDLE doubleBuffer[2];
};