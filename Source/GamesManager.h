#pragma once

#include <string>
#include <memory>

#include "Examples/MapExample.h"

namespace game
{

Engine::GamePtr getGame(const std::string& name) {
	Engine::Game* game = nullptr;
	
	if (name == "MapExample") {
		game = new MapExample();
	}
	else {
		game = new Engine::DefaultGame();
	}

	Engine::GamePtr gamePtr(game);
	return gamePtr;
}

};	// game