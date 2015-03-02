#include "Font.h"
#include "types.h"


Font::Font()
{
	m_charSet = NULL;
}

Font::~Font()
{
	if(m_charSet)
	{
		m_charSet->Release();
	}
	m_charSet = NULL;
}

RESULT Font::LoadFont(IDevice* device, char* filename)
{
	if(!filename) return E_UNKNOWN;
	m_device = device;
	
	FILE* f = NULL;
	
	f = fopen(filename, "r+b");
	if(!f) return FILE_DOES_NOT_EXIST;
	unsigned long tex_width;

	fseek(f, 0, SEEK_END);
	tex_width = (ftell(f)-8) / 4;
	
	tex_width = sqrt(tex_width);
	
	fseek(f, 0, SEEK_SET);
	fread(&cell_h, 1, sizeof(int), f);
	fseek(f, sizeof(int), SEEK_SET);
	fread(&cell_w, 1, sizeof(int), f);
	char* buffer = new char[tex_width*tex_width*4];
	fread(buffer, 1, tex_width*tex_width*4, f);
	RESULT r = device->CreateTextureFromMemory(m_charSet, tex_width, buffer, NULL);
	delete buffer;
	return r;
}

RESULT Font::DrawText(const char* text, RECT text_rect, char* attributes, float scale, DWORD color)
{
	if(!text) return INVALIDDATA;
	unsigned long text_len;
	
	long i;
	
	text_len = strlen(text);
	if(!text_len)
		return OK;
		
	unsigned long numVerts = text_len * 6;
	
	// if either of these are set, there must be only one line of text
	bool center = false;
	bool vcenter = true;
	int tcstart = 0;
	int tvcstart = 0;
	if(attributes)
	{
		unsigned long attr_len = strlen(attributes);
		if(attr_len >= 1)
		{
			center = attributes[0] == 'x';
			tcstart = ((text_rect.right - text_rect.left) - (text_len*((cell_w*0.75f/scale)*scale))) / 2;
		}
		if(attr_len >= 2)
		{
			vcenter = attributes[1] == 'x';
			tvcstart = ((text_rect.bottom - text_rect.top) - (cell_h*scale)) / 2;
		}
	}
	IVertexBuffer* vb = NULL;
	m_device->CreateVertexBuffer(numVerts, sizeof(TLTVERTEX), USAGE_WRITEONLY, TLTVERTFVF, vb, NULL);
	TLTVERTEX* verts;
	vb->Lock(0, 0, (BYTE**)&verts, 0);
	int r;
	int c;
	unsigned long count = 0;
	int linecount = 0;
	//char* text_ptr = text;
	for(i = 0; *text; ++text, ++i)
	{
		if(*text == '\n')
		{
			++linecount;
			i = -1;
			continue;
		}
		if(*text == ' ') continue;
		int index = (int)*text;
		index -= (int)' ';
		c = index / 20;
		r = index % 20;
		if(((text_rect.left + tcstart + ((i * cell_w) * scale))-(cell_w/4/scale)*i) > text_rect.right)
			continue;
		if((text_rect.top + tvcstart + (((linecount+1) * cell_h) * scale)) > text_rect.bottom)
		{
			break;
		}
		verts[count++] = 
			TLTVertex(text_rect.left + ((i * cell_w) * scale),
			 		 text_rect.top + tvcstart + ((linecount * cell_h) * scale),
					 0.0f,
					 1.0f,
					 color,
					 float(r*cell_w) / 255.0f,
					 float(c*cell_h) / 255.0f);
		verts[count-1].x -= (cell_w/4/scale)*i;
		verts[count-1].x += tcstart;
		verts[count++] = 
			TLTVertex(text_rect.left + (((i+1) * cell_w) * scale),
			 		 text_rect.top + tvcstart + ((linecount * cell_h) * scale),
					 0.0f,
					 1.0f,
					 color,
					 float((r+1)*cell_w) / 256.0f,
					 float(c*cell_h) / 256.0f);
		verts[count-1].x -= (cell_w/4/scale)*i;
		verts[count-1].x += tcstart;
		verts[count++] = 
			TLTVertex(text_rect.left + ((i * cell_w) * scale),
			 		 text_rect.top + tvcstart + (((linecount+1) * cell_h) * scale),
					 0.0f,
					 1.0f,
					 color,
					 float(r*cell_w) / 256.0f,
					 float((c+1)*cell_h) / 256.0f);
		verts[count-1].x -= (cell_w/4/scale)*i;
		verts[count-1].x += tcstart;
		verts[count++] = verts[count-2];
		//verts[count-1].x -= cell_w/8;
		verts[count++] = 
			TLTVertex(text_rect.left + (((i+1) * cell_w) * scale),
			 		 text_rect.top + tvcstart + (((linecount+1) * cell_h) * scale),
					 0.0f,
					 1.0f,
					 color,
					 float((r+1)*cell_w) / 256.0f,
					 float((c+1)*cell_h) / 256.0f);
		verts[count-1].x -= (cell_w/4/scale)*i;
		verts[count-1].x += tcstart;
		verts[count++] = verts[count-3];
		//verts[count-1].x -= cell_w/4;
		
		if(count + 6 > numVerts)
			break;

	}
	vb->Unlock();
	vb->Set();
	ITexture::MakeNull(m_device, 1);
	m_charSet->Set(0);
	m_device->SetTextureState(0, TS_COLORARG2, TA_DIFFUSE);
	m_device->SetTextureState(0, TS_MAGFILTER, TEXF_LINEAR);
	m_device->SetTextureState(0, TS_MINFILTER, TEXF_LINEAR);
	unsigned long fstate;
	if(count)
	{
		m_device->SetRenderState(RS_ZWRITEENABLE, 0);
		m_device->GetRenderState(RS_FILLMODE, &fstate);
		m_device->SetRenderState(RS_FILLMODE, FILL_SOLID);
		m_device->DrawPrim(PT_TRILIST, 0, (count)/3);
		m_device->SetRenderState(RS_ZWRITEENABLE, 1);
		m_device->SetRenderState(RS_FILLMODE, fstate);
		IVertexBuffer::MakeNull(m_device);
	}
	
	vb->Release();
	delete vb;
	
	return OK;
}

SIZE Font::GetCellSize()
{
	SIZE s = {cell_w, cell_h};
	return s;
}