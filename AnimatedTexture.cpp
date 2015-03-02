#include "AnimatedTexture.h"

AnimatedTexture::AnimatedTexture()
{
	m_factory = NULL;
	m_device = NULL;
	m_width = NULL;
	myFilename = new char[128];
}

AnimatedTexture::AnimatedTexture(char* id)
{
	m_id() = id;
	m_factory = NULL;
	m_device = NULL;
	m_width = NULL;
	myFilename = new char[128];

}

AnimatedTexture::~AnimatedTexture()
{
	Release();
}

void AnimatedTexture::SetFactory(IGraphObjFactory* factory)
{
	m_factory = factory;
}

RESULT AnimatedTexture::CreateTextureFromMemory(IDevice* device, char* bytestream, int width)
{
	
	return OK;
}

RESULT AnimatedTexture::LoadTexture(IDevice* device, char* filename)
{
	strcpy(myFilename, filename);
	FILE* f = fopen(filename, "r+b");
	if(!f)
		return FILE_DOES_NOT_EXIST;
	fread(&m_info, sizeof(AnimatedTexture::header), 1, f);
	m_width = m_info.width;
	char* b = new char[m_width*m_width*4];
	ITexture* t = NULL;
	for(unsigned int i = 0; i < m_info.num_frames; ++i)
	{
		fread(b, m_width*m_width*4, 1, f);
		m_factory->CreateTextureFromMemory(t, m_width, b, NULL);
		m_frames.push_back(t);
		t = NULL;
	}
	return OK;
}

RESULT AnimatedTexture::LoadTextureEx(IDevice* device, char* filename, DWORD transColor)
{
	return NOTIMPL;
}

void AnimatedTexture::Release()
{
	if(myFilename)
	{
		delete [] myFilename;
		myFilename = NULL;
	}
	m_factory = NULL;
	m_device = NULL;
	m_width = NULL;
}

void AnimatedTexture::ResetDeviceObjects()
{

}

RESULT AnimatedTexture::Set(unsigned long stage)
{
	float f = Core::GetTime();
	f = fmod(f, m_info.anim_length);
	int i = f*(float)m_info.num_frames;
	m_frames[i]->Set(stage);
	return OK;
}