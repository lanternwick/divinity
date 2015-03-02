
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DSCRIPT_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DSCRIPT_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifndef _DSCRIPT_H
#define _DSCRIPT_H

#ifdef DSCRIPT_EXPORTS
#define DSCRIPT_API __declspec(dllexport)
#else
#define DSCRIPT_API __declspec(dllimport)
#endif



#include <vector>
#include <result.h>
#include <cstack.h>

typedef VOID (IntFunction)(CStack<DWORD>* Stack);

RESULT DSCRIPT_API InitDScript();
RESULT DSCRIPT_API DestroyDScript();
RESULT DSCRIPT_API SetInterrupt(WORD ID, IntFunction* newFunc);
RESULT DSCRIPT_API LoadScript(HANDLE* scr, char* file);
RESULT DSCRIPT_API StartScript(HANDLE script);
RESULT DSCRIPT_API ResetScript(HANDLE script);
RESULT DSCRIPT_API StopScript(HANDLE script);
RESULT DSCRIPT_API UpdateScripts();
RESULT DSCRIPT_API RunScript(HANDLE script);
RESULT DSCRIPT_API CallScriptFunction(HANDLE script, char* function);

#endif