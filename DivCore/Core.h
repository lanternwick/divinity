#ifndef _CORE_H
#define _CORE_H

#include "..\Common\RESULT.h"
#include "..\Common\IMediaObject.h"
#include <string>

#define FUNCTION_STACK_LEN 4096

class _API Core
{
public:
	
	~Core();

	static Core* Instance();

	DWORD GetTickCount();
	float GetTimeDelta();
	void BeginFrame();
	void EndFrame();
	void SetExecutablePath(char* path);
	char* GetExecutablePath() const;
	static VOID Function_Enter(char* function);
	static VOID Function_Exit();
	static char* GetFunctionStack();
	CSmartPtr<IMediaObject>& GetMediaObject(char* identifier);
	void RegisterMediaObject(CSmartPtr<IMediaObject>& object);
	void CleanMediaObjects();
	//IWorldObject* GetWorldObject(char* identifier);
	//IUIObject* GetUIObject(char* identifier);
protected:
	Core();
	static Core* _instance;
	static __int64 m_Time;
	static __int64 m_LastTick;
	static __int64 m_frequency;
	static float m_timeD;
	static char* m_exePath;
	static char* m_function_stack;
	static unsigned int m_iStackDepth;

	struct IMediaContainer
	{
		IMediaContainer* next;
		CSmartPtr<IMediaObject> object;
		IMediaContainer() {next = NULL;}
	};
	IMediaContainer* m_mediaObjects;

};

LPVOID _API GimmeMem(size_t size);
VOID _API FreeMem(void* p);


#endif