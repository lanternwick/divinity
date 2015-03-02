#ifndef TEXTURE_STATE_H
#define TEXTURE_STATE_H

#include "boolean_state.h"

template<unsigned long ts, unsigned long value=0, bool toggle=false>
class texture_state : public boolean_state
{
private:
	unsigned long curr_state;
	IDevice* m_device;
public:
	void save_state()
	{
		m_device->GetTextureState(ts, &curr_state);
	}
	void restore_state()
	{
		m_device->SetTextureState(ts, curr_state);
	}
	texture_state(IDevice* device)
	{
		m_device = device;
		if(toggle)
			save_state();
		m_device->SetTextureState(ts, value);
	}
	~texture_state()
	{
		if(toggle)
			restore_state();
	}
};

#endif