#include "D3DDevice.h"

static DWORD FtoDW(float f)
// utility function
{
	return *(DWORD*)&f;
}

D3DDevice::D3DDevice()
{
	Core::Function_Enter("D3DDevice::Constructor()");
	myDevice = NULL;
	Core::Function_Exit();
}

D3DDevice::~D3DDevice()
{
	Core::Function_Enter("D3DDevice::Destructor()");
	Cleanup();
	Core::Function_Exit();
}

HWND D3DDevice::hWnd() const
{
	// get the associtated HWND
	Core::Function_Enter("D3DDevice::hWnd()");
	HWND h = my_hWnd;
	Core::Function_Exit();
	return h;
}

RESULT D3DDevice::Create(HWND hWnd, BOOL Window, int cx, int cy, DWORD format, bool force_format)
//Create a Device or otherwise initialize the rendering API
{
	Core::Function_Enter("D3DDevice::Create()");
	LPDIRECT3D8 D3D = NULL;
	D3DVIEWPORT8 vp;
	ScreenX = cx;
	ScreenY = cy;
	// initialize D3D
	try
	{
		if(NULL == (D3D = Direct3DCreate8(D3D_SDK_VERSION)))
		{
			MessageBox(0, "Device Creation Failed!\nD3D Object creation Failed.\nMake sure you have DirectX 8.1 installed", "", MB_OK);
			throw E_UNKNOWN;
		}
		if(FAILED(D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
		{
			throw "Could not query Default video adapter!";
		}
		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.Windowed = Window;
		// if the device should be windowed
		if(!(Window))
		{
			// set back buffer height and width
			d3dpp.BackBufferWidth = cx;
			d3dpp.BackBufferHeight = cy;
			d3dpp.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
			d3dpp.BackBufferFormat = (D3DFORMAT)format;
		}
		// set up other device parameters
		d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
		if(force_format)
			d3dpp.BackBufferFormat = (D3DFORMAT)format;
		else
			d3dpp.BackBufferFormat = d3ddm.Format;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		d3dpp.EnableAutoDepthStencil = TRUE;

		// try to create a hardware device
		if(FAILED(D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
									D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
									&d3dpp, &myDevice)))
		{
			Core::WriteLog("Failed to create Hardware Device...\n", 1);
			// try to create a software device
			if(FAILED(D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
									D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
									&d3dpp, &myDevice)))
			{
				// cant create anything
				MessageBox(0, "Device Creation Failed!", "", MB_OK);
				throw E_UNKNOWN;
			}
		}
		my_hWnd = hWnd;

		memset(states, 0xffffffff, 256*4);
		memset(tstates, 0xffffffff, 28*4*8);

		// turn Z Buffer on
		SetRenderState(D3DRS_ZENABLE, TRUE);
		// turn lighting off
		myDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		// set up blending modes
		SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
		SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		
		// set texture modes
		/*myDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		myDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		myDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		
		
		myDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		myDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);*/
    
		myDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
		myDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);

		// release D3D
		D3D->Release();
	}
	catch(RESULT r)
	{
		Core::Function_Exit();
		return r;
	}

	myDevice->GetViewport(&vp);
	ScreenX = vp.Width;
	ScreenY = vp.Height;


	Core::Function_Exit();
	return OK;
}
	
