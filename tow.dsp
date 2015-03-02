# Microsoft Developer Studio Project File - Name="tow" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=tow - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "tow.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tow.mak" CFG="tow - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tow - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "tow - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "tow - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "tow - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "tow___Win32_Debug"
# PROP BASE Intermediate_Dir "tow___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "LOGME" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 dxguid.lib dinput8.lib ws2_32.lib d3d8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "tow - Win32 Release"
# Name "tow - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\_socket.cpp
# End Source File
# Begin Source File

SOURCE=.\BinaryData.cpp
# End Source File
# Begin Source File

SOURCE=.\CCamera.cpp
# End Source File
# Begin Source File

SOURCE=.\client.cpp
# End Source File
# Begin Source File

SOURCE=.\console.cpp
# End Source File
# Begin Source File

SOURCE=.\D3DDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\D3DIndexBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\D3DTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\D3DVertexBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\DIDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\Font.cpp
# End Source File
# Begin Source File

SOURCE=.\FontManager.cpp
# End Source File
# Begin Source File

SOURCE=.\IMediaObject.cpp
# End Source File
# Begin Source File

SOURCE=.\Intersections.cpp
# End Source File
# Begin Source File

SOURCE=.\IObject.cpp
# End Source File
# Begin Source File

SOURCE=.\ITexture.cpp
# End Source File
# Begin Source File

SOURCE=.\IVertexBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\MATRIX.cpp
# End Source File
# Begin Source File

SOURCE=.\mutex.cpp
# End Source File
# Begin Source File

SOURCE=.\PLANE.cpp
# End Source File
# Begin Source File

SOURCE=.\Quad2D.cpp
# End Source File
# Begin Source File

SOURCE=.\ray.cpp
# End Source File
# Begin Source File

SOURCE=.\Terrain_Tile.cpp
# End Source File
# Begin Source File

SOURCE=.\tow.cpp
# End Source File
# Begin Source File

SOURCE=.\TOW_HeightMap.cpp
# End Source File
# Begin Source File

SOURCE=.\TOW_Map.cpp
# End Source File
# Begin Source File

SOURCE=.\TOW_TextureMap.cpp
# End Source File
# Begin Source File

SOURCE=.\UIBaseWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\UIButton.cpp
# End Source File
# Begin Source File

SOURCE=.\UICheckbox.cpp
# End Source File
# Begin Source File

SOURCE=.\UICommandMap.cpp
# End Source File
# Begin Source File

SOURCE=.\UIConsoleWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\UICursor.cpp
# End Source File
# Begin Source File

SOURCE=.\UIDisplaybox.cpp
# End Source File
# Begin Source File

SOURCE=.\UIInputHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\UIScrollbar.cpp
# End Source File
# Begin Source File

SOURCE=.\UITextbox.cpp
# End Source File
# Begin Source File

SOURCE=.\UITitlebar.cpp
# End Source File
# Begin Source File

SOURCE=.\UIWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\UserInterface.cpp

!IF  "$(CFG)" == "tow - Win32 Release"

!ELSEIF  "$(CFG)" == "tow - Win32 Debug"

# ADD CPP /W3

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\variant.cpp
# End Source File
# Begin Source File

SOURCE=.\VECTOR.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowManager.cpp

!IF  "$(CFG)" == "tow - Win32 Release"

!ELSEIF  "$(CFG)" == "tow - Win32 Debug"

# ADD CPP /W3

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\_socket.h
# End Source File
# Begin Source File

SOURCE=.\BinaryData.h
# End Source File
# Begin Source File

SOURCE=.\CCamera.h
# End Source File
# Begin Source File

SOURCE=.\client.h
# End Source File
# Begin Source File

SOURCE=.\console.h
# End Source File
# Begin Source File

SOURCE=.\console_functor.h
# End Source File
# Begin Source File

SOURCE=.\custmoniker.h
# End Source File
# Begin Source File

SOURCE=.\D3DDevice.h
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

SOURCE=.\DIDevice.h
# End Source File
# Begin Source File

SOURCE=.\Font.h
# End Source File
# Begin Source File

SOURCE=.\FontManager.h
# End Source File
# Begin Source File

