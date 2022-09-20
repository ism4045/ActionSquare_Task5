#include "GameManager.h"

GameManager::GameManager()
{	
	gameState = GameState::Intro;
	endMenu = true;

	mciSendString(TEXT("open IntroBGM.mp3 type mpegvideo alias intro"), NULL, 0, NULL);
	mciSendString(TEXT("open PlayBGM.mp3 type mpegvideo alias play"), NULL, 0, NULL);
	mciSendString(TEXT("open GameOverBGM.mp3 type mpegvideo alias gameover"), NULL, 0, NULL);

	mciSendString(TEXT("setaudio intro volume to 70"), NULL, 0, NULL);
	mciSendString(TEXT("setaudio play volume to 50"), NULL, 0, NULL);
	mciSendString(TEXT("setaudio gameover volume to 70"), NULL, 0, NULL);

	mciSendString(TEXT("play intro notify repeat"), NULL, 0, NULL);
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
		this->Initialize();
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
		this->Update();
		if (this->GetPlayGameState() == PlayGameState::GameOver) {
			mciSendString(TEXT("stop play"), NULL, 0, NULL);
			mciSendString(TEXT("seek gameover notify to start"), NULL, 0, NULL);
			mciSendString(TEXT("play gameover notify repeat"), NULL, 0, NULL);
			gameState = RestartOrEnd;
		}
	}
}
