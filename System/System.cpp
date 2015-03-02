// System.cpp : Defines the entry point for the DLL application.
//

#include <assert.h>
#include "stdafx.h"
#include <fvf.h>
#include "System.h"
								

#include <memmanager.h>

MemManager mem;

// overload new and delete to keep track of allocations and de-allocations

/*inline void* __cdecl operator new(size_t size)
{
	return mem.GimmeMem(size);
}

inline void __cdecl operator delete(void* p)
{
	mem.ReleaseMem(p);
	return;
}*/



#include <d3dx8.h>
#include <fstream>
#include <SYSDevice.h>


#include <CObject.cpp>
#include <CObject.h>
#include <queue2.cpp>
#include <CTexture.h>
#include <CVertexBuffer.h>
#include <CIndexBuffer.h>
#include <vector>
#include <CStack.h>
#include <vector.h>
#include <matrix.h>
#include "postoffice.h"
#include <cfont.h>
#include <clog.h>

// basic OutputDebugString() log object
CLog* Log = NULL;
// Object to handle and log messages
PostOffice* Messager = NULL;

struct GLOBALSETTINGS
{
	BOOL AllowText;
	BOOL Paused;

};


struct VIDSETTINGS
{
	INT		SizeX;
	INT		SizeY;
	BOOL	Windowed;
	API		api;
};

D3DXVECTOR3 VECTORtoD3DXVECTOR(VECTOR& v)
// conversion function...
{
	D3DXVECTOR3 v2;
	v2.x = v.x;
	v2.y = v.y;
	v2.z = v.z;
	return v2;
}

D3DMATRIX MATRIXtoD3DMATRIX(MATRIX& m)
// another conversion function....
{
	D3DMATRIX m2;
	memcpy(&m2, &m._11, sizeof(MATRIX));
	return m2;
}

MATRIX D3DMATRIXtoMATRIX(D3DMATRIX& m)
{
	MATRIX m2;
	memcpy(&m2, &m._11, sizeof(MATRIX));
	return m2;
}

D3DMATERIAL8 MATERIALtoD3DMATERIAL(MATERIAL m)
{
	D3DMATERIAL8 m2;
	memcpy(&m2, &m, sizeof(m));
	return m2;
}

D3DXPLANE PLANEtoD3DXPLANE(PLANE p)
{
	D3DXPLANE p2;
	p2.a = p.a;
	p2.b = p.b;
	p2.c = p.c;
	p2.d = p.d;

	return p2;
}

char ucase(char c) 
{ 
	return ((c >= 'a') && (c <= 'z') ? c - ('a' - 'A') : c); 
}


struct VIDSETTINGS video = { 0, 0 };
GLOBALSETTINGS settings;
// rendering device
SYSDevice* Device = NULL;

// object store...
std::vector<CObject*> Objects;
// magic numbers, indices match Object's above...
std::vector<WORD> magic_numbers;
// matrix store...
std::vector<MATRIX*> Matrices;
// registed modules allowed to use GetObjectPointer()
std::vector<HMODULE> RegisteredModules;
// info about loaded objects, so we dont load two of the same.
std::vector<char*> CachedObjects;
// same as above, for fonts
std::vector<char*> LoadedFonts;
// font store...
std::vector<CFont*> Fonts;

// render stack, to order rendering.
CStack<HANDLE> RenderStack;

// selected texture....
CTexture* SelectedTexture = NULL;
// selected VB...
CVertexBuffer* SelectedVB = NULL;
// current selected Object...
CObject* SelectedObj = NULL;
// current font...
CFont* SelectedFont = NULL;
// currently selected Index Buffer
CIndexBuffer* SelectedIB = NULL;

// view stuff, projection, view matrices, camera pos and look at vectors.
D3DXMATRIX D3DProjectionMatrix;
D3DXMATRIX D3DViewMatrix;
VECTOR CameraPos;
VECTOR CameraTarget;

// current material
D3DMATERIAL8 D3DMaterial;

// number of ticks since start, number of frames since start, number of
// primitives drawn this frame
DWORD Ticks = 0;
DWORD Time = 0;
DWORD lastTickTime = 0;
DWORD Frames = 0;
DWORD numPrims = 0;

HMODULE mod; // this DLL

char* myPath = NULL;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{

	mod = (HMODULE)hModule;
	if(!Log)
		Log = new CLog;
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
				OutputLogString("SYSTEM: :====> ENTER: DLLMAIN() : Process Attach\n");
				{
					RegisterModule(mod);
				}
				OutputLogString("SYSTEM: :====> EXIT: DLLMAIN() : Process Attach\n\n");
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			OutputLogString("SYSTEM: :====> ENTER: DLLMAIN() : Process Detach\n");
			OutputLogString("SYSTEM: :====> EXIT: DLLMAIN() : Process Detach\n\n");
			break;
    }

    return TRUE;
}

inline BOOL CheckHandle(HANDLE Obj)
// utility function to make sure we dont use an invalid object
{
	WORD m_number = HIWORD(Obj);
	WORD index = LOWORD(Obj);
	if(index >= Objects.size())
		return FALSE;
	return ((Objects[index] != NULL) && (magic_numbers[index] == m_number)) ? TRUE : FALSE; 
}

