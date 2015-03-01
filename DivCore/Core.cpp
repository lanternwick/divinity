

#include "stdafx.h"

#include "DivCore.h"
#include "core.h"
#include "malloc.h"
LPVOID __declspec(dllexport) GimmeMem(size_t size)
{
	return malloc(size);
}

VOID __declspec(dllexport) FreeMem(void* p)
{
	free(p);
	return;
}

Core* Core::_instance = NULL;
__int64 Core::m_Time = 0;
__int64 Core::m_LastTick = 0;
__int64 Core::m_frequency = 0;
float Core::m_timeD = 0.0f;
char* Core::m_exePath = new char[256];
char* Core::m_function_stack = new char[FUNCTION_STACK_LEN];
unsigned int Core::m_iStackDepth = 0;


Core::Core()
{
	// allocate size for the executable path
	
	// get Performance Counter frequency
	LARGE_INTEGER t; 
	QueryPerformanceFrequency(&t);
	m_frequency = t.QuadPart;
	BeginFrame();
	EndFrame();
}


Core::~Core()
{
 
}

Core* Core::Instance() // Singleton.  Get an instance
{
	if(_instance == NULL)
	{
		_instance = new Core;
	}
	return _instance;
}



DWORD Core::GetTickCount()
{
	return ::GetTickCount(); // get tick count
}

float Core::GetTimeDelta()
{
	return m_timeD; // time delta
}

void Core::BeginFrame()
{
	// get start time
	LARGE_INTEGER t;
	QueryPerformanceCounter(&t);
	m_LastTick = t.QuadPart;
	return;
}

void Core::EndFrame()
{
	// get end time
	LARGE_INTEGER t;
	QueryPerformanceCounter(&t);
	m_Time = t.QuadPart;
	// and calc time delta
	m_timeD = float((double(m_Time-m_LastTick)/(double)m_frequency));
	return;
}

void Core::SetExecutablePath(char* path)
{
	// set exe path
	strcpy(m_exePath, path);
	return;
}

char* Core::GetExecutablePath() const
{
	//get exe path
	return m_exePath;
}


VOID Core::Function_Enter(char* function)
{
	// start function
	if(m_iStackDepth == 0)
		strcpy(m_function_stack, function);
	else
	{
		strcat(m_function_stack, "->");
		strcat(m_function_stack, function);
	}

	
	++m_iStackDepth;
}

VOID Core::Function_Exit()
{
	// end function
	if(m_iStackDepth == 1)
		*m_function_stack = 0;
	else
	{
		for(int i = strlen(m_function_stack)-2; i > 0; --i)
		{
			if((m_function_stack[i] == '-') && (m_function_stack[i+1] == '>'))
			{
				m_function_stack[i] = 0;
				break;
			}
		}
	}
	--m_iStackDepth;
}

char* Core::GetFunctionStack()
{
	return m_function_stack;
}

CSmartPtr<IMediaObject>& Core::GetMediaObject(char* identifier)
{
	Core::IMediaContainer* temp = m_mediaObjects;
	while(temp)
	{
		if(!temp->object.Null())
		{
			if(strcmp(identifier, temp->object->Identifier()))
				return temp->object;
		}
		temp = temp->next;
	}
	throw "Media Not Found";
}

void Core::RegisterMediaObject(CSmartPtr<IMediaObject>& object)
{
	CSmartPtr<IMediaObject> p;
	try
	{
		p = GetMediaObject(object->Identifier());
	}
	catch(char*)
	{
		Core::IMediaContainer* temp;
		temp = new Core::IMediaContainer;
		temp->object = object;
		temp->next = m_mediaObjects;
		m_mediaObjects = temp;
	}
}

void Core::CleanMediaObjects()
{
	Core::IMediaContainer* temp = m_mediaObjects;
	while(temp)
	{
		if((!temp->object.Null()) || (temp->object.RefCount()==1))
		{
			temp->object.Release();
		}
		temp = temp->next;
	}
}