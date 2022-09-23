#pragma once
#ifndef TETRISAI_H_

#define TETRISAI_H_

#include <functional>
#include <vector>
#include <queue>
#include <ctime>
#include "Tetris.h"

typedef function<void(Tetris*)> FUNC_VOID;


struct Target
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
	void UpdatePlaying();

	void Evaluate();
	void Choose();
	void Execute();

	void FillAIBehaviors();
	Target GetTarget();
	//평가 항목 - 가중치 점검
	Block GetTargetPosANDRot();

	//실행 가능 여부 - 선택 시
	bool CanThisWay(Block block);

private:
	Tetris* tetris;

	vector<Target> targets;

	queue<FUNC_VOID> aiBehaviors;

	clock_t aiStart;
	clock_t aiCurrent;
};

#endif // !TETRISAI_H_