RESULT SYSTEM_API InitSystem()
// starts up the System
{
#ifdef DEBUG
	OutputLogString("SYSTEM: InitSystem()\n");
#endif
	
	if(!Log)  //create new log object
		Log = new CLog;
	strcpy(mem.owner, "SYSTEM.DLL");  // set the memory manager's owner...
	Device = new SYSDevice; // new device object...
	if(Device == NULL)
		return OUTOFMEMORY;
	Messager = new PostOffice; // new post office object, for messages
	Messager->m = mod; // allow post office to use GetObjectPointer()
	CObject* obj = new CObject; 
	Objects.resize(1);
	magic_numbers.resize(1);
	CachedObjects.resize(1);
	Objects[0] = obj;
	obj->me = 0;
	magic_numbers[0] = 0;
	CachedObjects[0] = new char[16];
	strcpy(CachedObjects[0], "ManagerObject");
	myPath = new char[512];
	return OK;
}

RESULT SYSTEM_API DestroySystem()
// frees all outstanding memory
{
#ifdef DEBUG
	OutputLogString("SYSTEM: Destroy()\n");
	OutputLogString("{");
	OutputLogString("\n");
#endif
	DWORD numNULL = 0;
	for(INT i = 0; i < Objects.size(); ++i)
	{
		if(Objects[i] != NULL)
			DeleteObj(Objects[i]->me);  // delete objects
		else
			numNULL++;
	}

	for(i = 0; i < Fonts.size(); ++i)
	{
		if(Fonts[i] != NULL)
			DeleteFont(Fonts[i]->me);  // delete fonts
	}

	for(i = 0; i < Matrices.size(); ++i)
	{
		if(Matrices[i] != NULL)
		{
			delete Matrices[i]; // delete matrices
		}
	}
	delete myPath;
	if(Device) Device->Cleanup();  // cleanup the device
	delete Device;
#ifdef DEBUG
	OutputLogString("}");
	OutputLogString("\n");
#endif
	return OK;
}

RESULT SYSTEM_API InitDevice(HWND hWnd)
// Sets up a device using hWnd
{
#ifdef DEBUG
	OutputLogString("SYSTEM: InitDevice()\n");
#endif
	if(Device == NULL)
		return E_UNKNOWN;
	Device->SelectAPI(video.api); // select the rendering API to use
	D3DMATERIAL8 mat;  // create default material
	ZeroMemory(&mat, sizeof(mat));
	mat.Ambient.a = 1.0f;
	mat.Ambient.r = 1.0f;
	mat.Ambient.g = 1.0f;
	mat.Ambient.b = 1.0f;

	mat.Diffuse.a = 1.0f;
	mat.Diffuse.r = 1.0f;
	mat.Diffuse.g = 1.0f;
	mat.Diffuse.b = 1.0f;
	
	// create the device.
	RESULT r = Device->Create((HWND)hWnd, video.Windowed, video.SizeX, video.SizeY);
	if(r == OK)
		Device->GetD3DDevice()->Device()->SetMaterial(&mat);
	return r;
}

RESULT SYSTEM_API LoadSettings()
{	
#ifdef DEBUG
	OutputLogString("SYSTEM: LoadSettings()\n");
#endif
	INT api;
	//Load Config File
	std::ifstream settings("default.cfg");
	//Input video settings
		//input video x size
		settings >> video.SizeX;
		//input video y size
		settings >> video.SizeY;
		//input video windowed mode
		settings >> video.Windowed;
		//input video api
		settings >> INT(api);
		video.api = (API)api;
	//close settings file
	settings.close();
	//return
	return OK;
}

RESULT SYSTEM_API GetSetting(SETTINGS type, DWORD* data)
// returns a setting....
{
	switch(type)
	{
	case SET_SIZEX:
		*data = video.SizeX;
		return OK;
	case SET_SIZEY:
		*data = video.SizeY;
		return OK;
	case SET_WINDOWED:
		*data = video.Windowed;
		return OK;
	case SET_API:
		*data = video.api;
		return OK;
	default:
		return E_UNKNOWN;
	}
}

RESULT SYSTEM_API SaveSystemSettings(char* filename)
{
#ifdef DEBUG
	OutputLogString("SYSTEM: SaveSettings()\n");
#endif
	//Load Config File
	std::ofstream settings(filename);
	//Output video settings
		//ouput video x size
		settings << video.SizeX << std::endl;
		//output video y size
		settings << video.SizeY << std::endl;
		//output video windowed mode
		settings << video.Windowed << std::endl;
		//output video api
		settings << video.api << std::endl;
	//close settings file
	settings.close();
	//return
	return OK;
}

RESULT SYSTEM_API BeginScene()
{
	// begin rendering
	Device->BeginScene();
	return OK;
}

RESULT SYSTEM_API EndScene()
{
	//render the objects in the stack...
	
	// end the scene
	Device->EndScene();
	return OK;
}

RESULT SYSTEM_API Flip()
{
	// draw stuff to the screen
	OutputLogString("Flipping...\n");
	Device->Flip();
	
	return OK;
}

RESULT SYSTEM_API Clear(DWORD Color)
{
	// clear the window with Color
	Device->Clear(Color);
	return OK;
}



inline BOOL CheckMatrixHandle(HANDLE m)
// same as above, except with Matrices
{
	WORD index = LOWORD(m);
	if(index >= Matrices.size())
		return FALSE;
	return (Matrices[index] != NULL);
}

