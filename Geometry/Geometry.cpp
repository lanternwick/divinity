// Geometry.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "Geometry.h"

#include "..\system\system.h"

#include "IMesh.h"
#include "GEOStaticMesh.h"

HMODULE mod;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	mod = (HMODULE)hModule;
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			OutputLogString("GEOMETRY: :====> ENTER: DLLMAIN() : Process Attach\n\n");
			{
				RegisterModule((HMODULE)hModule);
			}
			OutputLogString("GEOMETRY: :====> EXIT: DLLMAIN() : Process Attach\n\n");
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

RESULT GEOMETRY_API InitGeometry()
{
	return OK;
}

RESULT GEOMETRY_API LoadModel(char* file, HANDLE* mesh)
{
	RESULT r = OK;
	GEOStaticMesh* tmpMesh = NULL;
	tmpMesh = new GEOStaticMesh;
	if(tmpMesh == NULL)
		return OUTOFMEMORY;
	r = tmpMesh->LoadMesh(file);
	if(RFAILED(r))
	{
		delete tmpMesh;
		tmpMesh = NULL;
		// create new skinned mesh, etc
		return NOTIMPL;
	}
	*mesh = Register(tmpMesh);
	return OK;
}