# Microsoft Developer Studio Project File - Name="TurretDefense" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=TurretDefense - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TurretDefense.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TurretDefense.mak" CFG="TurretDefense - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TurretDefense - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "TurretDefense - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TurretDefense - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GR /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib d3dx9.lib d3d9.lib dinput8.lib dxguid.lib winmm.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "TurretDefense - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib dinput8.lib dxguid.lib d3d9.lib d3dx9dt.LIB winmm.lib /nologo /subsystem:windows /profile /debug /machine:I386

!ENDIF 

# Begin Target

# Name "TurretDefense - Win32 Release"
# Name "TurretDefense - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Low-level"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\algorithms.cpp
# End Source File
# Begin Source File

SOURCE=.\draw.cpp
# End Source File
# Begin Source File

SOURCE=.\input.cpp
# End Source File
# Begin Source File

SOURCE=.\LogFile.cpp
# End Source File
# Begin Source File

SOURCE=.\math.cpp
# End Source File
# Begin Source File

SOURCE=.\misc.cpp
# End Source File
# Begin Source File

SOURCE=.\music.cpp
# End Source File
# Begin Source File

SOURCE=.\strings.cpp
# End Source File
# End Group
# Begin Group "GUI system"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\button.cpp
# End Source File
# Begin Source File

SOURCE=.\checkbox.cpp
# End Source File
# Begin Source File

SOURCE=.\control.cpp
# End Source File
# Begin Source File

SOURCE=.\edit.cpp
# End Source File
# Begin Source File

SOURCE=.\progressbar.cpp
# End Source File
# Begin Source File

SOURCE=.\radio.cpp
# End Source File
# Begin Source File

SOURCE=.\text.cpp
# End Source File
# Begin Source File

SOURCE=.\window.cpp
# End Source File
# End Group
# Begin Group "High-level"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\object.cpp
# End Source File
# Begin Source File

SOURCE=.\projectile.cpp
# End Source File
# Begin Source File

SOURCE=.\sprite.cpp
# End Source File
# Begin Source File

SOURCE=.\tileset.cpp
# End Source File
# End Group
# Begin Group "Game Specific"

# PROP Default_Filter ""
# Begin Group "Units"

# PROP Default_Filter ".cpp .c .h"
# Begin Source File

SOURCE=.\tank.cpp
# End Source File
# Begin Source File

SOURCE=.\unit.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\engine.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "GUI system No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\button.h
# End Source File
# Begin Source File

SOURCE=.\checkbox.h
# End Source File
# Begin Source File

SOURCE=.\control.h
# End Source File
# Begin Source File

SOURCE=.\controls.h
# End Source File
# Begin Source File

SOURCE=.\edit.h
# End Source File
# Begin Source File

SOURCE=.\progressbar.h
# End Source File
# Begin Source File

SOURCE=.\radio.h
# End Source File
# Begin Source File

SOURCE=.\text.h
# End Source File
# Begin Source File

SOURCE=.\window.h
# End Source File
# End Group
# Begin Group "Low-level No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\algorithms.h
# End Source File
# Begin Source File

SOURCE=.\draw.h
# End Source File
# Begin Source File

SOURCE=.\input.h
# End Source File
# Begin Source File

SOURCE="..\..\..\Useful Code\RTF logger\LogFile.h"
# End Source File
# Begin Source File

SOURCE=.\math.h
# End Source File
# Begin Source File

SOURCE=.\misc.h
# End Source File
# Begin Source File

SOURCE=.\music.h
# End Source File
# Begin Source File

SOURCE=.\projectile.h
# End Source File
# Begin Source File

SOURCE=.\strings.h
# End Source File
# End Group
# Begin Group "High-level No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\object.h
# End Source File
# Begin Source File

SOURCE=.\sprite.h
# End Source File
# Begin Source File

SOURCE=.\tileset.h
# End Source File
# End Group
# Begin Group "Game Specific No. 1"

# PROP Default_Filter ""
# Begin Group "Units No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\tank.h
# End Source File
# Begin Source File

SOURCE=.\unit.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\engine.h
# End Source File
# Begin Source File

SOURCE=.\main.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\stdhdr.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
