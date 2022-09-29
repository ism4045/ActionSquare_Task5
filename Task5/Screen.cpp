#include "Screen.h"
#define PLAYBOX_POS {20,3}
#define INTROBOX_POS {20,3}
#define NEXTBOX_POS {48,2}
#define BOARD_POS {22,4}
#define NEXTBLOCK_POS {52,5}
#define INFO_POS {25,9}
#define MANUAL_POS {17,15}
#define PAUSE_POS {29,13}
#define PLAYER_POS -15
#define AI_POS 45

Screen::Screen(Tetris& t1, Tetris& t2)
{
	PlayerTetris = &t1;
	AITetris = &t2;

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

void Screen::DrawPlayBoard(Tetris* tetris) {
	vector<vector<int>> tempBoard = tetris->GetBoard();
	pair<short, short> startPos = BOARD_POS;
	tetris->GetAImode() ? startPos.first += AI_POS : startPos.first += PLAYER_POS;
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

void Screen::DrawNextBlock(Tetris* tetris) {
	Block tempNextBlock = tetris->GetNextBlock();
	pair<short, short> startPos = NEXTBLOCK_POS;
	tetris->GetAImode() ? startPos.first += AI_POS : startPos.first += PLAYER_POS;
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
	pair<int, int> playerBox1 = PLAYBOX_POS;
	pair<int, int> playerBox2 = PLAYBOX_POS;
	pair<int, int> nextBox1 = NEXTBOX_POS;
	pair<int, int> nextBox2 = NEXTBOX_POS;

	playerBox1.first += AI_POS;
	playerBox2.first += PLAYER_POS;
	nextBox1.first += AI_POS;
	nextBox2.first += PLAYER_POS;

	Draw2DVector(BG.playBox, playerBox1);
	Draw2DVector(BG.playBox, playerBox2);
	Draw2DVector(BG.nextBox, nextBox1);
	Draw2DVector(BG.nextBox, nextBox2);
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
	Draw1DVector(temp, INTROBOX_POS);
}

void Screen::DrawPlayerInfo(Tetris* tetris)
{
	pair<int, int> startPos = INFO_POS;
	tetris->GetAImode() ? startPos.first += AI_POS/2 : startPos.first += -8;
	vector<string> temp = BG.infoStr;
	temp[0] += to_string(tetris->GetLevel());
	temp[2] += to_string(tetris->GetLine());
	temp[4] += to_string(tetris->GetScore());
	Draw1DVector(temp, startPos);
}

void Screen::DrawROE(bool endMenu, bool winner)
{
	vector<string> temp = { BG.ROEStr[0],BG.ROEStr[1] };
	temp.push_back(endMenu ? BG.ROEStr[2] : BG.ROEStr[3]);
	temp.push_back(BG.ROEStr[4]);
	temp.push_back(BG.ROEStr[5]);
	temp.push_back(winner ? BG.ROEStr[6] : BG.ROEStr[7]);
	temp.push_back(BG.ROEStr[8]);
	temp.push_back(BG.ROEStr[9]);
	temp.insert(temp.end(), BG.infoStr.begin(), BG.infoStr.end());

	if (winner) {
		temp[8] = "    " + temp[8] + to_string(PlayerTetris->GetLevel());
		temp[10] = "    " + temp[10] + to_string(PlayerTetris->GetLine());
		temp[12] = "    " + temp[12] + to_string(PlayerTetris->GetScore());
	}
	else {
		temp[8] = "    " + temp[8] + to_string(AITetris->GetLevel());
		temp[10] = "    " + temp[10] + to_string(AITetris->GetLine());
		temp[12] = "    " + temp[12] + to_string(AITetris->GetScore());
	}
	Draw1DVector(temp, INTROBOX_POS);
}

void Screen::DrawManual()
{
	Draw1DVector(BG.manual, MANUAL_POS);
}

void Screen::DrawPause(Tetris* tetris)
{
	pair<int, int> startPos = PAUSE_POS;
	tetris->GetAImode() ? startPos.first += AI_POS : startPos.first += PLAYER_POS;
	COORD pos = { startPos.first, startPos.second };
	SetConsoleCursorPosition(doubleBuffer[screenIndex], pos);
	string str = "Pause";
	WriteFile(doubleBuffer[screenIndex], str.c_str(), (DWORD)strlen(str.c_str()), &dw, NULL);
}

void Screen::DrawGameOver(Tetris* tetris)
{
	pair<int, int> startPos = PAUSE_POS;
	startPos.first -= 1;
	tetris->GetAImode() ? startPos.first += AI_POS : startPos.first += PLAYER_POS;
	COORD pos = { startPos.first, startPos.second };
	SetConsoleCursorPosition(doubleBuffer[screenIndex], pos);
	string str = "Game Over";
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

void Screen::RecieveHandle(int stage, bool endMenu, bool winner)
{
	ScreenClear();

	switch (stage)
	{
	case 0:
		DrawIntro();
		break;

	case 1:
		DrawBackGround();

		if (PlayerTetris->GetStop())
			DrawPause(PlayerTetris);
		else if (PlayerTetris->GetPlayGameState() == PlayGameState::GameOver)
			DrawGameOver(PlayerTetris);
		else
			DrawPlayBoard(PlayerTetris);

		if (AITetris->GetStop())
			DrawPause(AITetris);
		else if (AITetris->GetPlayGameState() == PlayGameState::GameOver)
			DrawGameOver(AITetris);
		else
			DrawPlayBoard(AITetris);

		DrawNextBlock(PlayerTetris);
		DrawPlayerInfo(PlayerTetris);
		DrawNextBlock(AITetris);
		DrawPlayerInfo(AITetris);

		DrawManual();
		break;

	case 2:
		DrawROE(endMenu, winner);
		break;

	default:
		break;
	}

	ScreenFlipping();
}
