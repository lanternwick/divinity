#include "DIDevice.h"

DIDevice::DIDevice()
{
	m_device = NULL;
	m_keyboard = NULL;
	m_mouse = NULL;
	mouseZ = 0;
	mousePos.x = 0;
	mousePos.y = 0;
	memset(keys, 0, sizeof(bool)*256);
	memset(buttons, 0, sizeof(bool)*4);
	m_released = false;


}

DIDevice::DIDevice(HWND hWnd)
{
	m_device = NULL;
	m_keyboard = NULL;
	m_mouse = NULL;
	mouseZ = 0;
	mousePos.x = 0;
	mousePos.y = 0;
	memset(keys, 0, sizeof(bool)*256);
	memset(buttons, 0, sizeof(bool)*4);
	m_released = false;
	CreateDevice(hWnd);
}

DIDevice::~DIDevice()
{
	if(m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = NULL;
	}
	if(m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = NULL;
	}
	if(m_device)
	{
		m_device->Release();
		m_device = NULL;
	}
	for(int i = 0; i < m_inputHandlers.size(); ++i)
	{
		if(m_inputHandlers[i].free_me)
			delete m_inputHandlers[i].ih;
	}

	m_released = false;
}

RESULT DIDevice::CreateDevice(HWND hWnd)
{
	HRESULT hr;
	m_hWnd = hWnd;
	try
	{
		hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, 
								(void**)&m_device, NULL);
		if(FAILED(hr)) throw hr;
		hr = m_device->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
		if(FAILED(hr)) throw hr;
		hr = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
		if(FAILED(hr)) throw hr;
		hr = m_keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		//if(FAILED(hr)) throw hr;
		hr = m_keyboard->Acquire();
		//if(FAILED(hr)) throw hr;
		hr = m_device->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
		if(FAILED(hr)) throw hr;
		hr = m_mouse->SetDataFormat(&c_dfDIMouse);
		if(FAILED(hr)) throw hr;
		hr = m_mouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
		//if(FAILED(hr)) throw hr;
		hr = m_mouse->Acquire();
		//if(FAILED(hr)) throw hr;
	}
	catch(HRESULT h)
	{
		return HRtoR(h);
	}
	return OK;
}


RESULT DIDevice::CaptureDevices()
{
	m_mouse->Acquire();
	m_keyboard->Acquire();
	m_released = false;
	memset(keys, 0, sizeof(bool)*256);
	memset(buttons, 0, sizeof(bool)*4);
	return OK;
}

RESULT DIDevice::UncaptureDevices()
{
	m_mouse->Unacquire();
	m_keyboard->Unacquire();
	m_released = true;

	return OK;
}

bool DIDevice::GetKey(unsigned long keyID)
{
	return keys[keyID];
}

bool DIDevice::GetMouseButton(unsigned long buttonID)
{
	return buttons[buttonID];
}

RESULT DIDevice::GetMousePos(POINT* pt)
{
	*pt = mousePos;
	return OK;
}

RESULT DIDevice::GetMouseScroll(long* scroll)
{
	*scroll = mouseZ / 120;
	return OK;
}

RESULT DIDevice::Think()
{
	HRESULT hr;
	char key_buff[256];
	hr = m_keyboard->GetDeviceState(sizeof(key_buff), (void*)&key_buff);
	if(FAILED(hr))
	{
		if(hr == DIERR_INPUTLOST)
		{
			if(!m_released)
			{
				hr = m_keyboard->Acquire();
			}
		}
	}
	DIMOUSESTATE dms;
	memset(&dms, 0, sizeof(dms));
	hr = m_mouse->GetDeviceState(sizeof(dms), (void*)&dms);
	if(FAILED(hr))
	{
		if(hr == DIERR_INPUTLOST)
		{
			if(!m_released)
			{
				hr = m_mouse->Acquire();
			}
		}
	}

	ProcessBuffers(key_buff, &dms);

	return OK;
}

