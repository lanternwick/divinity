
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SOUND_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SOUND_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef SOUND_EXPORTS
#define SOUND_API __declspec(dllexport)
#else
#define SOUND_API __declspec(dllimport)
#endif

#include <result.h>

RESULT SOUND_API InitSound();
RESULT SOUND_API LoadSegment(HANDLE* Segment, char* Filename);
RESULT SOUND_API PlaySegmentAsMusic(HANDLE Segment);
RESULT SOUND_API PlaySegmentAsSndFX(HANDLE Segment);