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

Screen::Screen(GameManager& gm)
{
	game = gm.PG;
	gameManager = &gm;
	CONSOLE_CURSOR_INFO cci;

	doubleBuffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	doubleBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	cci.dwSize = 1;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(doubleBuffer[0], &cci);
	SetConsoleCursorInfo(doubleBuffer[1], &cci);

	screenIndex = true;
	centerPos = { 20,3 };
	introStart = clock();
	blink = false;
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

void Screen::DrawIntro()
{
	introCurrent = clock();
	if (introCurrent - introStart >= 500.0) {
		blink = !blink;
		introStart = introCurrent;
	}
	DWORD dw;
	pair<short, short> startPos = centerPos;
	for (int i = 0; i < BG.introStr.size(); i++) {
		COORD pos = { startPos.first, startPos.second+i};
		SetConsoleCursorPosition(doubleBuffer[screenIndex], pos);

		string str;
		if (!blink && i == 2)
			str = BG.introStr[i];
		else if (blink && i == 2) 
			str = " ";
		else 
			str = BG.introStr[i];

		WriteFile(doubleBuffer[screenIndex], str.c_str(), strlen(str.c_str()), &dw, NULL);
	}
}

void Screen::DrawBackGround()
{
	DWORD dw;
	pair<short, short> startPos = centerPos;
	for (int i = 0; i < BG.playBox.size(); i++) {
		for (int j = 0; j < BG.playBox[i].size(); j++) {
			COORD pos = { startPos.first+(short)j * 2,startPos.second+(short)i };
			SetConsoleCursorPosition(doubleBuffer[screenIndex], pos);
			string str = BG.playBox[i][j];
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
	vector<vector<int>> tempBoard = game->GetBoard();
	pair<short, short> startPos = { centerPos.first+2,centerPos.second+1 };
	for (int i = 0; i < tempBoard.size()-1; i++) {
		for (int j = 0; j < tempBoard[i].size(); j++) {
			COORD pos = { startPos.first + (short)j * 2,startPos.second + (short)i };
			SetConsoleCursorPosition(doubleBuffer[screenIndex], pos);
			DecisionBlockColor(tempBoard[i][j]);
			string str = "";
			if (tempBoard[i][j] == 8) 
				str = " ";
			else if (tempBoard[i][j] == 9) {
				ResetColor();
				str = "бр";
			}
			else 
				str = "бс";
			WriteFile(doubleBuffer[screenIndex], str.c_str(), strlen(str.c_str()), &dw, NULL);
		}
	}
	ResetColor();
}

void Screen::DrawNextBlock() {
	DWORD dw;
	Block tempNextBlock = game->GetNextBlock();
	pair<short, short> startPos = { centerPos.first+32,centerPos.second+2 };
	for (int j = 0; j < tempNextBlock.tileInfo.size(); j++) {
		COORD pos = { startPos.first + (short)tempNextBlock.tileInfo[j].Y * 2,startPos.second + (short)tempNextBlock.tileInfo[j].X };
		SetConsoleCursorPosition(doubleBuffer[screenIndex], pos);
		DecisionBlockColor((int)tempNextBlock.Type);
		string str = "бс";
		WriteFile(doubleBuffer[screenIndex], str.c_str(), strlen(str.c_str()), &dw, NULL);
	}
	ResetColor();

}

void Screen::DrawPlayerInfo()
{
	int tempLevel = game->GetLevel();
	int templine = game->GetLine();
	int tempScore = game->GetScore();
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

void Screen::DrawROE()
{
	DWORD dw;
	pair<short, short> startPos = centerPos;
	COORD pos = { startPos.first,startPos.second };
	SetConsoleCursorPosition(doubleBuffer[screenIndex], pos);
	string str = "      Game Over     ";
	WriteFile(doubleBuffer[screenIndex], str.c_str(), strlen(str.c_str()), &dw, NULL);
	pos.Y+=2;
	SetConsoleCursorPosition(doubleBuffer[screenIndex], pos);
	if (gameManager->GetEndMenu()) str = "> Restart       End     ";
	else str = "  Restart      >End     ";
	WriteFile(doubleBuffer[screenIndex], str.c_str(), strlen(str.c_str()), &dw, NULL);
}

void Screen::DrawManual()
{
	DWORD dw;
	pair<short, short> startPos = { centerPos.first + 5,centerPos.second + 12 };
	for (int i = 0; i < BG.manual.size(); i++) {
		COORD pos = { startPos.first * 2, startPos.second };
		SetConsoleCursorPosition(doubleBuffer[screenIndex], pos);
		string str = BG.manual[i];
		WriteFile(doubleBuffer[screenIndex], str.c_str(), strlen(str.c_str()), &dw, NULL);
		startPos.second += 2;
	}
}

void Screen::DrawPause()
{
	DWORD dw;
	COORD pos = { centerPos.first + 9,centerPos.second + 10 };
	SetConsoleCursorPosition(doubleBuffer[screenIndex], pos);
	string str = "Pause";
	WriteFile(doubleBuffer[screenIndex], str.c_str(), strlen(str.c_str()), &dw, NULL);
}

void Screen::Render()
{
	ScreenClear();
	switch (gameManager->GetGameState())
	{
	case GameState::Intro:
		DrawIntro();
		break;
	case GameState::Play:
		DrawBackGround();
		if (game->GetStop())
			DrawPause();
		else 
			DrawPlayBoard();
		DrawNextBlock();
		DrawPlayerInfo();
		DrawManual();
		break;
	case GameState::RestartOREnd:
		DrawROE();
		break;
	default:
		break;
	}
	ScreenFlipping();
}