HANDLE SYSTEM_API Register(CObject* Obj)
// Register an Object in the system, and give it a handle.
{
#ifdef DEBUG
	OutputLogString("SYSTEM: Register()\n");
#endif
	INT i;
	HANDLE tmpHandle;
	if(Obj->QueryObject() != OBJ_DFONT)  // if its not a font object...
	{
		for(i = 1; i < Objects.size(); ++i)
		{
			if(Objects[i] == NULL) break; // find a blank spot to put it in
		}
		if(i == Objects.size()) // if there arent any blank spots...
		{
			Objects.resize(i+1); // ...make a new one...
			CachedObjects.resize(i+1); // same as above...
			magic_numbers.resize(i+1); // same as above...
		}
		Objects[i] = Obj; // put the object in the vector
		//Obj = NULL;
		magic_numbers[i] = rand(); // get a new magic number
		tmpHandle = (HANDLE)MAKELONG(i, magic_numbers[i]); // make a handle
		Obj->me = tmpHandle; // tell the Object that that is it's handle
	}
	else // its a font
	{
		for(i = 0; i < Fonts.size(); ++i)
		{
			if(Fonts[i] == NULL) break; // find a blank spot
		}
		if(i == Fonts.size()) // if there arent any blank spots...
		{
			Fonts.resize(i+1); // .. make a new one...
			LoadedFonts.resize(i+1); //same as above...
		}
		Fonts[i] = (CFont*)Obj;  // put the font into the vector...
		//Obj = NULL;
		tmpHandle = (HANDLE)MAKELONG(i, 1); // make a handle
		Obj->me = tmpHandle; // tell the object it's handle
	}
#ifdef DEBUG
	// do some output stuff, for logging
	char a[10];
	char b[100];
	itoa(i, a, 10);
	sprintf(b, "Registered %s as # %i", Obj->QueryObject() != OBJ_DFONT ? "Object" : "Font", i);
	OutputLogString(b);
	OutputLogString("\n");

#endif
	return tmpHandle; // return the handle
}

RESULT SYSTEM_API SelectObject(HANDLE Obj)
// selects an object to be used for a future call
{
	if(!CheckHandle(Obj)) // make sure the handle is valid
		return INVALIDHANDLE;

	CObject* tmp = Objects[LOWORD(Obj)]; // get a pointer to the object
	OBJ_TYPES type = tmp->QueryObject(); // get its type
	switch(type)
	{
	case OBJ_VERTEXBUFFER:  // if iots a vertex buffer..
		SelectedVB = (CVertexBuffer*)tmp;  // select it
		return OK;
	case OBJ_TEXTURE: // if its a texture...
		SelectedTexture = (CTexture*)tmp; // select it...
		SelectedTexture->Set(Device);  // and set it... 
		return OK;
	case OBJ_INDEXBUFFER: // if its an IB...
		SelectedIB = (CIndexBuffer*)tmp; // select it
		return OK;
	default: // else
		SelectedObj = tmp; // select it.
		return OK;
	}
	return IMPOSSIBLE;  // duh
}

RESULT SYSTEM_API CreateVertexBuffer(HANDLE* VB, UINT size, UINT VertSize, DWORD Usage, DWORD FVF)
// creates a new vertex buffer
{
	CVertexBuffer* tmpVB = NULL;
	tmpVB = new CVertexBuffer;  // new vertex buffer
	// create it.  Takes in a SYSDevice, total length, 
	// size of one vertex, a usage flag, and an FVF
	RESULT r = tmpVB->Create(Device, size, VertSize, Usage, FVF); 
	if(RFAILED(r))
	{
		delete tmpVB;
		return r;
	}
	*VB = Register(tmpVB);  // register it
	sprintf(tmpVB->Name, "VertexBuffer%i%l", size, FVF); // set the name...
	GenUniqueID(&(tmpVB->Name), *VB); // Generate a Unique ID
	OutputLogString(tmpVB->Name); OutputLogString("\n"); // print the UID
	return OK;
}

RESULT SYSTEM_API LockVertexBuffer(INT OffsetToLock, INT SizeToLock, BYTE** ptr, DWORD Flags)
// lock a vertex buffer
{
	return SelectedVB->Lock(OffsetToLock, SizeToLock, ptr, Flags);
}

RESULT SYSTEM_API UnlockVertexBuffer()
// unlock a vertex buffer
{
	return SelectedVB->Unlock();
}

RESULT SYSTEM_API CreateIndexBuffer(HANDLE* IB, UINT size, DWORD Flags)
// creats a new index buffer
{
	CIndexBuffer* tmpIB = NULL;
	tmpIB = new CIndexBuffer; // new IB
	if(tmpIB == NULL)
		return OUTOFMEMORY;
	tmpIB->Create(Device, size, Flags);
	*IB = Register(tmpIB);
	sprintf(tmpIB->Name, "IndexBuffer%i", size); // set the name...
	GenUniqueID(&(tmpIB->Name), *IB); // Generate a Unique ID
	OutputLogString(tmpIB->Name); OutputLogString("\n"); // print the UID
	return OK;
}

RESULT SYSTEM_API LockIndexBuffer(INT OffsetToLock, INT SizeToLock, BYTE** ptr, DWORD Flags)
// lock a vertex buffer
{
	return SelectedIB->Lock(OffsetToLock, SizeToLock, ptr, Flags);
}

