#pragma once
#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <map>
#include "PlayGame.h"
#include "GameManager.h"
using namespace std;
enum PadState {
	Nothing,
	Press,
	Release,
	Hold
};

class kbState {
	friend class InputManager;
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

class InputManager {
public:
	InputManager(GameManager& gm);

	void PlayGameInput();
	void PlayGameSetInput();

	void IntroInput();
	void IntroSetInput();

	void ROE_Input();
	void ROE_SetInput();

	void Input();
private:
	map<pair<int, PadState>, void (PlayGame::*)()> playGameMappingKey;
	map<int, kbState*> playGameCheckKeyState;
	map<pair<int, PadState>, void (GameManager::*)()> introMappingKey;
	map<int, kbState*> introCheckKeyState;
	map<pair<int, PadState>, void (GameManager::*)()> ROEMappingKey;
	map<int, kbState*> ROECheckKeyState;
	PlayGame *game;
	GameManager* gameManager;
};