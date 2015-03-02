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

	E_UNKNOWN,

	RESULT_FORCE_DWORD = 0x7fffffff,
};

#define RFAILED(r) (r != OK ? TRUE : FALSE)

void ResultText(char* str, RESULT r)
{
	if(str == NULL) return;
	str[0] = NULL;
	switch(r)
	{
	case OK:
		strcpy(str, "OK");
		return;
	case OBJECTINVALID:
		strcpy(str, "OBJECTINVALID");
		return;
	case INVALIDHANDLE:
		strcpy(str, "INVALIDHANDLE");
		return;
	case VBNOTCREATED:
		strcpy(str, "VBNOTCREATED");
		return;
	case APINONE:
		strcpy(str, "APINONE");
		return;
	case OUTOFMEMORY:
		strcpy(str, "OUTOFMEMORY");
		return;
	case NOTIMPL:
		strcpy(str, "NOTIMPL");
		return;
	case NOEQUIV:
		strcpy(str, "NOEQUIV");
		return;
	case INVALID_TRANSFORM:
		strcpy(str, "INVALID_TRANSFORM");
		return;
	case BADMODULE:
		strcpy(str, "BADMODULE");
		return;
	case PACK_NOT_OPEN:
		strcpy(str, "PACK_NOT_OPEN");
		return;
	case NO_FILES_TO_EXTRACT:
		strcpy(str, "NO_FILES_TO_EXTRACT");
		return;
	case FAILED_OPEN:
		strcpy(str, "FAILED_OPEN");
		return;
	case FILE_DOES_NOT_EXIST:
		strcpy(str, "FILE_DOES_NOT_EXIST");
		return;
	case IMPOSSIBLE:
		strcpy(str, "IMPOSSIBLE");
		return;
	case INVALIDDATA:
		strcpy(str, "INVALIDDATA");
		return;
	case INVALIDEMITTER:
		strcpy(str, "INVALIDEMITTER");
		return;
	case NO_INTERSECTION:
		strcpy(str, "NO_INTERSECTION");
		return;
	case E_UNKNOWN:
		strcpy(str, "E_UNKNOWN");
		return;
	}
	return;
}

void ResultDescription(char* str, RESULT r)
{
	if(str == NULL) return;
	str[0] = NULL;
	switch(r)
	{
	case OK:
		strcpy(str, "OK");
		return;
	case OBJECTINVALID:
		strcpy(str, "Invalid Object Pointer");
		return;
	case INVALIDHANDLE:
		strcpy(str, "Invalid Object Handle");
		return;
	case VBNOTCREATED:
		strcpy(str, "Vertex Buffer not created and cannot be used");
		return;
	case APINONE:
		strcpy(str, "Rendering API not Selected");
		return;
	case OUTOFMEMORY:
		strcpy(str, "Out Of Memory");
		return;
	case NOTIMPL:
		strcpy(str, "Method or Function not yet Implemented");
		return;
	case NOEQUIV:
		strcpy(str, "No Equivelent Function or method");
		return;
	case INVALID_TRANSFORM:
		strcpy(str, "Invalid Transformation Specified");
		return;
	case BADMODULE:
		strcpy(str, "Bad Module Handle");
		return;
	case PACK_NOT_OPEN:
		strcpy(str, "Package File not Open");
		return;
	case NO_FILES_TO_EXTRACT:
		strcpy(str, "No Files exist in package");
		return;
	case FAILED_OPEN:
		strcpy(str, "File Open Failed for unknown reason");
		return;
	case FILE_DOES_NOT_EXIST:
		strcpy(str, "File specified does not exist");
		return;
	case IMPOSSIBLE:
		strcpy(str, "This should never return.  It is Impossible  =)");
		return;
	case INVALIDDATA:
		strcpy(str, "Invalid Data passed to a function");
		return;
	case INVALIDEMITTER:
		strcpy(str, "Particle System Dimension does not match Emitter Dimension");
		return;
	case NO_INTERSECTION:
		strcpy(str, "No Windows were intersected");
		return;
	case E_UNKNOWN:
		strcpy(str, "An Unknown Error has occured");
		return;
	}
	return;
}

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
