#include "AtlasManager.h"

#include <sstream>
#include <string>

#include "Win32Window.h"
#include "OpenGLRenderer.h"
#include "../AtlasUtil/AtlasMessageBox.h"
#include "../AtlasAPI/AtlasAPIHelper.h"

using namespace Atlas;
using namespace AtlasUtil;

/// <summary>
/// Construct the manager
/// </summary>
AtlasManager::AtlasManager()
	:_name("Atlas"), _applicationWindow(nullptr), _renderer(nullptr), _currentScene(nullptr)
{
	for (int i = 0; i < 256; i++) {
		_keyStates[i] = false;
	}

	std::stringstream fmt;
	fmt << AtlasAPI::AtlasAPIHelper::GetUserDataPath() << AtlasAPI::AtlasAPIHelper::GetPathSeparator() << _name;
	_mainDir = fmt.str();
	fmt << AtlasAPI::AtlasAPIHelper::GetPathSeparator() << _name << ".log";

	AtlasAPI::AtlasAPIHelper::EnsureDirectory(_mainDir);
	_log = new AtlasLog(fmt.str(), false, 5);

	_log->Debug("Atlas Engine Starting");

	_shaderManager = new ShaderManager(_log, _mainDir);
}

/// <summary>
/// Handle retrieving new data from API-based sources
/// </summary>
AtlasManager::~AtlasManager()
{
	_log->Debug("Atlas Engine Stopping");

	if (_currentScene != nullptr) {
		delete _currentScene;
	}

	if (_renderer != nullptr) {
		delete _renderer;
	}
	if (_applicationWindow != nullptr) {
		delete _applicationWindow;
	}

	if (_log != nullptr) {
		delete _log;
	}
}

/// <summary>
/// Creates the platform appropriate window and returns it
/// </summary>
/// <returns>a platform specific window based on the Window class</returns>
Window* AtlasManager::getWindow()
{
	if (_applicationWindow != nullptr) {
		return _applicationWindow;
	}

	// Get different windows based on platform, currently only Win32
#ifdef _WIN32
	_log->Debug("Creating new (Win32) window");
	_applicationWindow = new Win32Window(this);
#endif
#ifdef __linux__
	_log->Debug("Creating new (Linux) window");
	_applicationWindow = new LinuxWindow(this);
#endif

	return _applicationWindow;
}

/// <summary>
/// Initialises the sub-systems of the Atlas Engine
/// </summary>
/// <returns>true if initialised successfully, otherwise false</returns>
bool AtlasManager::Initialise()
{
	if (_applicationWindow == nullptr) {
		return false;
	}

	_log->Debug("Creating and initialising Atlas Engine");

	_renderer = new OpenGLRenderer();
#ifdef _WIN32
	HDC context = GetDC(((Win32Window*)_applicationWindow)->getWindowHandle());
	/*      Pixel format index
	*/
	int nPixelFormat;

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),          //size of structure
		1,                                      //default version
		PFD_DRAW_TO_WINDOW |                    //window drawing support
		PFD_SUPPORT_OPENGL |                    //opengl support
		PFD_DOUBLEBUFFER,                       //double buffering support
		PFD_TYPE_RGBA,                          //RGBA color mode
		32,                                     //32 bit color mode
		0, 0, 0, 0, 0, 0,                       //ignore color bits
		0,                                      //no alpha buffer
		0,                                      //ignore shift bit
		0,                                      //no accumulation buffer
		0, 0, 0, 0,                             //ignore accumulation bits
		16,                                     //16 bit z-buffer size
		0,                                      //no stencil buffer
		0,                                      //no aux buffer
		PFD_MAIN_PLANE,                         //main drawing plane
		0,                                      //reserved
		0, 0, 0 };                              //layer masks ignored

												/*      Choose best matching format*/
	nPixelFormat = ChoosePixelFormat(context, &pfd);

	/*      Set the pixel format to the device context*/
	SetPixelFormat(context, nPixelFormat, &pfd);
	if (!_renderer->Initialise(800, 600, context)) {
		_log->Debug("Win32 OpenGL renderer failed to initialise.");
	}
#endif

	std::string dataPath = AtlasAPI::AtlasAPIHelper::GetDataPath();

	std::string vertexShader = dataPath + "vertexshader.glsl";
	std::string fragShader = dataPath + "fragmentshader.glsl";
	std::string originVertShader = dataPath + "originshader.glsl";

	auto shader1 = _shaderManager->LoadShader(vertexShader, fragShader);
	auto shader2 = _shaderManager->LoadShader(originVertShader, fragShader);

	_currentScene = new Scene();
	_currentScene->LoadScene(shader1, shader2);

	AtlasAPI::AtlasAPIHelper::GetTicks();

	return true;
}

/// <summary>
/// Starts the engine running by entering the application loop (this may be better moved to the Window classes)
/// </summary>
/// <returns>returns the exit code when the application loop finishes</returns>
int AtlasManager::start()
{
#ifdef WIN32
	// Main message loop
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			frameProcessing();
		}
	}

	return (int)msg.wParam;
#endif
}

/// <summary>
/// Handles a key press (key up or down) by setting that key's state
/// </summary>
/// <param name="keyID">ID of the key to set/unset</param>
/// <param name="isDown">bool indicating if the new key state is down or up</param>
void AtlasManager::HandleKeyPress(unsigned int keyID, bool isDown)
{
	if (keyID > 256) {
		return;
	}

	_keyStates[keyID] = isDown;
}

/// <summary>
/// Called each time a frame can be processed.
/// </summary>
void AtlasManager::frameProcessing()
{
	// Update game state
	inputProcessing();

	_renderer->beginRender();

	// Render game objects
	_currentScene->DrawScene(_renderer->GetProjection());

	_renderer->endRender();
}

/// <summary>
/// Called once per frame, allows us to process any pending user inputs 
/// </summary>
void AtlasManager::inputProcessing()
{
	float ticks = AtlasAPI::AtlasAPIHelper::GetTicks() / 100;
	static float radius = 20.0f;
	static float yPos = 0.2f;

	static float factor = 4.0f;


	// Check bindings for key presses
	_renderer->ToggleWireframe(_keyStates[0x57]);

	if (_keyStates[VK_ADD]) {	// plus
		radius += 1.0f;
	}
	if (_keyStates[VK_SUBTRACT]) {	// subtract
		radius -= 1.0f;
	}

	float xPos = sin(ticks * radius) * factor;
	float zPos = cos(ticks * radius) * factor;

	_currentScene->GetCamera().SetPosition(xPos, yPos, zPos);

	//if (_keyStates[0x28]) {	// Down arrow
	//	if (_keyStates[0x10]) {
	//		yPos -= 0.01f;
	//	}
	//	else {
	//		zPos -= 0.01f;
	//	}
	//	_currentScene->GetCamera().SetPosition(xPos, yPos, zPos);
	//}
	//if (_keyStates[0x25]) {	// Left arrow
	//	xPos -= 0.01f;
	//	_currentScene->GetCamera().SetPosition(xPos, yPos, zPos);
	//}
	//if (_keyStates[0x27]) {	// Left arrow
	//	xPos += 0.01f;
	//	_currentScene->GetCamera().SetPosition(xPos, yPos, zPos);
	//}

	//if (_keyStates[VK_BACK]) {
	//	xPos = 0.0f;
	//	yPos = 1.0f;
	//	zPos = 3.0f;
	//	_currentScene->GetCamera().SetPosition(xPos, yPos, zPos);
	//}
}