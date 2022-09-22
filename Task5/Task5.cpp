#include "Screen.h"
#include "GameManager.h"
using namespace std;

int main() {
	GameManager gameManager;

	while (gameManager.GetGameState() != GameState::End)
	{
		gameManager.GameUpdate();
		gameManager.Render();
		gameManager.Input();
	}
}