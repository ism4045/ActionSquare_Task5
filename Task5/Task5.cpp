#include "Screen.h"
#include "PlayGame.h"
#include "Input.h"
#include <mmsystem.h>
#include <mciapi.h>
#include "GameManager.h"
#pragma comment(lib,"winmm.lib")
using namespace std;

int main() {
	PlayGame PG;
	GameManager GM(PG);
	Screen SCR(GM);
	InputManager I(GM);

	mciSendString(TEXT("open videoplayback.mp3 type mpegvideo alias PlaySound"), NULL, 0, NULL);
	MCIERROR error = mciSendString(TEXT("setaudio PlaySound volume to 50"), NULL, 0, NULL);
	mciSendString(TEXT("play PlaySound"), NULL, 0, NULL);
	
	while (GM.GetGameState() != GameState::End)
	{
		GM.Update();
		SCR.Render();
		I.Input();
	}
}