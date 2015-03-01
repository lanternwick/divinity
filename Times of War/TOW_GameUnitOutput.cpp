#include "TOW_GameUnitOutput.h"

IHeightMap* TOW_GameUnitOutput::m_hmp = NULL;

TOW_GameUnitOutput::TOW_GameUnitOutput()
{
	m_gu = NULL;
	m_device = NULL;
}

TOW_GameUnitOutput::TOW_GameUnitOutput(TOW_GameUnit* gu)
{
	m_gu = gu;
	m_device = NULL;
}

TOW_GameUnit* TOW_GameUnitOutput::GetGameUnit()
{
	return m_gu;
}

void TOW_GameUnitOutput::SetGameUnit(TOW_GameUnit* gu)
{
	m_gu = gu;
}

RESULT TOW_GameUnitOutput::Render()
{
	if(!m_gu)
		return INVALIDDATA;
	moniker mesh_id;
	moniker skin_id;
	try
	{
		mesh_id() = m_gu->properties.property(std::string("mesh")).as_if_string();
	}
	catch(...)
	{
		mesh_id = m_gu->GetMesh();
	}
	try
	{
		skin_id() = m_gu->properties.property(std::string("skin")).as_if_string();
	}
	catch(...)
	{
		skin_id = m_gu->GetSkin();
	}
	
	MATRIX m;
	MatrixMatrixTranslation(&m, m_gu->pos.x, m_hmp ? m_hmp->GetHeight(m_gu->pos.x, m_gu->pos.z) : m_gu->pos.y, m_gu->pos.z);
	ITexture* skin = (ITexture*)Core::GetMediaObject(skin_id);
	IMesh* mesh = (IMesh*)Core::GetMediaObject(mesh_id);
	if(!mesh) 
		return INVALIDDATA;
	if(!skin)
		return INVALIDDATA;
	//MatrixMatrixTranslation(&m, pos.x, pos.y, pos.z);
	//MatrixMatrixMultiply(&m3, &m2, &m);
	skin->Set(0);
	m_device->SetTransform(T_WORLD, &m);
	return mesh->Render();
}
	
void TOW_GameUnitOutput::SetHeightMap(IHeightMap* heightMap)
{
	m_hmp = heightMap;
}