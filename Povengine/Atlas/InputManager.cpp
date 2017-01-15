#include "InputManager.h"

using namespace Atlas;

InputManager::InputManager()
	: BaseManager()
{
	for (int i = 0; i < 256; i++) {
		_keyStates[i] = false;
	}

	_initialised = true;
}

/// <summary>
/// Handles a key press (key up or down) by setting that key's state
/// </summary>
/// <param name="keyID">ID of the key to set/unset</param>
/// <param name="isDown">bool indicating if the new key state is down or up</param>
void InputManager::HandleKeyPress(unsigned int keyID, bool isDown)
{
	if (keyID >= 256) {
		return;
	}

	_keyStates[keyID] = isDown;
}

/// <summary>
///	Get the pressed state of the given key
/// </summary>
/// <param name="keyID">ID of the key to set/unset</param>
/// <param name="isDown">bool indicating if the new key state is down or up</param>
/// <returns>true if key is pressed, otherwise false. False if invalid key</returns>
bool InputManager::IsKeyPressed(unsigned int keyID)
{
	if (keyID >= 256) {
		return false;
	}

	return _keyStates[keyID];
}