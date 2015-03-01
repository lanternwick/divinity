#ifndef _TIMESOFWAR_MOD_H
#define _TIMESOFWAR_MOD_H

#pragma warning (disable:4231)

#include "..\Common\IMod.h"
#include "..\DivEngine\DivEngine.h"
#include "..\Common\CSmartPtr.h"
#include "..\Terrain\Terrain.h"

class TimesOfWar_Mod : public IMod
{
public:
	TimesOfWar_Mod();
	~TimesOfWar_Mod();
	
	INT Run(char* cmdLine);

	RESULT DrawFrame(CFrame* frameToRender);
	RESULT UpdateFrame(CScene* sceneToUpdate);

	RESULT PreCache(char* file);






protected:
	CEngine* engine;
	CSmartPtr<IDevice> myDevice;


};

#endif
