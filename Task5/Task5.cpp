#include "Screen.h"
#include "GameManager.h"
using namespace std;

int main() {
	GameManager gameManager;
	Screen screen(gameManager);

	while (gameManager.GetGameState() != GameState::End)
	{
		gameManager.GameUpdate();
		screen.Render();
		gameManager.InputManage();
	}
}