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

void Screen::DrawPath(const vector<vector<int>>& map)
{
	DWORD dw;
	for (int i = 0; i < map.size(); i++) {
		for (int j = 0; j < map[i].size(); j++) {
			COORD pos = { (short)j * 2,(short)i };
			SetConsoleCursorPosition(doubleBuffer[screenIndex], pos);

			if (map[i][j] == 1) {
				WriteFile(doubleBuffer[screenIndex], "бс", strlen("бс"), &dw, NULL);
			}
		}
	}
}

void Screen::DrawCursor(const int& x, const int& y, const char* str)
{
	DWORD dw;
	COORD pos = { (short)x * 2,(short)y };

	SetConsoleCursorPosition(doubleBuffer[screenIndex], pos);
	WriteFile(doubleBuffer[screenIndex], str, strlen(str), &dw, NULL);
}

void Screen::Render(const int& x, const int& y, const vector<vector<int>>& map, const char* str)
{
	ScreenClear();

	DrawPath(map);
	DrawCursor(x, y, str);

	ScreenFlipping();
}
