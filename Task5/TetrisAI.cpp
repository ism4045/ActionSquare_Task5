#include "TetrisAI.h"

#define AI_PLAY_INTERVAL 200
#define DEFAULTPOS {1,4}
#define BLANK 8

TetrisAI::TetrisAI(Tetris* tetris)
	:tetris(tetris),
	aiCurrent(clock()),
	aiStart(aiCurrent - AI_PLAY_INTERVAL)
{
	Init();
}

TetrisAI::~TetrisAI()
{
	delete tetris;
}

void TetrisAI::Init()
{

}

void TetrisAI::Update()
{
	aiCurrent = clock();
	if ((double)aiCurrent - (double)aiStart >= AI_PLAY_INTERVAL){
		aiStart = aiCurrent;
		PlayAI();
	}
}

void TetrisAI::PlayAI()
{
	if (aiBehaviors.empty()) {
		FillAIBehaviors();
	}

	FUNC_VOID behaviorToDoNow = aiBehaviors.front();
	behaviorToDoNow(tetris);
	aiBehaviors.pop();
}

void TetrisAI::FillAIBehaviors()
{
	Destination dest = GetDestination();
	
	for (int i = 0; i < dest.rotate; i++){
		aiBehaviors.push(&Tetris::RotateBlock);
	}

	pair<int, int> defaultPos = DEFAULTPOS;
	for (int i = 0; i < abs(dest.pos.second - defaultPos.second); i++){
		aiBehaviors.push(dest.pos.second > defaultPos.second ? &Tetris::MoveR : &Tetris::MoveL);
	}

	aiBehaviors.push(&Tetris::DoHardDrop);
}

Destination TetrisAI::GetDestination()
{
	vector<Destination> destVect = Evaluate(*tetris);
	Destination dest = Choose(destVect);

	return dest;
}

vector<Destination> TetrisAI::Evaluate(Tetris t)
{
	vector<Destination> destinations;

	for (int i = 0; i < 4; i++) {
		pair<int, int> pos = t.GetPos();
		t.MoveL();
		while (t.GetPos() != pos) {
			pos = t.GetPos();
			t.MoveL();
		}

		while (t.CanMoveR()) {
			destinations.push_back(EvaluateWeight(t));
			t.MoveR();
		}

		t.RotateBlock();
	}
	

	return destinations;
}

Destination TetrisAI::Choose(vector<Destination> destVect)
{
	int maxWeight = 0; int maxIndex = 0;
	for (int i = 0; i < destVect.size(); i++) {
		if (maxWeight < destVect[i].weight) {
			maxWeight = destVect[i].weight;
			maxIndex = i;
		}
	}

	return destVect[maxIndex];
}

Destination TetrisAI::EvaluateWeight(Tetris t)
{
	Destination dest;
	dest.pos = t.GetPos();
	dest.rotate = t.GetCurrentRotate();
	dest.weight = 0;
	dest.weight += EvaluateHightestHeight(t);
	dest.weight += EvaluateContactTile(t);
	dest.weight += EvaluateBlankSpace(t);
	dest.weight += EvaluateClearLine(t);

	return dest;
}

int TetrisAI::EvaluateHightestHeight(Tetris& t)
{ 
	t.SetPreviewBlock();
	t.RemoveCurrentBlock();

	vector<vector<int>> board = t.GetBoard();
	for (int i = 0; i < board.size() - 1; i++){
		for (int j = 0; j < board[i].size(); j++){
			if (board[i][j] != BLANK){
				t.UpdateCurrentBlock();
				return i;
			}
				
		}
	}

	return 0;
}

int TetrisAI::EvaluateContactTile(Tetris& t)
{
	t.DoHardDrop();
	BlockContactMask mask = BlockContactMask(t.GetBlock().Type);
	int blankNum = 0;

	vector<vector<int>> board = t.GetBoard();

	for (int i = 0; i < t.GetCurrentRotate(); i++) {
		mask.RotateData();
	}

	int cursorX = t.GetPos().first + mask.StartPoint.first;
	int cursorY = t.GetPos().second + mask.StartPoint.second;
	for (int i = 0; i < mask.Data.size() - 1; i++){
		for (int j = 0; j < mask.Data[i].size(); j++){
			if (cursorX < board.size() - 1 && cursorY < board[i].size() &&
				cursorX > 0 && cursorY > 0 &&
				board[cursorX][cursorY] == BLANK && mask.Data[i][j]){
				blankNum++;
			}
			cursorY++;
		}
		cursorY = t.GetPos().second + mask.StartPoint.second;
		cursorX++;
	}

	return -blankNum;
}

int TetrisAI::EvaluateBlankSpace(Tetris& t)
{
	t.SetPreviewBlock();

	return 1;
}

int TetrisAI::EvaluateClearLine(Tetris& t)
{
	t.CheckCompleteLine();

	int clearLine = t.GetLine() - tetris->GetLine();
	
	if (clearLine > 2) {
		clearLine * 100;
	}

	return clearLine;
}



