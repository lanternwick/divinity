#ifndef RENDER_STATE_H
#define RENDER_STATE_H

#include "..\Common\boolean_state.h"

template<unsigned long st, unsigned long value=0, bool toggle=false>
class render_state : public boolean_state
{
private:
	unsigned long current_state;
	IDevice* m_device;

public:
	void save_state()
	{
		m_device->GetRenderState(st, &current_state);
	}
	void restore_state()
	{
		m_device->SetRenderState(st, current_state);
	}
	render_state(IDevice* device)
	{
		m_device = device;
		if(toggle)
			save_state();
		m_device->SetRenderState(st, value);
	}
	~render_state()
	{
		if(toggle)
			restore_state();
	}
};

#endif