#include "D3DDevice.h"

static DWORD FtoDW(float f)
// utility function
{
	return *(DWORD*)&f;
}

D3DDevice::D3DDevice()
{
	myDevice = NULL;
}

D3DDevice::~D3DDevice()
{
	Cleanup();
}

HWND D3DDevice::hWnd() const
{
	// get the associtated HWND
	HWND h = my_hWnd;
	return h;
}

RESULT D3DDevice::Create(HWND hWnd, BOOL Window, int cx, int cy, DWORD format, bool force_format)
//Create a Device or otherwise initialize the rendering API
{
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
		d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
		// try to create a pure device
		if(FAILED(D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
										D3DCREATE_PUREDEVICE | D3DCREATE_HARDWARE_VERTEXPROCESSING 
										| D3DCREATE_MULTITHREADED,
										&d3dpp, &myDevice)))
		{
			//Core::WriteLog("Failed to create Pure Device...\n", 0);
			// try to create a hardware device
			if(FAILED(D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
										D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
										&d3dpp, &myDevice)))
			{
				//Core::WriteLog("Failed to create Hardware Device...\n", 0);
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
		return r;
	}

	myDevice->GetViewport(&vp);
	ScreenX = vp.Width;
	ScreenY = vp.Height;

	return OK;
}
	
RESULT D3DDevice::Reset(HWND hWnd, BOOL Window, int cx, int cy, DWORD format, bool force_format)
//Create a Device or otherwise initialize the rendering API
{
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
		// if the device should be windowed, dont do this
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
			//Core::WriteLog("Reset Failed...\n", 1);
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
		return r;
	}

	myDevice->GetViewport(&vp);
	ScreenX = vp.Width;
	ScreenY = vp.Height;

	return OK;
}


VOID D3DDevice::Clear(DWORD Color)
//clear scene 
{
	myDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, Color, 1.0f, 0);
	return;
}

HRESULT D3DDevice::Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
//flip scene
{
	HRESULT hr = myDevice->Present(NULL, NULL, NULL, NULL);
	return hr;
}

VOID D3DDevice::Flip()
{
	// flip the scene
	Present(NULL, NULL, NULL, NULL);
	return;
}

VOID D3DDevice::Cleanup()
{
	if(NULL != myDevice)
	{
		myDevice->SetStreamSource(0, NULL, 0);
		SetNullTexture(0);
		
		// release the device
		myDevice->Release();
		myDevice = NULL;
	}
}
	
RESULT D3DDevice::SetRenderState(DWORD rs, DWORD val)
// cache renderstate changes
{
	HRESULT hr = S_OK;
	if(states[rs] != val) // if the state is different
	{
		states[rs] = val; // save this state
		// and update
		hr = myDevice->SetRenderState((D3DRENDERSTATETYPE)rs, val);
	}
	return HRtoR(hr);
}

RESULT D3DDevice::SetTextureState(DWORD stage, DWORD ts, DWORD val)
{
	HRESULT hr = S_OK;
	if(tstates[stage][ts] != val) // if the state is different
	{
		tstates[stage][ts] = val; // save this state
		// and update
		hr = myDevice->SetTextureStageState(stage, (D3DTEXTURESTAGESTATETYPE)ts, val);
	}
	return HRtoR(hr);
}

RESULT D3DDevice::GetRenderState(DWORD rs, DWORD* val)
{
	HRESULT hr = S_OK;
	hr = myDevice->GetRenderState((D3DRENDERSTATETYPE)rs, val);
	return HRtoR(hr);
}

RESULT D3DDevice::GetTextureState(DWORD stage, DWORD ts, DWORD* val)
{
	HRESULT hr = S_OK;
	hr = myDevice->GetTextureStageState(stage, (D3DTEXTURESTAGESTATETYPE)ts, val);
	return HRtoR(hr);
}

RESULT D3DDevice::BeginScene()
{
	HRESULT hr = myDevice->BeginScene(); // begin scene
	return HRtoR(hr);
}

RESULT D3DDevice::EndScene()
{
	// end scene
	HRESULT hr = myDevice->EndScene();
	return HRtoR(hr);
}

