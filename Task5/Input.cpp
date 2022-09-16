#include "Input.h"

kbState::kbState(int VK)
{
	virtualKey = VK;
	checkBtnIndex = false;
	for (int i = 0; i < 2; i++) checkBtn[i] = false;
}

PadState kbState::GetKbState()
{
	if (!checkBtn[0]) {
		if (GetAsyncKeyState(virtualKey) & 0x8000) {
			checkBtn[checkBtnIndex] = true;
			checkBtnIndex = true;
			return PadState::Press;
		}
		else {
			for (int i = 0; i < 2; i++) checkBtn[i] = false;
			return PadState::Nothing;
		}
	}
	else {
		if (GetAsyncKeyState(virtualKey) & 0x8000) {
			checkBtn[checkBtnIndex] = true;
			return PadState::Hold;
		}
		else {
			for (int i = 0; i < 2; i++) checkBtn[i] = false;
			checkBtnIndex = false;
			return PadState::Release;
		}
	}
}

Input::Input(PlayGame& P)
{
	game = &P;
	PlayGameSetInput();
}

void Input::PlayGameSetInput()
{
	playGameKey.mappingKey.insert(make_pair(make_pair(VK_ESCAPE, Press), &PlayGame::Stop));
	playGameKey.mappingKey.insert(make_pair(make_pair(VK_LEFT, Press), &PlayGame::MoveL));
	playGameKey.mappingKey.insert(make_pair(make_pair(VK_RIGHT, Press), &PlayGame::MoveR));
	playGameKey.mappingKey.insert(make_pair(make_pair(VK_DOWN, Hold), &PlayGame::ChangeSoftDrop));
	playGameKey.mappingKey.insert(make_pair(make_pair(VK_DOWN, Release), &PlayGame::ReturnNormalSpeed));
	playGameKey.mappingKey.insert(make_pair(make_pair(VK_UP, Press), &PlayGame::RotateBlock));
	playGameKey.mappingKey.insert(make_pair(make_pair(VK_SPACE, Press), &PlayGame::DoHardDrop));
	playGameKey.checkKeyState.insert(make_pair(VK_ESCAPE, new kbState(VK_ESCAPE)));
	playGameKey.checkKeyState.insert(make_pair(VK_LEFT, new kbState(VK_LEFT)));
	playGameKey.checkKeyState.insert(make_pair(VK_RIGHT, new kbState(VK_RIGHT)));
	playGameKey.checkKeyState.insert(make_pair(VK_DOWN, new kbState(VK_DOWN)));
	playGameKey.checkKeyState.insert(make_pair(VK_UP, new kbState(VK_UP)));
	playGameKey.checkKeyState.insert(make_pair(VK_SPACE, new kbState(VK_SPACE)));
}

void Input::PlayGameInput()
{
	if (_kbhit()) {
		for (auto iter = playGameKey.mappingKey.begin(); iter != playGameKey.mappingKey.end(); iter++) {
			if (playGameKey.checkKeyState[iter->first.first]->GetKbState() == iter->first.second) {
				if (iter->second == &PlayGame::Stop)
					(game->*(iter->second))();
				else 
					if (!game->GetStop())
						(game->*(iter->second))();
			}
		}
	}
}