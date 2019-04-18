#include "AtlasDemoGame.h"
#include "TestScene.h"

using namespace Atlas;
using namespace AtlasDemo;

AtlasDemoGame::AtlasDemoGame(std::string& basePath)
	:AtlasGame(basePath), _scene(nullptr)
{

}

AtlasDemoGame::~AtlasDemoGame()
{
	//delete _scene;
}

bool AtlasDemoGame::InitialiseGame()
{
	return true;
}

Atlas::Scene* AtlasDemoGame::GetPendingScene()
{
	return _scene == nullptr ? _scene = new TestScene() : nullptr;
}

void AtlasDemoGame::InputProcessing(Atlas::Input* input)
{
	if (_scene != nullptr) {
		_scene->InputProcessing(input);
	}
}

void AtlasDemoGame::UpdateGame(double frameDelta)
{

}