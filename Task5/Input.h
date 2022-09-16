#pragma once
#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <map>
#include "PlayGame.h"
using namespace std;
enum PadState {
	Nothing,
	Press,
	Release,
	Hold
};

class kbState {
	friend class Input;
	kbState(int VK);
	PadState GetKbState();
private:
	int virtualKey;
	bool checkBtn[2];
	bool checkBtnIndex;
};

struct MappingKey
{
	map<pair<int, PadState>, void (PlayGame::*)()> mappingKey;
	map<int, kbState*> checkKeyState;
};

class Input {
public:
	Input(PlayGame& g);
	void PlayGameInput();
	void PlayGameSetInput();

	void MenuInput();
	void MenuSetInput();

	void EndInput();
	void EndSetInput();
private:
	MappingKey playGameKey;
	MappingKey menuKey;
	MappingKey endKey;
	PlayGame *game;
};