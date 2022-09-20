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

	void Draw2DVector(vector<vector<string>> arr, pair<short, short> startPos);
	void Draw1DVector(vector<string> arr, pair<short, short> startPos);
	void DrawROE();

	void Render();

	void TextColor(int foreGround, int backGround);
	void DecisionBlockColor(int blockType);
	void ResetColor();

private:
	BackGround BG;
	GameManager* gameManager;

	bool screenIndex;
	HANDLE doubleBuffer[2];
	DWORD dw;

	clock_t introStart;
	clock_t introCurrent;
	bool blink;
};