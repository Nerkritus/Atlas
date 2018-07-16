#include "GameScene.h"
#include "../enums/Direction2D.h"

#include "Atlas/IO.h"


using namespace AtlasCheckers;
using namespace Atlas;

GameScene::GameScene(std::string name)
	: Scene(name)
{
	_turnLabel = nullptr;
	_redCapturesLabel = nullptr;
	_whiteCapturesLabel = nullptr;
	_redSelect = nullptr;
	_whiteSelect = nullptr;

	_state.NewGame();
	_stateUpdatePending = true;

	_uiTick = 0;
}

// Overidden scene methods
void GameScene::UpdateScene(double frameDelta)
{
	Scene::UpdateScene(frameDelta);

	if (_stateUpdatePending) {
		_whiteSelect->SetVisibility(_state.GetCurrentTurnPlayer() == PlayerColours::White);
		_redSelect->SetVisibility(_state.GetCurrentTurnPlayer() == PlayerColours::Red);
		if (_state.GetCurrentTurnPlayer() == PlayerColours::White) {
			_turnLabel->SetText("White's Turn");
			_turnLabel->SetColour(glm::vec3(1.0f, 1.0f, 1.0f));

		}
		else {
			_turnLabel->SetText("Red's Turn");
			_turnLabel->SetColour(glm::vec3(1.0f, 0.0f, 0.0f));
		}

		_turnCountLabel->SetText("Turns: " + std::to_string(_state.GetTurn()));
		_redCapturesLabel->SetText("Red Captures: " + std::to_string(_state.GetScore(PlayerColours::Red)));
		_whiteCapturesLabel->SetText("White Captures: " + std::to_string(_state.GetScore(PlayerColours::Red)));

		_whiteSelect->SetPosition(-7 + (_state.GetCursorPos(PlayerColours::White)._x*2), 0.1f, -7 + (_state.GetCursorPos(PlayerColours::White)._y*2));
		_redSelect->SetPosition(-7 + (_state.GetCursorPos(PlayerColours::Red)._x*2), 0.1f, -7 + (_state.GetCursorPos(PlayerColours::Red)._y*2));

		_uiTick = GetRuntimeMs();

		// Update the game board pieces
		for (int i = 0; i < BOARD_SIZE; i++) {
			for (int j = 0; j < BOARD_SIZE; j++) {
				auto tile = _state.GetBoardTile(i, j);
				if (tile._occupyingPiece._pieceID != 0) {
					auto piece = GetEntityById(tile._occupyingPiece._pieceID);
					auto pos = piece->GetPosition();
					piece->SetPosition(pos.x, 0.1f, pos.z);
				}
			}
		}
		_stateUpdatePending = false;
	}

	if (_state.GetSelectedPiece() > 0) {
		auto piece = GetEntityById(_state.GetSelectedPiece());
		if (piece != nullptr) {			
			// Animate the piece somehow so we know it's selected
			auto pos = piece->GetPosition();
			piece->SetPosition(pos.x, 0.5f, pos.z);
		}
	}
}

//
//
//
void GameScene::SceneLoaded()
{
	_turnLabel = GetTextById("m_playerTurn");
	_turnCountLabel = GetTextById("m_turnCounter");
	_redCapturesLabel = GetTextById("m_redScore");
	_whiteCapturesLabel = GetTextById("m_whiteScore");
	_redSelect = GetEntityById("red_overlay");
	_whiteSelect = GetEntityById("white_overlay");

	AddText("txt_info", "This is the text info label", 280, 50, FontStyleEnum::Large, TextAlignmentEnum::Centre, TextAlignmentEnum::Centre, false);
	_infoLabel = GetTextById("txt_info");

	_sndOK = GetSoundByName("ok.wav")->soundId;
	_sndNO = GetSoundByName("no.wav")->soundId;

	LoadDynamicAssets();

	_whiteCamHome = glm::vec3(-20, 15, 5);
	_redCamHome = glm::vec3(20, 15, 5);

	Scene::SceneLoaded();
}

