// DScript.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#include <memmanager.h>

MemManager mem;

inline void* __cdecl operator new(size_t size)
{
	return mem.GimmeMem(size);
}

inline void __cdecl operator delete(void* p)
{
	mem.ReleaseMem(p);
	return;
}

#include "DScript.h"
#include <vector>
#include <cstack.h>

#define MAX_LEN_IN_BYTES 1024
#define LBL_SIZE 1
#define OPCODE_SIZE 2
#define VAR_SIZE 2
#define VAL_SIZE 4

#define NOP				0x0000
#define MOV				0x0001
#define ADD				0x0002
#define MUL				0x0003
#define SUB				0x0004
#define DIV				0x0005
#define MOD				0x0006
#define SHR				0x0007
#define SHL				0x0008
#define CMP				0x0009
#define POPA			0x000A
#define OR				0x000B
#define AND				0x000C
#define XOR				0x000D
#define PUSH			0x000E
#define JMP				0x000F
#define JNZ				0x0010
#define JE				0x0011
#define JNE				0x0012
#define JG				0x0013
#define JGE				0x0014
#define	JL				0x0015
#define JLE				0x0016
#define INC				0x0017
#define DEC				0x0018
#define INTERRUPT		0x0019
#define JZ				0x001A
#define POP				0x001B
#define PEEK			0x001C
#define LA				0x001D
#define ALLOC			0x001E
#define CALL			0x001F
#define END				0x0020
#define RET				0x0021
#define SMOV			0x0022
#define SCAT			0x0023
#define SPUSH			0x0024
#define SPOP			0x0025
#define SINT			0x0026			


typedef INT OPCODE;
typedef INT VARIABLE;
typedef DWORD VALUE;

struct VAR
{
	BOOL isPtr;
	BOOL isString;
	VALUE index;
	VALUE length;
	VALUE* val;
	DWORD* ptr;
};

struct Memory
{
	VAR memChunk[676];

	Memory()
	{
		ZeroMemory(&memChunk, 676*sizeof(VAR));
		for(int i = 0; i < 676; ++i)
			memChunk[i].val = new VALUE;
	}
};

struct Script
{
	DWORD ScriptStartAddr;
	DWORD BaseAddr;
	BYTE* CurrAddr;
	DWORD EndAddr;
	DWORD numOpCodes;
	CStack<VALUE>* Stack;
	CStack<DWORD>* CallStack;
	CStack<float>* FloatStack;
	Memory* MemChunk;
	INT lastCmp;
	BOOL CMPZero;
	BOOL Running;

	~Script()
	{
		for(INT i = 0; i < 676; ++i)
		{
			if(MemChunk)
			{
				if(MemChunk->memChunk[i].isPtr)
				{
					delete ((VOID*)(MemChunk->memChunk[i].ptr));
				}
			}
		}
	}
};



std::vector<IntFunction*> Interrupts;
std::vector<Script*> Scripts;

BOOL CheckHandle(HANDLE t)
{
	if(LOWORD(t) > Scripts.size())
		return FALSE;
	if(Scripts[LOWORD(t)] == NULL)
		return FALSE;
	return TRUE;
}

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

VOID ExecuteNextStatement(Script* script);



RESULT DSCRIPT_API InitDScript()
{
	strcpy(mem.owner, "DSCRIPT.DLL");
	return OK;
}

RESULT DSCRIPT_API DestroyDScript()
{
	for(int i = 0; i < Scripts.size(); ++i)
	{
		if(Scripts[i] != NULL)
			delete Scripts[i];
	}
	return OK;
}

RESULT DSCRIPT_API SetInterrupt(WORD ID, IntFunction* newFunc)
{
	if(ID >= Interrupts.size()) Interrupts.resize(ID+1);
	Interrupts[ID] = newFunc;
	return OK;
}

