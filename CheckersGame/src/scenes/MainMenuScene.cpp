#include "MainMenuScene.h"
#include <string>

#include "Atlas/IO.h"


using namespace Atlas;
using namespace AtlasCheckers;

MainMenuScene::MainMenuScene(std::string name)
	:Scene(name), _selectionPending(false)
{
	//_activeColour = glm::vec3(1.0f, 0, 0);
	//_inactiveColour = glm::vec3(1.0f, 1.0f, 1.0f);

	_currentMenuSelection = MainMenuItemsEnum::NewResumeGame;

	_newGameLabel = nullptr;
	_resumeGameLabel = nullptr;
	_loadGameLabel = nullptr;
	_exitLabel = nullptr;
}

//
//
//
void MainMenuScene::SceneLoaded()
{
	//// Populate the items we need to track
	_menuBackground = GetSpriteById("s0");
	_newGameLabel = GetTextById("m_new");
	_resumeGameLabel = GetTextById("m_resume");
	_loadGameLabel = GetTextById("m_loadgame");
	_exitLabel = GetTextById("m_exit");
	_helpLabel = GetTextById("m_help");

	_helpBackground = GetSpriteById("s1");
	_helpText1 = GetTextById("m_helpText1");
	_helpText2 = GetTextById("m_helpText2");
	_helpText3 = GetTextById("m_helpText3");
	_helpText4 = GetTextById("m_helpText4");
	_helpText5 = GetTextById("m_helpText5");
	_helpText6 = GetTextById("m_helpText6");
	_helpText7 = GetTextById("m_helpText7");

	_sndPipId = GetSoundByName("pip01.wav")->soundId;
	_sndSelectId = GetSoundByName("porcSmall.wav")->soundId;

	SetSelectedLabel(_newGameLabel, true);
	SetGameRunning(false);

	LoadDynamicAssets();

	Scene::SceneLoaded();
}

/// LoadDynamicAssets
///
///	Called when the Scene is being loaded, after the file load is complete. This is the scene's opportunity to
/// load any dynamic assets before the scene finishes loading.
///
void MainMenuScene::LoadDynamicAssets()
{
}

///
///
///
void MainMenuScene::AddBoardTile(int xPos, int zPos, bool isWhite)
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
void MainMenuScene::AddPiece(int xPos, int zPos, int numID, bool isWhite)
{
	Atlas::EntityCreateInfo piece;
	piece.shader = _subsystems._shaderManager->GetShaderByName("lighting");
	piece.pos = glm::vec3(xPos, 0.1f, zPos);
	piece.uniformScale = 0.7f;
	piece.colour = isWhite ? glm::vec3(1.0f, 1.0f, 1.0f) : glm::vec3(1.0f, 0, 0);
	piece.type = Atlas::EntityTypeEnum::ET_Mesh;
	piece.numID = numID;

	std::string mesh = "piece.obj";

	AddMesh(mesh, piece);
}

//
//
//
void MainMenuScene::UpdateMenuSelection(bool prev)
{
	if (!_isLoaded) {
		return;
	}
	switch (_currentMenuSelection)
	{
	case MainMenuItemsEnum::NewResumeGame:
		_currentMenuSelection = prev ? MainMenuItemsEnum::Exit : MainMenuItemsEnum::LoadGame;
		break;
	case MainMenuItemsEnum::LoadGame:
		_currentMenuSelection = prev ? MainMenuItemsEnum::NewResumeGame : MainMenuItemsEnum::Help;
		break;
	case MainMenuItemsEnum::Help:
		_currentMenuSelection = prev ? MainMenuItemsEnum::LoadGame : MainMenuItemsEnum::Exit;
		break;
	case MainMenuItemsEnum::Exit:
		_currentMenuSelection = prev ? MainMenuItemsEnum::Help : MainMenuItemsEnum::NewResumeGame;
		break;
	default:
		break;
	}

	PlaySound(_sndPipId);

	SetSelectedLabel(_newGameLabel, _currentMenuSelection == MainMenuItemsEnum::NewResumeGame);
	SetSelectedLabel(_resumeGameLabel, _currentMenuSelection == MainMenuItemsEnum::NewResumeGame);
	SetSelectedLabel(_loadGameLabel, _currentMenuSelection == MainMenuItemsEnum::LoadGame);
	SetSelectedLabel(_helpLabel, _currentMenuSelection == MainMenuItemsEnum::Help);
	SetSelectedLabel(_exitLabel, _currentMenuSelection == MainMenuItemsEnum::Exit);
}

//
//
//
void MainMenuScene::SetSelectedLabel(Atlas::Sprite* label, bool isSelected)
{
	label->SetColour(isSelected ? glm::vec3(1.0f, 0.0f, 0.0f) : glm::vec3(1.0f, 1.0f, 1.0f));
}

///
///
///
void MainMenuScene::UpdateScene(double frameDelta)
{
	GetCamera().Strafe(false);

	Scene::UpdateScene(frameDelta);
}

///
///
///
void MainMenuScene::InputProcessing(Input* input)
{
	if (!_selectionPending) {
		if (input->IsKeyPressed((int)AtlasKey::Enter, 1000)) {
			PlaySound(_sndSelectId);
			_selectionPending = true;
		}

		if (input->IsKeyPressed((unsigned int)AtlasKey::Up, 200)) {
			// Change menu selection
			UpdateMenuSelection(true);
		}
		if (input->IsKeyPressed((unsigned int)AtlasKey::Down, 200)) {
			UpdateMenuSelection(false);
		}
	}
}

///
///
///
void MainMenuScene::ShowHelp(bool show)
{
	_showingHelp = show;

	_menuBackground->SetVisibility(!show);
	_newGameLabel->SetVisibility(!show);
	_loadGameLabel->SetVisibility(!show);
	_exitLabel->SetVisibility(!show);
	_helpLabel->SetVisibility(!show);

	_helpBackground->SetVisibility(show);
	_helpText1->SetVisibility(show);
	_helpText2->SetVisibility(show);
	_helpText3->SetVisibility(show);
	_helpText4->SetVisibility(show);
	_helpText5->SetVisibility(show);
	_helpText6->SetVisibility(show);
	_helpText7->SetVisibility(show);
}

///
//
//
void MainMenuScene::SetGameRunning(bool running)
{
	_newGameLabel->SetVisibility(!running);
	_resumeGameLabel->SetVisibility(running);
}