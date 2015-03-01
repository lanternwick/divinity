# Microsoft Developer Studio Project File - Name="Times of War" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Times of War - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Times of War.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Times of War.mak" CFG="Times of War - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Times of War - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Times of War - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Times of War", MAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Times of War - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TIMESOFWAR_EXPORTS" /Yu"stdafx.h" /FD /c
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
# ADD LINK32 compf.lib lua.lib shlwapi.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"Release/TimesofWar.dll"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy release\timesofwar.dll "c:\development\current\copy (2) of divinity\release\Times Of War\timesofwar.dll"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Times of War - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TIMESOFWAR_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_EXPORTS" /FR /Fp"Debug/TimesOfWar.pch" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 compf_d.lib lua.lib shlwapi.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386 /out:"Debug/TimesOfWar.dll"
# SUBTRACT LINK32 /nodefaultlib
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy debug\timesofwar.dll timesofwar.dll
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Times of War - Win32 Release"
# Name "Times of War - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\lua_glue.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\MATRIX.cpp

!IF  "$(CFG)" == "Times of War - Win32 Release"

!ELSEIF  "$(CFG)" == "Times of War - Win32 Debug"

# ADD CPP /YX"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PostOffice.cpp
# End Source File
# Begin Source File

SOURCE=.\render_thread.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "Times of War - Win32 Release"

# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "Times of War - Win32 Debug"

# ADD CPP /YX"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Terrain_Tile.cpp
# End Source File
# Begin Source File

SOURCE=".\Times of War.cpp"

!IF  "$(CFG)" == "Times of War - Win32 Release"

!ELSEIF  "$(CFG)" == "Times of War - Win32 Debug"

# ADD CPP /YX"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\Times of War.def"
# End Source File
# Begin Source File

SOURCE=.\TimesOfWar_Mod.cpp

!IF  "$(CFG)" == "Times of War - Win32 Release"

!ELSEIF  "$(CFG)" == "Times of War - Win32 Debug"

# ADD CPP /YX"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TOW_GameObject.cpp

!IF  "$(CFG)" == "Times of War - Win32 Release"

!ELSEIF  "$(CFG)" == "Times of War - Win32 Debug"

# ADD CPP /YX"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TOW_GameObjectFactory.cpp

!IF  "$(CFG)" == "Times of War - Win32 Release"

!ELSEIF  "$(CFG)" == "Times of War - Win32 Debug"

# ADD CPP /YX"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TOW_GameObjectOutput.cpp
# End Source File
# Begin Source File

SOURCE=.\TOW_GameOutput.cpp

!IF  "$(CFG)" == "Times of War - Win32 Release"

!ELSEIF  "$(CFG)" == "Times of War - Win32 Debug"

# ADD CPP /YX"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TOW_GameUnit.cpp
# End Source File
# Begin Source File

SOURCE=.\TOW_HeightMap.cpp

!IF  "$(CFG)" == "Times of War - Win32 Release"

!ELSEIF  "$(CFG)" == "Times of War - Win32 Debug"

# ADD CPP /YX"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TOW_InputHandler.inc
# End Source File
# Begin Source File

SOURCE=.\TOW_Map.cpp

!IF  "$(CFG)" == "Times of War - Win32 Release"

!ELSEIF  "$(CFG)" == "Times of War - Win32 Debug"

# ADD CPP /YX"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\tow_networking.cpp
# End Source File
# Begin Source File

SOURCE=.\TOW_ObjectManager.cpp
# End Source File
# Begin Source File

SOURCE=.\TOW_OutputManager.inc
# End Source File
# Begin Source File

SOURCE=.\TOW_PathingMap.cpp
# End Source File
# Begin Source File

SOURCE=.\TOW_Projectile.cpp
# End Source File
# Begin Source File

SOURCE=.\TOW_SelectBox.cpp
# End Source File
# Begin Source File

SOURCE=.\TOW_SelectIndicator.cpp
# End Source File
# Begin Source File

SOURCE=.\TOW_Selector.cpp
# End Source File
# Begin Source File

SOURCE=.\TOW_TemplateManager.cpp

!IF  "$(CFG)" == "Times of War - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Times of War - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TOW_TextureMap.cpp

!IF  "$(CFG)" == "Times of War - Win32 Release"

!ELSEIF  "$(CFG)" == "Times of War - Win32 Debug"

# ADD CPP /YX"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TOW_World.cpp
# End Source File
# Begin Source File

