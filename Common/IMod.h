#ifndef _IMOD_H
#define _IMOD_H

#include "Singleton.h"
#include "Result.h"
#include "..\DivEngine\DivEngine.h"

class IMod
{
public:
	virtual ~IMod() {}

	void Set_hWnd(HWND hWnd) { this->hWnd = hWnd; }

	virtual INT Run(char* cmdLine)=0;

	virtual RESULT UpdateFrame(CScene* sceneToUpdate)=0;
	virtual RESULT DrawFrame(CFrame* frameToRender)=0;
protected:
	IMod(){}
	HWND hWnd;
};

#endif