RESULT DSCRIPT_API LoadScript(HANDLE* scr, char* file)
{
	Script* tmpScript = NULL;
	BYTE* script = NULL;
	BYTE* tempPtr;
	DWORD numOps;
	DWORD lenFunctions;
	FILE* f;
	f = NULL;
	f = fopen(file, "r+b");
	if(!f) goto bail;
	fread(&numOps, 4, 1, f);
	fread(&lenFunctions, 4, 1, f);
	script = new BYTE[numOps*8 + lenFunctions];
	tempPtr = script;
	if(script == NULL)
		return OUTOFMEMORY;
	tmpScript = new Script;
	if(tmpScript == NULL)
		return OUTOFMEMORY;
	tmpScript->MemChunk = new Memory;
	if(tmpScript->MemChunk == NULL)
		goto bail;
	tmpScript->Stack = new CStack<VALUE>;
	if(tmpScript->Stack == NULL)
		goto bail;
	tmpScript->CallStack = new CStack<VALUE>;
	if(tmpScript->CallStack == NULL)
		goto bail;
	if(!f) goto bail;
	tmpScript->ScriptStartAddr  = (DWORD)script;
	tmpScript->numOpCodes = numOps;
	tmpScript->BaseAddr = (DWORD)script + (DWORD)lenFunctions;
	while(!feof(f))
	{
		fread(&tempPtr[0], 1, 1, f);
		++tempPtr;
	}
	tmpScript->EndAddr = (DWORD)tempPtr;
	tmpScript->CurrAddr = (BYTE*)tmpScript->BaseAddr;
	INT i;
	for(i = 0; (i < Scripts.size()) && (Scripts[i] != NULL); ++i);
	if(i == Scripts.size())
	{
		Scripts.resize(i+1);
	}
	Scripts[i] = tmpScript;
	*scr = (HANDLE)MAKELONG(i, 0);
	fclose(f);
	return OK;
bail:
	if(tmpScript != NULL) delete tmpScript;
	return OUTOFMEMORY;

}

RESULT DSCRIPT_API StartScript(HANDLE Script)
{
	if(!CheckHandle(Script))
		return INVALIDHANDLE;

	Scripts[LOWORD(Script)]->Running = TRUE;
	Scripts[LOWORD(Script)]->Stack->push((DWORD)Script);
	return OK;
}

RESULT DSCRIPT_API ResetScript(HANDLE Script)
{
	if(!CheckHandle(Script))
		return INVALIDHANDLE;
	Scripts[LOWORD(Script)]->CurrAddr = (BYTE*)Scripts[LOWORD(Script)]->BaseAddr;
	return OK;
}

RESULT DSCRIPT_API StopScript(HANDLE Script)
{
	if(!CheckHandle(Script))
		return INVALIDHANDLE;

	Scripts[LOWORD(Script)]->Running = FALSE;
	return OK;
}

RESULT DSCRIPT_API UpdateScripts()
{
	for(INT i = 0; i < Scripts.size(); ++i)
	{
		if(Scripts[i] != NULL)
		{
			if(Scripts[i]->Running)
			{
				ExecuteNextStatement(Scripts[i]);
			}
		}
	}
	return OK;
}

RESULT DSCRIPT_API RunScript(HANDLE script)
{
	if(!CheckHandle(script))
		return INVALIDHANDLE;
	INT i = LOWORD(script);
	Script* sc = Scripts[i];
	while((sc->CurrAddr < (BYTE*)sc->EndAddr) && (sc->Running))
	{
		
		ExecuteNextStatement(sc);
	}
	return OK;
}

RESULT DSCRIPT_API CallScriptFunction(HANDLE script, char* function)
{
	Script* tmpScript;
	if(!CheckHandle(script))
		return INVALIDHANDLE;
	tmpScript = Scripts[LOWORD(script)];
	DWORD lenF = strlen(function);
	DWORD match = 0;
	DWORD loc;
	char fn[64];
	strcpy(fn, function);
	char* fniter = fn;
	char* start = (char*)tmpScript->ScriptStartAddr;
	char* end = (char*)tmpScript->BaseAddr;
	char* iter = start;
	while(iter != end)
	{
		if(*fniter == *iter)
			++match;
		else
		{
			match = 0;
			fniter = fn - 1;
		}
		if(match == lenF)
		{
			loc = (*(DWORD*)(iter+2));
			
			break;
		}
		++iter;
		++fniter;
	}
	if(iter == end)
	{
		return NOEQUIV;
	}
	tmpScript->CurrAddr = (BYTE*)(tmpScript->BaseAddr + loc);
	RunScript(script);
	return OK;
}


