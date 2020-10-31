#pragma once

#include "Game.h"
#include <string>

namespace Engine { class Callback; }
class Greed;

class BrainGame final : public Engine::Game
{
public:
	~BrainGame();
	std::string getName() override { return "BrainGame"; }
	std::filesystem::path getreSourcesDir() override { return "..\\..\\Source\\Resources\\Files"; }

	void init() override;
	void update() override;
	void draw() override;
	void resize() override;

	void initCallback();
	bool load();
	void save();

private:
	Engine::Callback* _callback = nullptr;
	Greed* _greed = nullptr;
	bool _pause = true;
};