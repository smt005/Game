
#include <iostream>
#include <functional>
#include "json/json.h"
#include "glm/vec3.hpp"

#include "Game2.h"
#include "Engine.h"
#include "FileManager.h"
#include "Common/Help.h"

void Game2::fun() {
	std::cout << "Game::fun()" << std::endl;

	{
		std::function<std::string(glm::vec3)> printVec = [](const glm::vec3& vector) {
			return " [" + std::to_string(vector.x) + ", " + std::to_string(vector.y) + ", " + std::to_string(vector.z) + "] ";
		};

		glm::vec3 start1(-0.5, -1.0, 0.0);
		glm::vec3 end1(1.5, 1.0, 0.0);
		glm::vec3 start2(2.0, 0.5, 0.0);
		glm::vec3 end2(-0.5, 2.0, 0.0);
		glm::vec3 out_intersection(0.0, 0.0, 0.0);
		help::intersection(start1, end1, start2, end2, out_intersection);

		help::log("intersection: " + printVec(start1) + printVec(end1) + " x " + printVec(start2) + printVec(end2) + " => " + printVec(out_intersection));
	}

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
