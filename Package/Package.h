
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the PACKAGE_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// PACKAGE_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef PACKAGE_EXPORTS
#define PACKAGE_API __declspec(dllexport)
#else
#define PACKAGE_API __declspec(dllimport)
#endif

#include <result.h>

typedef BOOL EnumFunc(LPSTR filename, DWORD filesize);

RESULT PACKAGE_API OpenPackage(char* file);
RESULT PACKAGE_API NewPackage(char* file);
RESULT PACKAGE_API AddFileToPackage(char* file, char* dir);
RESULT PACKAGE_API ExtractFile(char* to, UINT id);
RESULT PACKAGE_API DeleteFileFromPackage(UINT id);
RESULT PACKAGE_API ExtractFileByName(char* to, char* name);
RESULT PACKAGE_API DeleteFileByName(char* name);
RESULT PACKAGE_API ClosePackage();
BOOL   PACKAGE_API PackageIsOpen();
DWORD  PACKAGE_API EnumFiles(EnumFunc* funct);
RESULT PACKAGE_API ExtractAllFiles(char* dir);
RESULT PACKAGE_API GetNumFiles(DWORD* numFiles);