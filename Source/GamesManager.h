#pragma once

#include <string>
#include <memory>

#include "Examples/MapExample.h"
#include "PhysX/MapPhysX.h"
#include "BrainGame/BrainGame.h"
#include "GUI/GuiGame.h"

namespace game
{
	//std::string currentGame = "MapExample";
	//std::string currentGame = "MapPhysX";
	//std::string currentGame = "BrainGame";
	std::string currentGame = "GuiGame";

	Engine::GamePtr getGame(const std::string& name) {
		Engine::Game* game = nullptr;
		std::string selectGame;

		if (name.empty()) {
			selectGame = currentGame;
		}

		if (selectGame == "BrainGame") {
			game = new BrainGame();
		}
		else if (selectGame == "MapExample") {
			game = new MapExample();
		}
		else if (selectGame == "MapPhysX") {
			game = new MapPhysX();
		}
		else if (selectGame == "GuiGame") {
			game = new GuiGame();
		}
		else {
			game = new Engine::DefaultGame();
		}

		Engine::GamePtr gamePtr(game);
		return gamePtr;
	}

};	// game
