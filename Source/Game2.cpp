
#include <iostream>
#include "json/json.h"

#include "Game2.h"
#include "Engine.h"
#include "FileManager.h"
#include "Common/Help.h"

void Game2::fun() {
	std::cout << "Game::fun()" << std::endl;
	Engine::exeForGame();
}

bool Game2::loadJson(Json::Value& jsonValue, const std::string nameFile) {
	if (help::loadJson(nameFile, jsonValue)) {
		help::log("Successfully, jsoin "+ nameFile +", is loaded");
		return true;
	}
	else {
		help::log("Fail, jsoin " + nameFile + ", not loaded");
		return false;
	}
}

void Game2::saveJson(const Json::Value& jsonValue, const std::string nameFile) {
	help::saveJson(nameFile, jsonValue);
}
