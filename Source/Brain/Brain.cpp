
#include <iostream>
#include <fstream>
#include <algorithm>

#include "json/json.h"
#include <glm/glm.hpp>
#include "glm/mat4x4.hpp"

#include "Brain.h"

#include "Core.h"
#include "FileManager.h"
#include "Screen.h"
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

const std::string saveFileName("Saves/BrainSave.json");
const std::string mapFileName("Brain/Brain");

Brain::~Brain()
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

void Brain::init()
{
	Engine::FileManager::setResourcesDir("..\\..\\Source\\Resources\\Files");
	if (!load())
	{
		Camera::current.setFromEye(true);
		Camera::current.setPos(glm::vec3(0.0f, 0.0f, 0.0f));
		Camera::current.setDist(1.0f);
	}

	Camera::current.setDist(0.1f);

	Draw::setClearColor(0.9f, 0.6f, 0.3f, 1.0f);

	if (!_mapGame) {
		_mapGame = new Map(mapFileName);


		std::string nameCar = "Car_red";
		std::vector<Object*> objects = _mapGame->objects;

		for (Object* object : objects) {
			if (object && object->getName() == nameCar) {
				_car = object;
			}
		}

		if (_car) {
			std::cout << "Car " << _car->getName() << " found" << std::endl;
		}
		else {
			std::cout << "Car " << nameCar << " not found" << std::endl;
		}
	}

	_greed = new Greed (100.0f, 10.0f);
	_greed->setPos({0.0f, 0.0f, 0.1f});
	
	initCallback();
}

