#pragma once

#include <Windows.h>
#include <ctime>
#include <vector>
#include "BackGround.h"
using namespace std;

class Screen {
public:
	BackGround BG;

	Screen();
	void ScreenFlipping();
	void ScreenClear();

	void DrawBackGround();
	void Render();
private:
	bool screenIndex;
	HANDLE doubleBuffer[2];
};