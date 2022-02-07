
#include "Game.h"

#define NAME_GAME MapPhysX
#include "Template/MapTemplate.h"
#include "Examples/MapExample.h"
#include "PhysX/MapPhysX.h"
#include "NeuralNetwork/NeuralNetwork.h"

Engine::Game::Ptr Engine::Game::GetGame(const std::string& params) {
	Engine::Game::Ptr gamePtr(new NAME_GAME());
	return gamePtr;
}
