
#pragma once

#include "Game.h"
#include "Callback/Callback.h"
#include <string>

class Map;
class Greed;

class MapTemplate : public Engine::Game, public Engine::Callback
{
public:
	MapTemplate();
	virtual ~MapTemplate();
	virtual std::string getName() override { return "MapTemplate"; }
	virtual std::filesystem::path getreSourcesDir() override { return "..\\..\\Source\\Resources\\Files"; }

	virtual void init() override;
	virtual void update() override;
	virtual void draw() override;
	virtual void resize() override;
	virtual void initCallback();

	virtual bool load();
	virtual void save();

private:
	Map* _mapGame = nullptr;
	Greed* _greed = nullptr;
};