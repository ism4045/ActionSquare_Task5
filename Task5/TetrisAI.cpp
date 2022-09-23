#include "TetrisAI.h"

#define AI_PLAY_INTERVAL 200

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
	if ((double)aiCurrent - (double)aiStart >= AI_PLAY_INTERVAL)
	{
		aiStart = aiCurrent;
		UpdatePlaying();
	}
}

void TetrisAI::UpdatePlaying()
{
	if (aiBehaviors.empty())
	{
		FillAIBehaviors();
	}
	else
	{
		FUNC_VOID behavior = aiBehaviors.front();
		behavior(tetris);
		aiBehaviors.pop();
	}
}

void TetrisAI::FillAIBehaviors()
{
	Target target = GetTarget();
	
	for (int i = 0; i < target.rotate; i++)
	{
		aiBehaviors.push(&Tetris::RotateBlock);
	}

	pair<int, int> defaultPos = DEFAULTPOS;
	int gap = abs(target.pos.second - defaultPos.second);
	for (int i = 0; i < gap; i++)
	{
		if(target.pos.second > defaultPos.second)
			aiBehaviors.push(&Tetris::MoveR);
		else
			aiBehaviors.push(&Tetris::MoveL);
	}

	aiBehaviors.push(&Tetris::DoHardDrop);
}

Target TetrisAI::GetTarget()
{
	pair<int, int> pa = { 1,1 };
	int rotate = 2;
	return { pa, rotate };
}

void TetrisAI::Evaluate()
{
	Block block = tetris->GetCurrentBlock();
	vector<vector<int>> board = tetris->GetBoard();

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < board.size(); j++)
		{
			block.
		}
		block.rotateBlock();
	}
}

void TetrisAI::Choose()
{

}

void TetrisAI::Execute()
{

}

