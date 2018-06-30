#include "Game.hpp"

int main(void) {
	srand(time(NULL));		//for any call to rand()
	Game::Game Valkyrie;

	Valkyrie.Init();
	Valkyrie.loop();

	return 0;
}