
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the D3DDEV_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// D3DDEV_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef _EXPORTS
#define _API __declspec(dllexport)
#else
#define _API __declspec(dllimport)
#endif

#include "D3DGraphObjFactory.h"
#include "D3DDevice.h"

_API IDevice* GetDevice();
_API IGraphObjFactory* GetFactory();