RESULT SYSTEM_API UnlockIndexBuffer()
// unlock a vertex buffer
{
	return SelectedIB->Unlock();
}


RESULT SYSTEM_API MatrixProjection(float farClip, float FOV)
// create a projection matrix
{
	switch(video.api)
	{
	case API_D3D:
		D3DXMatrixPerspectiveLH(&D3DProjectionMatrix, FOV, 1.0f, 1.0f, farClip);
		return OK;
	case API_OGL:
		return NOTIMPL;
	default:
		return APINONE;
	}
}


RESULT SYSTEM_API MatrixLookAt(VECTOR* Pos, VECTOR* Target)
{
	D3DXVECTOR3 D3Dpos, D3Dtarg;
	CameraPos = *Pos;
	CameraTarget = *Target;
	switch(video.api)
	{
	case API_D3D:
		D3Dpos = VECTORtoD3DXVECTOR(*Pos);
		D3Dtarg = VECTORtoD3DXVECTOR(*Target);
		D3DXMatrixLookAtLH(&D3DViewMatrix, &D3Dpos, &D3Dtarg, &D3DXVECTOR3(0, 1, 0));
		return OK;
	case API_OGL:
		return NOTIMPL;
	default:
		return APINONE;
	}
}

RESULT SYSTEM_API SetProjection()
{
	switch(video.api)
	{
	case API_D3D:
		Device->GetD3DDevice()->Device()->SetTransform(D3DTS_PROJECTION, &D3DProjectionMatrix);
		return OK;
	case API_OGL:
		return NOTIMPL;
	default:
		return APINONE;
	}
}

RESULT SYSTEM_API SetView()
{
	switch(video.api)
	{
	case API_D3D:
		Device->GetD3DDevice()->Device()->SetTransform(D3DTS_VIEW, &D3DViewMatrix);
		return OK;
	case API_OGL:
		return NOTIMPL;
	default:
		return APINONE;
	}
}

RESULT SYSTEM_API DrawTriFan(UINT startVert, UINT numPrims)
{
	if(SelectedVB == NULL)
		return OBJECTINVALID;
	SelectedVB->Set(Device);
	return Device->DrawPrim(PT_TRIFAN, startVert, numPrims);
}

RESULT SYSTEM_API DrawTriList(UINT startVert, UINT numPrims)
{
	if(SelectedVB == NULL)
		return OBJECTINVALID;
	SelectedVB->Set(Device);
	return Device->DrawPrim(PT_TRILIST, startVert, numPrims);
}

RESULT SYSTEM_API DrawTriStrip(UINT startVert, UINT numPrims)
{
	if(SelectedVB == NULL)
		return OBJECTINVALID;
	SelectedVB->Set(Device);
	return Device->DrawPrim(PT_TRISTRIP, startVert, numPrims);
}

RESULT SYSTEM_API DrawPoints(UINT startVert, UINT numPrims)
{
	if(SelectedVB == NULL)
		return OBJECTINVALID;
	SelectedVB->Set(Device);
	return Device->DrawPrim(PT_POINTS, startVert, numPrims);
}

RESULT SYSTEM_API DrawLineList(UINT startVert, UINT numPrims)
{
	if(SelectedVB == NULL)
		return OBJECTINVALID;
	SelectedVB->Set(Device);
	return Device->DrawPrim(PT_LINELIST, startVert, numPrims);
}

RESULT SYSTEM_API DrawIndexedTriFan(UINT minIndex, UINT numVerts, UINT startIndex, UINT numPrims)
{
	if(SelectedVB == NULL)
		return OBJECTINVALID;
	if(SelectedIB == NULL)
		return OBJECTINVALID;
	SelectedVB->Set(Device);
	SelectedIB->Set(Device);
	return Device->DrawIndexedPrim(PT_TRIFAN, minIndex, numVerts, startIndex, numPrims);
}

RESULT SYSTEM_API DrawIndexedTriList(UINT minIndex, UINT numVerts, UINT startIndex, UINT numPrims)
{
	if(SelectedVB == NULL)
		return OBJECTINVALID;
	if(SelectedIB == NULL)
		return OBJECTINVALID;
	SelectedVB->Set(Device);
	SelectedIB->Set(Device);
	return Device->DrawIndexedPrim(PT_TRILIST, minIndex, numVerts, startIndex, numPrims);
}

RESULT SYSTEM_API DrawIndexedTriStrip(UINT minIndex, UINT numVerts, UINT startIndex, UINT numPrims)
{
	if(SelectedVB == NULL)
		return OBJECTINVALID;
	if(SelectedIB == NULL)
		return OBJECTINVALID;
	SelectedVB->Set(Device);
	SelectedIB->Set(Device);
	return Device->DrawIndexedPrim(PT_TRISTRIP, minIndex, numVerts, startIndex, numPrims);
}

HANDLE SYSTEM_API CreateMatrix()
{
	HANDLE m;
	for(INT i = 0; (i < Matrices.size()) && (Matrices[i] != NULL); ++i);
	if(i == Matrices.size())
	{
		Matrices.resize(i+1);
	}
	Matrices[i] = new MATRIX;
	m = (VOID*)MAKELONG(i, 0);
	return m;
}

RESULT SYSTEM_API MatrixIdentity(HANDLE r)
{
	if(!CheckMatrixHandle(r))
		return INVALIDHANDLE;

	MatrixMatrixIdentity(&Matrices[LOWORD(r)]);
	return OK;
}

