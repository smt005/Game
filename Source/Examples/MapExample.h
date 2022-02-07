
#pragma once

#include "Game.h"
#include <string>

namespace Engine { class Callback; }
class Map;
class Greed;

class MapExample final : public Engine::Game
{
public:
	~MapExample();
	std::filesystem::path getSourcesDir() override { return "..\\..\\Source\\Resources\\Files"; }

	void init() override;
	void update() override;
	void draw() override;
	void resize() override;

	void initCallback();
	bool load();
	void save();

private:
	Engine::Callback* _callback = nullptr;
	Map* _mapGame = nullptr;
	Greed* _greed = nullptr;
};