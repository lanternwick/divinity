
#include "CEngine.h"

Core* CEngine::_ECore = Core::Instance();
Graphics* CEngine::_EGraphics = Graphics::Instance(); 
BOOL CEngine::running = false;

CEngine::CEngine()
{
	// get Core and Graphics sub system components
	_ECore = Core::Instance();
	_EGraphics = Graphics::Instance();
}

CEngine::~CEngine()
{
	// delete the sub system components
	delete _ECore;
	delete _EGraphics;
}

Core* CEngine::ECore()
{
	return _ECore; // get the Core sub system
}

Graphics* CEngine::EGraphics()
{
	return _EGraphics; //get the Graphics sub system
}

VOID CEngine::bRunning(BOOL b)
{
	running = b; // is the application running?
}

BOOL CEngine::bRunning()
{
	return running; // get running state
}