RESULT SYSTEM_API MatrixMultiply(HANDLE r, HANDLE a, HANDLE b)
{
	if(!CheckMatrixHandle(r))
		return INVALIDHANDLE;
	if(!CheckMatrixHandle(a))
		return INVALIDHANDLE;
	if(!CheckMatrixHandle(b))
		return INVALIDHANDLE;

	MatrixMatrixMultiply(Matrices[LOWORD(r)], Matrices[LOWORD(a)], Matrices[LOWORD(b)]);
	return OK;
}

RESULT SYSTEM_API MatrixTranslation(HANDLE r, float x, float y, float z)
{
	if(!CheckMatrixHandle(r))
		return INVALIDHANDLE;

	MatrixMatrixTranslation(Matrices[LOWORD(r)], x, y, z);
	return OK;
}

RESULT SYSTEM_API MatrixScaling(HANDLE r, float x, float y, float z)
{
	if(!CheckMatrixHandle(r))
		return INVALIDHANDLE;

	MatrixMatrixScaling(Matrices[LOWORD(r)], x, y, z);
	return OK;
}

RESULT SYSTEM_API MatrixRotationX(HANDLE r, float t)
{
	if(!CheckMatrixHandle(r))
		return INVALIDHANDLE;

	MatrixMatrixRotationX(Matrices[LOWORD(r)], t);
	return OK;
}

RESULT SYSTEM_API MatrixRotationY(HANDLE r, float t)
{
	if(!CheckMatrixHandle(r))
		return INVALIDHANDLE;

	MatrixMatrixRotationY(Matrices[LOWORD(r)], t);
	return OK;
}

RESULT SYSTEM_API MatrixRotationZ(HANDLE r, float t)
{
	if(!CheckMatrixHandle(r))
		return INVALIDHANDLE;

	MatrixMatrixRotationZ(Matrices[LOWORD(r)], t);
	return OK;
}

RESULT SYSTEM_API MatrixTranspose(HANDLE* result, HANDLE a)
{
	if(!CheckMatrixHandle(a))
		return INVALIDHANDLE;
	if(!CheckMatrixHandle(*result))
		*result = CreateMatrix();
	MatrixMatrixTranspose(Matrices[LOWORD(*result)], Matrices[LOWORD(a)]);
	return OK;
}

RESULT SYSTEM_API MatrixBillboard(HANDLE* result, HANDLE a, float x, float y, float z)
{
	if(!CheckMatrixHandle(a))
		return INVALIDHANDLE;
	if(!CheckMatrixHandle(*result))
		*result = CreateMatrix();
	MatrixMatrixBillboardTranspose(Matrices[LOWORD(*result)], Matrices[LOWORD(a)], x, y, z);
	return OK;
}

RESULT SYSTEM_API MatrixVectorApply(VECTOR* r, HANDLE m, VECTOR* v)
{
	if(!CheckMatrixHandle(m))
		return INVALIDHANDLE;
	MatrixMatrixVectorApply(r, Matrices[LOWORD(m)], v);
	return OK;
}

RESULT SYSTEM_API LoadViewMatrix(HANDLE m)
{
	if(!CheckMatrixHandle(m))
		return INVALIDHANDLE;
	*Matrices[LOWORD(m)] = D3DMATRIXtoMATRIX(D3DViewMatrix);
	return OK;
}

RESULT SYSTEM_API SetMatrix(TRANSFORMATION type, HANDLE m)
{
	if(!CheckMatrixHandle(m))
		return INVALIDHANDLE;
	D3DMATRIX mat;
	switch(type)
	{
	case T_WORLD:
		switch(video.api)
		{
		case API_D3D:
			mat = MATRIXtoD3DMATRIX(*Matrices[LOWORD(m)]);
			Device->GetD3DDevice()->Device()->SetTransform(D3DTS_WORLD, &mat);
			return OK;
		case API_OGL:
			return NOTIMPL;
		default:
			return APINONE;
		}

	default:
		return INVALID_TRANSFORM;
	}
}

RESULT SYSTEM_API GetCameraPos(VECTOR* ret)
{
	*ret = CameraPos;
	return OK;
}

RESULT SYSTEM_API SetRenderState(DWORD rs, DWORD val)
{
	return Device->SetRenderState(rs, val);
}

RESULT SYSTEM_API GetRenderState(DWORD rs, DWORD* val)
{
	switch(video.api)
	{
	case API_D3D:
		return HRtoR(Device->GetD3DDevice()->Device()->GetRenderState((D3DRENDERSTATETYPE)rs, val));
	case API_OGL:
		return NOTIMPL;
	default:
		return APINONE;
	}
}

RESULT SYSTEM_API DeleteObj(HANDLE Obj)
{
	
	if(!CheckHandle(Obj))
		return INVALIDHANDLE;
	if(Obj == 0)
		return INVALIDHANDLE;
#ifdef _DEBUG
	char a[10];
	sprintf(a, "%i", LOWORD(Obj));
	OutputLogString("SYSTEM: DeleteObj() :  Deleting Obj # ");
	OutputLogString(a);
	OutputLogString("\n");
#endif
	delete Objects[LOWORD(Obj)];
	delete CachedObjects[LOWORD(Obj)];
	CachedObjects[LOWORD(Obj)] = NULL;
	Objects[LOWORD(Obj)] = NULL;
	return OK;
}

