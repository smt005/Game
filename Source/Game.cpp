
#include <iostream>

#include "Game.h"
#include "Game2.h"
#include "Engine.h"

void Game::exe() {
	std::cout << "Game::exe()" << std::endl;
	Game2::fun();
	Engine::exeForGame();
}
