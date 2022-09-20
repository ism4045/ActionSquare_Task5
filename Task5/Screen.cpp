#include "Screen.h"
#define PLAYBOX_POS {20,3}
#define NEXTBOX_POS {48,2}
#define BOARD_POS {22,4}
#define NEXTBLOCK_POS {52,5}
#define INFO_POS {25,9}
#define MANUAL_POS {25,15}
#define PAUSE_POS {29,13}

Screen::Screen(GameManager& gm)
{
	gameManager = &gm;

	CONSOLE_CURSOR_INFO cci;
	doubleBuffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	doubleBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	dw = 0;

	cci.dwSize = 1;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(doubleBuffer[0], &cci);
	SetConsoleCursorInfo(doubleBuffer[1], &cci);

	screenIndex = true;
	introStart = clock();
	introCurrent = introStart;
	blink = false;
}

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

void Screen::ScreenFlipping()
{
	SetConsoleActiveScreenBuffer(doubleBuffer[screenIndex]);
	screenIndex = !screenIndex;
}

void Screen::ScreenClear()
{
	COORD Coor = { 0, 0 };
	FillConsoleOutputCharacter(doubleBuffer[screenIndex], ' ', 100 * 100, Coor, &dw);
}

void Screen::DrawPlayBoard() {
	vector<vector<int>> tempBoard = gameManager->GetBoard();
	pair<short, short> startPos = BOARD_POS;

	for (int i = 0; i < tempBoard.size() - 1; i++) {
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
			WriteFile(doubleBuffer[screenIndex], str.c_str(), (DWORD)strlen(str.c_str()), &dw, NULL);
		}
	}
	ResetColor();
}

void Screen::DrawNextBlock() {
	Block tempNextBlock = gameManager->GetNextBlock();
	pair<short, short> startPos = NEXTBLOCK_POS;

	for (int j = 0; j < tempNextBlock.tileInfo.size(); j++) {
		COORD pos = { startPos.first + (short)tempNextBlock.tileInfo[j].Y * 2,startPos.second + (short)tempNextBlock.tileInfo[j].X };
		SetConsoleCursorPosition(doubleBuffer[screenIndex], pos);

		DecisionBlockColor((int)tempNextBlock.Type);
		string str = "бс";
		WriteFile(doubleBuffer[screenIndex], str.c_str(), (DWORD)strlen(str.c_str()), &dw, NULL);
	}
	ResetColor();

}

void Screen::DrawBackGround()
{
	Draw2DVector(BG.playBox, PLAYBOX_POS);
	Draw2DVector(BG.nextBox, NEXTBOX_POS);
}

void Screen::DrawIntro()
{
	introCurrent = clock();
	if (introCurrent - introStart >= 500.0) {
		blink = !blink;
		introStart = introCurrent;
	}
	vector<string> temp = BG.introStr;
	if (blink) temp[2] = " ";
	Draw1DVector(temp, PLAYBOX_POS);
}

void Screen::DrawPlayerInfo()
{
	vector<string> temp = BG.infoStr;
	temp[0] += to_string(gameManager->GetLevel());
	temp[2] += to_string(gameManager->GetLine());
	temp[4] += to_string(gameManager->GetScore());
	Draw1DVector(temp, INFO_POS);
}

void Screen::DrawROE()
{
	vector<string> temp = { BG.ROEStr[0],BG.ROEStr[1] };
	temp.push_back(gameManager->GetEndMenu() ? BG.ROEStr[2] : BG.ROEStr[3]);
	Draw1DVector(temp, PLAYBOX_POS);
}

void Screen::DrawManual()
{
	Draw1DVector(BG.manual, MANUAL_POS);
}

void Screen::DrawPause()
{
	COORD pos = PAUSE_POS;
	SetConsoleCursorPosition(doubleBuffer[screenIndex], pos);
	string str = "Pause";
	WriteFile(doubleBuffer[screenIndex], str.c_str(), (DWORD)strlen(str.c_str()), &dw, NULL);
}

void Screen::Draw2DVector(vector<vector<string>> arr, pair<short, short> startPos)
{
	for (int i = 0; i < arr.size(); i++) {
		for (int j = 0; j < arr[i].size(); j++) {
			COORD pos = { startPos.first + (short)j * 2,startPos.second + (short)i };
			SetConsoleCursorPosition(doubleBuffer[screenIndex], pos);
			string str = arr[i][j];
			WriteFile(doubleBuffer[screenIndex], str.c_str(), (DWORD)strlen(str.c_str()), &dw, NULL);
		}
	}
}

void Screen::Draw1DVector(vector<string> arr, pair<short, short> startPos)
{
	for (int i = 0; i < arr.size(); i++) {
		COORD pos = { startPos.first * 2, startPos.second };
		SetConsoleCursorPosition(doubleBuffer[screenIndex], pos);
		string str = arr[i];
		WriteFile(doubleBuffer[screenIndex], str.c_str(), (DWORD)strlen(str.c_str()), &dw, NULL);
		startPos.second++;
	}
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
		if (gameManager->GetStop())
			DrawPause();
		else 
			DrawPlayBoard();

		DrawNextBlock();
		DrawPlayerInfo();
		DrawManual();
		break;

	case GameState::RestartOrEnd:
		DrawROE();
		break;

	default:
		break;
	}

	ScreenFlipping();
}
