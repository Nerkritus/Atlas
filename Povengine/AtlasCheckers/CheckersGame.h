#pragma once

#include "..\Atlas\AtlasGame.h"
#include "..\AtlasUtil\AtlasStopwatch.h"

#include "BoardStatesEnum.h"

#include "MainMenuScene.h"
#include "GameScene.h"

namespace AtlasCheckers
{
	enum CheckersStateEnum
	{
		MainMenu,
		Playing
	};

	class CheckersGame : public Atlas::AtlasGame
	{
	public:
		CheckersGame();
		virtual ~CheckersGame() { }

		virtual bool InitialiseGame();

		virtual Atlas::Scene* GetPendingScene();

		virtual void InputProcessing(const Atlas::Input* input);
		virtual void UpdateGame();

		void SetState(CheckersStateEnum newState);

	private:
		void SetupGame();
		void NewGame();
		void EndTurn();


		int gameBoard[8][8];
		int _turnsCount;
		BoardStatesEnum _playerTurn;

		int _whiteSelectedSquareX;
		int _whiteSelectedSquareY;
		int _redSelectedSquareX;
		int _redSelectedSquareY;

		CheckersStateEnum _gameState;
		
		MainMenuScene* _menuScene;
		GameScene* _gameScene;

		Atlas::Scene* _pendingScene;

		AtlasUtil::AtlasStopwatch _uiKeystrokeDelay;
		AtlasUtil::AtlasStopwatch _gameClock;

		AtlasUtil::AtlasStopwatch _gameTick;

	};
}