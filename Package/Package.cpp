// Package.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "package.h"
#include "classPackage.h"

Package* thisPack = NULL;



BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			OutputDebugString("SOUND: :====> ENTER: DLLMAIN() : Process Attach\n");
			{
				thisPack = new Package;
			}
			OutputDebugString("SOUND: :====> EXIT: DLLMAIN() : Process Attach\n");
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}


RESULT PACKAGE_API OpenPackage(char* file)
{
	return thisPack->Open(file);
}

RESULT PACKAGE_API NewPackage(char* file)
{
	return thisPack->New(file);
}

RESULT PACKAGE_API AddFileToPackage(char* file, char* dir)
{
	return thisPack->AddFile(file, dir);
}

RESULT PACKAGE_API ExtractFile(char* to, UINT id)
{
	return thisPack->Extract(to, id);
}

RESULT PACKAGE_API DeleteFileFromPackage(UINT id)
{
	return thisPack->Delete(id);
}

RESULT PACKAGE_API ExtractFileByName(char* to, char* name)
{
	PACKDESC pd;
	char* f;
	thisPack->GetDesc(&pd);
	f = pd.filenames;

	for(INT i = 0; UINT(i) < pd.numFiles; ++i, f+=strlen(f)+1)
	{
		if(strcmp((char*)name, f) == 0)
			break;
	}
	if(UINT(i) == pd.numFiles) return FILE_DOES_NOT_EXIST;
	return thisPack->Extract((char*)to, i-1);
}

RESULT PACKAGE_API DeleteFileByName(char* name)
{
	PACKDESC pd;
	char* f;
	thisPack->GetDesc(&pd);
	f = pd.filenames;

	for(INT i = 0; UINT(i) < pd.numFiles; ++i, f+=strlen(f)+1)
	{
		if(strcmp((char*)name, f) == 0)
			break;
	}
	if(UINT(i) == pd.numFiles) return FILE_DOES_NOT_EXIST;
	return thisPack->Delete(i-1);
}
	
RESULT PACKAGE_API ClosePackage()
{
	return thisPack->Close();
}

BOOL PACKAGE_API PackageIsOpen()
{
	return thisPack->Open();
}

DWORD PACKAGE_API EnumFiles(EnumFunc* funct)
{
	PACKDESC pd;
	char* f;
	DWORD* dw;
	thisPack->GetDesc(&pd);
	f = pd.filenames;
	dw = pd.filesizes;

	for(UINT i = 0; i < pd.numFiles; ++i, f+=strlen(f)+1, dw+=4)
	{
		if(!((*funct)(f, *dw)))
			break;
	}
	return i;
}

RESULT PACKAGE_API GetNumFiles(DWORD* numFiles)
{
	PACKDESC pd;
	thisPack->GetDesc(&pd);
	*numFiles = pd.numFiles;
	return OK;
}

RESULT PACKAGE_API ExtractAllFiles(char* dir)
{
	PACKDESC pd;
	thisPack->GetDesc(&pd);
	char path[256];
	for(UINT i = 0; i < pd.numFiles; ++i, pd.filesizes++, pd.filenames+=strlen(pd.filenames)+1)
	{
		sprintf(path, "%s\\%s", dir, pd.filenames);
		thisPack->Extract(path, i);
	}
	return OK;
}