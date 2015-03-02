#include "TOW_LogPrint.h"

TOW_LogPrint::TOW_LogPrint(IDevice* device, UserInterface* ui, moniker& font_name, DWORD color, RECT font_rect)
{
	m_font = font_name;
	m_color = color;
	m_ui = ui;
	m_device = device;
	m_rect = font_rect;
	m_scene = 0;
}

RESULT TOW_LogPrint::Render()
{
	unsigned long FState = (DWORD)FILL_SOLID;
	m_device->GetRenderState(RS_FILLMODE, &FState);
	m_device->SetRenderState(RS_FILLMODE, FILL_SOLID);
	
	RESULT r; // = m_ui->DrawText(m_font.c_str(), Core::ReadLog(0), m_rect, NULL, 1.0f, m_color);
	RECT re = {5, 400, 480, 480};
	char buff[128];
	CCamera cam;
	m_scene->GetCamera(&cam);
	sprintf(buff, "CamPos: %f, %f, %f\nLookAt: %f, %f, %f", cam.vecPos.x, cam.vecPos.y, cam.vecPos.z, cam.vecAt.x,
			cam.vecAt.y, cam.vecAt.z);
	re.top = 410;
	r = m_ui->DrawText(m_font, buff, re, NULL, 1.0f, 0xff0000ff);
	m_device->SetRenderState(RS_FILLMODE, FState);
	return r;
}

void TOW_LogPrint::SetScene(CScene* scene)
{
	m_scene = scene;
}