//
//
//
void GameScene::InputProcessing(Atlas::Input* input)
{
	if (input->IsKeyPressed((int)AtlasKey::Space, 200)) {
		_stateUpdatePending = true;

		// Select a piece or complete move if a piece is selected
		if (_state.HandleSelection()) {
			PlaySound(_sndOK);
		}
		else {
			PlaySound(_sndNO);
		}
	}

	if (input->IsKeyPressed((int)AtlasKey::Enter, 1000 )) {
		_state.EndTurn();
		_stateUpdatePending = true;

		if (_state.GetCurrentTurnPlayer() == PlayerColours::White) {
			_cam.SetPosition(_whiteCamHome.x, _whiteCamHome.y, _whiteCamHome.z);
		}
		else {
			_cam.SetPosition(_redCamHome.x, _redCamHome.y, _redCamHome.z);
		}
	}

	// Check for camera movement
	if (input->IsKeyPressed((int)AtlasKey::Shift)) {
		if (input->IsKeyPressed((int)AtlasKey::Up)) {
			GetCamera().MoveForward();
		}
		if (input->IsKeyPressed((int)AtlasKey::Down)) {
			GetCamera().Backpedal();
		}
		if (input->IsKeyPressed((int)AtlasKey::Left)) {
			GetCamera().Strafe(true);
		}
		if (input->IsKeyPressed((int)AtlasKey::Right)) {
			GetCamera().Strafe(false);
		}
	}
	else {
		int cursorPause = 180;
		if (input->IsKeyPressed((int)AtlasKey::Up, cursorPause)) {
			_state.MoveCursor(Direction2D::Up);
			_stateUpdatePending = true;

		}
		if (input->IsKeyPressed((int)AtlasKey::Down, cursorPause)) {
			_state.MoveCursor(Direction2D::Down);
			_stateUpdatePending = true;

		}
		if (input->IsKeyPressed((int)AtlasKey::Left, cursorPause)) {
			_state.MoveCursor(Direction2D::Left);
			_stateUpdatePending = true;

		}
		if (input->IsKeyPressed((int)AtlasKey::Right, cursorPause)) {
			_state.MoveCursor(Direction2D::Right);
			_stateUpdatePending = true;

		}
	}
}

/// LoadDynamicAssets
///
///	Called when the Scene is being loaded, after the file load is complete. This is the scene's opportunity to
/// load any dynamic assets before the scene finishes loading.
///
void GameScene::LoadDynamicAssets()
{
	int xPos = -7, zPos = -7;

	int id = 1000;

	// Generate game board
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			auto tile = _state.GetBoardTile(i, j);

			AddBoardTile(xPos, zPos, tile._isWhite);

			if (tile._occupyingPiece._colour != PlayerColours::None) {
				AddPiece(xPos, zPos, id, tile._occupyingPiece._colour);
				_state.SetTilePiece(Position2D(i,j), id);

				id++;
			}

			zPos += 2;
		}
		xPos += 2;
		zPos = -7;
	}

	_whiteSelect->SetVisibility(false);
	_redSelect->SetVisibility(false);
	//_whiteSelect->SetPosition(0, 0.1, 0);
	//_redSelect->SetPosition(0, 0.1, 0);
}

///
///
///
void GameScene::AddBoardTile(int xPos, int zPos, bool isWhite)
{
	std::string texDir = Atlas::IO::GetTextureDirectory();
	std::string black = texDir + "black_leather_01.png";
	std::string white = texDir + "white_leather_01.png";

	Atlas::EntityCreateInfo tile;
	tile.shader = _subsystems._shaderManager->GetShaderByName("littex");
	tile.pos = glm::vec3(xPos, 0, zPos);
	tile.texCount = 1;
	tile.uniformScale = 1;
	tile.texRepeat = 1;
	tile.type = Atlas::EntityTypeEnum::ET_Plane;
	tile.textureID[0] = _subsystems._texManager->LoadTexture(isWhite ? white : black);

	this->AddEntity(_subsystems._geometry->CreateEntity(tile, _subsystems._phys));
}

///
///
///
void GameScene::AddPiece(int xPos, int zPos, int numID, PlayerColours colour)
{
	Atlas::EntityCreateInfo piece;
	piece.shader = _subsystems._shaderManager->GetShaderByName("lighting");
	piece.pos = glm::vec3(xPos, 0.1f, zPos);
	piece.uniformScale = 0.7f;
	piece.colour = colour == PlayerColours::White ? glm::vec3(1.0f, 1.0f, 1.0f) : glm::vec3(1.0f, 0, 0);
	piece.type = Atlas::EntityTypeEnum::ET_Mesh;
	piece.numID = numID;

	std::string mesh = "piece.obj";

	AddMesh(mesh, piece);
}