#include "UIWindow.h"

UIWindow::UIWindow()
{

}

UIWindow::~UIWindow()
{
	m_device.Release();
	m_vb.Release();
}

void UIWindow::Create(CSmartPtr<IDevice> device, CSmartPtr<IGraphObjFactory> factory, int x, int y, TLTVERTEX* vertices, int numVerts)
{
	Core::Function_Enter("UIWindow::Create()");
	if(numVerts % 4) throw "Not Enough Vertices";

	m_verts = new TLTVERTEX[numVerts];
	memcpy(m_verts, vertices, numVerts*sizeof(TLTVERTEX));
	factory->CreateVertexBuffer(numVerts, sizeof(TLTVERTEX), 0, TLTVERTFVF, m_vb, NULL);
	TLTVERTEX* p;
	m_vb->Lock(0, 0, (BYTE**)&p, 0);
	memcpy(p, vertices, sizeof(TLTVERTEX)*numVerts);
	m_vb->Unlock();
	m_device = device;
	m_numVerts = numVerts;
	Core::Function_Exit();
}

RESULT UIWindow::Render()
{
	m_vb->Set(m_device);
	m_device->DrawPrim(PT_TRILIST, 0, m_numVerts/3); 

	return OK;
}

RESULT UIWindow::Think()
{

	return OK;
}