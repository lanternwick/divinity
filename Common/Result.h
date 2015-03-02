#ifndef _RESULT_H
#define _RESULT_H

#include <windows.h>

enum RESULT
{
	OK = 0,
	OBJECTINVALID,
	INVALIDHANDLE,
	VBNOTCREATED,
	APINONE,
	OUTOFMEMORY,
	NOTIMPL,
	NOEQUIV,
	INVALID_TRANSFORM,
	BADMODULE,
	PACK_NOT_OPEN,
	NO_FILES_TO_EXTRACT,
	FAILED_OPEN,
	FILE_DOES_NOT_EXIST,
	IMPOSSIBLE,
	INVALIDDATA,
	INVALIDEMITTER,
	NO_INTERSECTION, 
	DESTROY,
	END_STATE,

	E_UNKNOWN,

	RESULT_FORCE_DWORD = 0x7fffffff,
};

#define RFAILED(r) (r != OK ? TRUE : FALSE)

void ResultText(char* str, RESULT r);

void ResultDescription(char* str, RESULT r);


inline RESULT HRtoR(HRESULT hr)
{
	switch(hr)
	{
	case S_OK:
		return OK;
	default:
		return E_UNKNOWN;
	}
}

#endif
