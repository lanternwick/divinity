#ifndef _POSTOFFICE_H
#define _POSTOFFICE_H

#include <CLogFile.h>
#include "..\ui\messages.h"


struct MWrapper
{
	HANDLE src;
	HANDLE dest;
	DWORD time;
	DMSG msg;
};

class PostOffice
{
public:
	PostOffice();
	~PostOffice();

	RESULT SendMessage(HANDLE src, HANDLE dest, DMSG& msg);
	RESULT SendDelayedMessage(HANDLE src, HANDLE dest, DMSG& msg, DWORD tickToDeliver);
	RESULT Update();
	HMODULE m;
private:
	std::vector<MWrapper> DelayedMessages;
	CLogFile* log;
};

PostOffice::PostOffice()
{
	log = new CLogFile("logs\\message.towlog");
}

PostOffice::~PostOffice()
{

}

RESULT PostOffice::SendMessage(HANDLE src, HANDLE dest, DMSG& msg)
{
	CObject* Obj = NULL;
	CObject* Obj2 = NULL;
	char* buff = new char[256];
	char* msgText = new char[64];
	char* n = new char[128];
	char* n2 = new char[128];
	GetObjectPointer(m, dest, OBJ_ANY, &Obj);
	if(!Obj)
		return INVALIDHANDLE;
	GetObjectPointer(m, src, OBJ_ANY, &Obj2);
	msg.src = src;
	Obj->SendMessage(msg);
	if(Obj2 == NULL)
		return OK;
	UIGetMessageText(msg.Message, &msgText);
	/*sprintf(buff, "Message sent from %s to %s\n\tMessage: %s\n\tParam1: %i\n\tParam2: %i\n\tParam3: %i\n", 
				  Obj2->Name, Obj->Name, msgText, msg.param1, msg.param2, msg.param3);*/
	if(!n)
		return OUTOFMEMORY;
	if(!n2)
		return OUTOFMEMORY;
	if(!buff)
		return OUTOFMEMORY;
	if(!msgText)
		return OUTOFMEMORY;
	strcpy(n, Obj2->Name);
	strcpy(n2, Obj->Name);
	for(INT i = 0; i < strlen(n); ++i)
	{
		if(n[i] == '\n') n[i] = ' ';
	}
	for(i = 0; i < strlen(n2); ++i)
	{
		if(n2[i] == '\n') n2[i] = ' ';
	}
	sprintf(buff, "RIM#%s#%s#%s#%i#%i#%i\n", n, n2, msgText, msg.param1, msg.param2, msg.param3);
	if(msg.Message != DM_UPDATETICK)
		log->OutputData(buff);
	delete buff;
	delete msgText;
	delete n;
	delete n2;
	return OK;
}

RESULT PostOffice::SendDelayedMessage(HANDLE src, HANDLE dest, DMSG& msg, DWORD tickToDeliver)
{
	CObject* c = NULL;
	CObject* c2 = NULL;
	char* buff = new char[128];
	char* n = new char[128];
	char* n2 = new char[128];
	MWrapper ms;
	ms.src = src;
	ms.dest = dest;
	ms.msg = msg;
	ms.time = tickToDeliver;
	DelayedMessages.push_back(ms);
	GetObjectPointer(m, src, OBJ_ANY, &c);
	if(c == NULL)
		return OK;
	GetObjectPointer(m, dest, OBJ_ANY, &c2);
	if(c2 == NULL)
		return OK;
	//sprintf(buff, "Message Recieved from %s, to %s, delivered at %i\n", c->Name, c2->Name, tickToDeliver);
	strcpy(n, c->Name);
	strcpy(n2, c2->Name);
	for(INT i = 0; i < strlen(n); ++i)
	{
		if(n[i] == '\n') n[i] = ' ';
	}
	for(i = 0; i < strlen(n2); ++i)
	{
		if(n2[i] == '\n') n2[i] = ' ';
	}
	sprintf(buff, "RDM#%s#%s#%i\n", n, n2, tickToDeliver);
	log->OutputData(buff);
	delete buff;
	delete n;
	delete n2;
	return OK;
}

RESULT PostOffice::Update()
{
	DWORD time = GetTicks();
	char* buff = new char[128];
	sprintf(buff, "%i\n", time);
	if(time % 1000 == 0)
		log->Reset();
	log->OutputData(buff);
	for(INT i = 0; i < DelayedMessages.size(); ++i)
	{
		if(DelayedMessages[i].time <= time)
		{
			this->SendMessage(DelayedMessages[i].src, DelayedMessages[i].dest, DelayedMessages[i].msg);
			DelayedMessages[i] = DelayedMessages[DelayedMessages.size()-1];
			DelayedMessages.resize(DelayedMessages.size() - 1);
		}
	}
	delete buff;
	return OK;
}

#endif