RESULT D3DDevice::Reset(HWND hWnd, BOOL Window, int cx, int cy, DWORD format, bool force_format)
//Create a Device or otherwise initialize the rendering API
{
	Core::Function_Enter("D3DDevice::Reset()");
	//LPDIRECT3D8 D3D = NULL;
	D3DVIEWPORT8 vp;
	ScreenX = cx;
	ScreenY = cy;
	// initialize D3D
	try
	{
		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.Windowed = Window;
		// if the device should be windowed
		if(!(Window))
		{
			// set back buffer height and width
			d3dpp.BackBufferWidth = cx;
			d3dpp.BackBufferHeight = cy;
			d3dpp.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
			if(force_format)
				d3dpp.BackBufferFormat = (D3DFORMAT)format;
		}
		// set up other device parameters
		d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
		if(force_format)
			d3dpp.BackBufferFormat = (D3DFORMAT)format;
		else
			d3dpp.BackBufferFormat = d3ddm.Format;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		d3dpp.EnableAutoDepthStencil = TRUE;


		rset_iter i;
		std::vector<IMediaObject*> r_obj_copy(reset_objects);
		for(i = reset_objects.begin(); i != reset_objects.end(); i++)
			(*i)->Release_SaveData();

		// try to create a hardware device
		if(FAILED(myDevice->Reset(&d3dpp)))
		{
			Core::WriteLog("Reset Failed...\n", 1);
			//throw E_UNKNOWN;
		}
		for(i = r_obj_copy.begin(); i != r_obj_copy.end(); i++)
			(*i)->ResetDeviceObjects();
		my_hWnd = hWnd;

		memset(states, 0xffffffff, 256*4);
		memset(tstates, 0xffffffff, 28*4*8);

		// turn Z Buffer on
		SetRenderState(D3DRS_ZENABLE, TRUE);
		// turn lighting off
		myDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		// set up blending modes
		SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
		SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		
		// set texture modes
		/*myDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		myDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		myDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		
		
		myDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		myDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);*/
    
		myDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
		myDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);

	}
	catch(RESULT r)
	{
		Core::Function_Exit();
		return r;
	}

	myDevice->GetViewport(&vp);
	ScreenX = vp.Width;
	ScreenY = vp.Height;


	Core::Function_Exit();
	return OK;
}


VOID D3DDevice::Clear(DWORD Color)
//clear scene 
{
	Core::Function_Enter("D3DDevice::Clear()");
	myDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, Color, 1.0f, 0);
	Core::Function_Exit();
	return;
}

HRESULT D3DDevice::Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
//flip scene
{
	Core::Function_Enter("D3DDevice::Present()");
	HRESULT hr = myDevice->Present(NULL, NULL, NULL, NULL);
	Core::Function_Exit();
	return hr;
}

VOID D3DDevice::Flip()
{
	// flip the scene
	Core::Function_Enter("D3DDevice::Flip()");
	Present(NULL, NULL, NULL, NULL);
	Core::Function_Exit();
	return;
}

VOID D3DDevice::Cleanup()
{
	Core::Function_Enter("D3DDevice::Cleanup()");
	if(NULL != myDevice)
	{
		myDevice->SetStreamSource(0, NULL, 0);
		SetNullTexture(0);
		
		// release the device
		myDevice->Release();
		myDevice = NULL;
	}

	Core::Function_Exit();
}
	
RESULT D3DDevice::SetRenderState(DWORD rs, DWORD val)
// cache renderstate changes
{
	Core::Function_Enter("D3DDevice::SetRenderState()");
	HRESULT hr = S_OK;
	if(states[rs] != val) // if the state is different
	{
		states[rs] = val; // save this state
		// and update
		hr = myDevice->SetRenderState((D3DRENDERSTATETYPE)rs, val);
	}
	Core::Function_Exit();
	return HRtoR(hr);
}

RESULT D3DDevice::SetTextureState(DWORD stage, DWORD ts, DWORD val)
{
	Core::Function_Enter("D3DDevice::SetTextureState()");
	HRESULT hr = S_OK;
	if(tstates[stage][ts] != val) // if the state is different
	{
		tstates[stage][ts] = val; // save this state
		// and update
		hr = myDevice->SetTextureStageState(stage, (D3DTEXTURESTAGESTATETYPE)ts, val);
	}
	Core::Function_Exit();
	return HRtoR(hr);
}

RESULT D3DDevice::GetRenderState(DWORD rs, DWORD* val)
{
	Core::Function_Enter("D3DDevice::GetRenderState()");
	HRESULT hr = S_OK;
	hr = myDevice->GetRenderState((D3DRENDERSTATETYPE)rs, val);
	Core::Function_Exit();
	return HRtoR(hr);
}

RESULT D3DDevice::GetTextureState(DWORD stage, DWORD ts, DWORD* val)
{
	Core::Function_Enter("D3DDevice::GetTextureState()");
	HRESULT hr = S_OK;
	hr = myDevice->GetTextureStageState(stage, (D3DTEXTURESTAGESTATETYPE)ts, val);
	Core::Function_Exit();
	return HRtoR(hr);
}