RESULT D3DDevice::SetLight(DWORD index, LIGHT* light)
{	
	// set a light with index
	HRESULT hr = myDevice->SetLight(index, (const D3DLIGHT8*)light);
	return HRtoR(hr);
}

RESULT D3DDevice::LightEnable(DWORD index, BOOL enabled)
{
	//enable a light at index
	HRESULT hr = myDevice->LightEnable(index, enabled);
	return HRtoR(hr);
}

RESULT D3DDevice::DrawIndexedPrim(PRIM_TYPES pt, UINT minIndex, UINT startVert, UINT startIndex, UINT numPrims)
// draw an indexed primitive
{
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
	return r;
	
}

RESULT D3DDevice::DrawPrim(PRIM_TYPES pt, UINT startVert, UINT numPrims)
// draw primitive
{
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
	
	return r;
}

RESULT D3DDevice::GetTransform(TRANSFORMATION t, MATRIX* m)
// get current transform
{
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
		return r;
	}
	catch(RESULT cr)
	{
		return cr;
	}
}

RESULT D3DDevice::SetTransform(TRANSFORMATION t, MATRIX* m)
// set a new transformation matrix
{
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
		return r;
	}
	catch(RESULT cr)
	{
		return cr;
	}
	
	
}

RESULT D3DDevice::CreateVertexBuffer(UINT numVerts, UINT vertSize, DWORD usage, DWORD FVF, IVertexBuffer*& vb, const char* key)
{
	// create a new vertex buffer
	D3DVertexBuffer* d3dvb = NULL;
	if(!vb)
	{
		d3dvb = new D3DVertexBuffer(key);
	
		if(!d3dvb)
		{
			return OUTOFMEMORY;
		}
		vb = d3dvb;
	}
	d3dvb->myDev = this;
	d3dvb->myFVF = FVF;
	d3dvb->mySize = numVerts;
	d3dvb->myUsage = usage;
	d3dvb->myVertSize = vertSize;
	if(!d3dvb->myData) d3dvb->myData = new unsigned char[numVerts*vertSize];
	HRESULT hr;
	hr = myDevice->CreateVertexBuffer(numVerts * vertSize, usage, FVF, D3DPOOL_DEFAULT, &d3dvb->myD3DVB);
	if(std::find(reset_objects.begin(), reset_objects.end(), vb)==reset_objects.end())
		reset_objects.push_back(vb);
	
	return HRtoR(hr);
}

RESULT D3DDevice::CreateIndexBuffer(UINT numIndices, DWORD usage, char type, IIndexBuffer*& ib, const char* key)
{
	// create a new index buffer
	D3DIndexBuffer* d3dib = NULL;
	if(!ib)
	{
		d3dib = new D3DIndexBuffer(key);
		if(!d3dib)
			return OUTOFMEMORY;
	
		ib = d3dib;
	}
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
		return cr;
	}
	d3dib->myDevice = this;
	d3dib->mySize = numIndices;
	d3dib->myUsage = usage;
	d3dib->myType = type;
	if(!d3dib->myData) d3dib->myData = new unsigned char[numIndices*(type==16 ? 2 : 4)];
	// create the buffer
	r = HRtoR(myDevice->CreateIndexBuffer(numIndices*sMul, usage, (D3DFORMAT)r_type, D3DPOOL_DEFAULT, &d3dib->myD3DIB));
	if(std::find(reset_objects.begin(), reset_objects.end(), ib)==reset_objects.end())
		reset_objects.push_back(ib);
	return r;
}

RESULT D3DDevice::LoadTexture(const char* filename, ITexture*& texture)
{
	// load a texture
	for(int i = 0; i < m_textures.size(); ++i)
	{
		if(m_textures[i]->Identifier() == filename)
		{
			texture = m_textures[i];
			texture->IncRef();
			return OK;
		}
	}
	
	unsigned long tex_width;
	FILE* f = NULL;
	
	f = fopen(filename, "r+b");
	if(!f) return FILE_DOES_NOT_EXIST;
	

	fseek(f, 0, SEEK_END);
	tex_width = ftell(f) / 4;
	
	tex_width = sqrt(tex_width);
	fseek(f, 0, SEEK_SET);
	char* buffer = new char[tex_width*tex_width*4];
	fread(buffer, 1, tex_width*tex_width*4, f);
	RESULT r = CreateTextureFromMemory(texture, tex_width, buffer, filename);
	if(r != ALREADY_LOADED)
	{
		D3DTexture* d3dtex = (D3DTexture*)texture;
		strcpy(d3dtex->myFilename, filename);
		d3dtex->myDev = this;
		d3dtex->Ex = FALSE;
	}
	delete buffer;
	return OK;
}