SOURCE=.\FVF.h
# End Source File
# Begin Source File

SOURCE=.\g_block.h
# End Source File
# Begin Source File

SOURCE=.\g_deparser.h
# End Source File
# Begin Source File

SOURCE=.\g_exception.h
# End Source File
# Begin Source File

SOURCE=.\g_input.h
# End Source File
# Begin Source File

SOURCE=.\g_output.h
# End Source File
# Begin Source File

SOURCE=.\g_parser.h
# End Source File
# Begin Source File

SOURCE=.\gwen.h
# End Source File
# Begin Source File

SOURCE=.\IConsoleBind.h
# End Source File
# Begin Source File

SOURCE=.\IDevice.h
# End Source File
# Begin Source File

SOURCE=.\IHeightMap.h
# End Source File
# Begin Source File

SOURCE=.\IIndexBuffer.h
# End Source File
# Begin Source File

SOURCE=.\IInputDevice.h
# End Source File
# Begin Source File

SOURCE=.\IInputHandler.h
# End Source File
# Begin Source File

SOURCE=.\IMediaObject.h
# End Source File
# Begin Source File

SOURCE=.\Intersections.h
# End Source File
# Begin Source File

SOURCE=.\IObject.h
# End Source File
# Begin Source File

SOURCE=.\IQuad.h
# End Source File
# Begin Source File

SOURCE=.\IRenderable.h
# End Source File
# Begin Source File

SOURCE=.\ITexture.h
# End Source File
# Begin Source File

SOURCE=.\ITextureMap.h
# End Source File
# Begin Source File

SOURCE=.\IThinker.h
# End Source File
# Begin Source File

SOURCE=.\IVertexBuffer.h
# End Source File
# Begin Source File

SOURCE=.\KeyConstants.h
# End Source File
# Begin Source File

SOURCE=.\log_stream.h
# End Source File
# Begin Source File

SOURCE=.\MATRIX.h
# End Source File
# Begin Source File

SOURCE=.\mutex.h
# End Source File
# Begin Source File

SOURCE=.\PLANE.h
# End Source File
# Begin Source File

SOURCE=.\Quad2D.h
# End Source File
# Begin Source File

SOURCE=.\Ray.h
# End Source File
# Begin Source File

SOURCE=.\Result.h
# End Source File
# Begin Source File

SOURCE=.\string_tokenizer.h
# End Source File
# Begin Source File

SOURCE=.\SYSWindow.h
# End Source File
# Begin Source File

SOURCE=.\Terrain_Tile.h
# End Source File
# Begin Source File

SOURCE=.\TOW_HeightMap.h
# End Source File
# Begin Source File

SOURCE=.\TOW_Map.h
# End Source File
# Begin Source File

SOURCE=.\TOW_PathingMap.h
# End Source File
# Begin Source File

SOURCE=.\TOW_TextureMap.h
# End Source File
# Begin Source File

SOURCE=.\types.h
# End Source File
# Begin Source File

SOURCE=.\UIBaseWindow.h
# End Source File
# Begin Source File

SOURCE=.\UIButton.h
# End Source File
# Begin Source File

SOURCE=.\UICheckbox.h
# End Source File
# Begin Source File

SOURCE=.\UICommandMap.h
# End Source File
# Begin Source File

SOURCE=.\UIConsoleWnd.h
# End Source File
# Begin Source File

SOURCE=.\UICursor.h
# End Source File
# Begin Source File

SOURCE=.\UIDisplaybox.h
# End Source File
# Begin Source File

SOURCE=.\UIInputHandler.h
# End Source File
# Begin Source File

SOURCE=.\UIScrollbar.h
# End Source File
# Begin Source File

SOURCE=.\UITextbox.h
# End Source File
# Begin Source File

SOURCE=.\UITitlebar.h
# End Source File
# Begin Source File

SOURCE=.\UIWindow.h
# End Source File
# Begin Source File

SOURCE=.\UserInterface.h
# End Source File
# Begin Source File

SOURCE=.\variant.h
# End Source File
# Begin Source File

SOURCE=.\VECTOR.h
# End Source File
# Begin Source File

SOURCE=.\WindowManager.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
