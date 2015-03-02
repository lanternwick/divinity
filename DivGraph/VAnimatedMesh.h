#ifndef VANIMATEDMESH_H
#define VANIMATEDMESH_H

#include "..\Common\define_exports.h"

#include "IMesh.h"
#include "..\Common\custmoniker.h"

class _API VAnimatedMesh : public IMesh
{
public:
	VAnimatedMesh(char* id);
	~VAnimatedMesh();

	RESULT Think();
	RESULT Render();

	RESULT LoadMesh(IGraphObjFactory* factory, char* file);

	void DrawSubset(unsigned long subset);

	void SetAnimation(moniker& animname);
	
	RESULT ExportMesh(char* file);

private:
	VAnimatedMesh();
	struct frame
	{
		mesh m;
	};
	
	struct animation
	{
		moniker name;
		std::vector<frame> frames;
		float length;
	};
	
	struct animation_set
	{
		std::vector<animation> anims;
	};
	void stream_frame(FILE* f, frame& fr);
	void stream_anim(FILE* f, animation& anim);
	void stream_set(FILE* f, animation_set& set);
	animation_set m_anims;
	IGraphObjFactory* m_factory;
	IDevice* m_device;
	moniker m_curr_anim;
};

#endif
