#pragma once
#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <map>
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
	kbState(int VK, PadState PS);
	bool CanDoFunction() { return conditionPadState == currentPadState; }
	void UpdatePadState();
	bool operator<(const kbState& kbs) const;

private:
	int virtualKey;
	PadState conditionPadState;
	PadState currentPadState;
	bool checkBtn[2];
	bool checkBtnIndex;
};

class InputManager {
public:
	InputManager(GameManager& gm);
	~InputManager();

	void PlayGameSetInput();
	void IntroSetInput();
	void ROE_SetInput();

	template<class T>
	void CheckInput(map<kbState*, void(T::*)()> mappingKey);
	void Input();
private:
	map<kbState*, void (GameManager::*)()> introMappingKey;
	map<kbState*, void (PlayGame::*)()> playGameMappingKey;
	map<kbState*, void (GameManager::*)()> ROEMappingKey;

	PlayGame *playGame;
	GameManager* gameManager;
};
