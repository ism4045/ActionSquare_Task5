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

InputManager::InputManager(GameManager& gm)
{
	playGame = gm.playGame;
	gameManager = &gm;
	PlayGameSetInput();
	IntroSetInput();
	ROE_SetInput();
}

InputManager::~InputManager()
{
	for (auto iter = playGameMappingKey.begin(); iter != playGameMappingKey.end(); iter++) {
		delete iter->first;
	}
	for (auto iter = introMappingKey.begin(); iter != introMappingKey.end(); iter++) {
		delete iter->first;
	}
	for (auto iter = ROEMappingKey.begin(); iter != ROEMappingKey.end(); iter++) {
		delete iter->first;
	}
}

void InputManager::PlayGameSetInput()
{
	playGameMappingKey.insert(make_pair(new kbState(VK_ESCAPE,Press), &PlayGame::Stop));
	playGameMappingKey.insert(make_pair(new kbState(VK_LEFT, Press), &PlayGame::MoveL));
	playGameMappingKey.insert(make_pair(new kbState(VK_RIGHT, Press), &PlayGame::MoveR));
	playGameMappingKey.insert(make_pair(new kbState(VK_DOWN, Hold), &PlayGame::ChangeSoftDrop));
	playGameMappingKey.insert(make_pair(new kbState(VK_DOWN, Release), &PlayGame::ReturnNormalSpeed));
	playGameMappingKey.insert(make_pair(new kbState(VK_UP, Press), &PlayGame::RotateBlock));
	playGameMappingKey.insert(make_pair(new kbState(VK_SPACE, Press), &PlayGame::DoHardDrop));
}

void InputManager::PlayGameInput()
{
	for (auto iter = playGameMappingKey.begin(); iter != playGameMappingKey.end(); iter++) {
		iter->first->UpdatePadState();
		if (iter->first->CanDoFunction()) {
			(playGame->*(iter->second))();
		}
	}
}

void InputManager::IntroSetInput()
{
	introMappingKey.insert(make_pair(new kbState(VK_RETURN, Press), &GameManager::EnterPlay));
}

void InputManager::IntroInput()
{
	for (auto iter = introMappingKey.begin(); iter != introMappingKey.end(); iter++) {
		iter->first->UpdatePadState();
		if (iter->first->CanDoFunction()) {
			(gameManager->*(iter->second))();
		}
	}
}

void InputManager::ROE_SetInput()
{
	ROEMappingKey.insert(make_pair(new kbState(VK_LEFT, Press), &GameManager::SelectROE_L));
	ROEMappingKey.insert(make_pair(new kbState(VK_RIGHT, Press), &GameManager::SelectROE_R));
	ROEMappingKey.insert(make_pair(new kbState(VK_RETURN, Press), &GameManager::DecisionEnd));
}

void InputManager::ROE_Input()
{
	for (auto iter = ROEMappingKey.begin(); iter != ROEMappingKey.end(); iter++) {
		iter->first->UpdatePadState();
		if (iter->first->CanDoFunction()) {
			(gameManager->*(iter->second))();
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
	case GameState::RestartOrEnd:
		ROE_Input();
		break;
	default:
		break;
	}
}
