
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the EFFECTS_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// EFFECTS_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef EFFECTS_EXPORTS
#define EFFECTS_API __declspec(dllexport)
#else
#define EFFECTS_API __declspec(dllimport)
#endif



struct PARTICLE
{
	VECTOR myPos;
	VECTOR myVel;
	DWORD myBirth;
	DWORD myLifeTime;
	DWORD color;
	DWORD StartColor;
	DWORD EndColor;
	float StartSize;
	float EndSize;
	float mySize;
	BOOL alive;
};

typedef VOID InitPointProc(PARTICLE* p);
typedef VOID UpdateFunction(PARTICLE* p, INT numParticles, VECTOR* basePos, INT* ret, DWORD param, HANDLE emitter);

RESULT EFFECTS_API ConstructParticleSystem(HANDLE emitter, INT numParticles, VECTOR* Pos, UpdateFunction* ufunc, HANDLE texture, HANDLE attach, DWORD param, BOOL deleteOnDead, HANDLE* PSys);
RESULT EFFECTS_API ConstructEmitter(HANDLE* emitter, BOOL is3D);
BOOL   EFFECTS_API CanInitParticle(HANDLE emitter);

