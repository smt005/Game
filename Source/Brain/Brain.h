
#pragma once

#include "Game.h"
#include <string>

namespace Engine { class Callback; }
class Map;
class Greed;
class Object;
//typedef std::shared_ptr<Object> ObjectPtr;

class Brain final : public Engine::Game
{
public:
	~Brain();
	std::string getName() override { return "Brain"; }

	void init() override;
	void update() override;
	void draw() override;
	void resize() override;

	void initCallback();
	bool load();
	void save();

private:
	void updateCar(const char key);

private:
	Engine::Callback* _callback = nullptr;
	Map* _mapGame = nullptr;
	Greed* _greed = nullptr;
	Object* _car;
	bool _typeCamera = true;
};