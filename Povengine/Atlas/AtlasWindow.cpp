#include "AtlasWindow.h"


using namespace Atlas;

AltasWindow::AltasWindow(AtlasManager* parent)
	: _parent(parent), _mouseCaptured(false)
{

}

AltasWindow::~AltasWindow() 
{

}

void AltasWindow::setCaptureMouse(bool enable) 
{
	_mouseCaptured = enable; 
}

