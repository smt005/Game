
#pragma once

class Json::Value;

class GameTest2 {
public:
	static void fun();

	static bool loadJson(Json::Value& jsonValue, const std::string nameFile);
	static void saveJson(const Json::Value& jsonValue, const std::string nameFile);
};