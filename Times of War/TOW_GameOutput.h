#ifndef TOW_GAMEOUTPUT_H
#define TOW_GAMEOUTPUT_H

#include "..\Common\IEntity.h"
#include "..\DivGraph\DivGraph.h"

class TOW_GameOutput : public IEntity
{
public:
	virtual RESULT Render()=0;
	virtual RESULT Think(){return OK;}
	void SetDevice(IDevice* device);
protected:
	IDevice* m_device;
};

#endif