RESULT D3DDevice::BeginScene()
{
	Core::Function_Enter("D3DDevice::BeginScene()");
	HRESULT hr = myDevice->BeginScene(); // begin scene
	Core::Function_Exit();
	return HRtoR(hr);
}

RESULT D3DDevice::EndScene()
{
	// end scene
	Core::Function_Enter("D3DDevice::EndScene()");
	HRESULT hr = myDevice->EndScene(); // begin scene
	Core::Function_Exit();
	return HRtoR(hr);
}

RESULT D3DDevice::SetLight(DWORD index, LIGHT* light)
{	
	// set a light with index
	Core::Function_Enter("D3DDevice::SetLight()");
	HRESULT hr = myDevice->SetLight(index, (const D3DLIGHT8*)light);
	Core::Function_Exit();
	return HRtoR(hr);
}

RESULT D3DDevice::LightEnable(DWORD index, BOOL enabled)
{
	//enable a light at index
	Core::Function_Enter("D3DDevice::LightEnable()");
	HRESULT hr = myDevice->LightEnable(index, enabled);
	Core::Function_Exit();
	return HRtoR(hr);
}

RESULT D3DDevice::DrawIndexedPrim(PRIM_TYPES pt, UINT minIndex, UINT startVert, UINT startIndex, UINT numPrims)
// draw an indexed primitive
{
	Core::Function_Enter("D3DDevice::DrawIndexedPrim()");
	RESULT r;
	switch(pt)
	{
	case PT_TRILIST: // draw a triangle list
		r = HRtoR(myDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, minIndex, startVert, startIndex, numPrims));
		break;

	case PT_TRISTRIP: // draw a triangle strip
		r = HRtoR(myDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, minIndex, startVert, startIndex, numPrims));
		break;
		
	case PT_TRIFAN: // draw a triangle fan
		r = HRtoR(myDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, minIndex, startVert, startIndex, numPrims));
		break;

	case PT_POINTS: // draw points
		r = IMPOSSIBLE;
		break;

	case PT_LINELIST:
		r = HRtoR(myDevice->DrawIndexedPrimitive(D3DPT_LINELIST, minIndex, startVert, startIndex, numPrims));
		break;

	case PT_LINESTRIP:
		r = HRtoR(myDevice->DrawIndexedPrimitive(D3DPT_LINESTRIP, minIndex, startVert, startIndex, numPrims));
		break;
	
	}
	Core::Function_Exit();
	return r;
	
}

RESULT D3DDevice::DrawPrim(PRIM_TYPES pt, UINT startVert, UINT numPrims)
// draw primitive
{
	Core::Function_Enter("D3DDevice::DrawPrim()");
	RESULT r;
	switch(pt)
	{
	case PT_TRILIST: // draw triangle list
		r = HRtoR(myDevice->DrawPrimitive(D3DPT_TRIANGLELIST, startVert, numPrims));
		break;
		
	case PT_TRISTRIP: // draw triangle strip
		r = HRtoR(myDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, startVert, numPrims));
		break;
		
	case PT_TRIFAN: // draw triangle fan
		r = HRtoR(myDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, startVert, numPrims));
		break;
		
	case PT_POINTS: // draw points
		r = HRtoR(myDevice->DrawPrimitive(D3DPT_POINTLIST, startVert, numPrims));
		break;
		
	case PT_LINELIST: // draw line list
		r = HRtoR(myDevice->DrawPrimitive(D3DPT_LINELIST, startVert, numPrims));
		break;

	case PT_LINESTRIP:
		r = HRtoR(myDevice->DrawPrimitive(D3DPT_LINELIST, startVert, numPrims));
		break;
	}
	
	Core::Function_Exit();
	return r;
}

