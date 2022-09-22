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
	delete tetris;
	delete inputManager;
}

void GameManager::Init()
{
	tetris = new Tetris();
	inputManager = new InputManager();
	screen = new Screen(*tetris);

	inputManager->BindFunction(GameState::Intro, VK_RETURN, Press, bind(&GameManager::EnterPlay, this));

	inputManager->BindFunction(GameState::Play, VK_ESCAPE, Press, bind(&Tetris::Stop, tetris));
	inputManager->BindFunction(GameState::Play, VK_LEFT, Press, bind(&Tetris::MoveL, tetris));
	inputManager->BindFunction(GameState::Play, VK_RIGHT, Press, bind(&Tetris::MoveR, tetris));
	inputManager->BindFunction(GameState::Play, VK_DOWN, Hold, bind(&Tetris::ChangeSoftDrop, tetris));
	inputManager->BindFunction(GameState::Play, VK_DOWN, Release, bind(&Tetris::ReturnNormalSpeed, tetris));
	inputManager->BindFunction(GameState::Play, VK_UP, Press, bind(&Tetris::RotateBlock, tetris));
	inputManager->BindFunction(GameState::Play, VK_SPACE, Press, bind(&Tetris::DoHardDrop, tetris));

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
		tetris->Initialize();
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
		tetris->Update();
		if (tetris->GetPlayGameState() == PlayGameState::GameOver) {
			mciSendString(TEXT("stop play"), NULL, 0, NULL);
			mciSendString(TEXT("seek gameover notify to start"), NULL, 0, NULL);
			mciSendString(TEXT("play gameover notify repeat"), NULL, 0, NULL);
			gameState = RestartOrEnd;
		}
	}
}

void GameManager::Input() {
	switch (gameState)
	{
	case Intro:
		inputManager->ReceiveHandle(GameState::Intro);
		break;
	case Play:
		inputManager->ReceiveHandle(GameState::Play);
		break;
	case RestartOrEnd:
		inputManager->ReceiveHandle(GameState::RestartOrEnd);
		break;
	default:
		break;
	}
}

void GameManager::Render()
{
	screen->RecieveHandle(gameState, endMenu);
}


