#ifndef _CORE_H
#define _CORE_H

#include "..\Common\RESULT.h"
#include "..\Common\IMediaObject.h"
#include "..\Common\IDestroyable.h"
#include "log.h"
#include <list>
#include <vector>
#include "ITimer.h"
#include "..\Common\custmoniker.h"
#include <string>
#include <sstream>
#include "mutex.h"
#define FUNCTION_STACK_LEN 4096

#pragma warning(disable : 4251)

class _API Core
{
public:
	
	~Core();

	static Core* Instance();
	static void Init();
	static void Destroy();
	static DWORD GetTickCount();
	static float GetTimeDelta();
	static float GetRawTimeDelta();
	static void SetGameSpeed(float speed);
	static float GetGameSpeed();
	static float GetTime();
	static void BeginFrame();
	static void EndFrame();
	static void SetExecutablePath(char* path);
	static char* GetExecutablePath();
	static VOID Function_Enter(char* function);
	static VOID Function_Exit();
	static char* GetFunctionStack();
	static void WriteLog(const char* log_string, long reserved);
	static void ClearLog();
	static const char* ReadLog(int reserved);
	static void SetDebugLogLevel(int level);
	static IMediaObject* GetMediaObject(moniker& identifier);
	static bool RegisterMediaObject(IMediaObject* object);
	static void RemoveMediaObject(moniker& id, bool free=false);
	//IWorldObject* GetWorldObject(char* identifier);
	//IUIObject* GetUIObject(char* identifier);


	static void AddTimer(ITimer* timer);
	static void RemoveTimer(ITimer* timer);
	static bool TimerExists(ITimer* timer);
	static void ResetTime();

	static void Destroy(IDestroyable* obj);

#ifdef _DEBUG

	static void DebugDump();

#endif

protected:
	Core();
	static Core* _instance;
	__int64 m_Time;
	__int64 m_LastTick;
	__int64 m_frequency;
	float m_timeD;
	float m_game_speed;
	char* m_exePath;
	char* m_function_stack;
	unsigned int m_iStackDepth;
	int debug_level;
	struct IMC
	{
		IMediaObject* object;
		bool operator == (const IMC rhs)
		{
			return object == rhs.object;
		}
	};
	std::vector<IMC> m_mediaObjects;
	Log* m_log;
	std::list<ITimer*> timers;
	float m_time;
	std::vector<IDestroyable*> m_to_be_destroyed;
	mutex m_lock;
};

LPVOID _API GimmeMem(size_t size);
VOID _API FreeMem(void* p);

inline void* operator new(size_t size)
{
	return GimmeMem(size);
}

inline void operator delete(void* p)
{
	FreeMem(p);
	return;
}


#include "log_stream.h"

#endif