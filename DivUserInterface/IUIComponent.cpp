#include "..\Common\IUIComponent.h"

IUIComponent::IUIComponent()
{
	Core::Function_Enter("IUIComponent::Constructor()");
	Core::Function_Exit();

}

IUIComponent::IUIComponent(char* id)
{
	Core::Function_Enter("IUIComponent::Constructor()");
	m_id() = id;
	Core::Function_Exit();

}

IUIComponent::~IUIComponent()
{
	Core::Function_Enter("IUIComponent::Destructor()");
	Core::Function_Exit();
}



POINT IUIComponent::GetPos()
{
	return m_pos;
}

void IUIComponent::SetPos(POINT p)
{
	m_pos = p;
}

SIZE IUIComponent::Size()
{
	return m_size;
}

void IUIComponent::SetSize(SIZE s)
{
	m_size = s;
}

void IUIComponent::AddKeyToBuffer(char c)
{
	
}

std::string IUIComponent::GetBuffer()
{
	return m_input_buffer;
}

void IUIComponent::ClearBuffer()
{
	m_input_buffer = "";
}

void IUIComponent::SetInputHandler(IInputHandler* iih)
{
	m_input_handler = iih;
}

void IUIComponent::Activate()
{
	active = true;
}

void IUIComponent::Deactivate()
{
	active = false;
}

bool IUIComponent::Active()
{
	return active;
}