SOURCE=.\UIGameQuitButton.cpp
# End Source File
# Begin Source File

SOURCE=.\UIGameStartButton.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\attack_state.h
# End Source File
# Begin Source File

SOURCE=.\game_state.h
# End Source File
# Begin Source File

SOURCE=.\IHeightMap.h
# End Source File
# Begin Source File

SOURCE=..\Common\IMod.h
# End Source File
# Begin Source File

SOURCE=.\ITextureMap.h
# End Source File
# Begin Source File

SOURCE=.\lua_glue.h
# End Source File
# Begin Source File

SOURCE=.\moving_state.h
# End Source File
# Begin Source File

SOURCE=.\PostOffice.h
# End Source File
# Begin Source File

SOURCE=.\property_map.h
# End Source File
# Begin Source File

SOURCE=..\Common\rect_temp.h
# End Source File
# Begin Source File

SOURCE=.\render_thread.h
# End Source File
# Begin Source File

SOURCE=..\Common\Singleton.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Terrain_Tile.h
# End Source File
# Begin Source File

SOURCE=.\texture_state.h
# End Source File
# Begin Source File

SOURCE=".\Times of War.h"
# End Source File
# Begin Source File

SOURCE=.\TimesOfWar_Mod.h
# End Source File
# Begin Source File

SOURCE=.\Console\TOW_Console_Functors.h
# End Source File
# Begin Source File

SOURCE=.\Console\TOW_Functor.h
# End Source File
# Begin Source File

SOURCE=.\TOW_GameBuilding.h
# End Source File
# Begin Source File

SOURCE=.\TOW_GameObject.h
# End Source File
# Begin Source File

SOURCE=.\TOW_GameObjectFactory.h
# End Source File
# Begin Source File

SOURCE=.\TOW_GameObjectOutput.h
# End Source File
# Begin Source File

SOURCE=.\TOW_GameObjectTemplate.h
# End Source File
# Begin Source File

SOURCE=.\TOW_GameOutput.h
# End Source File
# Begin Source File

SOURCE=.\TOW_GameUnit.h
# End Source File
# Begin Source File

SOURCE=.\TOW_HeightMap.h
# End Source File
# Begin Source File

SOURCE=.\TOW_InputHandler.h
# End Source File
# Begin Source File

SOURCE=.\TOW_Map.h
# End Source File
# Begin Source File

SOURCE=.\tow_networking.h
# End Source File
# Begin Source File

SOURCE=.\TOW_ObjectManager.h
# End Source File
# Begin Source File

SOURCE=.\TOW_OutputManager.h
# End Source File
# Begin Source File

SOURCE=.\TOW_PathingMap.h
# End Source File
# Begin Source File

SOURCE=.\TOW_Player.h
# End Source File
# Begin Source File

SOURCE=.\TOW_Projectile.h
# End Source File
# Begin Source File

SOURCE=.\TOW_SelectBox.h
# End Source File
# Begin Source File

SOURCE=.\TOW_SelectIndicator.h
# End Source File
# Begin Source File

SOURCE=.\TOW_Selector.h
# End Source File
# Begin Source File

SOURCE=.\TOW_TemplateManager.h
# End Source File
# Begin Source File

SOURCE=.\TOW_TextureMap.h
# End Source File
# Begin Source File

SOURCE=.\TOW_Tile.h
# End Source File
# Begin Source File

SOURCE=.\TOW_World.h
# End Source File
# Begin Source File

SOURCE=.\UIGameQuitButton.h
# End Source File
# Begin Source File

SOURCE=.\UIGameStartButton.h
# End Source File
# End Group
# Begin Group "Console"

# PROP Default_Filter ""
# Begin Group "Console Source Files"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\Console\BinaryData.cpp
# End Source File
# Begin Source File

SOURCE=.\Console\console.cpp
# End Source File
# Begin Source File

SOURCE=.\Console\variant.cpp
# End Source File
# End Group
# Begin Group "Console Header Files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\Console\BinaryData.h
# End Source File
# Begin Source File

SOURCE=.\Console\console.h
# End Source File
# Begin Source File

SOURCE=.\Console\console_functor.h
# End Source File
# Begin Source File

SOURCE=.\Console\IConsoleBind.h
# End Source File
# Begin Source File

SOURCE=.\Console\string_tokenizer.h
# End Source File
# Begin Source File

SOURCE=.\Console\variant.h
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=".\old main.txt"
# End Source File
# End Target
# End Project
