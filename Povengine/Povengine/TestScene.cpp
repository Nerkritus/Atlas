#include "TestScene.h"
#include "../Atlas/IO.h"
#include "../AtlasUtil/Random.h"

using namespace Atlas;
using namespace AtlasDemo;
using namespace AtlasUtil;

TestScene::TestScene()
	: Scene("demo1.as")
{

}

void TestScene::SceneLoaded()
{
	Scene::SceneLoaded();

	std::string texDir = IO::GetTextureDirectory();
	_crateTex = _subsystems._texManager->LoadTexture(texDir + "crate.jpg");
}

void TestScene::UpdateScene(double frameDelta)
{
	Scene::UpdateScene(frameDelta);

	if (GetRuntimeMs() - _lastSpawn > 1000) {
		int random = Random::GetInt(10);
		int random2 = Random::GetInt(10);
		double diff = (Random::GetInt(10)) / 10.0;

		// Test finite
		EntityCreateInfo ei;
		ei.type = EntityTypeEnum::ET_Cube;
		ei.pos = glm::vec3(random - 5, 5, random2 - 5);
		ei.uniformScale = 1;
		ei.textureID[0] = _crateTex;
		ei.shader = _subsystems._shaderManager->GetShaderByName("littex");
		ei.lifetimeMs = 10000;
		ei.enablePhysics = true;
		ei.mass = 1.0f;
		AddEntity(_subsystems._geometry->CreateEntity(ei, _subsystems._phys));

		_lastSpawn = GetRuntimeMs();
	}
}
