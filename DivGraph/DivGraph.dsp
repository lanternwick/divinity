# Microsoft Developer Studio Project File - Name="DivGraph" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=DivGraph - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DivGraph.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DivGraph.mak" CFG="DivGraph - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DivGraph - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "DivGraph - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/DivGraph", DAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DivGraph - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DIVGRAPH_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DIVGRAPH_EXPORTS" /Yu"stdafx.h" /FD /c
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

!ELSEIF  "$(CFG)" == "DivGraph - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DIVGRAPH_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_EXPORTS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 d3d8.lib d3dx8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy debug\divgraph.dll ..\..\bin\ext\divgraph.ext	copy debug\divgraph.dll ..\..\bin\ext\divgraph.dll	copy debug\divgraph.dll ..\DivShell\divgraph.dll	copy debug\divgraph.dll ..\Butterflies\divgraph.dll
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "DivGraph - Win32 Release"
# Name "DivGraph - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CFrame.cpp

!IF  "$(CFG)" == "DivGraph - Win32 Release"

!ELSEIF  "$(CFG)" == "DivGraph - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\Common\CObject.cpp

!IF  "$(CFG)" == "DivGraph - Win32 Release"

!ELSEIF  "$(CFG)" == "DivGraph - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CScene.cpp

!IF  "$(CFG)" == "DivGraph - Win32 Release"

!ELSEIF  "$(CFG)" == "DivGraph - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\Common\CSmartPtr.inc
# End Source File
# Begin Source File

SOURCE=.\D3DDevice.cpp

!IF  "$(CFG)" == "DivGraph - Win32 Release"

!ELSEIF  "$(CFG)" == "DivGraph - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\D3DGraphObjFactory.cpp

!IF  "$(CFG)" == "DivGraph - Win32 Release"

!ELSEIF  "$(CFG)" == "DivGraph - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\D3DIndexBuffer.cpp

!IF  "$(CFG)" == "DivGraph - Win32 Release"

!ELSEIF  "$(CFG)" == "DivGraph - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\D3DTexture.cpp

!IF  "$(CFG)" == "DivGraph - Win32 Release"

!ELSEIF  "$(CFG)" == "DivGraph - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\D3DVertexBuffer.cpp

!IF  "$(CFG)" == "DivGraph - Win32 Release"

!ELSEIF  "$(CFG)" == "DivGraph - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DivGraph.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\FileGuard.cpp

!IF  "$(CFG)" == "DivGraph - Win32 Release"

!ELSEIF  "$(CFG)" == "DivGraph - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphics.cpp
# End Source File
# Begin Source File

SOURCE=.\IGraphObjFactory.cpp

!IF  "$(CFG)" == "DivGraph - Win32 Release"

!ELSEIF  "$(CFG)" == "DivGraph - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IGraphPlugin.cpp

!IF  "$(CFG)" == "DivGraph - Win32 Release"

!ELSEIF  "$(CFG)" == "DivGraph - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IMesh.cpp

!IF  "$(CFG)" == "DivGraph - Win32 Release"

!ELSEIF  "$(CFG)" == "DivGraph - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ITexture.cpp

!IF  "$(CFG)" == "DivGraph - Win32 Release"

!ELSEIF  "$(CFG)" == "DivGraph - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\Common\MATRIX.cpp

!IF  "$(CFG)" == "DivGraph - Win32 Release"

!ELSEIF  "$(CFG)" == "DivGraph - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\Common\RESULT.cpp

!IF  "$(CFG)" == "DivGraph - Win32 Release"

!ELSEIF  "$(CFG)" == "DivGraph - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\StaticMesh.cpp

!IF  "$(CFG)" == "DivGraph - Win32 Release"

!ELSEIF  "$(CFG)" == "DivGraph - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\UserInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\VECTOR.cpp

!IF  "$(CFG)" == "DivGraph - Win32 Release"

!ELSEIF  "$(CFG)" == "DivGraph - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\WorldObj.cpp

!IF  "$(CFG)" == "DivGraph - Win32 Release"

!ELSEIF  "$(CFG)" == "DivGraph - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CCamera.h
# End Source File
# Begin Source File

SOURCE=.\CFrame.h
# End Source File
# Begin Source File

SOURCE=..\Common\CObject.h
# End Source File
# Begin Source File

SOURCE=..\DivCore\Core.h
# End Source File
# Begin Source File

SOURCE=.\CScene.h
# End Source File
# Begin Source File

SOURCE=..\Common\CSmartPtr.h
# End Source File
# Begin Source File

SOURCE=.\D3DDevice.h
# End Source File
# Begin Source File

SOURCE=.\D3DGraphObjFactory.h
# End Source File
# Begin Source File

SOURCE=.\D3DIndexBuffer.h
# End Source File
# Begin Source File

SOURCE=.\D3DTexture.h
# End Source File
# Begin Source File

SOURCE=.\D3DVertexBuffer.h
# End Source File
# Begin Source File

SOURCE=..\DivCore\DivCore.h
# End Source File
# Begin Source File

SOURCE=.\DivGraph.h
# End Source File
# Begin Source File

SOURCE=..\Common\FileGuard.h
# End Source File
# Begin Source File

SOURCE=..\Common\FVF.h
# End Source File
# Begin Source File

SOURCE=.\Graphics.h
# End Source File
# Begin Source File

SOURCE=.\IDevice.h
# End Source File
# Begin Source File

SOURCE=.\IGraphObjFactory.h
# End Source File
# Begin Source File

SOURCE=.\IGraphPlugin.h
# End Source File
# Begin Source File

SOURCE=.\IIndexBuffer.h
# End Source File
# Begin Source File

SOURCE=.\IMesh.h
# End Source File
# Begin Source File

SOURCE=.\ITexture.h
# End Source File
# Begin Source File

SOURCE=.\IVertexBuffer.h
# End Source File
# Begin Source File

SOURCE=..\Common\MATRIX.h
# End Source File
# Begin Source File

SOURCE=.\Media_Types.h
# End Source File
# Begin Source File

SOURCE=..\Common\Result.h
# End Source File
# Begin Source File

SOURCE=.\StaticMesh.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\UserInterface.h
# End Source File
# Begin Source File

SOURCE=..\Common\VECTOR.h
# End Source File
# Begin Source File

SOURCE=..\Common\WorldObj.h
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
