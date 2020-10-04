#pragma once

#include <string>
#include <memory>

#include "Examples/MapExample.h"
#include "PhysX/MapPhysX.h"

namespace game
{
	std::string currentGame = "MapPhysX";

	Engine::GamePtr getGame(const std::string& name) {
		Engine::Game* game = nullptr;
		std::string selectGame;

		if (name.empty()) {
			selectGame = currentGame;
		}

		if (selectGame == "MapExample") {
			game = new MapExample();
		}
		else if (selectGame == "MapPhysX") {
			game = new MapPhysX();
		}
		else {
			game = new Engine::DefaultGame();
		}

		Engine::GamePtr gamePtr(game);
		return gamePtr;
	}

};	// game