RESULT D3DDevice::CreateTextureFromMemory(ITexture*& texture, int width, char* memory, const char* key)
{
	if(key)
	{
		for(int i = 0; i < m_textures.size(); ++i)
		{
			if(m_textures[i]->Identifier() == key)
			{
				texture = m_textures[i];
				texture->IncRef();
				return ALREADY_LOADED;
			}
		}
	}
	
	D3DTexture* d3dtexture =NULL;
	if(!texture)
	{
		d3dtexture = new D3DTexture(key);
		if(!d3dtexture)
			return OUTOFMEMORY;

		texture = d3dtexture;
	}
	LPDIRECT3DTEXTURE8 temp_text = NULL;
	RESULT r = HRtoR(myDevice->CreateTexture(width, width, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &temp_text));
	if(RFAILED(r))
		return r;
	r = HRtoR(myDevice->CreateTexture(width, width, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &d3dtexture->texture));
	if(RFAILED(r)) 
		return r;
	D3DLOCKED_RECT re;
	temp_text->LockRect(0, &re, NULL, 0);
	unsigned long c = 0;
	char* b = (char*)re.pBits;
	memcpy(b, memory, width*width*4);
	if(!d3dtexture->myData)
	{
		d3dtexture->myData = new unsigned char[width*width*4];
		memcpy(d3dtexture->myData, memory, width*width*4);
	}
	temp_text->UnlockRect(0);
	r = HRtoR(myDevice->UpdateTexture(temp_text, d3dtexture->texture));
	
	temp_text->Release();
	d3dtexture->myDev = this;
	//myFilename = NULL;
	d3dtexture->m_width = width;

	if(std::find(reset_objects.begin(), reset_objects.end(), texture)==reset_objects.end())
		reset_objects.push_back(texture);
	
	return r;
}

RESULT D3DDevice::LoadTextureEx(const char* filename, DWORD transColor, ITexture*& texture)
{
	// load a texture with a transparent color
	texture = new D3DTexture(filename);
	if(!texture)
		return OUTOFMEMORY;
	

	// register and load texture here
	
	return OK;
}


// deprecated and unsafe
/*RESULT D3DDevice::CreateVertexBuffer(IMediaObject* boundObj, UINT numVerts, UINT VertSize, DWORD Usage, DWORD FVF, VOID** p_vertexBuffer)
{
	// create a vertex buffer
		
	HRESULT hr;
	hr = myDevice->CreateVertexBuffer(numVerts * VertSize, Usage, FVF, D3DPOOL_DEFAULT, (LPDIRECT3DVERTEXBUFFER8*)p_vertexBuffer);
	if(boundObj)
	{
		if(std::find(reset_objects.begin(), reset_objects.end(), boundObj)==reset_objects.end())
			reset_objects.push_back(boundObj);
	}
	
	return HRtoR(hr);
}*/

RESULT D3DDevice::SetVertexShader(DWORD shader)
{
	// set vertex shader
	HRESULT hr = myDevice->SetVertexShader(shader);
	return HRtoR(hr);
}

RESULT D3DDevice::SetVertexBuffer(VOID* p_vertexBuffer, DWORD stride)
{
	
	// set the vertex buffer for rendering
	HRESULT hr = myDevice->SetStreamSource(0, (LPDIRECT3DVERTEXBUFFER8)p_vertexBuffer, stride);
	return HRtoR(hr);
}