RESULT SYSTEM_API DeleteMatrix(HANDLE r)
{
	if(!CheckMatrixHandle(r))
		return INVALIDHANDLE;
	
	delete Matrices[LOWORD(r)];
	Matrices[LOWORD(r)] = NULL;
	return OK;
}


RESULT SYSTEM_API SendObjectMessage(HANDLE src, HANDLE dest, UINT Msg, DWORD param, DWORD param2, DWORD param3)
{
	if(!CheckHandle(dest))
		return INVALIDHANDLE;
	if(!CheckHandle(src))
		return INVALIDHANDLE;
	DMSG msg;
	msg.Message = Msg;
	msg.param1 = param;
	msg.param2 = param2;
	msg.param3 = param3;
	Messager->SendMessage(src, dest, msg);
	return OK;
}

RESULT SYSTEM_API SendDelayedMessage(HANDLE src, HANDLE dest, DWORD timeToDeliver, UINT Msg, DWORD param, DWORD param2, DWORD param3)
{
	if(!CheckHandle(dest))
		return INVALIDHANDLE;
	if(!CheckHandle(src))
		return INVALIDHANDLE;
	DMSG msg;
	msg.Message = Msg;
	msg.param1 = param;
	msg.param2 = param2;
	msg.param3 = param3;
	Messager->SendDelayedMessage(src, dest, msg, timeToDeliver);
	return OK;
}

RESULT SYSTEM_API SetChild(HANDLE Parent, HANDLE newChild)
{
	CObject* p = NULL;
	CObject* Child = NULL;
	GetObjectPointer(mod, Parent, OBJ_ANY, &p);
	GetObjectPointer(mod, newChild, OBJ_ANY, &Child);
	if((!p) || (!Child))
		return INVALIDHANDLE;
	Child->Sibling = p->FirstChild;
	p->FirstChild = Child;
	Child->Parent = p;
	return OK;
}

RESULT SYSTEM_API LoadTexture(char* file, HANDLE* tex)
{
	CTexture* t = NULL;
	for(int i = 0; i < CachedObjects.size(); ++i)
	{
		if((CachedObjects[i]) && (strcmp(file, CachedObjects[i]) == 0))
		{
			if(Objects[i] != NULL)
				*tex = Objects[i]->me;
			return OK;
		}
	}
	t = new CTexture;
	if(t == NULL)
		return OUTOFMEMORY;
	t->LoadTexture(Device, file);
	*tex = Register(t);
	CachedObjects[LOWORD(*tex)] = new char[128];
	strcpy(CachedObjects[LOWORD(*tex)], file);
	strcpy(t->Name, file);
	GenUniqueID(&(t->Name), *tex);
	OutputLogString(t->Name); OutputLogString("\n");
	return OK;
}

RESULT SYSTEM_API LoadTextureEx(char* file, DWORD transColor, HANDLE* tex)
{
	CTexture* t = NULL;
	for(int i = 0; i < CachedObjects.size(); ++i)
	{
		if(CachedObjects[i] == NULL)
			continue;
		if(strcmp(file, CachedObjects[i]) == 0)
		{
			if(Objects[i] != NULL)
				*tex = Objects[i]->me;
			return OK;
		}
	}
	t = new CTexture;
	if(t == NULL)
		return OUTOFMEMORY;
	t->LoadTextureEx(Device, file, transColor);
	*tex = Register(t);
	CachedObjects[LOWORD(*tex)] = new char[128];
	strcpy(CachedObjects[LOWORD(*tex)], file);
	strcpy(t->Name, file);
	GenUniqueID(&(t->Name), *tex);
	OutputLogString(t->Name); OutputLogString("\n");
	return OK;
}

RESULT SYSTEM_API SelectTexture(HANDLE Obj)
{
	if(!CheckHandle(Obj))
		return INVALIDHANDLE;

	CObject* tmp = Objects[LOWORD(Obj)];
	if(tmp->QueryObject() != OBJ_TEXTURE)
		return OBJECTINVALID;

	SelectedTexture = (CTexture*)tmp;
	return OK;
}

RESULT SYSTEM_API RenderObject(HANDLE Obj)
{
	if(CheckHandle(Obj))
	{
		Objects[LOWORD(Obj)]->Render(numPrims);
	}
	return OK;
}

RESULT SYSTEM_API RegisterModule(HMODULE Mod)
{
	char ModName[125];
	
	GetModuleFileName(Mod, ModName, 125);
	int lenFile = strlen(ModName);
	char Filename[64];
	INT j = 0;
	for(INT i = lenFile-1; (i > 0) && (ModName[i] != '\\'); --i);
	//assert(i > 0);
	++i;
	for( ; i < lenFile; ++i, ++j) {
		Filename[j] = ucase(ModName[i]); }
	Filename[j] = '\0';
	OutputLogString(Filename);
	OutputLogString("\n");
	if(!strcmp("GAME.DLL", Filename))
		return BADMODULE;
	if(strcmp(".DLL", &Filename[j-4]))
		return BADMODULE;

	RegisteredModules.push_back(Mod);
	OutputLogString("Module "), OutputLogString(Filename), OutputLogString(" Registered\n");
	return OK;
}

