
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the TIMESOFWAR_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TIMESOFWAR_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

#pragma warning(disable:4786)

#ifdef TIMESOFWAR_EXPORTS
#define TIMESOFWAR_API __declspec(dllexport)
#else
#define TIMESOFWAR_API __declspec(dllimport)
#endif

//#include "..\DivEngine\DivEngine.h"

#include "TimesOfWar_Mod.h"


_API IMod* Game();
_API void DestroyGame();
_API dn_networking* Network();
_API void DestroyNet();