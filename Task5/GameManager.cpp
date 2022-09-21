#include "GameManager.h"

GameManager::GameManager()
{
	gameState = GameState::Intro;
	endMenu = true;

	mciSendString(TEXT("open Sound\\IntroBGM.mp3 type mpegvideo alias intro"), NULL, 0, NULL);
	mciSendString(TEXT("open Sound\\PlayBGM.mp3 type mpegvideo alias play"), NULL, 0, NULL);
	mciSendString(TEXT("open Sound\\GameOverBGM.mp3 type mpegvideo alias gameover"), NULL, 0, NULL);

	mciSendString(TEXT("setaudio intro volume to 70"), NULL, 0, NULL);
	mciSendString(TEXT("setaudio play volume to 50"), NULL, 0, NULL);
	mciSendString(TEXT("setaudio gameover volume to 70"), NULL, 0, NULL);

	mciSendString(TEXT("play intro notify repeat"), NULL, 0, NULL);
	Init();
}

GameManager::~GameManager()
{
	delete playGame;
	delete inputManager;
}

void GameManager::Init()
{
	playGame = new Tetris();
	inputManager = new Input();

	inputManager->BindFunction(GameState::Intro, VK_RETURN, Press, bind(&GameManager::EnterPlay, this));

	inputManager->BindFunction(GameState::Play, VK_ESCAPE, Press, bind(&Tetris::Stop, playGame));
	inputManager->BindFunction(GameState::Play, VK_LEFT, Press, bind(&Tetris::MoveL, playGame));
	inputManager->BindFunction(GameState::Play, VK_RIGHT, Press, bind(&Tetris::MoveR, playGame));
	inputManager->BindFunction(GameState::Play, VK_DOWN, Hold, bind(&Tetris::ChangeSoftDrop, playGame));
	inputManager->BindFunction(GameState::Play, VK_DOWN, Release, bind(&Tetris::ReturnNormalSpeed, playGame));
	inputManager->BindFunction(GameState::Play, VK_UP, Press, bind(&Tetris::RotateBlock, playGame));
	inputManager->BindFunction(GameState::Play, VK_SPACE, Press, bind(&Tetris::DoHardDrop, playGame));

	inputManager->BindFunction(GameState::RestartOrEnd, VK_LEFT, Press, bind(&GameManager::SelectROE_L, this));
	inputManager->BindFunction(GameState::RestartOrEnd, VK_RIGHT, Press, bind(&GameManager::SelectROE_R, this));
	inputManager->BindFunction(GameState::RestartOrEnd, VK_RETURN, Press, bind(&GameManager::DecisionEnd, this));
}

void GameManager::EnterPlay()
{
	gameState = GameState::Play;
	mciSendString(TEXT("stop intro"), NULL, 0, NULL);
	mciSendString(TEXT("play play notify repeat"), NULL, 0, NULL);
}

void GameManager::DecisionEnd()
{
	if (endMenu) {
		gameState = GameState::Play;
		playGame->Initialize();
		mciSendString(TEXT("stop gameover"), NULL, 0, NULL);
		mciSendString(TEXT("seek play notify to start"), NULL, 0, NULL);
		mciSendString(TEXT("play play notify repeat"), NULL, 0, NULL);
	}
	else
		gameState = GameState::End;
}

void GameManager::SelectROE_L()
{
	endMenu = true;
}

void GameManager::SelectROE_R()
{
	endMenu = false;
}

void GameManager::GameUpdate()
{
	if (gameState == Play) {
		playGame->Update();
		if (playGame->GetPlayGameState() == PlayGameState::GameOver) {
			mciSendString(TEXT("stop play"), NULL, 0, NULL);
			mciSendString(TEXT("seek gameover notify to start"), NULL, 0, NULL);
			mciSendString(TEXT("play gameover notify repeat"), NULL, 0, NULL);
			gameState = RestartOrEnd;
		}
	}
}

void GameManager::InputManage() {
	switch (gameState)
	{
	case Intro:
		inputManager->CheckInput(GameState::Intro);
		break;
	case Play:
		inputManager->CheckInput(GameState::Play);
		break;
	case RestartOrEnd:
		inputManager->CheckInput(GameState::RestartOrEnd);
		break;
	default:
		break;
	}
}


