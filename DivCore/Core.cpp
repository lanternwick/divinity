

#include "stdafx.h"

#include "DivCore.h"
#include "core.h"
#include "malloc.h"
LPVOID __declspec(dllexport) GimmeMem(size_t size)
{
	void* m = malloc(size);
	if(size == 80)
	{
		m = m;
	}
	if((unsigned long(m) == 0x001A300F8))
	{
		m = m;
	}
	return m;
}

VOID __declspec(dllexport) FreeMem(void* p)
{
	free(p);
	return;
}

Core* Core::_instance = NULL;


Core::Core()
{
	// allocate size for the executable path
	m_Time = 0;
	m_LastTick = 0;
	m_frequency = 0;
	m_timeD = 0.0f;
	m_game_speed = 1.0f;
	m_time = 0.0f;
	m_exePath = NULL;
	m_function_stack = NULL;
	m_iStackDepth = 0;
	// get Performance Counter frequency
	
}


Core::~Core()
{
	if(m_log)
		delete m_log;
	m_log = NULL;
	if(m_exePath)
		delete m_exePath;
	m_exePath = NULL;
	if(m_function_stack)
		delete m_function_stack;
	m_function_stack = NULL;
}

Core* Core::Instance() // Singleton.  Get an instance
{
	return _instance;
}

void Core::Init()
{
	_instance = new Core();
	Core* c = Core::Instance();
	if(c->m_frequency)
	{
		return;
	}
	
	c->m_log = new Log;
	
	
	
	c->debug_level = 0;
	
	LARGE_INTEGER t; 
	QueryPerformanceFrequency(&t);
	c->m_frequency = t.QuadPart;
	c->m_exePath = new char[128];
	c->m_function_stack = new char[FUNCTION_STACK_LEN];
	c->BeginFrame();
	c->EndFrame();
}

void Core::Destroy()
{
	Core* c = Core::Instance();
	delete c->_instance;
	
}

DWORD Core::GetTickCount()
{
	return ::GetTickCount(); // get tick count
}

float Core::GetTime()
{
	Core* c = Core::Instance();
	return c->m_time;
}

float Core::GetTimeDelta()
{
	Core* c = Core::Instance();
	return c->m_timeD * c->m_game_speed; // time delta * game speed
}

float Core::GetRawTimeDelta()
{
	Core* c = Core::Instance();
	return c->m_timeD;
}

void Core::SetGameSpeed(float speed)
{
	Core* c = Core::Instance();
	c->m_game_speed = speed;
}

float Core::GetGameSpeed()
{
	Core* c = Core::Instance();
	return c->m_game_speed;
}

void Core::BeginFrame()
{
	Core* c = Core::Instance();
	// get start time
	LARGE_INTEGER t;
	c->m_lock.lock();
	QueryPerformanceCounter(&t);
	c->m_LastTick = t.QuadPart;
	c->m_lock.unlock();
	return;
}

void Core::EndFrame()
{
	Core* c = Core::Instance();
	// get end time
	LARGE_INTEGER t;
	c->m_lock.lock();
	for(std::list<ITimer*>::iterator i = c->timers.begin(); i != c->timers.end(); i++)
	{
		(*i)->On_Timer(c->m_time);
	}
	
	QueryPerformanceCounter(&t);
	c->m_Time = t.QuadPart;
	// and calc time delta
	c->m_timeD = float((double(c->m_Time-c->m_LastTick)/(double)c->m_frequency));
	c->m_time += c->m_timeD;
	c->m_lock.unlock();
	return;
}

void Core::SetExecutablePath(char* path)
{
	Core* c = Core::Instance();
	// set exe path
	c->m_lock.lock();
	strcpy(c->m_exePath, path);
	c->m_lock.unlock();
	return;
}

char* Core::GetExecutablePath()
{
	Core* c = Core::Instance();
	//get exe path
	return c->m_exePath;
}


VOID Core::Function_Enter(char* function)
{
#ifdef STACK_TRACE
	Core* c = Core::Instance();
	// start function
	c->m_lock.lock();
	if(c->m_iStackDepth == 0)
		strcpy(c->m_function_stack, function);
	else
	{
		strcat(c->m_function_stack, "->");
		strcat(c->m_function_stack, function);
	}

	
	++c->m_iStackDepth;
	c->m_lock.unlock();
#endif
}

