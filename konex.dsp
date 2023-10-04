# Microsoft Developer Studio Project File - Name="konex" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=konex - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "konex.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "konex.mak" CFG="konex - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "konex - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "konex - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "konex - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 opengl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "konex - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 opengl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "konex - Win32 Release"
# Name "konex - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\ai.cpp
# End Source File
# Begin Source File

SOURCE=.\src\config.cpp
# End Source File
# Begin Source File

SOURCE=.\src\console.cpp
# End Source File
# Begin Source File

SOURCE=.\src\entity.cpp
# End Source File
# Begin Source File

SOURCE=.\src\explosion.cpp
# End Source File
# Begin Source File

SOURCE=.\src\game.cpp
# End Source File
# Begin Source File

SOURCE=.\src\in_main.cpp
# End Source File
# Begin Source File

SOURCE=.\src\interface.cpp
# End Source File
# Begin Source File

SOURCE=.\src\item.cpp
# End Source File
# Begin Source File

SOURCE=.\src\konex.cpp
# End Source File
# Begin Source File

SOURCE=.\src\map.cpp
# End Source File
# Begin Source File

SOURCE=.\src\menu.cpp
# End Source File
# Begin Source File

SOURCE=.\src\missile.cpp
# End Source File
# Begin Source File

SOURCE=.\src\planets.cpp
# End Source File
# Begin Source File

SOURCE=.\src\player.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ships.cpp
# End Source File
# Begin Source File

SOURCE=.\src\snd_main.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\src\spawn.cpp
# End Source File
# Begin Source File

SOURCE=.\src\stars.cpp
# End Source File
# Begin Source File

SOURCE=.\src\sys_linux.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\src\sys_main.cpp
# End Source File
# Begin Source File

SOURCE=.\src\sys_win.cpp
# End Source File
# Begin Source File

SOURCE=.\src\target.cpp
# End Source File
# Begin Source File

SOURCE=.\src\tinyxml\tinystr.cpp
# End Source File
# Begin Source File

SOURCE=.\src\tinyxml\tinyxml.cpp
# End Source File
# Begin Source File

SOURCE=.\src\tinyxml\tinyxmlerror.cpp
# End Source File
# Begin Source File

SOURCE=.\src\tinyxml\tinyxmlparser.cpp
# End Source File
# Begin Source File

SOURCE=.\src\tm_linux.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\src\tm_win32.cpp
# End Source File
# Begin Source File

SOURCE=.\src\vid_image.cpp
# End Source File
# Begin Source File

SOURCE=.\src\vid_linux.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\src\vid_main.cpp
# End Source File
# Begin Source File

SOURCE=.\src\vid_model.cpp
# End Source File
# Begin Source File

SOURCE=.\src\vid_win32.cpp
# End Source File
# Begin Source File

SOURCE=.\src\view.cpp
# End Source File
# Begin Source File

SOURCE=.\src\xml.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\src\ai.h
# End Source File
# Begin Source File

SOURCE=.\src\config.h
# End Source File
# Begin Source File

SOURCE=.\src\console.h
# End Source File
# Begin Source File

SOURCE=.\src\defs.h
# End Source File
# Begin Source File

SOURCE=.\src\entity.h
# End Source File
# Begin Source File

SOURCE=.\src\explosion.h
# End Source File
# Begin Source File

SOURCE=.\src\game.h
# End Source File
# Begin Source File

SOURCE=.\src\in_linux.h
# End Source File
# Begin Source File

SOURCE=.\src\in_win.h
# End Source File
# Begin Source File

SOURCE=.\src\interface.h
# End Source File
# Begin Source File

SOURCE=.\src\item.h
# End Source File
# Begin Source File

SOURCE=.\src\konex.h
# End Source File
# Begin Source File

SOURCE=.\src\map.h
# End Source File
# Begin Source File

SOURCE=.\src\menu.h
# End Source File
# Begin Source File

SOURCE=.\src\missile.h
# End Source File
# Begin Source File

SOURCE=.\src\planets.h
# End Source File
# Begin Source File

SOURCE=.\src\player.h
# End Source File
# Begin Source File

SOURCE=.\src\ships.h
# End Source File
# Begin Source File

SOURCE=.\src\snd_main.h
# End Source File
# Begin Source File

SOURCE=.\src\spawn.h
# End Source File
# Begin Source File

SOURCE=.\src\stars.h
# End Source File
# Begin Source File

SOURCE=.\src\sys_main.h
# End Source File
# Begin Source File

SOURCE=.\src\target.h
# End Source File
# Begin Source File

SOURCE=.\src\tinyxml\tinystr.h
# End Source File
# Begin Source File

SOURCE=.\src\tinyxml\tinyxml.h
# End Source File
# Begin Source File

SOURCE=.\src\tm_main.h
# End Source File
# Begin Source File

SOURCE=.\src\vector.h
# End Source File
# Begin Source File

SOURCE=.\src\vid_image.h
# End Source File
# Begin Source File

SOURCE=.\src\vid_linux.h
# End Source File
# Begin Source File

SOURCE=.\src\vid_main.h
# End Source File
# Begin Source File

SOURCE=.\src\vid_model.h
# End Source File
# Begin Source File

SOURCE=.\src\vid_win32.h
# End Source File
# Begin Source File

SOURCE=.\src\view.h
# End Source File
# Begin Source File

SOURCE=.\src\xml.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
