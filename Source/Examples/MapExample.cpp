
#include <iostream>
#include <fstream>

#include "json/json.h"
#include <glm/glm.hpp>
#include "glm/mat4x4.hpp"

#include "MapExample.h"
#include "Screen.h"
#include "Core.h"
#include "FileManager.h"
#include "Draw/Draw.h"
#include "Draw/DrawLine.h"
#include "Draw/Camera.h"
#include "Draw/Shader.h"
#include "Object/Mesh.h"
#include "Common/Help.h"
#include "Callback/Callback.h"
#include "Object/Object.h"
#include "Object/Map.h"
#include "Object/Line.h"
#include "Object/Triangle.h"
#include "Object/Color.h"
#include "Object/Identify.h"
//#include "UI/UIElement.h"
#include "UI/UISample.h"
#include "UI/UIScreen.h"

const std::string saveFileName("Saves/MapExampleSave.json");
const std::string mapFileName("Examples/MapExample");

Greed greed(100.0f, 10.0f);

MapExample::~MapExample()
{
	if (!_callback) {
		delete _callback;
		_callback = nullptr;
	}

	if (_mapGame) {
		delete _mapGame;
		_mapGame = nullptr;
	}
}

void MapExample::init()
{
	/*Engine::Resize* _resizeEvent = new Engine::Resize([this]() {
		Camera::getCurrent().setPerspective(45.0f, Engine::Screen::aspect(), 0.1f, 1000.0f);
		});*/

	Engine::FileManager::setResourcesDir("..\\..\\Source\\Resources\\Files");
	if (!load())
	{
		Camera::getCurrent().setFromEye(true);
		Camera::getCurrent().setPos(glm::vec3(0.0f, 0.0f, 0.0f));
		Camera::getCurrent().setDist(1.0f);
	}

	Draw::setClearColor(0.9f, 0.6f, 0.3f, 1.0f);

	if (!_mapGame) {
		_mapGame = new Map(mapFileName);
	}

	/*{
		_elementPtr = UI::Element::make(glm::vec2(1.0f, 1.0f), "Test/strip_1.png", "Test");
		_elementPtr->_triangles[0].setPos({ 0.125f, 0.0f, 1.0 });
		//_elementPtr->_triangles[0].setRotate(1.57f, { 1.0f, 0.0f, 0.0f });
		_elementPtr->_triangles[0].setRotate(0.785f, { 1.0f, 0.0f, 0.0f });
		_elementPtr->add(Engine::CallbackType::RELEASE_TAP, [this](const Engine::CallbackEventPtr& callbackEventPtr) {
			if (_elementPtr->hit()) {
				help::log("Test ");
			}
		});
	}*/

	{
		UI::Screen::init();
	}

	/*{
		{
			Rect* r = new Rect();
			RectPtr rect(r);
			rect->_triangle.setPos({ 2.0f, 0.0f, 1.0 });
			_rects.push_back(rect);
		}
		{
			Rect* r = new Rect(2);
			RectPtr rect(r);
			rect->_triangle.setPos({ 1.0f, 1.1f, 1.0 });
			_rects.push_back(rect);
		}
		{
			Rect* r = new Rect(3);
			RectPtr rect(r);
			rect->_triangle.setPos({ 0.0f, 2.2f, 1.0 });
			_rects.push_back(rect);
		}*/

		/*{
			Rect3* r = new Rect3();
			Rect3Ptr rect(r);
			rect->_triangles[0].setPos({ 0.0f, -1.0f, 1.0 });
			rect->_triangles[1].setPos({ 0.0f, 0.0f, 1.0 });
			rect->_triangles[2].setPos({ 0.0f, 1.0f, 1.0 });
			_rect3s.push_back(rect);
		}
		{
			Rect3Callback* r = new Rect3Callback();
			r->add(Engine::CallbackType::RELEASE_TAP, [r](const Engine::CallbackEventPtr& callbackEventPtr) {
				if (r->hint()) {
					//Engine::Core::close();

					UI::UIDataPtr dataPtr;
					if (UI::UIData::load(dataPtr, "Test")) {
						UI::UIData::save(dataPtr, "Temp/Test_save");
					}
				}
			});
			Rect3Ptr rect(r);
			rect->_triangles[0].setPos({ 5.0f, -1.0f, 1.0 });
			rect->_triangles[1].setPos({ 5.0f, 0.0f, 1.0 });
			rect->_triangles[2].setPos({ 5.0f, 1.0f, 1.0 });
			_rect3s.push_back(rect);
		}
	}*/

	greed.setPos({0.0f, 0.0f, 0.1f});
	
	initCallback();
}

