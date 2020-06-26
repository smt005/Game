
#include <iostream>
#include "json/json.h"
#include "Common/Help.h"

#include "Game.h"
#include "Game2.h"
#include "Engine.h"
#include "FileManager.h"

void jsonFun(const std::string nameFile) {
	Json::Value jsonValue;

	Game2::loadJson(jsonValue, nameFile);

	if (!jsonValue.empty()) {
		help::log("json [" + nameFile + "]:");
		help::log(help::stringFroJson(jsonValue));

		if (jsonValue.isObject()) {
			int width = jsonValue["window"]["width"].asInt();
			int height = jsonValue["window"]["height"].asInt();

			width += 10;
			height += 10;

			jsonValue["window"]["width"] = width;
			jsonValue["window"]["height"] = height;

			help::log("json [" + nameFile + "]: AFTER CHANGE");
			help::log(help::stringFroJson(jsonValue));

			Game2::saveJson(jsonValue, nameFile);
		}
	}
}

void Game::exe() {
	std::cout << "Game::exe()" << std::endl;
	Game2::fun();

	Engine::FileManager::setResourcesDir("..\\..\\Source\\Resources");
	jsonFun("Base/Setting_FAIL.json");
	jsonFun("Base/Setting.json");

	Engine::exeForGame();
}