// deprecated and unsafe
/*RESULT D3DDevice::CreateIndexBuffer(IMediaObject* boundObj, UINT numIndices, DWORD Usage, BYTE type, VOID** p_indexBuffer)
{
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
		return cr;
	}
	// create the buffer
	r = HRtoR(myDevice->CreateIndexBuffer(numIndices*sMul, Usage, (D3DFORMAT)r_type, D3DPOOL_DEFAULT, (LPDIRECT3DINDEXBUFFER8*)p_indexBuffer));
	if(boundObj)
	{
		if(std::find(reset_objects.begin(), reset_objects.end(), boundObj)==reset_objects.end())
			reset_objects.push_back(boundObj);
	}
	return r;
}*/

RESULT D3DDevice::SetIndices(VOID* p_indexBuffer, UINT startIndices)
{
	RESULT r = HRtoR(myDevice->SetIndices((LPDIRECT3DINDEXBUFFER8)p_indexBuffer, startIndices));
	return r;
}

RESULT D3DDevice::SetMaterial(MATERIAL* mat)
{
	// set material
	RESULT r = HRtoR(myDevice->SetMaterial((D3DMATERIAL8*)mat));
	return r;
}

RESULT D3DDevice::SetNullTexture(unsigned long stage)
{
	// clear the texture
	RESULT r = HRtoR(myDevice->SetTexture(stage, NULL));
	return r;
}


// deprecated
/*RESULT D3DDevice::LoadTexture(const char* filename, VOID** p_texture)
{
	// load normal texture
	//RESULT r = HRtoR(D3DXCreateTextureFromFile(myDevice, filename, (LPDIRECT3DTEXTURE8*)p_texture));
	return NOTIMPL;
}*/

// deprecated and unsafe
/*RESULT D3DDevice::CreateTexture(IMediaObject* boundObj, unsigned int width, POOL pool, VOID** p_texture)
{
	RESULT r = HRtoR(myDevice->CreateTexture(width, width, 0, 0, D3DFMT_A8R8G8B8, (D3DPOOL)pool, (LPDIRECT3DTEXTURE8*)p_texture));
	if(boundObj)
	{
		if(std::find(reset_objects.begin(), reset_objects.end(), boundObj)==reset_objects.end())
			reset_objects.push_back(boundObj);
	}
	return r;
}*/

// unsafe
RESULT D3DDevice::UpdateTexture(VOID* src, VOID* dest)
{
	RESULT r = HRtoR(myDevice->UpdateTexture((LPDIRECT3DTEXTURE8)src, (LPDIRECT3DTEXTURE8)dest));
	return r;
}

// deprecated
/*RESULT D3DDevice::LoadTextureEx(const char* filename, DWORD transColor, VOID** p_texture)
{
	// load texture with trans color
	RESULT r = HRtoR(D3DXCreateTextureFromFileEx(myDevice, filename, D3DX_DEFAULT, D3DX_DEFAULT,
											 0, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, 
											 D3DX_FILTER_NONE, D3DX_FILTER_NONE, transColor, NULL, NULL, 
											 (LPDIRECT3DTEXTURE8*)p_texture));
	return NOTIMPL;
}*/

// unsafe
RESULT D3DDevice::SetTexture(DWORD stage, VOID* p_texture)
{
	// set the texture
	RESULT r = HRtoR(myDevice->SetTexture(stage, (LPDIRECT3DTEXTURE8)p_texture));
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
	if(i == reset_objects.end()) 
	{
		return;
	}
	(*i) = reset_objects.back();
	reset_objects.pop_back();
}

void D3DDevice::BufferDump(char* filename)
{
	IDirect3DSurface8* bb;
	D3DSURFACE_DESC desc;
	D3DLOCKED_RECT r;
	unsigned long* buffer;
	myDevice->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &bb);
	bb->GetDesc(&desc);
	buffer = new unsigned long[desc.Width*desc.Height*4];
	bb->LockRect(&r, NULL, 0);
	memcpy(buffer, r.pBits, desc.Width*desc.Height*4);
	FILE* f = fopen(filename, "w+b");
	fwrite(&desc.Width, 1, sizeof(desc.Width), f);
	fwrite(&desc.Height, 1, sizeof(desc.Height), f);
	fwrite(buffer, 1, desc.Width*desc.Height*4, f);
	bb->UnlockRect();
	bb->Release();
	fclose(f);
	delete [] buffer;

}