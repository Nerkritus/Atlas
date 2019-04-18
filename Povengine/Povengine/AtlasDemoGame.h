#pragma once

#include "..\Atlas\AtlasGame.h"
#include <string>


namespace AtlasDemo
{
	class AtlasDemoGame : public Atlas::AtlasGame
	{
	public:
		AtlasDemoGame(std::string& basePath);
		virtual ~AtlasDemoGame();

		virtual bool InitialiseGame();
		virtual Atlas::Scene* GetPendingScene();
		virtual void InputProcessing(Atlas::Input* input);
		virtual void UpdateGame(double frameDelta);

	private:
		Atlas::Scene* _scene;


	};
}