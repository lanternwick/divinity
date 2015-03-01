#include "TOW_GameObjectOutput.h"

IHeightMap* TOW_GameObjectOutput::m_hmp = NULL;

TOW_GameObjectOutput::TOW_GameObjectOutput()
{
	m_go = NULL;
	m_device = NULL;
}

TOW_GameObjectOutput::TOW_GameObjectOutput(TOW_GameObject* go)
{
	m_go = go;
	m_device = NULL;
}

TOW_GameObject* TOW_GameObjectOutput::GetGameObject()
{
	return m_go;
}

void TOW_GameObjectOutput::SetGameObject(TOW_GameObject* go)
{
	m_go = go;
}

RESULT TOW_GameObjectOutput::Render()
{
	if(!m_go)
		return INVALIDDATA;
	moniker mesh_id;
	moniker skin_id;
	try
	{
		mesh_id() = m_go->property(std::string("mesh")).as_if_string();
	}
	catch(...)
	{
		return INVALIDDATA;
		//mesh_id = m_go->GetMesh();
	}
	try
	{
		skin_id() = m_go->property(std::string("skin")).as_if_string();
	}
	catch(...)
	{
		return INVALIDDATA;
		//skin_id = m_go->GetSkin();
	}
	
	MATRIX m;
	MatrixMatrixTranslation(&m, m_go->pos.x, m_hmp ? m_hmp->GetHeight(m_go->pos.x, m_go->pos.z) + m_go->pos.y : m_go->pos.y, m_go->pos.z);
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
	
void TOW_GameObjectOutput::SetHeightMap(IHeightMap* heightMap)
{
	m_hmp = heightMap;
}