#ifndef _CSEGMENT_H
#define _CSEGMENT_H

#include <CObject.h>
#define INITGUID
#include <dmusici.h>

class CSegment : public CObject
{
public:
	CSegment();
	~CSegment();
	IDirectMusicSegment8* Segment;
};

CSegment::CSegment()
{
	Segment = NULL;
	type = OBJ_SEGMENT;
}

CSegment::~CSegment()
{
	if(Segment != NULL) Segment->Release();
}

#endif


