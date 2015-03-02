
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the UIDEMO_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// UIDEMO_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef UIDEMO_EXPORTS
#define UIDEMO_API __declspec(dllexport)
#else
#define UIDEMO_API __declspec(dllimport)
#endif

#include <result.h>

RESULT UIDEMO_API InitMod();
RESULT UIDEMO_API RenderScene();
RESULT UIDEMO_API Update();
RESULT UIDEMO_API DestroyMod();