#ifndef ANIMATEDTEXTURE_H
#define ANIMATEDTEXTURE_H

#include "ITexture.h"
#include <vector>
#include "IDevice.h"
#include "IGraphObjFactory.h"


class _API AnimatedTexture : public ITexture
{
public:
	AnimatedTexture();
	AnimatedTexture(char* id);
	~AnimatedTexture();

	virtual RESULT LoadTexture(IDevice* device, char* filename);
	virtual RESULT LoadTextureEx(IDevice* device, char* filename, DWORD transColor);

	virtual RESULT CreateTextureFromMemory(IDevice* device, char* bytestream, int width);

	virtual void Release();
	virtual void ResetDeviceObjects();
	virtual RESULT Set(unsigned long stage);
	void SetFactory(IGraphObjFactory* factory);
private:
	struct header
	{
		unsigned long num_frames;
		unsigned long width;
		float anim_length;
	};
	std::vector<ITexture*> m_frames;
	header m_info;
	IDevice* m_device;
	IGraphObjFactory* m_factory;
};

#endif