RESULT DIDevice::ProcessBuffers(char* kb_buffer, DIMOUSESTATE* mouse_data)
{
	RECT r;
	GetClientRect(m_hWnd, &r);
	
	for(int i = 0; i < 256; ++i)
	{
		if(kb_buffer[i] & 0x80)
		{
			if(!keys[i])
			{
				keys[i] = true;
				// dispatch key up message
				for(int j = 0; j < m_inputHandlers.size(); ++j)
				{
					if(!m_inputHandlers[j].ih->Keyboard_KeyDown(this, char(i)))
						break;
				}
			}
			
		}
		else
		{
			if(keys[i])
			{
				keys[i] = false;
				// dispatch key up message
				for(int j = 0; j < m_inputHandlers.size(); ++j)
				{
					if(!m_inputHandlers[j].ih->Keyboard_KeyUp(this, char(i)))
						break;
				}
			}
		}
	}
	for(i = 0; i < 4; ++i)
	{
		if(mouse_data->rgbButtons[i] & 0x80)
		{
			if(!buttons[i])
			{
				buttons[i] = true;
				// dispatch key up message
				for(int j = 0; j < m_inputHandlers.size(); ++j)
				{
					if(!m_inputHandlers[j].ih->Mouse_ButtonDown(this, i))
						break;
				}
			}
			
		}
		else
		{
			if(buttons[i])
			{
				buttons[i] = false;
				// dispatch key up message
				for(int j = 0; j < m_inputHandlers.size(); ++j)
				{
					if(!m_inputHandlers[j].ih->Mouse_ButtonUp(this, i))
						break;
				}
			}
		}
	}
	if(abs(mouse_data->lX) > 5) mouse_data->lX <<= 1;
	if(abs(mouse_data->lY) > 5) mouse_data->lY <<= 1;
	mousePos.x += mouse_data->lX;
	if(mousePos.x < 0) mousePos.x = 0;
	if(mousePos.x > (r.right - r.left)) mousePos.x = (r.right - r.left);
	mousePos.y += mouse_data->lY;
	if(mousePos.y < 0) mousePos.y = 0;
	if(mousePos.y > (r.bottom - r.top)) mousePos.y = (r.bottom - r.top);
	mouseZ = mouse_data->lZ;
	if(mouse_data->lX || mouse_data->lY)
	{
		// dispatch mouse move message
		for(int j = 0; j < m_inputHandlers.size(); ++j)
		{
			if(!m_inputHandlers[j].ih->Mouse_Move(this, mousePos, buttons[0], buttons[1], buttons[2], buttons[3]))
				break;
		}
	}
	if(mouse_data->lZ)
	{
		// dispatch mouse wheel scroll message
		for(int j = 0; j < m_inputHandlers.size(); ++j)
		{
			if(!m_inputHandlers[j].ih->Mouse_Scroll(this, mouseZ))
				break;
		}
	}
	

	return OK;
}
	


RESULT DIDevice::SetInputHandler(IInputHandler* iih, bool free)
{
	IHContainer ihc;
	ihc.free_me = free;
	ihc.ih = iih;
	m_inputHandlers.push_back(ihc);
	return OK;
}

char DIDevice::KeyToChar(DWORD keyConst, bool shift)
{
#define KEYCASE(x, y, z) case DIV_KEY_##x: return (!shift) ?  ##y : ##z
	switch(keyConst)
	{
		KEYCASE(A, 'a', 'A');
		KEYCASE(B, 'b', 'B');
		KEYCASE(C, 'c', 'C');
		KEYCASE(D, 'd', 'D');
		KEYCASE(E, 'e', 'E');
		KEYCASE(F, 'f', 'F');
		KEYCASE(G, 'g', 'G');
		KEYCASE(H, 'h', 'H');
		KEYCASE(I, 'i', 'I');
		KEYCASE(J, 'j', 'J');
		KEYCASE(K, 'k', 'K');
		KEYCASE(L, 'l', 'L');
		KEYCASE(M, 'm', 'M');
		KEYCASE(N, 'n', 'N');
		KEYCASE(O, 'o', 'O');
		KEYCASE(P, 'p', 'P');
		KEYCASE(Q, 'q', 'Q');
		KEYCASE(R, 'r', 'R');
		KEYCASE(S, 's', 'S');
		KEYCASE(T, 't', 'T');
		KEYCASE(U, 'u', 'U');
		KEYCASE(V, 'v', 'V');
		KEYCASE(W, 'w', 'W');
		KEYCASE(X, 'x', 'X');
		KEYCASE(Y, 'y', 'Y');
		KEYCASE(Z, 'z', 'Z');
		KEYCASE(1, '1', '!');
		KEYCASE(2, '2', '@');
		KEYCASE(3, '3', '#');
		KEYCASE(4, '4', '$');
		KEYCASE(5, '5', '%');
		KEYCASE(6, '6', '^');
		KEYCASE(7, '7', '&');
		KEYCASE(8, '8', '*');
		KEYCASE(9, '9', '(');
		KEYCASE(0, '0', ')');
		KEYCASE(RETURN, '\n', '\n');
		KEYCASE(SPACE, ' ', ' ');
		KEYCASE(COMMA, ',', '<');
		KEYCASE(PERIOD, '.', '>');
		KEYCASE(SLASH, '/', '?');
		KEYCASE(BACKSPACE, '\b', '\b');
		KEYCASE(SEMICOLON, ';', ':');
		KEYCASE(APOSTROPHE, '\'', '\"');
		KEYCASE(EQUALS, '=', '+');
		KEYCASE(MINUS, '-', '_');
		KEYCASE(BACKSLASH, '\\', '|');
		KEYCASE(LBRACKET, '[', '{');
		KEYCASE(RBRACKET, ']', '}');
		KEYCASE(GRAVE, '`', '~');


	default: return '\0';
	}
#undef KEYCASE
}