void Brain::initCallback()
{
	if (!_callback) {
		_callback = new Engine::Callback(Engine::CallbackType::PINCH_TAP, [](const Engine::CallbackEventPtr& callbackEventPtr) {
			if (Engine::Callback::pressTap(Engine::VirtualTap::RIGHT)) {
				Camera::current.rotate(Engine::Callback::deltaMousePos());
			}

			if (Engine::Callback::pressTap(Engine::VirtualTap::MIDDLE)) {
				Camera::current.move(Engine::Callback::deltaMousePos() * 1000.0f * Engine::Core::deltaTime());
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

			if (key == Engine::VirtualKey::P && Engine::Callback::pressKey(Engine::VirtualKey::CONTROL)) {
				_typeCamera = _typeCamera ? false : true;
				Camera::current.setFromEye(_typeCamera);
				Camera::current.setDist(10.0f);
			}
		});

		_callback = new Engine::Callback(Engine::CallbackType::PINCH_KEY, [this](const Engine::CallbackEventPtr& callbackEventPtr) {
			if (Engine::Callback::pressKey(Engine::VirtualKey::CONTROL)) {
				return;
			}

			// ”правление камеры
			float speedCamera = 5.0f * Engine::Core::deltaTime();
			if (Engine::Callback::pressKey(Engine::VirtualKey::SHIFT)) {
				speedCamera = 30.0f * Engine::Core::deltaTime();
			}

			if (Engine::Callback::pressKey(Engine::VirtualKey::S)) {
				Camera::current.move(CAMERA_FORVARD, speedCamera);
			}

			if (Engine::Callback::pressKey(Engine::VirtualKey::W)) {
				Camera::current.move(CAMERA_BACK, speedCamera);
			}

			if (Engine::Callback::pressKey(Engine::VirtualKey::D)) {
				Camera::current.move(CAMERA_RIGHT, speedCamera);
			}

			if (Engine::Callback::pressKey(Engine::VirtualKey::A)) {
				Camera::current.move(CAMERA_LEFT, speedCamera);
			}

			if (Engine::Callback::pressKey(Engine::VirtualKey::R)) {
				Camera::current.move(CAMERA_TOP, speedCamera);
			}

			if (Engine::Callback::pressKey(Engine::VirtualKey::F)) {
				Camera::current.move(CAMERA_DOWN, speedCamera);
			}

			// ”правление машинкой
			if (Engine::Callback::pressKey(Engine::VirtualKey::I)) {
				updateCar('I');
			}
			if (Engine::Callback::pressKey(Engine::VirtualKey::K)) {
				updateCar('K');
			}
			if (Engine::Callback::pressKey(Engine::VirtualKey::J)) {
				updateCar('J');
			}
			if (Engine::Callback::pressKey(Engine::VirtualKey::L)) {
				updateCar('L');
			}
		});
	}
}

bool Brain::load()
{
	Json::Value saveData;
	if (!help::loadJson(saveFileName, saveData) || saveData.empty()) {
		return false;
	}

	if (!saveData["camera"].empty()) {
		Json::Value& cameraData = saveData["camera"];
		Camera::current.setJsonData(cameraData);
	}

#if _DEBUG
	Engine::Core::log("LOAD: " + saveFileName + "\n" + help::stringFroJson(saveData));
#endif // _DEBUG

	return true;
}

void Brain::save()
{
	Json::Value saveData;

	Json::Value cameraData;
	Camera::current.getJsonData(cameraData);

	saveData["camera"] = cameraData;
	saveData["testKey"] = "testValue";

	help::saveJson(saveFileName, saveData);

#if _DEBUG
	Engine::Core::log("SAVE: " + saveFileName + "\n" + help::stringFroJson(saveData));
#endif // _DEBUG
}

void Brain::updateCar(const char key) {
	glm::vec3 pos(0.0f, 0.0f, 0.0f);

	if (_car) {
		pos = _car->getPos();

		if (key == 'I') {
			float speed = 0.005f;
			pos.x += speed;
		}
		if (key == 'K') {
			float speed = 0.0025f;
			pos.x -= speed;
		}
		if (key == 'J') {
			float speed = 0.0025f;
			pos.y += speed;
		}
		if (key == 'L') {
			float speed = 0.0025f;
			pos.y -= speed;
		}

		_car->setPos(pos);
	}

	if (!_typeCamera && !(pos.x == 0.0f && pos.y == 0.0f && pos.z == 0.0f)) {
		Camera::getCurrent().setPos(pos);
	}

}

void Brain::update()
{
	if (_mapGame) {
		_mapGame->action();
	}
}

void Brain::draw()
{
	Draw::viewport();
	Draw::clearColor();

	//	DrawLine

	draw::DrawLine::prepare();

	/*{
		float points[] = { 0.0f, 0.0f, 0.0f, 10.0f, 20.0f, 20.0f,
							10.0f, 20.0f, 20.0f, 20.0f, 20.0f, 20.0f };
		Line line(points, 4, Line::LINE);
		line.setLineWidth(5.0f);
		line.color = Color::GREEN;

		draw::DrawLine::draw(line);
	}

	{
		float points[] = { 20.0f, 30.0f, 0.0f,
							20.0f, 30.0f, 20.0f,
							30.0f, 30.0f, 20.0f };
		Line line(points, 3, Line::LOOP);
		line.setLineWidth(5.0f);
		line.color = { 0.3f, 0.6f, 0.9f,0.5f };

		draw::DrawLine::draw(line);
	}

	{
		float points[] = { 30.0f, 40.0f, 0.0f,
							30.0f, 40.0f, 20.0f,
							40.0f, 40.0f, 20.0f };
		Line line(points, 3, Line::STRIP);
		line.setLineWidth(5.0f);
		line.color = Color::RED;
		line.color.setAlpha(0.5);

		draw::DrawLine::draw(line);
	}*/

	if (_greed) {
		draw::DrawLine::draw(*_greed);
	}
	// Draw

	Draw::prepare();

	if (_mapGame) {
		Draw::draw(*_mapGame);
	}
}

void Brain::resize()
{
	Camera::getCurrent().setPerspective(45.0f, Engine::Screen::aspect(), 0.1f, 1000.0f);
}
