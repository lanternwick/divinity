#ifndef _UICURSOR_H
#define _UICURSOR_H

class UICursor : public CObject
{
public:
	UICursor();
	~UICursor();

	RESULT Create(HANDLE texture);
	RESULT Render(DWORD& numPrims);
	RESULT Update();
private:
	HANDLE myVB;
	HANDLE myText;
	int cx;
	int cy;
};

UICursor::UICursor()
{

}

UICursor::~UICursor()
{
	DeleteObj(myVB);
}

RESULT UICursor::Create(HANDLE texture)
{
	CreateVertexBuffer(&myVB, 4 * sizeof(TLTVERTEX), sizeof(TLTVERTEX), USAGE_DYNAMIC, TLTVERTFVF); 
	cx = 24;
	cy = 24;
	myText = texture;
	Update();
	return OK;
}

RESULT UICursor::Render(DWORD& numPrims)
{
	SelectObject(myVB);
	SelectObject(myText);
	UIBeginDraw();
	SetRenderState(RS_ALPHABLENDENABLE, TRUE);
		DrawTriFan(0, 2);
	SetRenderState(RS_ALPHABLENDENABLE, FALSE);
	UIEndDraw();
	return OK;
}

RESULT UICursor::Update()
{
	TLTVERTEX* v = NULL;
	POINT myDrawPos = GetMousePos();
	
	SelectObject(myVB);
	RESULT r = LockVertexBuffer(0, 0, (BYTE**)&v, LOCK_DISCARD);
	if((RFAILED(r)) || (v == NULL))
	{
		OutputLogString("Cursor Update Failed!\n");
		return OUTOFMEMORY;
	}
	OutputLogString("Cursor Update Success!\n");
	v[0] = WindowVertex(float(myDrawPos.x), float(myDrawPos.y), 0.1f, 1.0f, 0xffffffff, 0.0f, 0.0f);
	v[1] = WindowVertex(float(cx+myDrawPos.x), float(myDrawPos.y), 0.1f, 1.0f, 0xffffffff, 1.0f, 0.0f);			
	v[2] = WindowVertex(float(cx+myDrawPos.x), float(cy+myDrawPos.y), 0.1f, 1.0f, 0xffffffff, 1.0f, 1.0f);
	v[3] = WindowVertex(float(myDrawPos.x), float(cy+myDrawPos.y), 0.1f, 1.0f, 0xffffffff, 0.0f, 1.0f);
	UnlockVertexBuffer();
	return OK;
}

#endif