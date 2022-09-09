#include "Screen.h"

Screen::Screen()
{

	CONSOLE_CURSOR_INFO cci;

	doubleBuffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	doubleBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	cci.dwSize = 1;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(doubleBuffer[0], &cci);
	SetConsoleCursorInfo(doubleBuffer[1], &cci);

	screenIndex = true;
}

void Screen::ScreenFlipping()
{
	SetConsoleActiveScreenBuffer(doubleBuffer[screenIndex]);
	screenIndex = !screenIndex;
}

void Screen::ScreenClear()
{
	COORD Coor = { 0, 0 };
	DWORD dw;
	FillConsoleOutputCharacter(doubleBuffer[screenIndex], ' ', 100 * 100, Coor, &dw);
}

void Screen::DrawBackGround()
{
	DWORD dw;
	pair<short, short> startPos = { 5,3 };
	for (int i = 0; i < BG.PlayBox.size(); i++) {
		for (int j = 0; j < BG.PlayBox[i].size(); j++) {
			COORD pos = { startPos.first+(short)j * 2,startPos.second+(short)i };
			SetConsoleCursorPosition(doubleBuffer[screenIndex], pos);
			string str = BG.PlayBox[i][j];
			WriteFile(doubleBuffer[screenIndex], str.c_str(), strlen(str.c_str()), &dw, NULL);
		}
	}
}

void Screen::Render()
{
	ScreenClear();
	
	DrawBackGround();

	ScreenFlipping();
}
