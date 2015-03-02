#ifndef TOW_FUNCTOR_H
#define TOW_FUNCTOR_H

#include "..\..\Console\console_functor.h"
#include "..\DivGraph\IDevice.h"
#include "..\DivGraph\render_state.h"


class TOW_Functor : public console_functor
{
public:
	void SetDevice(IDevice* device)
	{
		m_device = device;
	}

protected:
	IDevice* m_device;
};

#endif
