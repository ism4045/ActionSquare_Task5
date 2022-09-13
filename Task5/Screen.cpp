#include "Screen.h"

Screen::Screen(Player& P)
{
	player = &P;

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

	startPos = { 29,2 };
	for (int i = 0; i < BG.nextBox.size(); i++) {
		for (int j = 0; j < BG.nextBox[i].size(); j++) {
			COORD pos = { startPos.first + (short)j * 2,startPos.second + (short)i };
			SetConsoleCursorPosition(doubleBuffer[screenIndex], pos);
			string str = BG.nextBox[i][j];
			WriteFile(doubleBuffer[screenIndex], str.c_str(), strlen(str.c_str()), &dw, NULL);
		}
	}
}

void Screen::DrawPlayBoard() {
	DWORD dw;
	pair<short, short> startPos = { 7,4 };
	for (int i = 0; i < player->board.size()-1; i++) {
		for (int j = 0; j < player->board[i].size(); j++) {
			COORD pos = { startPos.first + (short)j * 2,startPos.second + (short)i };
			SetConsoleCursorPosition(doubleBuffer[screenIndex], pos);
			string str = "";
			if (player->board[i][j] == 8) str = " ";
			else str = "бс";
			WriteFile(doubleBuffer[screenIndex], str.c_str(), strlen(str.c_str()), &dw, NULL);
		}
	}
}

void Screen::DrawNextBlock() {
	DWORD dw;
	pair<short, short> startPos = { 33,5 };
	for (int i = 0; i < player->nextBlock.size(); i++) {
		for (int j = 0; j < player->nextBlock[i].tileInfo.size(); j++) {
			COORD pos = { startPos.first + (short)player->nextBlock[i].tileInfo[j].Y * 2,startPos.second + (short)player->nextBlock[i].tileInfo[j].X };
			SetConsoleCursorPosition(doubleBuffer[screenIndex], pos);
			string str = "бс";
			WriteFile(doubleBuffer[screenIndex], str.c_str(), strlen(str.c_str()), &dw, NULL);
		}
		startPos = { 33,startPos.second + 3 };
	}

}

void Screen::Render()
{
	ScreenClear();
	
	DrawBackGround();
	DrawPlayBoard();
	DrawNextBlock();

	ScreenFlipping();
}
