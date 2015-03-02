# Microsoft Developer Studio Project File - Name="DivCore" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=DivCore - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DivCore.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DivCore.mak" CFG="DivCore - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DivCore - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "DivCore - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/DivCore", PAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DivCore - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DIVCORE_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy release\divcore.dll "c:\development\current\copy (2) of divinity\release\core\divcore.dll"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "DivCore - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DIVCORE_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_EXPORTS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy debug\divcore.dll ..\..\bin\ext\divcore.ext	copy debug\divcore.dll ..\..\bin\ext\divcore.dll	copy debug\divcore.dll ..\DivShell\divcore.dll
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "DivCore - Win32 Release"
# Name "DivCore - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Core.cpp

!IF  "$(CFG)" == "DivCore - Win32 Release"

!ELSEIF  "$(CFG)" == "DivCore - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DivCore.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\Common\IMediaObject.cpp

!IF  "$(CFG)" == "DivCore - Win32 Release"

!ELSEIF  "$(CFG)" == "DivCore - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\Common\IObject.cpp

!IF  "$(CFG)" == "DivCore - Win32 Release"

!ELSEIF  "$(CFG)" == "DivCore - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Log.cpp

!IF  "$(CFG)" == "DivCore - Win32 Release"

!ELSEIF  "$(CFG)" == "DivCore - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\mutex.cpp

!IF  "$(CFG)" == "DivCore - Win32 Release"

!ELSEIF  "$(CFG)" == "DivCore - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\Common\RESULT.cpp

!IF  "$(CFG)" == "DivCore - Win32 Release"

!ELSEIF  "$(CFG)" == "DivCore - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\thread.cpp

!IF  "$(CFG)" == "DivCore - Win32 Release"

!ELSEIF  "$(CFG)" == "DivCore - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\Common\boolean_state.h
# End Source File
# Begin Source File

SOURCE=.\Core.h
# End Source File
# Begin Source File

SOURCE=..\Common\custmoniker.h
# End Source File
# Begin Source File

SOURCE=..\Common\define_exports.h
# End Source File
# Begin Source File

SOURCE=.\DivCore.h
# End Source File
# Begin Source File

SOURCE=..\Common\g_block.h
# End Source File
# Begin Source File

SOURCE=..\Common\g_deparser.h
# End Source File
# Begin Source File

SOURCE=..\Common\g_exception.h
# End Source File
# Begin Source File

SOURCE=..\Common\g_input.h
# End Source File
# Begin Source File

SOURCE=..\Common\g_output.h
# End Source File
# Begin Source File

SOURCE=..\Common\g_parser.h
# End Source File
# Begin Source File

SOURCE=..\Common\IDestroyable.h
# End Source File
# Begin Source File

SOURCE=..\Common\IMediaObject.h
# End Source File
# Begin Source File

SOURCE=..\Common\IObject.h
# End Source File
# Begin Source File

SOURCE=.\ITimer.h
# End Source File
# Begin Source File

SOURCE=..\Common\IWorldObject.h
# End Source File
# Begin Source File

SOURCE=.\Log.h
# End Source File
# Begin Source File

SOURCE=.\log_stream.h
# End Source File
# Begin Source File

SOURCE=.\log_stream.inc
# End Source File
# Begin Source File

SOURCE=.\mutex.h
# End Source File
# Begin Source File

SOURCE=..\Common\property_map.h
# End Source File
# Begin Source File

SOURCE=..\Common\Result.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\thread.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
