
#include <iostream>
#include <fstream>

#include "json/json.h"
#include <glm/glm.hpp>
#include "glm/mat4x4.hpp"

#include "MapPhysX.h"

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
#include "Object/Model.h"
#include "Object/Map.h"
#include "Object/Line.h"
#include "Object/Triangle.h"
#include "Object/Color.h"
#include "Object/Identify.h"
#include "Physics/Physics.h"

const std::string saveFileName("Saves/MapPhysXSave.json");
const std::string mapFileName("PhysX/MapPhysX");

MapPhysX::~MapPhysX()
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

void MapPhysX::init()
{
	Engine::FileManager::setResourcesDir("..\\..\\Source\\Resources\\Files");
	if (!load())
	{
		Camera::current.setFromEye(true);
		Camera::current.setPos(glm::vec3(0.0f, 0.0f, 0.0f));
		Camera::current.setDist(1.0f);
	}

	Draw::setClearColor(0.9f, 0.6f, 0.3f, 1.0f);

	if (!_mapGame) {
		_mapGame = new Map(mapFileName);
	}

	_greed = new Greed(100.0f, 10.0f);
	_greed->setPos({ 0.0f, 0.0f, 0.1f });
	
	initCallback();
	initPhysic();
}

void MapPhysX::initCallback()
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
		});
	}
}

void MapPhysX::initPhysic() {
	if (Engine::initPhysics()) {
		if (Engine::createScene()) {
			{ // Triangle
				Object* triangleObject = _mapGame->getObjectByName("Triangle");
				if (triangleObject) {
					Model& model = triangleObject->getModel();
					//const Shape& shape = model.getShape();
					Mesh& mesh = model.getMesh();
					glm::vec3 vPos = triangleObject->getPos();
					float pos[] = { vPos.x, vPos.y, vPos.z };

					if (Engine::createActorTriangle(&mesh, pos)) {
						help::log("Game: physics is create.");
					}
					else {
						help::log("Game: physics is create FAIL.");
					}
				}
			}

			/*for (int i = 0; i < 4; ++i)
			{ // Box
				//float rect[] = { 1.0f, 1.0f, 3.0f };
				//float pos[] = { 0.0f, 0.0f, 10.0f * i };

				float xR = help::random_f(0.5f, 5.0f, 100.0f);
				float yR = help::random_f(0.5f, 5.0f, 100.0f);
				float zR = help::random_f(0.5f, 5.0f, 100.0f);

				float rect[] = { xR, yR, zR };

				float x = 1.0f + std::cos((float)i / 50.0f) * 0.1f;
				float y = 1.0f + std::sin((float)i / 50.0f) * 0.1f;
				float z =  ((float)i * 5.0f)  + 50.0f;

				float pos[] = { x, y, z };

				Engine::createActorBox(rect, pos);
			}*/

			{ // Convex
				Object* object = _mapGame->getObjectByName("Box1");
				if (object) {
					Model& model = object->getModel();
					Mesh& mesh = model.getMesh();
					glm::vec3 vPos = object->getPos();
					float pos[] = { vPos.x, vPos.y, vPos.z };

					Engine::ActorPhyscs* actorPhyscs = Engine::createActorConvex(&mesh, pos);
					if (actorPhyscs) {
						object->_actorPhyscs = actorPhyscs;
					}
				}
			}

			{ // Convex
				Object* object = _mapGame->getObjectByName("Sphere");
				if (object) {
					Model& model = object->getModel();
					Mesh& mesh = model.getMesh();
					glm::vec3 vPos = object->getPos();
					float pos[] = { vPos.x, vPos.y, vPos.z };

					Engine::ActorPhyscs* actorPhyscs = Engine::createActorConvex(&mesh, pos);
					if (actorPhyscs) {
						object->_actorPhyscs = actorPhyscs;
					}
				}
			}

			{ // Convex
				Object* object = _mapGame->getObjectByName("SphereBox");
				if (object) {
					Model& model = object->getModel();
					Mesh& mesh = model.getMesh();
					glm::vec3 vPos = object->getPos();
					float pos[] = { vPos.x, vPos.y, vPos.z };

					Engine::ActorPhyscs* actorPhyscs = Engine::createActorConvex(&mesh, pos);
					if (actorPhyscs) {
						object->_actorPhyscs = actorPhyscs;
					}
				}
			}

			{ // Convex
				Object* object = _mapGame->getObjectByName("Sphere_01");
				if (object) {
					Model& model = object->getModel();
					Mesh& mesh = model.getMesh();
					glm::vec3 vPos = object->getPos();
					float pos[] = { vPos.x, vPos.y, vPos.z };

					Engine::ActorPhyscs* actorPhyscs = Engine::createActorConvex(&mesh, pos);
					if (actorPhyscs) {
						object->_actorPhyscs = actorPhyscs;
					}
				}
			}

			for (int i = 0; i < 1000; ++i) {
				float x = 1.0f + std::cos((float)i / 50.0f) * 15.1f;
				float y = 1.0f + std::sin((float)i / 50.0f) * 15.1f;
				float z = ((float)i * 1.0f) + 10.0f;

				float pos[] = { x, y, z };
				glm::vec3 vPos(pos[0], pos[1], pos[2]);

				//Object& object = _mapGame->addObjectToPos("Car");
				Object& object = _mapGame->addObjectToPos("Box1");
				Mesh& mesh = object.getModel().getMesh();

				Engine::ActorPhyscs* actorPhyscs = Engine::createActorConvex(&mesh, pos);
				if (actorPhyscs) {
					object._actorPhyscs = actorPhyscs;
				}
			}	
		}
	}
}

bool MapPhysX::load()
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

void MapPhysX::save()
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

void MapPhysX::update()
{
	if (Engine::updateScene(Engine::Core::deltaTime())) {
		for (auto object : _mapGame->objects) {
			if (!object->_actorPhyscs) {
				continue;
			}

			Position pos = Engine::getActorPos(object->_actorPhyscs);
			object->setPos(pos);
		}
	}

	if (_mapGame) {
		_mapGame->action();
	}
}

void MapPhysX::draw()
{
	Draw::viewport();
	Draw::clearColor();

	//	DrawLine

	draw::DrawLine::prepare();
	draw::DrawLine::draw(*_greed);

	// Draw

	Draw::prepare();

	if (_mapGame) {
		Draw::draw(*_mapGame);
	}
}

void MapPhysX::resize()
{
	Camera::getCurrent().setPerspective(45.0f, Engine::Screen::aspect(), 0.1f, 1000.0f);
}
