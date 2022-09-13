#include "Screen.h"




void Screen::TextColor(int foreGround, int backGround) {
	int color = foreGround + backGround * 16;
	SetConsoleTextAttribute(doubleBuffer[screenIndex], color);
}


void Screen::DecisionBlockColor(int blockType) {
	if (blockType == Z1) TextColor(12, 0);
	else if (blockType == Z2) TextColor(10, 0);
	else if (blockType == L1) TextColor(9, 0);
	else if (blockType == L2) TextColor(6, 0);
	else if (blockType == T) TextColor(11, 0);
	else if (blockType == I) TextColor(5, 0);
	else if (blockType == O) TextColor(14, 0);
}

void Screen::ResetColor() { TextColor(15, 0); }

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
	centerPos = { 20,3 };
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
	pair<short, short> startPos = centerPos;
	for (int i = 0; i < BG.PlayBox.size(); i++) {
		for (int j = 0; j < BG.PlayBox[i].size(); j++) {
			COORD pos = { startPos.first+(short)j * 2,startPos.second+(short)i };
			SetConsoleCursorPosition(doubleBuffer[screenIndex], pos);
			string str = BG.PlayBox[i][j];
			WriteFile(doubleBuffer[screenIndex], str.c_str(), strlen(str.c_str()), &dw, NULL);
		}
	}

	startPos = { centerPos.first + 28,centerPos.second - 1 };
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
	vector<vector<int>> tempBoard = player->getBoard();
	pair<short, short> startPos = { centerPos.first+2,centerPos.second+1 };
	for (int i = 0; i < tempBoard.size()-1; i++) {
		for (int j = 0; j < tempBoard[i].size(); j++) {
			COORD pos = { startPos.first + (short)j * 2,startPos.second + (short)i };
			SetConsoleCursorPosition(doubleBuffer[screenIndex], pos);
			DecisionBlockColor(tempBoard[i][j]);
			string str = "";
			if (tempBoard[i][j] == 8) str = " ";
			else str = "бс";
			WriteFile(doubleBuffer[screenIndex], str.c_str(), strlen(str.c_str()), &dw, NULL);
		}
	}
	ResetColor();
}

void Screen::DrawNextBlock() {
	DWORD dw;
	vector<Block> tempNextBlock = player->getNextBlock();
	pair<short, short> startPos = { centerPos.first+32,centerPos.second+2 };
	for (int j = 0; j < tempNextBlock[0].tileInfo.size(); j++) {
		COORD pos = { startPos.first + (short)tempNextBlock[0].tileInfo[j].Y * 2,startPos.second + (short)tempNextBlock[0].tileInfo[j].X };
		SetConsoleCursorPosition(doubleBuffer[screenIndex], pos);
		DecisionBlockColor((int)tempNextBlock[0].Type);
		string str = "бс";
		WriteFile(doubleBuffer[screenIndex], str.c_str(), strlen(str.c_str()), &dw, NULL);
	}
	ResetColor();

}

void Screen::DrawPlayerInfo()
{
	int tempLevel = player->getLevel();
	int templine = player->getLine();
	int tempScore = player->getScore();
	DWORD dw;
	pair<short, short> startPos = { centerPos.first + 5,centerPos.second +6 };
	COORD pos = { startPos.first * 2, startPos.second };
	SetConsoleCursorPosition(doubleBuffer[screenIndex], pos);
	string str = "Level : " + to_string(tempLevel);
	WriteFile(doubleBuffer[screenIndex], str.c_str(), strlen(str.c_str()), &dw, NULL);
	pos.Y += 2;
	SetConsoleCursorPosition(doubleBuffer[screenIndex], pos);
	str = "Line : " + to_string(templine);
	WriteFile(doubleBuffer[screenIndex], str.c_str(), strlen(str.c_str()), &dw, NULL);
	pos.Y += 2;
	SetConsoleCursorPosition(doubleBuffer[screenIndex], pos);
	str = "Score : " + to_string(tempScore);
	WriteFile(doubleBuffer[screenIndex], str.c_str(), strlen(str.c_str()), &dw, NULL);
}

void Screen::DrawManual()
{
	DWORD dw;
	pair<short, short> startPos = { centerPos.first + 5,centerPos.second + 14 };
	for (int i = 0; i < BG.manual.size(); i++) {
		COORD pos = { startPos.first * 2, startPos.second };
		SetConsoleCursorPosition(doubleBuffer[screenIndex], pos);
		string str = BG.manual[i];
		WriteFile(doubleBuffer[screenIndex], str.c_str(), strlen(str.c_str()), &dw, NULL);
		startPos.second += 2;
	}
}

void Screen::Render()
{
	ScreenClear();
	
	DrawBackGround();
	DrawPlayBoard();
	DrawNextBlock();
	DrawPlayerInfo();
	DrawManual();

	ScreenFlipping();
}
