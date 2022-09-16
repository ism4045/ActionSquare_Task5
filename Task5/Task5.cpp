#include "Screen.h"
#include "PlayGame.h"
#include "Input.h"
#include <mmsystem.h>
#include <mciapi.h>
#pragma comment(lib,"winmm.lib")
using namespace std;

int main() {
	PlayGame PG;
	Screen SCR(PG);
	Input I(PG);
	mciSendString(TEXT("open videoplayback.mp3 type mpegvideo alias PlaySound"), NULL, 0, NULL);
	MCIERROR error = mciSendString(TEXT("setaudio PlaySound volume to 50"), NULL, 0, NULL);
	mciSendString(TEXT("play PlaySound"), NULL, 0, NULL);
	
	while (1)
	{
		PG.Update();
		SCR.Render();
		I.PlayGameInput();
	}
}