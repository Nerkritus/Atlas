#pragma once
#include "AtlasUtil/AtlasStopwatch.h"
#include "enums/Direction2D.h"

namespace AtlasCheckers
{
	enum PlayerColours
	{
		White,
		Red,
		None
	};

	struct Position2D
	{
		Position2D() : Position2D(0, 0) {}
		Position2D(int x, int y) : _x(x), _y(y) {}
		int _x;
		int _y;

		bool operator==(Position2D& cmp)
		{
			return (_x == cmp._x && _y == cmp._y);
		}
	};

	struct Man
	{
		int _pieceID;
		PlayerColours _colour;
		bool _isKing;
	};

	struct BoardTile
	{
		bool _isWhite;
		Man _occupyingPiece;
	};

	const int BOARD_SIZE = 8;

	class CheckersGameState
	{
	public:
		//		CheckersGameState();
		//
		//		void InitialiseGame();
		void NewGame();
		void MoveCursor(Direction2D direction);
		void EndTurn();
		bool HandleSelection();
		//
		//		const PlayStateEnum GetPlayState() { return _sceneState; }

		PlayerColours GetCurrentTurnPlayer() const { return _currentTurnColour; }
		int GetTurn() const { return _turnsCount; }

		int GetScore(PlayerColours colour) const { return colour == PlayerColours::Red ? _redCaptures : _whiteCaptures; }

		const BoardTile GetBoardTile(const int x, const int y) { return _gameBoard[x][y]; }

		const Position2D& GetCursorPos(PlayerColours colour) const { return colour == PlayerColours::Red ? _redCursor : _whiteCursor; }
		int GetSelectedPiece() { return _selectedPieceID; }

		void SetTilePiece(Position2D pos, int pieceID);
//
	private:

		bool CheckLegalMove(Position2D destination, Man* capturedPiece);
		bool CheckMove(Position2D origin, Position2D destination);

		int _turnsCount;

		int _redCaptures;
		int _whiteCaptures;
//		BoardStatesEnum _playerTurn;
//
		Position2D _whiteCursor;
		Position2D _redCursor;
		int _whiteCursorSquareX;
		int _whiteCursorSquareY;
		int _redCursorSquareX;
		int _redCursorSquareY;

		// Game state
		int _selectedPieceID;
		Position2D _selectedPiecePos;
		Man _selectedPiece;
		bool _moveChaining;

		
//
		BoardTile _gameBoard[BOARD_SIZE][BOARD_SIZE];
//
//		PlayStateEnum _sceneState;
//
		AtlasUtil::AtlasStopwatch _gameClock;

		PlayerColours _currentTurnColour;
//
	};
} 