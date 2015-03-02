
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SYSTEM_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SYSTEM_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifndef _SYSTEM_H
#define _SYSTEM_H

#ifdef SYSTEM_EXPORTS
#define SYSTEM_API __declspec(dllexport)
#else
#define SYSTEM_API __declspec(dllimport)
#endif

typedef VOID (MessageProc)(HANDLE hWnd, HANDLE source, UINT msg, DWORD param1, DWORD param2, DWORD param3);

enum TRANSFORMATION
{
	T_WORLD = 0,

	T_FORCE_DWORD = 0x79999999,
};

enum SETTINGS
{
	SET_SIZEX = 0,
	SET_SIZEY,
	SET_WINDOWED,
	SET_API,

	SETINGS_FORCE_WORD = 0x79999999,
};

LPVOID SYSTEM_API GimmeMem(DWORD size);
VOID   SYSTEM_API FreeMem(VOID* p);

#ifndef SYSTEM_EXPORTS
inline void* __cdecl operator new(size_t size)
{
	return GimmeMem(size);
}

inline void __cdecl operator delete(void* p)
{
	FreeMem(p);
	return;
}
#endif

#include <result.h>
#include <CObject.h>
#include <VECTOR.h>
#include <CLog.h>
#include <fvf.h>

RESULT SYSTEM_API InitSystem();
RESULT SYSTEM_API DestroySystem();
RESULT SYSTEM_API InitDevice(HWND hWnd);
RESULT SYSTEM_API LoadSettings();
RESULT SYSTEM_API GetSetting(SETTINGS type, DWORD* data);
RESULT SYSTEM_API SaveSystemSettings(char* filename="default.cfg");
RESULT SYSTEM_API BeginScene();
RESULT SYSTEM_API EndScene();
RESULT SYSTEM_API Flip();
RESULT SYSTEM_API Clear(DWORD Color);
HANDLE SYSTEM_API Register(CObject* Obj);
RESULT SYSTEM_API SelectObject(HANDLE Obj);
RESULT SYSTEM_API CreateVertexBuffer(HANDLE* VB, UINT size, UINT VertSize, DWORD Usage, DWORD FVF);
RESULT SYSTEM_API LockVertexBuffer(INT OffsetToLock, INT SizeToLock, BYTE** ptr, DWORD Flags);
RESULT SYSTEM_API UnlockVertexBuffer();
RESULT SYSTEM_API CreateIndexBuffer(HANDLE* IB, UINT size, DWORD Flags);
RESULT SYSTEM_API LockIndexBuffer(INT OffsetToLock, INT SizeToLock, BYTE** ptr, DWORD Flags);
RESULT SYSTEM_API UnlockIndexBuffer();
RESULT SYSTEM_API MatrixProjection(float farClip, float FOV);
RESULT SYSTEM_API MatrixLookAt(VECTOR* Pos, VECTOR* Target);
RESULT SYSTEM_API SetProjection();
RESULT SYSTEM_API SetView();
RESULT SYSTEM_API DrawTriFan(UINT startVert, UINT numPrims);
RESULT SYSTEM_API DrawTriList(UINT startVert, UINT numPrims);
RESULT SYSTEM_API DrawTriStrip(UINT startVert, UINT numPrims);
RESULT SYSTEM_API DrawPoints(UINT startVert, UINT numPrims);
RESULT SYSTEM_API DrawLineList(UINT startVert, UINT numPrims);
RESULT SYSTEM_API DrawIndexedTriFan(UINT minIndex, UINT numVerts, UINT startIndex, UINT numPrims);
RESULT SYSTEM_API DrawIndexedTriList(UINT minIndex, UINT numVerts, UINT startIndex, UINT numPrims);
RESULT SYSTEM_API DrawIndexedTriStrip(UINT minIndex, UINT numVerts, UINT startIndex, UINT numPrims);
HANDLE SYSTEM_API CreateMatrix();
RESULT SYSTEM_API MatrixIdentity(HANDLE r);
RESULT SYSTEM_API MatrixMultiply(HANDLE r, HANDLE a, HANDLE b);
RESULT SYSTEM_API MatrixTranslation(HANDLE r, float x, float y, float z);
RESULT SYSTEM_API MatrixScaling(HANDLE r, float x, float y, float z);
RESULT SYSTEM_API MatrixRotationX(HANDLE r, float t);
RESULT SYSTEM_API MatrixRotationY(HANDLE r, float t);
RESULT SYSTEM_API MatrixRotationZ(HANDLE r, float t);
RESULT SYSTEM_API MatrixTranspose(HANDLE* result, HANDLE a);
RESULT SYSTEM_API MatrixBillboard(HANDLE* result, HANDLE a, float x, float y, float z);
RESULT SYSTEM_API MatrixVectorApply(VECTOR* r, HANDLE m, VECTOR* v);
RESULT SYSTEM_API LoadViewMatrix(HANDLE m);
RESULT SYSTEM_API SetMatrix(TRANSFORMATION type, HANDLE r);
RESULT SYSTEM_API GetCameraPos(VECTOR* ret);
RESULT SYSTEM_API SetRenderState(DWORD rs, DWORD val);
RESULT SYSTEM_API GetRenderState(DWORD rs, DWORD* val);
RESULT SYSTEM_API DeleteObj(HANDLE Obj);
RESULT SYSTEM_API DeleteMatrix(HANDLE r);
RESULT SYSTEM_API SendObjectMessage(HANDLE src, HANDLE Obj, UINT Msg, DWORD param, DWORD param2, DWORD param3);
RESULT SYSTEM_API SendDelayedMessage(HANDLE src, HANDLE Obj, DWORD timeToDeliver, UINT Msg, DWORD param, DWORD param2, DWORD param3);
RESULT SYSTEM_API RenderObject(HANDLE Obj);
RESULT SYSTEM_API SetChild(HANDLE Parent, HANDLE newChild);
RESULT SYSTEM_API LoadTexture(char* file, HANDLE* tex);
RESULT SYSTEM_API LoadTextureEx(char* file, DWORD transColor, HANDLE* tex);
RESULT SYSTEM_API SelectTexture(HANDLE Obj);
RESULT SYSTEM_API RegisterModule(HMODULE Mod);
RESULT SYSTEM_API GetObjectPointer(HMODULE ModID, HANDLE ObjID, OBJ_TYPES type, CObject** Obj);
RESULT SYSTEM_API UpdateObjects();
DWORD  SYSTEM_API GetTicks();
VOID   SYSTEM_API GameTick();
RESULT SYSTEM_API SetNullObject(OBJ_TYPES type);
DWORD  SYSTEM_API GetFrames();
VOID   SYSTEM_API FrameTick();
float  SYSTEM_API TimeDelta();
float  SYSTEM_API GetFPS();
RESULT SYSTEM_API LoadFont(char* fontname, INT size, INT bold, BOOL italic, BOOL underline, HANDLE* font);
RESULT SYSTEM_API SelectFont(HANDLE font);
RESULT SYSTEM_API DeleteFont(HANDLE font);
RESULT SYSTEM_API DrawTextOut(char* text, RECT fontRect, DWORD format, DWORD color);
RESULT SYSTEM_API OutputLogString(char* lpDebugStr);
RESULT SYSTEM_API GenUniqueID(char** result, HANDLE Obj);
RESULT SYSTEM_API AddLogRepresentation(CLog* newRep);
RESULT SYSTEM_API SetObjTimeout(HANDLE obj, DWORD time);
DWORD  SYSTEM_API GetDrawnPrimitives();
RESULT SYSTEM_API GetParentHandle(HANDLE Obj, HANDLE* Parent);
RESULT SYSTEM_API GetRootHandle(HANDLE Obj, HANDLE* Root);
RESULT SYSTEM_API GetExtraData(HANDLE Obj, DWORD* extra);
RESULT SYSTEM_API SetExtraData(HANDLE Obj, DWORD extra);
RESULT SYSTEM_API GetObjectName(HANDLE Obj, char** name);
RESULT SYSTEM_API Set(char* setting, BOOL b);
RESULT SYSTEM_API SetMaterial(MATERIAL mat);
RESULT SYSTEM_API Unproject(VECTOR* r, VECTOR* a);
RESULT SYSTEM_API GetPath(char** path);
RESULT SYSTEM_API SetPath(const char* path);

#endif