RESULT SYSTEM_API GetObjectPointer(HMODULE ModID, HANDLE ObjID, OBJ_TYPES type, CObject** Obj)
{
	CObject* c;
	*Obj = NULL;
	for(INT i = 0; (i < RegisteredModules.size()) && (RegisteredModules[i] != ModID); ++i);
	if(i == RegisteredModules.size())
		return BADMODULE;
	if(!CheckHandle(ObjID))
	{
		if((Fonts.size() > LOWORD(ObjID)) && (Fonts[LOWORD(ObjID)] != NULL))
		{
			*Obj = Fonts[LOWORD(ObjID)];
			return OK;
		}
		return INVALIDHANDLE;
	}
	else
	{
		c = Objects[LOWORD(ObjID)];
		if(((c->QueryObject() & type) != type) && (type != OBJ_ANY))
			return OBJECTINVALID;
		*Obj = c;
	}
	return OK;
}

RESULT SYSTEM_API UpdateObjects()
{
	CObject* c;
	Messager->Update();
	for(INT i = 0; i < Objects.size(); ++i)
	{
		if((c = Objects[i]) != NULL)
		{

			if(c->ShouldDelete())
			{
#ifdef _DEBUG
				OutputLogString("SYSTEM: UpdateObjects() : AutoDeleting Object...\n");
#endif
				DeleteObj(c->me);
			}
			else
			{
				if(!settings.Paused)
				{
					SendObjectMessage(NULL, c->me, DM_UPDATETICK, 0, 0, 0);
					c->Update();
				}
			}
		}
	}
	return OK;
}

DWORD SYSTEM_API GetTicks()
{
	return Ticks;
}

VOID SYSTEM_API GameTick()
{
	lastTickTime = Time;
	Time = GetTickCount();
	++Ticks;
	return;
}

RESULT SYSTEM_API SetNullObject(OBJ_TYPES type)
{
	switch(type)
	{
	case OBJ_VERTEXBUFFER:
		SelectedVB = NULL;
		return OK;
	case OBJ_TEXTURE:
		if(SelectedTexture) SelectedTexture->MakeNull(Device);
		return OK;
	case OBJ_DFONT:
		SelectedFont = NULL;
		return OK;
	default:
		SelectedObj = NULL;
		return OK;
	}
	return IMPOSSIBLE;
}

DWORD SYSTEM_API GetFrames()
{
	return Frames;
}

VOID SYSTEM_API FrameTick()
{
	++Frames;
	numPrims = 0;
}

float SYSTEM_API TimeDelta()
{
	return (Time - lastTickTime) / 1000.0f;
}

float SYSTEM_API GetFPS()
{
	return 1000.0f / (Time - lastTickTime == 0 ? 1 : Time - lastTickTime);
}

LPVOID SYSTEM_API GimmeMem(DWORD size)
{
//#ifdef _DEBUG
	//return mem.GimmeMem(size);
//#else
	return new BYTE[size];
//#endif
}

VOID SYSTEM_API FreeMem(VOID* p)
{
//#ifdef _DEBUG
//	mem.ReleaseMem(p);
//#else
	delete p;
//#endif
	return;
}

RESULT SYSTEM_API LoadFont(char* fontname, INT size, INT bold, BOOL italic, BOOL underline, HANDLE* font)
{
	CFont* tmp = NULL;
	char* tmpName = new char[128];
	if(fontname[strlen(fontname)-1] == '\n')
		fontname[strlen(fontname)-1] = 0;
	sprintf(tmpName, "%s%i%s%s%s", fontname, size, bold ? "b" : "n", italic ? "i" : "n", underline ? "u" : "n");
	for(INT i = 0; i < LoadedFonts.size(); ++i)
	{
		if(strcmp(tmpName, LoadedFonts[i]) == 0)
		{
			*font = Fonts[i]->me;
			return OK;
		}
	}
	tmp = new CFont;
	tmp->LoadFont(Device, fontname, size, bold, italic, underline);
	*font = Register((CObject*)tmp);
	LoadedFonts[LOWORD(*font)] = new char[128];
	strcpy(LoadedFonts[LOWORD(*font)], tmpName);
	strcpy(tmp->Name, tmpName);
	GenUniqueID(&(tmp->Name), *font);
	OutputLogString(tmp->Name); OutputLogString("\n");
	return OK;
}

RESULT SYSTEM_API SelectFont(HANDLE font)
{
	if(Fonts.size() <= LOWORD(font))
		return INVALIDHANDLE;
	if(Fonts[LOWORD(font)] == NULL)
		return INVALIDHANDLE;
	SelectedFont = Fonts[LOWORD(font)];
	return OK;
}

RESULT SYSTEM_API DeleteFont(HANDLE font)
{
	if(Fonts.size() <= LOWORD(font))
		return INVALIDHANDLE;
	if(Fonts[LOWORD(font)] == NULL)
		return INVALIDHANDLE;

	delete Fonts[LOWORD(font)];
	delete LoadedFonts[LOWORD(font)];
	LoadedFonts[LOWORD(font)] = NULL;
	Fonts[LOWORD(font)] = NULL;
	return OK;
}

RESULT SYSTEM_API DrawTextOut(char* text, RECT fontRect, DWORD format, DWORD color)
{
	if(!SelectedFont)
		return INVALIDDATA;
	if(!settings.AllowText)
		return OK;
	return SelectedFont->DrawTextOut(text, fontRect, format, color);
}

