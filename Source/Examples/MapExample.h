
#pragma once

#include "Game.h"
#include <string>
#include "Callback/Event.h"
#include "Screen.h"

namespace Engine { class Callback; }
class Map;
class Rect;
typedef std::shared_ptr<Rect> RectPtr;
class Rect3;
typedef std::shared_ptr<Rect3> Rect3Ptr;

namespace UI {
	class Element;
	typedef std::shared_ptr<Element> ElementPtr;
};


class MapExample final : public Engine::Game
{
public:
	~MapExample();
	std::string getName() override { return "MapExample"; }

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

	std::vector<RectPtr> _rects;
	std::vector<Rect3Ptr> _rect3s;

	UI::ElementPtr _elementPtr;
};
