#pragma once
#ifndef TETRISAI_H_

#define TETRISAI_H_

#include <functional>
#include <vector>
#include <queue>
#include <ctime>
#include "Tetris.h"
#include "BlockContactMask.h"

typedef function<void(Tetris*)> FUNC_VOID;

struct Destination
{
	pair<int, int> pos;
	int rotate;
	int weight;
};

using namespace std;

class TetrisAI
{
public:
	TetrisAI(Tetris* tetris);
	~TetrisAI();


	void Init();
	void Update();
	void PlayAI();

	vector<Destination> Evaluate(Tetris t);
	Destination Choose(vector<Destination>);


	void FillAIBehaviors();
	Destination GetDestination();


	//평가 항목 - 가중치 점검
	Destination EvaluateWeight(Tetris t);
	int EvaluateHightestHeight(Tetris& t);
	int EvaluateContactTile(Tetris& t);
	int EvaluateClearLine(Tetris& t);
	int EvaluateBlankSpace(Tetris& t);

private:
	Tetris* tetris;

	queue<FUNC_VOID> aiBehaviors;

	clock_t aiStart;
	clock_t aiCurrent;
};

#endif // !TETRISAI_H_
