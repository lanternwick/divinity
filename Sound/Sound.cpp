// Sound.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#include "..\System\system.h"

#include "Sound.h"
#include "CSegment.h"


IDirectMusicLoader8* Loader = NULL;
IDirectMusicPerformance8* Performance = NULL;
IDirectMusicComposer8* Composer = NULL;
CSegment* Playing = NULL;
HMODULE me;

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
				RegisterModule((HMODULE)hModule);
				me = (HMODULE)hModule;
			}
			OutputDebugString("SOUND: :====> EXIT: DLLMAIN() : Process Attach\n\n");
			break;
		case DLL_THREAD_ATTACH: break;
		case DLL_THREAD_DETACH: break;
		case DLL_PROCESS_DETACH:
			OutputDebugString("SOUND: :====> ENTER: DLLMAIN() : Process Detach\n");
			OutputDebugString("SOUND: :====> EXIT: DLLMAIN() : Process Detach\n");
			break;
    }
    return TRUE;
}

RESULT SOUND_API InitSound()
{
	OutputDebugString("SOUND: InitSound()\n");
	CoInitialize(NULL);
	CoCreateInstance(CLSID_DirectMusicLoader, NULL, CLSCTX_INPROC, IID_IDirectMusicLoader8, (VOID**)&Loader);
	CoCreateInstance(CLSID_DirectMusicPerformance, NULL, CLSCTX_INPROC, IID_IDirectMusicPerformance8, (VOID**)&Performance);
	CoCreateInstance(CLSID_DirectMusicComposer, NULL, CLSCTX_INPROC, IID_IDirectMusicComposer8, (VOID**)&Composer);
	if(Loader == NULL)
		return OUTOFMEMORY;
	if(Performance == NULL)
		return OUTOFMEMORY;
	if(Composer == NULL)
		return OUTOFMEMORY;

	Performance->InitAudio(NULL, NULL, NULL, DMUS_APATH_DYNAMIC_STEREO, 64, DMUS_AUDIOF_ALL, NULL);
	return OK;
}

RESULT SOUND_API LoadSegment(HANDLE* Segment, char* Filename)
{
	CSegment* tmpSeg = NULL;
	tmpSeg = new CSegment;
	if(tmpSeg == NULL)
		return OUTOFMEMORY;

	WCHAR wcharFilename[256];
	MultiByteToWideChar(CP_ACP, 0, Filename, -1, wcharFilename, 256);
	HRESULT hr = Loader->LoadObjectFromFile(CLSID_DirectMusicSegment, 
		IID_IDirectMusicSegment8, 
		wcharFilename, 
		(VOID**)&tmpSeg->Segment);
	int i = 0;
	switch(hr)
	{
	case E_FAIL:
		i++;
		break;
	case E_INVALIDARG:
		i++;
		break;
	case E_OUTOFMEMORY:
		i++;
		break;
	case E_POINTER:
		i++;
		break;
	case DMUS_E_LOADER_NOCLASSID:
		i++;
		break;
	case DMUS_E_LOADER_FAILEDOPEN:
		i++;
		break;
	case DMUS_E_LOADER_FAILEDCREATE:
		i++;
		break;
	case DMUS_E_LOADER_FORMATNOTSUPPORTED:
		i++;
		break;
	case REGDB_E_CLASSNOTREG:
		i++;
		break;
	}
	*Segment = Register(tmpSeg);
	return HRtoR(hr);
}

RESULT SOUND_API PlaySegmentAsMusic(HANDLE Segment)
{
	CSegment* Seg = NULL;
	RESULT r = GetObjectPointer(me, Segment, (CObject**)&Seg);
	if(RFAILED(r))
		return r;

	Seg->Segment->Download(Performance);
	if(Playing == NULL)
	{
		Performance->PlaySegment(Seg->Segment, NULL, 0, NULL);
		Playing = Seg;
		return OK;
	}
	else
	{
		
		Performance->PlaySegment(Seg->Segment, NULL, 0, NULL);
		Playing = Seg;
		return OK;
	}
}

RESULT SOUND_API PlaySegmentAsSndFX(HANDLE Segment)
{
	CSegment* Seg = NULL;
	RESULT r = GetObjectPointer(me, Segment, (CObject**)&Seg);
	if(RFAILED(r))
		return r;

	Seg->Segment->Download(Performance);
	Performance->PlaySegment(Seg->Segment, DMUS_SEGF_SECONDARY, 0, NULL);
	return OK;
}
	
	
	

