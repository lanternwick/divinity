

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
float Core::m_game_speed = 1.0f;
float Core::m_time = 0.0f;
char* Core::m_exePath = NULL;
char* Core::m_function_stack = NULL;
unsigned int Core::m_iStackDepth = 0;
Log Core::m_log;
std::vector<Core::IMC> Core::m_mediaObjects;
std::list<ITimer*> Core::timers;
std::vector<IDestroyable*> Core::m_to_be_destroyed;
int Core::debug_level=0;
mutex Core::m_lock;

Core::Core()
{
	// allocate size for the executable path
	
	// get Performance Counter frequency
	LARGE_INTEGER t; 
	QueryPerformanceFrequency(&t);
	m_frequency = t.QuadPart;
	m_exePath = new char[128];
	m_function_stack = new char[FUNCTION_STACK_LEN];
	BeginFrame();
	EndFrame();
}


Core::~Core()
{
	delete m_exePath;
	delete m_function_stack;
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

float Core::GetTime()
{
	return m_time;
}

float Core::GetTimeDelta()
{
	return m_timeD * m_game_speed; // time delta * game speed
}

float Core::GetRawTimeDelta()
{
	return m_timeD;
}

void Core::SetGameSpeed(float speed)
{
	m_game_speed = speed;
}

float Core::GetGameSpeed()
{
	return m_game_speed;
}

void Core::BeginFrame()
{
	// get start time
	LARGE_INTEGER t;
	m_lock.lock();
	QueryPerformanceCounter(&t);
	m_LastTick = t.QuadPart;
	m_lock.unlock();
	return;
}

void Core::EndFrame()
{
	// get end time
	LARGE_INTEGER t;
	m_lock.lock();
	for(std::list<ITimer*>::iterator i = timers.begin(); i != timers.end(); i++)
	{
		(*i)->On_Timer(m_time);
	}
	
	QueryPerformanceCounter(&t);
	m_Time = t.QuadPart;
	// and calc time delta
	m_timeD = float((double(m_Time-m_LastTick)/(double)m_frequency));
	m_time += m_timeD;
	m_lock.unlock();
	return;
}

void Core::SetExecutablePath(char* path)
{
	// set exe path
	m_lock.lock();
	strcpy(m_exePath, path);
	m_lock.unlock();
	return;
}

char* Core::GetExecutablePath()
{
	//get exe path
	return m_exePath;
}


VOID Core::Function_Enter(char* function)
{
#ifdef STACK_TRACE
	// start function
	m_lock.lock();
	if(m_iStackDepth == 0)
		strcpy(m_function_stack, function);
	else
	{
		strcat(m_function_stack, "->");
		strcat(m_function_stack, function);
	}

	
	++m_iStackDepth;
	m_lock.unlock();
#endif
}

VOID Core::Function_Exit()
{
#ifdef STACK_TRACE
	// end function
	m_lock.lock();
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
	m_lock.unlock();
#endif
}

char* Core::GetFunctionStack()
{
	return m_function_stack;
}

IMediaObject* Core::GetMediaObject(moniker& identifier)
{
	m_lock.lock();
	for(std::vector<IMC>::iterator i = m_mediaObjects.begin(); i != m_mediaObjects.end(); i++)
	{
		if(identifier() == (*i).object->Identifier()())
		{
			m_lock.unlock();
			return (*i).object;
		}
	}
	m_lock.unlock();
	return NULL;
}


bool Core::RegisterMediaObject(IMediaObject* object)
{
	m_lock.lock();
	IMediaObject* p;
	p = GetMediaObject(object->Identifier());
	Core::IMC temp;
	if(!p)
	{
		
		
		temp.object = object;
		m_mediaObjects.push_back(temp);
		m_lock.unlock();
		return true;
	}
	m_lock.unlock();
	return false;
}

void Core::RemoveMediaObject(moniker& id, bool free)
{
	m_lock.lock();
	for(std::vector<IMC>::iterator i = m_mediaObjects.begin(); i != m_mediaObjects.end(); i++)
	{
		if((*i).object->Identifier()() == id())
		{
			if(free)
			{
				(*i).object->Release();
				delete (*i).object;
			}
			(*i) = m_mediaObjects.back();
			m_mediaObjects.pop_back();
			break;
		}
	}
	m_lock.unlock();
	return;
	
	/*Core::IMediaContainer* temp = m_mediaObjects;
	Core::IMediaContainer* temp2 = NULL;
	while(temp)
	{
		if(temp->object)
		{
			if(id() == temp->object->Identifier()())
			{
				if(temp2 == NULL)
				{
					if(free) 
						delete temp->object;
					m_mediaObjects = temp->next;
					delete temp;
					return;
				}
				else
				{
					temp2->next = temp->next;
					if(free) 
						delete temp->object;
					temp->object = NULL;
					delete temp;
					return;
				}
			}
		}
		temp2 = temp;
		temp = temp->next;
	}
	return;*/
	
}

void Core::WriteLog(const char* log_string, long reserved)
{
	m_lock.lock();
	if(debug_level >= reserved)
		m_log.Write(log_string);
	m_lock.unlock();
	return;
	
}

const char* Core::ReadLog(int reserved)
{
	return m_log.Contents(reserved);
}

void Core::ClearLog()
{
	m_lock.lock();
	m_log.clear();
	m_lock.unlock();
}

void Core::SetDebugLogLevel(int level)
{
	debug_level = level;
}

void Core::AddTimer(ITimer* timer)
{
	m_lock.lock();
	if(!TimerExists(timer))
		timers.push_back(timer);
	m_lock.unlock();
}

void Core::RemoveTimer(ITimer* timer)
{
	m_lock.lock();
	if(TimerExists(timer))
		timers.remove(timer);
	m_lock.unlock();
}

bool Core::TimerExists(ITimer* timer)
{
	m_lock.lock();
	std::list<ITimer*>::iterator i;
	for(i = timers.begin(); i != timers.end(); i++)
	{
		if((*i) == timer) 
		{
			m_lock.unlock();
			return true;
		}
	}
	m_lock.unlock();
	return false;
}

void Core::ResetTime()
{
	m_time = 0.0f;
}

#ifdef _DEBUG

void Core::DebugDump()
{
	std::vector<IMC>::iterator i;
	std::stringstream ss;
	for(i = m_mediaObjects.begin(); i != m_mediaObjects.end(); i++)
	{
		ss << (*i).object->Identifier()();
	}
	ss << std::endl;
	WriteLog(ss.str().c_str(), 1);
}

#endif

void Core::Destroy(IDestroyable* obj)
{
	m_lock.lock();
	bool b = obj->Destroy(m_time);
	if(!b)
	{
		delete obj;
		return;
	}
	m_to_be_destroyed.push_back(obj);
	m_lock.unlock();
}