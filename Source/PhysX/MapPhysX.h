
#pragma once

#include "Game.h"
#include <string>

namespace Engine { class Callback; }
class Map;
class Greed;

class MapPhysX final : public Engine::Game
{
public:
	~MapPhysX();
	std::string getName() override { return "MapPhysX"; }

	void init() override;
	void update() override;
	void draw() override;
	void resize() override;

	void initCallback();
	void initPhysic();
	bool load();
	void save();

private:
	Engine::Callback* _callback = nullptr;
	Map* _mapGame = nullptr;
	Greed* _greed = nullptr;
};