RESULT SYSTEM_API OutputLogString(char* lpDebugStr)
{
	static char tabs[10];
	static int n = 0;
	char buff[256];
	if(Log)
	{
		if(strcmp(lpDebugStr, "}") == 0)
		{
			tabs[n-1] = 0;
			--n;
		}
		sprintf(buff, "%s%s", tabs, lpDebugStr);
		Log->OutputData(buff);
		if(strcmp(lpDebugStr, "{") == 0)
		{
			tabs[n] = '\t';
			tabs[n+1] = 0;
			++n;
		}
		
	}
	return OK;
}

RESULT SYSTEM_API GenUniqueID(char** result, HANDLE Obj)
{
	CObject* ref;
	RESULT r = GetObjectPointer(mod, Obj, OBJ_ANY,  &ref);
	if(RFAILED(r))
	{
		return r;
	}
	char* UName = new char[128];
	char* tmpBuff = new char[128];
	

	ObjText(ref->QueryObject(), &tmpBuff);
	sprintf(UName, "%s%s%d", tmpBuff, ref->Name, (DWORD)Obj);
	strcpy(ref->Name, UName);
	FreeMem(UName);
	FreeMem(tmpBuff);
	return OK;
}

RESULT SYSTEM_API AddLogRepresentation(CLog* newRep)
{
	if(Log)
	{
		newRep->NextRep = Log->NextRep;
		Log->NextRep = newRep;
	}
	else
	{
		Log = newRep;
	}
	return OK;
}
	
RESULT SYSTEM_API SetObjTimeout(HANDLE obj, DWORD time)
{
	CObject* Obj = NULL;
	RESULT r = GetObjectPointer(mod, obj, OBJ_ANY, &Obj);
	if(RFAILED(r))
		return r;
	Obj->SetTimeout(time);
	return OK;
}

DWORD SYSTEM_API GetDrawnPrimitives()
{
	return numPrims;
}

RESULT SYSTEM_API GetParentHandle(HANDLE Obj, HANDLE* Parent)
{
	CObject* c;
	GetObjectPointer(mod, Obj, OBJ_ANY, &c);
	if(c == NULL)
		return INVALIDHANDLE;
	*Parent = c->Parent->me;
	return OK;
}

RESULT SYSTEM_API GetRootHandle(HANDLE Obj, HANDLE* Root)
{
	CObject* c;
	GetObjectPointer(mod, Obj, OBJ_ANY, &c);
	if(c == NULL)
		return INVALIDHANDLE;
	while(c->Parent)
	{
		c = c->Parent;
	}
	*Root = c->me;
	return OK;
}

RESULT SYSTEM_API GetExtraData(HANDLE Obj, DWORD* extra)
{
	CObject* c;
	GetObjectPointer(mod, Obj, OBJ_ANY, &c);
	if(c == NULL)
		return INVALIDHANDLE;
	*extra = c->extra;
	return OK;
}

RESULT SYSTEM_API SetExtraData(HANDLE Obj, DWORD extra)
{
	CObject* c;
	GetObjectPointer(mod, Obj, OBJ_ANY, &c);
	if(c == NULL)
		return INVALIDHANDLE;
	c->extra = extra;
	return OK;
}

RESULT SYSTEM_API GetObjectName(HANDLE Obj, char** name)
{
	CObject* c;
	GetObjectPointer(mod, Obj, OBJ_ANY, &c);
	if(c == NULL)
		return INVALIDHANDLE;
	strcpy(*name, c->Name);
	return OK;
}

#define SET(x, y, z) if(strcmp(x, #y) == 0) z.##y = !z.##y
#define SET2(x, y, z, a) if(strcmp(x, #y) == 0) z.##y = a

RESULT SYSTEM_API Set(char* setting, BOOL b)
{
	//if(strcmp(setting, "DrawText") == 0)
	//	settings.DrawText = !settings.DrawText;
	SET2(setting, AllowText, settings, b);
	SET2(setting, Paused, settings, b);

	return OK;
}

#undef SET
#undef SET2

RESULT SYSTEM_API SetMaterial(MATERIAL mat)
{
	D3DMaterial = MATERIALtoD3DMATERIAL(mat);
	// unsafe behavior!!!
	Device->GetD3DDevice()->Device()->SetMaterial(&D3DMaterial);
	return OK;
}

RESULT SYSTEM_API Unproject(VECTOR* r, VECTOR* a)
{
	D3DVIEWPORT8 vp;
	D3DXVECTOR3 vr;
	D3DXMATRIX world;
	
	switch(video.api)
	{
	case API_D3D:
		D3DXMatrixIdentity(&world);
		Device->GetD3DDevice()->Device()->GetViewport(&vp);
		D3DXVec3Unproject(&vr, &D3DXVECTOR3(a->x, a->y, a->z), &vp, &D3DProjectionMatrix, &D3DViewMatrix, &world);
		r->x = vr.x;
		r->y = vr.y;
		r->z = vr.z;
		return OK;


	case API_OGL:
		return NOTIMPL;
	default:
		return APINONE;
	}
}

RESULT SYSTEM_API GetPath(char** path)
{
	strcpy(*path, myPath);
	OutputLogString(myPath);
	OutputLogString("\n");
	OutputLogString(*path);
	OutputLogString("\n");
	return OK;
}

RESULT SYSTEM_API SetPath(const char* path)
{
	
	strcpy(myPath, path);
	OutputLogString(myPath);
	OutputLogString("\n");
	return OK;
}