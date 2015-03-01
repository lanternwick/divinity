#ifndef _UIWINDOW_H
#define _UIWINDOW_H


#include "..\Common\Result.h"

#include "..\Common\IUIComponent.h"
#include "..\DivGraph\DivGraph.h"

class _API UIWindow : public IUIComponent
{
public:
	UIWindow();
	~UIWindow();

	void Create(CSmartPtr<IDevice> device, CSmartPtr<IGraphObjFactory> factory, int x, int y, TLTVERTEX* vertices, int numVerts);
	virtual RESULT Render();
	virtual RESULT Think();
private:
	CSmartPtr<ITexture> m_texture;
	CSmartPtr<IVertexBuffer> m_vb;
	CSmartPtr<IDevice> m_device;
	TLTVERTEX* m_verts;
	int m_numVerts;
	void m_SyncVerts();

};

#endif
	