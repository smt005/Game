
#pragma once

#include "Game.h"
#include <string>

namespace Engine { class Callback; }
class Map;

class MapExample final : public Engine::Game
{
public:
	~MapExample();
	std::string getName() override { return "MapExample"; }

	void init() override;
	void update() override;
	void draw() override;

	void initCallback();
	bool load();
	void save();

private:
	Engine::Callback* _callback = nullptr;
	Map* _mapGame = nullptr;
};