VOID ExecuteNextStatement(Script* script)
{
#ifdef _DEBUG
	char a[10];
	char b[10];
#endif
	WORD Opcode;
	WORD lhs;
	signed long rhs;
	BOOL rhsIsVar
#ifdef _DEBUG
		= FALSE;
#else
	;
#endif
	Opcode = *(WORD*)script->CurrAddr;
	script->CurrAddr+=2;
	lhs = *(WORD*)script->CurrAddr;
	script->CurrAddr+=2;
	rhs = *(DWORD*)script->CurrAddr;
	script->CurrAddr+=4;
#ifdef _DEBUG
	itoa(lhs, a, 10);
	itoa(rhs, b, 10);
#endif
	switch(Opcode)
	{
	case NOP:
#ifdef _DEBUG
		OutputDebugString("NOP\n");
#endif
		break;
	case MOV:
#ifdef _DEBUG
		OutputDebugString("MOV ");
		OutputDebugString(a), OutputDebugString(" "), OutputDebugString(b), OutputDebugString("\n");
#endif
		//if(HIBYTE(HIWORD(rhs)) == 0xf0)
		//	rhsIsVar = TRUE;
		*script->MemChunk->memChunk[lhs].val = ((rhs >> 24) == 0xf0 ? *script->MemChunk->memChunk[LOWORD(rhs)].val : rhs);
		break;
	case ADD:
#ifdef _DEBUG
		OutputDebugString("ADD ");
		OutputDebugString(a), OutputDebugString(" "), OutputDebugString(b), OutputDebugString("\n");
#endif
		*script->MemChunk->memChunk[lhs].val += ((rhs>> 24) ? *script->MemChunk->memChunk[LOWORD(rhs)].val : rhs);
		
		break;
	case MUL:
#ifdef _DEBUG
		OutputDebugString("MUL ");
		OutputDebugString(a), OutputDebugString(" "), OutputDebugString(b), OutputDebugString("\n");
#endif
		*script->MemChunk->memChunk[lhs].val *= ((rhs >> 24) ? *script->MemChunk->memChunk[LOWORD(rhs)].val : rhs);
		break;
	case SUB:
#ifdef _DEBUG
		OutputDebugString("SUB ");
		OutputDebugString(a), OutputDebugString(" "), OutputDebugString(b), OutputDebugString("\n");
#endif
		*script->MemChunk->memChunk[lhs].val -= ((rhs >> 24) ? *script->MemChunk->memChunk[LOWORD(rhs)].val : rhs);
		break;
	case DIV:
#ifdef _DEBUG
		OutputDebugString("DIV ");
		OutputDebugString(a), OutputDebugString(" "), OutputDebugString(b), OutputDebugString("\n");
#endif
		*script->MemChunk->memChunk[lhs].val /= ((rhs >> 24) ? *script->MemChunk->memChunk[LOWORD(rhs)].val : rhs);
		break;
	case SHR:
#ifdef _DEBUG
		OutputDebugString("SHR ");
		OutputDebugString(a), OutputDebugString(" "), OutputDebugString(b), OutputDebugString("\n");
#endif
		if(HIBYTE(HIWORD(rhs)) == 0xf0)
			rhsIsVar = TRUE;
			*script->MemChunk->memChunk[lhs].val >>= (rhsIsVar ? *script->MemChunk->memChunk[LOWORD(rhs)].val : rhs);
		break;
	case SHL:
#ifdef _DEBUG
		OutputDebugString("SHL ");
		OutputDebugString(a), OutputDebugString(" "), OutputDebugString(b), OutputDebugString("\n");
#endif
		if(HIBYTE(HIWORD(rhs)) == 0xf0)
			rhsIsVar = TRUE;
		*script->MemChunk->memChunk[lhs].val <<= (rhsIsVar ? *script->MemChunk->memChunk[LOWORD(rhs)].val : rhs);
		break;
	case CMP:
#ifdef _DEBUG
		OutputDebugString("CMP ");
		OutputDebugString(a), OutputDebugString(" "), OutputDebugString(b), OutputDebugString("\n");
#endif
		if(*script->MemChunk->memChunk[lhs].val == ((rhs >> 24) ? *script->MemChunk->memChunk[LOWORD(rhs)].val : rhs)) 
			script->lastCmp = 0;
		if(*script->MemChunk->memChunk[lhs].val > ((rhs >> 24) ? *script->MemChunk->memChunk[LOWORD(rhs)].val : rhs)) 
			script->lastCmp = 1;
		if(*script->MemChunk->memChunk[lhs].val < ((rhs >> 24) ? *script->MemChunk->memChunk[LOWORD(rhs)].val : rhs)) 
			script->lastCmp = -1;
		script->CMPZero = *script->MemChunk->memChunk[lhs].val == 0;
		break;
	

	case POPA:
#ifdef _DEBUG
		OutputDebugString("POPA\n");
#endif
		script->Stack->Clear();
		break;
	case OR:
#ifdef _DEBUG
		OutputDebugString("OR ");
		OutputDebugString(a), OutputDebugString(" "), OutputDebugString(b), OutputDebugString("\n");
#endif
		if(HIBYTE(HIWORD(rhs)) == 0xf0)
			rhsIsVar = TRUE;
		*script->MemChunk->memChunk[lhs].val |= (rhsIsVar ? *script->MemChunk->memChunk[LOWORD(rhs)].val : rhs);
		break;
	case AND:
#ifdef _DEBUG
		OutputDebugString("AND ");
		OutputDebugString(a), OutputDebugString(" "), OutputDebugString(b), OutputDebugString("\n");
#endif
		if(HIBYTE(HIWORD(rhs)) == 0xf0)
			rhsIsVar = TRUE;
		*script->MemChunk->memChunk[lhs].val &= (rhsIsVar ? *script->MemChunk->memChunk[LOWORD(rhs)].val : rhs);
		break;
	case XOR:
#ifdef _DEBUG
		OutputDebugString("XOR ");
		OutputDebugString(a), OutputDebugString(" "), OutputDebugString(b), OutputDebugString("\n");
#endif
		if(HIBYTE(HIWORD(rhs)) == 0xf0)
			rhsIsVar = TRUE;
			*script->MemChunk->memChunk[lhs].val ^= (rhsIsVar ? *script->MemChunk->memChunk[LOWORD(rhs)].val : rhs);
		break;
	case PUSH:
#ifdef _DEBUG
		OutputDebugString("PUSH ");
		OutputDebugString(a), OutputDebugString(" "), OutputDebugString(b), OutputDebugString("\n");
#endif
		script->Stack->push(*script->MemChunk->memChunk[lhs].val);
		break;
	case POP:
#ifdef _DEBUG
		OutputDebugString("POP ");
		OutputDebugString(a), OutputDebugString(" "), OutputDebugString(b), OutputDebugString("\n");
#endif
		*script->MemChunk->memChunk[lhs].val = script->Stack->pop();
		break;
	case CALL:
#ifdef _DEBUG
		OutputDebugString("CALL ");
		OutputDebugString(a), OutputDebugString(" "), OutputDebugString(b), OutputDebugString("\n");
#endif
		script->CallStack->push((DWORD)script->CurrAddr);
		script->CurrAddr = (BYTE*)(script->BaseAddr + lhs);
		break;
	case JMP:
#ifdef _DEBUG
		OutputDebugString("JMP ");
		OutputDebugString(a), OutputDebugString(" "), OutputDebugString(b), OutputDebugString("\n");
#endif
		script->CurrAddr = (BYTE*)(script->BaseAddr + lhs);
		break;
	case JNZ:
#ifdef _DEBUG
		OutputDebugString("JNZ ");
		OutputDebugString(a), OutputDebugString(" "), OutputDebugString(b), OutputDebugString("\n");
#endif
		if(!script->CMPZero) script->CurrAddr = (BYTE*)(script->BaseAddr + lhs);
		break;
	case JZ:
#ifdef _DEBUG
		OutputDebugString("JZ ");
		OutputDebugString(a), OutputDebugString(" "), OutputDebugString(b), OutputDebugString("\n");
#endif
		if(script->CMPZero) script->CurrAddr = (BYTE*)(script->BaseAddr + lhs);
		break;
	case JE:
#ifdef _DEBUG
		OutputDebugString("JE ");
		OutputDebugString(a), OutputDebugString(" "), OutputDebugString(b), OutputDebugString("\n");
#endif
		if(script->lastCmp == 0) script->CurrAddr = (BYTE*)(script->BaseAddr + lhs);
		break;
	case JNE:
#ifdef _DEBUG
		OutputDebugString("JNE ");
		OutputDebugString(a), OutputDebugString(" "), OutputDebugString(b), OutputDebugString("\n");
#endif
		if(script->lastCmp != 0) script->CurrAddr = (BYTE*)(script->BaseAddr + lhs);
		break;
	case JG:
#ifdef _DEBUG
		OutputDebugString("JG ");
		OutputDebugString(a), OutputDebugString(" "), OutputDebugString(b), OutputDebugString("\n");
#endif
		if(script->lastCmp == 1) script->CurrAddr = (BYTE*)(script->BaseAddr + lhs);
		break;
	case JGE:
#ifdef _DEBUG
		OutputDebugString("JGE ");
		OutputDebugString(a), OutputDebugString(" "), OutputDebugString(b), OutputDebugString("\n");
#endif
		if((script->lastCmp == 0) || (script->lastCmp == 1)) script->CurrAddr = (BYTE*)(script->BaseAddr + lhs);
		break;
	case JL:
#ifdef _DEBUG
		OutputDebugString("JL ");
		OutputDebugString(a), OutputDebugString(" "), OutputDebugString(b), OutputDebugString("\n");
#endif
		if(script->lastCmp == -1) script->CurrAddr = (BYTE*)(script->BaseAddr + lhs);
		break;
	case JLE:
#ifdef _DEBUG
		OutputDebugString("JLE ");
		OutputDebugString(a), OutputDebugString(" "), OutputDebugString(b), OutputDebugString("\n");
#endif
		if((script->lastCmp == 0) || (script->lastCmp == -1)) script->CurrAddr = (BYTE*)(script->BaseAddr + lhs);
		break;
	case INC:
#ifdef _DEBUG
		OutputDebugString("INC ");
		OutputDebugString(a), OutputDebugString(" "), OutputDebugString(b), OutputDebugString("\n");
#endif
			++(*script->MemChunk->memChunk[lhs].val);
		break;
	case DEC:
#ifdef _DEBUG
		OutputDebugString("DEC ");
		OutputDebugString(a), OutputDebugString(" "), OutputDebugString(b), OutputDebugString("\n");
#endif
		--(*script->MemChunk->memChunk[lhs].val);
		break;
	case INTERRUPT:
#ifdef _DEBUG
		OutputDebugString("INT ");
		OutputDebugString(a), OutputDebugString(" "), OutputDebugString(b), OutputDebugString("\n");
#endif
		(Interrupts[lhs])(script->Stack);
		break;
	case PEEK:
#ifdef _DEBUG
		OutputDebugString("PEEK ");
		OutputDebugString(a), OutputDebugString(" "), OutputDebugString(b), OutputDebugString("\n");
#endif
		*script->MemChunk->memChunk[lhs].val = script->Stack->peek();
		break;
	case ALLOC:
#ifdef _DEBUG
		OutputDebugString("ALLOC ");
		OutputDebugString(a), OutputDebugString(" "), OutputDebugString(b), OutputDebugString("\n");
#endif
		if(HIBYTE(HIWORD(rhs)) == 0xf0)
			rhsIsVar = TRUE;
		if(script->MemChunk->memChunk[lhs].isPtr)
			delete script->MemChunk->memChunk[lhs].ptr;
		else delete script->MemChunk->memChunk[lhs].val;
		script->MemChunk->memChunk[lhs].isPtr = TRUE;
		script->MemChunk->memChunk[lhs].ptr = new DWORD[(rhsIsVar ? *script->MemChunk->memChunk[LOWORD(rhs)].val : rhs)];
		if(script->MemChunk->memChunk[lhs].ptr == NULL)
		{
#ifdef _DEBUG
			OutputDebugString("ALLOC:  Out of Memory!\n");
#endif
			script->Running = FALSE;
			break;
		}

		script->MemChunk->memChunk[lhs].length = (rhsIsVar ? *script->MemChunk->memChunk[LOWORD(rhs)].val : rhs);
		script->MemChunk->memChunk[lhs].index = 0;
		script->MemChunk->memChunk[lhs].val = &script->MemChunk->memChunk[lhs].ptr[0];
		break;
	case LA:
#ifdef _DEBUG
		OutputDebugString("LA ");
		OutputDebugString(a), OutputDebugString(" "), OutputDebugString(b), OutputDebugString("\n");
#endif
		if(script->MemChunk->memChunk[lhs].isPtr != TRUE)
			break;
		if(HIBYTE(HIWORD(rhs)) == 0xf0)
			rhsIsVar = TRUE;
		script->MemChunk->memChunk[lhs].index = (rhsIsVar ? *script->MemChunk->memChunk[LOWORD(rhs)].val : rhs);
		script->MemChunk->memChunk[lhs].val = &script->MemChunk->memChunk[lhs].ptr[script->MemChunk->memChunk[lhs].index];
		break;
	case END:
#ifdef _DEBUG
		OutputDebugString("END\n");
#endif
		script->Running = FALSE;
		break;
	case RET:
#ifdef _DEBUG
		OutputDebugString("RET\n");
#endif
		if(script->CallStack->Items() == 0)
		{
			script->Running = FALSE;
			break;
		}
		script->CurrAddr = (BYTE*)script->CallStack->pop();
		break;
	case SMOV:
#ifdef _DEBUG
		OutputDebugString("SMOV : Opcode not Implemented\n");
#endif

		break;
	case SCAT:
#ifdef _DEBUG
		OutputDebugString("SCAT : Opcode not Implemented\n");
#endif
		break;
	case SPUSH:
#ifdef _DEBUG
		OutputDebugString("SPUSH : Opcode not Implemented\n");
#endif
		break;
	case SPOP:
#ifdef _DEBUG
		OutputDebugString("SPOP : Opcode not Implemented\n");
#endif
		break;
	case SINT:
#ifdef _DEBUG
		OutputDebugString("SINT : Opcode not Implemented\n");
#endif
		break;
	}
	//script->CurrAddr +=8;
	return;
}
