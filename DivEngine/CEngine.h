#ifndef CENGINE_H
#define CENGINE_H

#include "..\DivCore\DivCore.h"
#include "..\DivGraph\DivGraph.h"
#include "..\DivUserInterface\DivUserInterface.h"

class _API CEngine
{
public:
	CEngine();
	~CEngine();

	static Core* ECore();
	static Graphics* EGraphics();
	static VOID bRunning(BOOL b);
	static BOOL bRunning();
private:
	static BOOL running;
	static Core* _ECore;
	static Graphics* _EGraphics;
	
};

#endif