void MapExample::initCallback()
{
	{
		std::function<void()> fun = []() {};
		const type_info& typeInfo = fun.target_type();
		std::string nameFun = typeInfo.name();
		help::log(nameFun);

		//decltype(fun.target()) t = fun.target();
	}


	if (!_callback) {
		_callback = new Engine::Callback(Engine::CallbackType::PINCH_TAP, [](const Engine::CallbackEventPtr& callbackEventPtr) {
			if (Engine::Callback::pressTap(Engine::VirtualTap::RIGHT)) {
				Camera::getCurrent().rotate(Engine::Callback::deltaMousePos());
			}

			if (Engine::Callback::pressTap(Engine::VirtualTap::MIDDLE)) {
				Camera::getCurrent().move(Engine::Callback::deltaMousePos() * 1000.0f * Engine::Core::deltaTime());
			}
		});

		_callback->add(Engine::CallbackType::RELEASE_KEY, [this](const Engine::CallbackEventPtr& callbackEventPtr) {
			Engine::KeyCallbackEvent* releaseKeyEvent = (Engine::KeyCallbackEvent*)callbackEventPtr->get();
			Engine::VirtualKey key = releaseKeyEvent->getId();

			if (key == Engine::VirtualKey::ESCAPE) {
				Engine::Core::close();
			}

			if (key == Engine::VirtualKey::S && Engine::Callback::pressKey(Engine::VirtualKey::CONTROL)) {
				save();
			}

			if (key == Engine::VirtualKey::L && Engine::Callback::pressKey(Engine::VirtualKey::CONTROL)) {
				load();
			}
		});

		_callback = new Engine::Callback(Engine::CallbackType::PINCH_KEY, [](const Engine::CallbackEventPtr& callbackEventPtr) {
			if (Engine::Callback::pressKey(Engine::VirtualKey::CONTROL)) {
				return;
			}

			float speedCamera = 5.0f * Engine::Core::deltaTime();
			if (Engine::Callback::pressKey(Engine::VirtualKey::SHIFT)) {
				speedCamera = 30.0f * Engine::Core::deltaTime();
			}

			if (Engine::Callback::pressKey(Engine::VirtualKey::S)) {
				Camera::getCurrent().move(CAMERA_FORVARD, speedCamera);
			}

			if (Engine::Callback::pressKey(Engine::VirtualKey::W)) {
				Camera::getCurrent().move(CAMERA_BACK, speedCamera);
			}

			if (Engine::Callback::pressKey(Engine::VirtualKey::D)) {
				Camera::getCurrent().move(CAMERA_RIGHT, speedCamera);
			}

			if (Engine::Callback::pressKey(Engine::VirtualKey::A)) {
				Camera::getCurrent().move(CAMERA_LEFT, speedCamera);
			}

			if (Engine::Callback::pressKey(Engine::VirtualKey::R)) {
				Camera::getCurrent().move(CAMERA_TOP, speedCamera);
			}

			if (Engine::Callback::pressKey(Engine::VirtualKey::F)) {
				Camera::getCurrent().move(CAMERA_DOWN, speedCamera);
			}
		});
	}
}

bool MapExample::load()
{
	Json::Value saveData;
	if (!help::loadJson(saveFileName, saveData) || saveData.empty()) {
		return false;
	}

	if (!saveData["camera"].empty()) {
		Json::Value& cameraData = saveData["camera"];
		Camera::getCurrent().setJsonData(cameraData);
	}

#if _DEBUG
	Engine::Core::log("LOAD: " + saveFileName + "\n" + help::stringFroJson(saveData));
#endif // _DEBUG

	return true;
}

void MapExample::save()
{
	Json::Value saveData;

	Json::Value cameraData;
	Camera::getCurrent().getJsonData(cameraData);

	saveData["camera"] = cameraData;
	saveData["testKey"] = "testValue";

	help::saveJson(saveFileName, saveData);

#if _DEBUG
	Engine::Core::log("SAVE: " + saveFileName + "\n" + help::stringFroJson(saveData));
#endif // _DEBUG
}

void MapExample::update()
{
	if (_mapGame) {
		_mapGame->action();
	}
}

void MapExample::draw()
{
	Draw::viewport();
	Draw::clearColor();

	//	DrawLine

	draw::DrawLine::prepare();

	{
		float points[] = {	0.0f, 0.0f, 0.0f, 10.0f, 20.0f, 20.0f,
							10.0f, 20.0f, 20.0f, 20.0f, 20.0f, 20.0f };
		Line line(points, 4, Line::LINE);
		line.setLineWidth(5.0f);
		line.color = Color::GREEN;

		draw::DrawLine::draw(line);
	}

	{
		float points[] = {	20.0f, 30.0f, 0.0f,
							20.0f, 30.0f, 20.0f,
							30.0f, 30.0f, 20.0f };
		Line line(points, 3, Line::LOOP);
		line.setLineWidth(5.0f);
		line.color = { 0.3f, 0.6f, 0.9f,0.5f };

		draw::DrawLine::draw(line);
	}

	{
		float points[] = {	30.0f, 40.0f, 0.0f,
							30.0f, 40.0f, 20.0f,
							40.0f, 40.0f, 20.0f };
		Line line(points, 3, Line::STRIP);
		line.setLineWidth(5.0f);
		line.color = Color::RED;
		line.color.setAlpha(0.5);

		draw::DrawLine::draw(line);
	}

	draw::DrawLine::draw(greed);

	// Draw

	Draw::prepare();

	if (_mapGame) {
		Draw::draw(*_mapGame);
	}

	// Draw interface

	UI::Screen::prepare();
	UI::Screen::draw();
	// Interface

	/*for (RectPtr& rect : _rects) {
		Draw::draw(rect->_triangle);
	}

	for (Rect3Ptr& rect : _rect3s) {
		Draw::draw(rect->_triangles[0]);
		Draw::draw(rect->_triangles[1]);
		Draw::draw(rect->_triangles[2]);
	}*/


	/*UI::Element* element = _elementPtr.get();
	if (element) {
		for (int i = 0; i < element->_count; ++i) {
			const Triangle& trinagle = element->_triangles[i];
			Draw::draw(trinagle);
		}
	}*/
}

void MapExample::resize()
{
	Camera::getCurrent().setPerspective(45.0f, Engine::Screen::aspect(), 0.1f, 1000.0f);
}