VOID Core::Function_Exit()
{
#ifdef STACK_TRACE
	Core* c = Core::Instance();
	// end function
	c->m_lock.lock();
	if(c->m_iStackDepth == 0)
		throw "up";
	if(c->m_iStackDepth == 1)
		*(c->m_function_stack) = 0;
	else
	{
		for(int i = strlen(c->m_function_stack)-2; i > 0; --i)
		{
			if((c->m_function_stack[i] == '-') && (c->m_function_stack[i+1] == '>'))
			{
				c->m_function_stack[i] = 0;
				break;
			}
		}
	}
	--c->m_iStackDepth;
	c->m_lock.unlock();
#endif
}

char* Core::GetFunctionStack()
{
	Core* c = Core::Instance();
	return c->m_function_stack;
}

IMediaObject* Core::GetMediaObject(moniker& identifier)
{
	Core* c = Core::Instance();
	c->m_lock.lock();
	for(std::vector<IMC>::iterator i = c->m_mediaObjects.begin(); i != c->m_mediaObjects.end(); i++)
	{
		if(identifier() == (*i).object->Identifier()())
		{
			c->m_lock.unlock();
			return (*i).object;
		}
	}
	c->m_lock.unlock();
	return NULL;
}


bool Core::RegisterMediaObject(IMediaObject* object)
{
	Core* c = Core::Instance();
	c->m_lock.lock();
	IMediaObject* p;
	p = GetMediaObject(object->Identifier());
	Core::IMC temp;
	if(!p)
	{
		
		
		temp.object = object;
		c->m_mediaObjects.push_back(temp);
		c->m_lock.unlock();
		return true;
	}
	c->m_lock.unlock();
	return false;
}

void Core::RemoveMediaObject(moniker& id, bool free)
{
	Core* c = Core::Instance();
	c->m_lock.lock();
	for(std::vector<IMC>::iterator i = c->m_mediaObjects.begin(); i != c->m_mediaObjects.end(); i++)
	{
		if((*i).object->Identifier()() == id())
		{
			if(free)
			{
				(*i).object->Release();
				delete (*i).object;
			}
			(*i) = c->m_mediaObjects.back();
			c->m_mediaObjects.pop_back();
			break;
		}
	}
	c->m_lock.unlock();
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
	Core* c = Core::Instance();
	c->m_lock.lock();
	if(c->debug_level >= reserved)
		c->m_log->Write(log_string);
	c->m_lock.unlock();
	return;
	
}

const char* Core::ReadLog(int reserved)
{
	Core* c = Core::Instance();
	return c->m_log->Contents(reserved);
}

void Core::ClearLog()
{
	Core* c = Core::Instance();
	c->m_lock.lock();
	c->m_log->clear();
	c->m_lock.unlock();
}

void Core::SetDebugLogLevel(int level)
{
	Core* c = Core::Instance();
	c->debug_level = level;
}

void Core::AddTimer(ITimer* timer)
{
	Core* c = Core::Instance();
	c->m_lock.lock();
	if(!TimerExists(timer))
		c->timers.push_back(timer);
	c->m_lock.unlock();
}

void Core::RemoveTimer(ITimer* timer)
{
	Core* c = Core::Instance();
	c->m_lock.lock();
	if(TimerExists(timer))
		c->timers.remove(timer);
	c->m_lock.unlock();
}

bool Core::TimerExists(ITimer* timer)
{
	Core* c = Core::Instance();
	c->m_lock.lock();
	std::list<ITimer*>::iterator i;
	for(i = c->timers.begin(); i != c->timers.end(); i++)
	{
		if((*i) == timer) 
		{
			c->m_lock.unlock();
			return true;
		}
	}
	c->m_lock.unlock();
	return false;
}

void Core::ResetTime()
{
	Core* c = Core::Instance();
	c->m_time = 0.0f;
}

#ifdef _DEBUG

void Core::DebugDump()
{
	/*Core* c = Core::Instance();
	std::vector<IMC>::iterator i;
	std::stringstream ss;
	for(i = m_mediaObjects.begin(); i != m_mediaObjects.end(); i++)
	{
		ss << (*i).object->Identifier()();
	}
	ss << std::endl;
	WriteLog(ss.str().c_str(), 1);*/
}

#endif

void Core::Destroy(IDestroyable* obj)
{
	Core* c = Core::Instance();
	c->m_lock.lock();
	bool b = obj->Destroy(c->m_time);
	if(!b)
	{
		delete obj;
		c->m_lock.unlock();
		return;
	}
	c->m_to_be_destroyed.push_back(obj);
	c->m_lock.unlock();
}