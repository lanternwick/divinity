
#ifdef _EXPORTS
#define _API __declspec(dllexport)
#else
#define _API __declspec(dllimport)
#endif

#include "CEngine.h"