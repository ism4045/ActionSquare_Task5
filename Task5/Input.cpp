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

InputManager::InputManager(GameManager& gm)
{
	game = gm.PG;
	gameManager = &gm;
	PlayGameSetInput();
	IntroSetInput();
	ROE_SetInput();
}

void InputManager::PlayGameSetInput()
{
	playGameMappingKey.insert(make_pair(make_pair(VK_ESCAPE, Press), &PlayGame::Stop));
	playGameMappingKey.insert(make_pair(make_pair(VK_LEFT, Press), &PlayGame::MoveL));
	playGameMappingKey.insert(make_pair(make_pair(VK_RIGHT, Press), &PlayGame::MoveR));
	playGameMappingKey.insert(make_pair(make_pair(VK_DOWN, Hold), &PlayGame::ChangeSoftDrop));
	playGameMappingKey.insert(make_pair(make_pair(VK_DOWN, Release), &PlayGame::ReturnNormalSpeed));
	playGameMappingKey.insert(make_pair(make_pair(VK_UP, Press), &PlayGame::RotateBlock));
	playGameMappingKey.insert(make_pair(make_pair(VK_SPACE, Press), &PlayGame::DoHardDrop));
	playGameCheckKeyState.insert(make_pair(VK_ESCAPE, new kbState(VK_ESCAPE)));
	playGameCheckKeyState.insert(make_pair(VK_LEFT, new kbState(VK_LEFT)));
	playGameCheckKeyState.insert(make_pair(VK_RIGHT, new kbState(VK_RIGHT)));
	playGameCheckKeyState.insert(make_pair(VK_DOWN, new kbState(VK_DOWN)));
	playGameCheckKeyState.insert(make_pair(VK_UP, new kbState(VK_UP)));
	playGameCheckKeyState.insert(make_pair(VK_SPACE, new kbState(VK_SPACE)));
}

void InputManager::PlayGameInput()
{
	if (_kbhit()) {
		for (auto iter = playGameMappingKey.begin(); iter != playGameMappingKey.end(); iter++) {
			if (playGameCheckKeyState[iter->first.first]->GetKbState() == iter->first.second) {
				if (iter->second == &PlayGame::Stop)
					(game->*(iter->second))();
				else 
					if (!game->GetStop())
						(game->*(iter->second))();
			}
		}
	}
}

void InputManager::IntroSetInput()
{
	introMappingKey.insert(make_pair(make_pair(VK_RETURN, Press), &GameManager::EnterPlay));
	introCheckKeyState.insert(make_pair(VK_RETURN, new kbState(VK_RETURN)));
}

void InputManager::IntroInput()
{
	if (_kbhit()) {
		for (auto iter = introMappingKey.begin(); iter != introMappingKey.end(); iter++) {
			if (introCheckKeyState[iter->first.first]->GetKbState() == iter->first.second) {
				(gameManager->*(iter->second))();
			}
		}
	}
}

void InputManager::ROE_SetInput()
{
	ROEMappingKey.insert(make_pair(make_pair(VK_LEFT, Press), &GameManager::SelectROE_L));
	ROEMappingKey.insert(make_pair(make_pair(VK_RIGHT, Press), &GameManager::SelectROE_R));
	ROEMappingKey.insert(make_pair(make_pair(VK_RETURN, Press), &GameManager::DecisionEnd));
	ROECheckKeyState.insert(make_pair(VK_RETURN, new kbState(VK_RETURN)));
	ROECheckKeyState.insert(make_pair(VK_LEFT, new kbState(VK_LEFT)));
	ROECheckKeyState.insert(make_pair(VK_RIGHT, new kbState(VK_RIGHT)));
}

void InputManager::ROE_Input()
{
	if (_kbhit()) {
		for (auto iter = ROEMappingKey.begin(); iter != ROEMappingKey.end(); iter++) {
			if (ROECheckKeyState[iter->first.first]->GetKbState() == iter->first.second) {
				(gameManager->*(iter->second))();
			}
		}
	}
}

void InputManager::Input()
{
	switch (gameManager->GetGameState())
	{
	case GameState::Intro:
		IntroInput();
		break;
	case GameState::Play:
		PlayGameInput();
		break;
	case GameState::RestartOREnd:
		ROE_Input();
		break;
	default:
		break;
	}
}
