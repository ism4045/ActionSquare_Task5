#include "Input.h"

kbState::kbState(int VK, PadState PS)
{
	virtualKey = VK;
	conditionPadState = PS;
	currentPadState = Nothing;
	checkBtnIndex = false;
	for (int i = 0; i < 2; i++) checkBtn[i] = false;
}

void kbState::UpdatePadState()
{
	if (!checkBtn[0]) {
		if (GetAsyncKeyState(virtualKey) & 0x8000) {
			checkBtn[checkBtnIndex] = true;
			checkBtnIndex = true;
			currentPadState = Press;
		}
		else {
			for (int i = 0; i < 2; i++) checkBtn[i] = false;
			currentPadState = Nothing;
		}
	}
	else {
		if (GetAsyncKeyState(virtualKey) & 0x8000) {
			checkBtn[checkBtnIndex] = true;
			currentPadState = Hold;
		}
		else {
			for (int i = 0; i < 2; i++) checkBtn[i] = false;
			checkBtnIndex = false;
			currentPadState = Release;
		}
	}
}

bool kbState::operator<(const kbState& kbs) const
{
	if (conditionPadState == kbs.conditionPadState && virtualKey == kbs.virtualKey)
		return false;
	return true;
}

void InputManager::ReceiveHandle(int stage)
{
	for (auto& func : functions[stage]) {
		func.first->UpdatePadState();
		if (func.first->CanDoFunction()) 
			func.second();
	}
}

void InputManager::BindFunction(int stage, int vk, PadState ps, function<void()> const& func)
{
	functions[stage].push_back({ make_unique<kbState>(kbState(vk, ps)),func });
}