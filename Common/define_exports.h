#pragma warning (disable:4231)
#ifdef _EXPORTS
	#define _API __declspec(dllexport)
	#define _EXTERN_
#else
	#define _API __declspec(dllimport)
	#define _EXTERN_ extern
#endif