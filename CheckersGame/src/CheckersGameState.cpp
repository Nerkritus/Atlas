#include "CheckersGameState.h"

using namespace AtlasCheckers;

///
///
//
void CheckersGameState::NewGame()
{
	_turnsCount = 0;
	_whiteCaptures = 0;
	_redCaptures = 0;
	_currentTurnColour = PlayerColours::White;

	_whiteCursor._x = 0;
	_whiteCursor._y = 0;

	_redCursor._x = 0;
	_redCursor._y = 0;

	_selectedPieceID = 0;

	_gameClock.Restart();

	bool isWhite = true;
	//Set the game board
	for (int i = 0; i < BOARD_SIZE; i++) {
		isWhite = i % 2 != 0;

		for (int j = 0; j < BOARD_SIZE; j++) {
			_gameBoard[i][j]._isWhite = isWhite;
			_gameBoard[i][j]._occupyingPiece._isKing = false;

			if (!isWhite && (i <= 2 || i >= 5)) {
				_gameBoard[i][j]._occupyingPiece._colour = i <= 2 ? PlayerColours::White : PlayerColours::Red;
			}
			else {
				_gameBoard[i][j]._occupyingPiece._colour = PlayerColours::None;
				_gameBoard[i][j]._occupyingPiece._pieceID = 0;
			}

			isWhite = !isWhite;
		}
	}
	EndTurn();
}

///
void CheckersGameState::EndTurn()
{
	_currentTurnColour = _currentTurnColour == PlayerColours::White ? PlayerColours::Red : PlayerColours::White;
	_moveChaining = false;
	_selectedPieceID = 0; 

	if (_currentTurnColour == PlayerColours::White) {
		_turnsCount++;
	}
}

void CheckersGameState::MoveCursor(Direction2D direction)
{
	auto currentPos = _currentTurnColour == PlayerColours::Red ? &_redCursor : &_whiteCursor;

	int increment = 1;

	// Red's view is upside down, so we need to invert their directions;
	if (_currentTurnColour == PlayerColours::Red) {
		increment = -1;
	}

	switch (direction) {
	case Direction2D::Up:
		currentPos->_x+=increment;
		break;
	case Direction2D::Down:
		currentPos->_x -= increment;

		break;
	case Direction2D::Left:
		currentPos->_y -= increment;

		break;
	case Direction2D::Right:
		currentPos->_y += increment;
		break;
	}

	if (currentPos->_y < 0) {
		currentPos->_y = 0;
	}
	else if (currentPos->_y >= BOARD_SIZE) {
		currentPos->_y = BOARD_SIZE-1;
	}	
	if (currentPos->_x < 0) {
		currentPos->_x = 0;
	}
	else if (currentPos->_x >= BOARD_SIZE) {
		currentPos->_x = BOARD_SIZE-1;
	}

}

bool CheckersGameState::HandleSelection()
{
	auto currentPos = _currentTurnColour == PlayerColours::Red ? _redCursor : _whiteCursor;

	auto selectedTile = _gameBoard[currentPos._x][currentPos._y];

	// Piece can never be on white tiles
	if (selectedTile._isWhite) {
		return false;
	}

	// If the tile contains an enemy piece, this is never a valid selection
	if (selectedTile._occupyingPiece._colour == (_currentTurnColour == PlayerColours::White ? PlayerColours::Red : PlayerColours::White)) {
		return false;
	}

	// Unless there is a move in progress, selecting your own piece should select it
	if (!_moveChaining && selectedTile._occupyingPiece._colour == _currentTurnColour) {
		_selectedPieceID = selectedTile._occupyingPiece._pieceID;
		_selectedPiecePos = currentPos;
		_selectedPiece = selectedTile._occupyingPiece;
		return true;
	}
	else {
		Man capturedPiece;
		// If we get here, then we need to check for a potentially legal move 
		if (CheckLegalMove(currentPos, &capturedPiece)) {

			// Check if a piece was captured, if yes, allow another jump
			_moveChaining = true;

			return true;
		}
		else {
			return false;
		}
	}
}

void CheckersGameState::SetTilePiece(Position2D pos, int pieceID)
{
	_gameBoard[pos._x][pos._y]._occupyingPiece._pieceID = pieceID;
}

bool CheckersGameState::CheckMove(Position2D origin, Position2D destination)
{
	auto xDiff = abs(origin._x - destination._x);
	if (xDiff <= 0 || xDiff > 2) {
		return false;
	}
	auto yDiff = abs(origin._y - destination._y);
	if (yDiff <= 0 || yDiff > 2) {
		return false;
	}

	// If the difference is 2, make sure there is an enemy piece being jumped
	if (xDiff && yDiff == 2) {

	}

	// Now check direction
	if (!_selectedPiece._isKing && _selectedPiece._colour == PlayerColours::White) {
		// Only forward is allowed
	}
	if (!_selectedPiece._isKing && _selectedPiece._colour == PlayerColours::Red) {
		// Only backward is allowed
	}

	return true;
}

/// CheckLegalMove
///
///	Check the destination position again
///
bool CheckersGameState::CheckLegalMove(Position2D destination, Man* capturedPiece)
{
	auto destinationTile = _gameBoard[destination._x][destination._y];
	// Eliminate some other simple cases that aren't valid moves
	if (_selectedPieceID == 0 || _selectedPiecePos == destination || destinationTile._occupyingPiece._colour != PlayerColours::None) {
		return false;
	}

	return CheckMove(_selectedPiecePos, destination);

	//int increment = 1;

	//// Red's view is upside down, so we need to invert their directions;
	//if (_currentTurnColour == PlayerColours::Red) {
	//	increment = -1;
	//}

	//// Check position bounds

	//return true;
}