RESULT D3DDevice::GetTransform(TRANSFORMATION t, MATRIX* m)
// get current transform
{
	Core::Function_Enter("D3DDevice::GetTransform()");
	DWORD trans;
	try
	{
		switch(t)
		{
		case T_WORLD: // get world transform
			trans = D3DTS_WORLD;
			break;
		case T_VIEW: // get view transform
			trans = D3DTS_VIEW;
			break;
		case T_PROJECTION: // get projection transformation
			trans = D3DTS_PROJECTION;
			break;
		default:
			throw INVALID_TRANSFORM;
		}
		// get current transform
		RESULT r = HRtoR(myDevice->GetTransform((D3DTRANSFORMSTATETYPE)trans, (D3DMATRIX*)m));
		Core::Function_Exit();
		return r;
	}
	catch(RESULT cr)
	{
		Core::Function_Exit();
		return cr;
	}
}

RESULT D3DDevice::SetTransform(TRANSFORMATION t, MATRIX* m)
// set a new transformation matrix
{
	Core::Function_Enter("D3DDevice::SetTransform()");
	DWORD trans;
	try
	{
		switch(t)
		{
		case T_WORLD: // set world transform
			trans = D3DTS_WORLD;
			break;
		case T_VIEW: // set view transform
			trans = D3DTS_VIEW;
			break;
		case T_PROJECTION: // set projection transformation
			trans = D3DTS_PROJECTION;
			break;
		default:
			throw INVALID_TRANSFORM;
		}
		// set current transform
		RESULT r = HRtoR(myDevice->SetTransform((D3DTRANSFORMSTATETYPE)trans, (D3DMATRIX*)m));
		Core::Function_Exit();
		return r;
	}
	catch(RESULT cr)
	{
		Core::Function_Exit();
		return cr;
	}
	
	
}

RESULT D3DDevice::CreateVertexBuffer(IMediaObject* boundObj, UINT numVerts, UINT VertSize, DWORD Usage, DWORD FVF, VOID** p_vertexBuffer)
{
	// create a vertex buffer
	Core::Function_Enter("D3DDevice::CreateVertexBuffer()");
	HRESULT hr;
	hr = myDevice->CreateVertexBuffer(numVerts * VertSize, Usage, FVF, D3DPOOL_DEFAULT, (LPDIRECT3DVERTEXBUFFER8*)p_vertexBuffer);
	if(boundObj)
	{
		if(std::find(reset_objects.begin(), reset_objects.end(), boundObj)==reset_objects.end())
			reset_objects.push_back(boundObj);
	}
	Core::Function_Exit();
	return HRtoR(hr);
}

RESULT D3DDevice::SetVertexShader(DWORD shader)
{
	Core::Function_Enter("D3DDevice::SetVertexShader()");
	// set vertex shader
	HRESULT hr = myDevice->SetVertexShader(shader);
	Core::Function_Exit();
	return HRtoR(hr);
}

RESULT D3DDevice::SetVertexBuffer(VOID* p_vertexBuffer, DWORD stride)
{
	Core::Function_Enter("D3DDevice::SetVertexBuffer()");
	// set the vertex buffer for rendering
	HRESULT hr = myDevice->SetStreamSource(0, (LPDIRECT3DVERTEXBUFFER8)p_vertexBuffer, stride);
	Core::Function_Exit();
	return HRtoR(hr);
}

RESULT D3DDevice::CreateIndexBuffer(IMediaObject* boundObj, UINT numIndices, DWORD Usage, BYTE type, VOID** p_indexBuffer)
{
	Core::Function_Enter("D3DDevice::CreateIndexBuffer()");
	// create an index buffer
	DWORD r_type = 0;
	DWORD sMul = 0;
	RESULT r;
	try
	{
		switch(type)
		{
		case 16: // if 16 bits 
			r_type = (DWORD)D3DFMT_INDEX16;
			sMul = 2;
			break;
		case 32: // if 32 bits
			r_type = (DWORD)D3DFMT_INDEX32;
			sMul = 4;
			break;
		default:
			throw INVALIDDATA;
		}
	}
	catch(RESULT cr)
	{
		Core::Function_Exit();
		return cr;
	}
	// create the buffer
	r = HRtoR(myDevice->CreateIndexBuffer(numIndices*sMul, Usage, (D3DFORMAT)r_type, D3DPOOL_DEFAULT, (LPDIRECT3DINDEXBUFFER8*)p_indexBuffer));
	if(boundObj)
	{
		if(std::find(reset_objects.begin(), reset_objects.end(), boundObj)==reset_objects.end())
			reset_objects.push_back(boundObj);
	}
	Core::Function_Exit();
	return r;
}

