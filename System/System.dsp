# Microsoft Developer Studio Project File - Name="System" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=System - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "System.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "System.mak" CFG="System - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "System - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "System - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "System - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SYSTEM_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SYSTEM_EXPORTS" /FA /Fa"Release/asm.asm" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Gy
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 d3d8.lib d3dx8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386

!ELSEIF  "$(CFG)" == "System - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SYSTEM_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SYSTEM_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 d3d8.lib d3dx8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"..\system.dll "
# SUBTRACT LINK32 /profile

!ENDIF 

# Begin Target

# Name "System - Win32 Release"
# Name "System - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\System.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\Classes\CFont.h
# End Source File
# Begin Source File

SOURCE=..\Classes\CIndexBuffer.h
# End Source File
# Begin Source File

SOURCE=..\Classes\CLog.h
# End Source File
# Begin Source File

SOURCE=..\Classes\CLogFile.h
# End Source File
# Begin Source File

SOURCE=..\Classes\CObject.h
# End Source File
# Begin Source File

SOURCE=..\Classes\CStack.h
# End Source File
# Begin Source File

SOURCE=..\Classes\CTexture.h
# End Source File
# Begin Source File

SOURCE=..\Classes\CVertexBuffer.h
# End Source File
# Begin Source File

SOURCE="..\..\Program Files\DirectX 8.1 SDK\include\d3d8.h"
# End Source File
# Begin Source File

SOURCE="..\..\Program Files\DirectX 8.1 SDK\include\d3d8caps.h"
# End Source File
# Begin Source File

SOURCE="..\..\Program Files\DirectX 8.1 SDK\include\d3d8types.h"
# End Source File
# Begin Source File

SOURCE="..\..\Program Files\DirectX 8.1 SDK\include\d3dx8.h"
# End Source File
# Begin Source File

SOURCE="..\..\Program Files\DirectX 8.1 SDK\include\d3dx8core.h"
# End Source File
# Begin Source File

SOURCE="..\..\Program Files\DirectX 8.1 SDK\include\d3dx8effect.h"
# End Source File
# Begin Source File

SOURCE="..\..\Program Files\DirectX 8.1 SDK\include\d3dx8math.h"
# End Source File
# Begin Source File

SOURCE="..\..\Program Files\DirectX 8.1 SDK\include\d3dx8math.inl"
# End Source File
# Begin Source File

SOURCE="..\..\Program Files\DirectX 8.1 SDK\include\d3dx8mesh.h"
# End Source File
# Begin Source File

SOURCE="..\..\Program Files\DirectX 8.1 SDK\include\d3dx8shape.h"
# End Source File
# Begin Source File

SOURCE="..\..\Program Files\DirectX 8.1 SDK\include\d3dx8tex.h"
# End Source File
# Begin Source File

SOURCE="..\..\Program Files\DirectX 8.1 SDK\include\dxfile.h"
# End Source File
# Begin Source File

SOURCE=..\Classes\FVF.h
# End Source File
# Begin Source File

SOURCE=..\Classes\MATRIX.h
# End Source File
# Begin Source File

SOURCE=..\Classes\MemManager.h
# End Source File
# Begin Source File

SOURCE=..\UI\messages.h
# End Source File
# Begin Source File

SOURCE=.\PostOffice.h
# End Source File
# Begin Source File

SOURCE=..\Classes\Queue2.h
# End Source File
# Begin Source File

SOURCE=..\Classes\Result.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\Classes\SYSD3D.h
# End Source File
# Begin Source File

SOURCE=..\Classes\SYSDevice.h
# End Source File
# Begin Source File

SOURCE=.\System.h
# End Source File
# Begin Source File

SOURCE=..\Classes\utils.h
# End Source File
# Begin Source File

SOURCE=..\Classes\VECTOR.h
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
