#pragma once
#ifndef _INPUT_H

#define _INPUT_H

#include <Windows.h>
#include <map>
#include <functional>
#include <vector>
#include <memory>
using namespace std;

enum PadState {
	Nothing,
	Press,
	Release,
	Hold
};

class kbState {
	friend class InputManager;
public:
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
	void ReceiveHandle(int stage);
	void BindFunction(int stage, int vk, PadState ps, function<void()> const& func);
private:
	map<int, vector<pair<unique_ptr<kbState>, function<void()>>>> functions;
};
#endif // !_INPUT_H