RESULT D3DDevice::SetIndices(VOID* p_indexBuffer, UINT startIndices)
{
	Core::Function_Enter("D3DDevice::SetIndices()");
	RESULT r = HRtoR(myDevice->SetIndices((LPDIRECT3DINDEXBUFFER8)p_indexBuffer, startIndices));
	Core::Function_Exit();
	return r;
}

RESULT D3DDevice::SetMaterial(MATERIAL* mat)
{
	// set material
	Core::Function_Enter("D3DDevice::SetMaterial()");
	RESULT r = HRtoR(myDevice->SetMaterial((D3DMATERIAL8*)mat));
	Core::Function_Exit();
	return r;
}

RESULT D3DDevice::SetNullTexture(unsigned long stage)
{
	// clear the texture
	Core::Function_Enter("D3DDevice::SetNullTexture()");
	RESULT r = HRtoR(myDevice->SetTexture(stage, NULL));
	Core::Function_Exit();
	return r;
}

RESULT D3DDevice::LoadTexture(const char* filename, VOID** p_texture)
{
	// load normal texture
	Core::Function_Enter("D3DDevice::LoadTexture()");
	//RESULT r = HRtoR(D3DXCreateTextureFromFile(myDevice, filename, (LPDIRECT3DTEXTURE8*)p_texture));
	Core::Function_Exit();
	return NOTIMPL;
}

RESULT D3DDevice::CreateTexture(IMediaObject* boundObj, unsigned int width, POOL pool, VOID** p_texture)
{
	Core::Function_Enter("D3DDevice::CreateTexture()");
	RESULT r = HRtoR(myDevice->CreateTexture(width, width, 0, 0, D3DFMT_A8R8G8B8, (D3DPOOL)pool, (LPDIRECT3DTEXTURE8*)p_texture));
	if(boundObj)
	{
		if(std::find(reset_objects.begin(), reset_objects.end(), boundObj)==reset_objects.end())
			reset_objects.push_back(boundObj);
	}
	Core::Function_Exit();
	return r;
}

RESULT D3DDevice::UpdateTexture(VOID* src, VOID* dest)
{
	Core::Function_Enter("D3DDevice::UpdateTwexture()");
	RESULT r = HRtoR(myDevice->UpdateTexture((LPDIRECT3DTEXTURE8)src, (LPDIRECT3DTEXTURE8)dest));
	Core::Function_Exit();
	return r;
}

RESULT D3DDevice::LoadTextureEx(const char* filename, DWORD transColor, VOID** p_texture)
{
	// load texture with trans color
	Core::Function_Enter("D3DDevice::LoadTextureEx()");
	/*RESULT r = HRtoR(D3DXCreateTextureFromFileEx(myDevice, filename, D3DX_DEFAULT, D3DX_DEFAULT,
											 0, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, 
											 D3DX_FILTER_NONE, D3DX_FILTER_NONE, transColor, NULL, NULL, 
											 (LPDIRECT3DTEXTURE8*)p_texture));*/
	Core::Function_Exit();
	return NOTIMPL;
}

RESULT D3DDevice::SetTexture(DWORD stage, VOID* p_texture)
{
	// set the texture
	Core::Function_Enter("D3DDevice::SetTexture()");
	RESULT r = HRtoR(myDevice->SetTexture(stage, (LPDIRECT3DTEXTURE8)p_texture));
	Core::Function_Exit();
	return r;
}

VECTOR* D3DDevice::Unproject(VECTOR* r, VECTOR* src, float fAspect)
{
	/*
	void calcRay(int x,int y,D3DVECTOR &p1,D3DVECTOR &p2)
{
	float dx,dy;
	D3DMATRIX invMatrix,viewMatrix;

	dx=tanf(FOV*0.5f)*(x/WIDTH_DIV_2-1.0f)/ASPECT;
	dy=tanf(FOV*0.5f)*(1.0f-y/HEIGHT_DIV_2);
	lpDevice->GetTransform(D3DTRANSFORMSTATE_VIEW,&viewMatrix);
	D3DMath_MatrixInvert(invMatrix,viewMatrix);
	p1=D3DVECTOR(dx*NEAR,dy*NEAR,NEAR);
	p2=D3DVECTOR(dx*FAR,dy*FAR,FAR);
	D3DMath_VectorMatrixMultiply(p1,p1,invMatrix);
	D3DMath_VectorMatrixMultiply(p2,p2,invMatrix);
}
*/
	
	MATRIX mProj, mView;
	GetTransform(T_VIEW, &mView);
	GetTransform(T_PROJECTION, &mProj);
	VECTOR v;
	/*v.x = ( ( +( 2.0f * src->x ) / ScreenX  ) - 1 ) / mProj._11;
	v.y = ( ( -( 2.0f * src->y ) / ScreenY ) + 1 ) / mProj._22;
	v.z =  src->z;*/
	v.x = (tanf(fAspect*0.5f)*(src->x/((float)ScreenX*0.5f)-1.0f)/fAspect)*src->z;
	v.y = (tanf(fAspect*0.5f)*(1.0f-src->y/((float)ScreenY*0.5f)))*src->z;
	v.z = src->z;
	MATRIX mViewInv;
	MatrixMatrixInverse(&mView, &mViewInv);
	//MatrixMatrixVectorApply(r, &mViewInv, &v);

	r->x  = v.x*mViewInv._11 + v.y*mViewInv._21 + v.z*mViewInv._31;
	r->y  = v.x*mViewInv._12 + v.y*mViewInv._22 + v.z*mViewInv._32;
	r->z  = v.x*mViewInv._13 + v.y*mViewInv._23 + v.z*mViewInv._33;

	VectorNormalize(&v, r);

	r->x = mViewInv._41 + src->z * v.x;
	r->y = mViewInv._42 + src->z * v.y;
	r->z = mViewInv._43 + src->z * v.z;

	return r;
}

VECTOR* D3DDevice::Project(VECTOR* r, VECTOR* src)
{
	
	throw NOTIMPL;

	return r;
}

ray* D3DDevice::ComputeRay(ray* r, VECTOR* s, CCamera* cam)
{
	if(!r) return NULL;

	VECTOR v;
		
	MATRIX mProj, mView;
	GetTransform(T_VIEW, &mView);
	GetTransform(T_PROJECTION, &mProj);

	ray r2;

	v.x = ( ( +( 2.0f * s->x ) / ScreenX ) - 1 ) / mProj._11;
	v.y = ( ( -( 2.0f * s->y ) / ScreenY ) + 1 ) / mProj._22;
	v.z =  1.0f;

	MATRIX mInvView;
	MatrixMatrixInverse(&mView, &mInvView);

	// Transform the camera space pick ray into world space
	r->dir.x  = v.x * mInvView._11 + v.y * mInvView._21 + v.z * mInvView._31;
	r->dir.y  = v.x * mInvView._12 + v.y * mInvView._22 + v.z * mInvView._32;
	r->dir.z  = v.x * mInvView._13 + v.y * mInvView._23 + v.z * mInvView._33;
	
	//MatrixMatrixVectorApply(&r->dir, &mInvView, &v);
	VectorNormalize(&r->dir, &r->dir);
	r->orig.x = mInvView._41 + cam->fNear * r->dir.x;
	r->orig.y = mInvView._42 + cam->fNear * r->dir.y;
	r->orig.z = mInvView._43 + cam->fNear * r->dir.z;

	return r;
}

int D3DDevice::width()
{
	return ScreenX;
}

int D3DDevice::height()
{
	return ScreenY;
}

void D3DDevice::RemoveFromResetList(IMediaObject* obj)
{
	rset_iter i = std::find(reset_objects.begin(), reset_objects.end(), obj);
	if(i == reset_objects.end()) return;
	(*i) = reset_objects.back();
	reset_objects.pop_back();
}