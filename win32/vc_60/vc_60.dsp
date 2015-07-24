# Microsoft Developer Studio Project File - Name="vc_60" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=vc_60 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "vc_60.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "vc_60.mak" CFG="vc_60 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "vc_60 - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "vc_60 - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "vc_60 - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../src/engine" /I "../../src/sprites" /I "../../src/core" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D INLINE=inline /D "USE_SOUND" /YX /FD /c /Tp
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /out:"airstrike.exe"

!ELSEIF  "$(CFG)" == "vc_60 - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GX /ZI /Od /I "../../src/engine" /I "../../src/sprites" /I "../../src/core" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D INLINE=inline /D "USE_SOUND" /YX /FD /GZ /c /Tp
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"airstrike.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "vc_60 - Win32 Release"
# Name "vc_60 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "core"

# PROP Default_Filter "*.c"
# Begin Source File

SOURCE=..\..\src\core\animation.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\bitmask.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\coresprite.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\display.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\image.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\input.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\sound.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\text.c
# End Source File
# Begin Source File

SOURCE=..\..\src\core\xalloc.c
# End Source File
# End Group
# Begin Group "engine"

# PROP Default_Filter "*.c"
# Begin Source File

SOURCE=..\..\src\engine\ai.c
# End Source File
# Begin Source File

SOURCE=..\..\src\engine\array.c
# End Source File
# Begin Source File

SOURCE=..\..\src\engine\convenience.c
# End Source File
# Begin Source File

SOURCE=..\..\src\engine\engine.c
# End Source File
# Begin Source File

SOURCE=..\..\src\engine\glue.c
# End Source File
# Begin Source File

SOURCE=..\..\src\engine\level.c
# End Source File
# Begin Source File

SOURCE=..\..\src\engine\maths.c
# End Source File
# Begin Source File

SOURCE=..\..\src\engine\mech.c
# End Source File
# Begin Source File

SOURCE=..\..\src\engine\random.c
# End Source File
# Begin Source File

SOURCE=..\..\src\engine\schedule.c
# End Source File
# Begin Source File

SOURCE=..\..\src\engine\sound_fx.c
# End Source File
# Begin Source File

SOURCE=..\..\src\engine\sprite.c
# End Source File
# Begin Source File

SOURCE=..\..\src\engine\sprite_types.c
# End Source File
# Begin Source File

SOURCE=..\..\src\engine\ui.c
# End Source File
# End Group
# Begin Group "sprites"

# PROP Default_Filter "*.c"
# Begin Source File

SOURCE=..\..\src\sprites\airballoon.c
# End Source File
# Begin Source File

SOURCE=..\..\src\sprites\asteroid.c
# End Source File
# Begin Source File

SOURCE=..\..\src\sprites\biplane.c
# End Source File
# Begin Source File

SOURCE=..\..\src\sprites\bird.c
# End Source File
# Begin Source File

SOURCE=..\..\src\sprites\bomb.c
# End Source File
# Begin Source File

SOURCE=..\..\src\sprites\bonus.c
# End Source File
# Begin Source File

SOURCE=..\..\src\sprites\bonusmachine.c
# End Source File
# Begin Source File

SOURCE=..\..\src\sprites\bouncer.c
# End Source File
# Begin Source File

SOURCE=..\..\src\sprites\bullet.c
# End Source File
# Begin Source File

SOURCE=..\..\src\sprites\cannon.c
# End Source File
# Begin Source File

SOURCE=..\..\src\sprites\cloud.c
# End Source File
# Begin Source File

SOURCE=..\..\src\sprites\draco.c
# End Source File
# Begin Source File

SOURCE=..\..\src\sprites\dust.c
# End Source File
# Begin Source File

SOURCE=..\..\src\sprites\explosions.c
# End Source File
# Begin Source File

SOURCE=..\..\src\sprites\fire.c
# End Source File
# Begin Source File

SOURCE=..\..\src\sprites\hangar.c
# End Source File
# Begin Source File

SOURCE=..\..\src\sprites\hippo.c
# End Source File
# Begin Source File

SOURCE=..\..\src\sprites\mark.c
# End Source File
# Begin Source File

SOURCE=..\..\src\sprites\missile.c
# End Source File
# Begin Source File

SOURCE=..\..\src\sprites\pine.c
# End Source File
# Begin Source File

SOURCE=..\..\src\sprites\pingu.c
# End Source File
# Begin Source File

SOURCE=..\..\src\sprites\puff.c
# End Source File
# Begin Source File

SOURCE=..\..\src\sprites\smoke.c
# End Source File
# Begin Source File

SOURCE=..\..\src\sprites\ufo.c
# End Source File
# Begin Source File

SOURCE=..\..\src\sprites\wave.c
# End Source File
# Begin Source File

SOURCE=..\..\src\sprites\zeppelin.c
# End Source File
# End Group
# Begin Group "tests"

# PROP Default_Filter "*.c"
# Begin Source File

SOURCE=..\..\src\tests\testengine.c
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "core_h"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=..\..\src\core\animation.h
# End Source File
# Begin Source File

SOURCE=..\..\src\core\bitmask.h
# End Source File
# Begin Source File

SOURCE=..\..\src\core\core.h
# End Source File
# Begin Source File

SOURCE=..\..\src\core\coresprite.h
# End Source File
# Begin Source File

SOURCE=..\..\src\core\display.h
# End Source File
# Begin Source File

SOURCE=..\..\src\core\image.h
# End Source File
# Begin Source File

SOURCE=..\..\src\core\input.h
# End Source File
# Begin Source File

SOURCE=..\..\src\core\sound.h
# End Source File
# Begin Source File

SOURCE=..\..\src\core\text.h
# End Source File
# Begin Source File

SOURCE=..\..\src\core\xalloc.h
# End Source File
# End Group
# Begin Group "engine_h"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=..\..\src\engine\ai.h
# End Source File
# Begin Source File

SOURCE=..\..\src\engine\array.h
# End Source File
# Begin Source File

SOURCE=..\..\src\engine\convenience.h
# End Source File
# Begin Source File

SOURCE=..\..\src\engine\engine.h
# End Source File
# Begin Source File

SOURCE=..\..\src\engine\glue.h
# End Source File
# Begin Source File

SOURCE=..\..\src\engine\level.h
# End Source File
# Begin Source File

SOURCE=..\..\src\engine\maths.h
# End Source File
# Begin Source File

SOURCE=..\..\src\engine\mech.h
# End Source File
# Begin Source File

SOURCE=..\..\src\engine\msg.h
# End Source File
# Begin Source File

SOURCE=..\..\src\engine\msg_types.h
# End Source File
# Begin Source File

SOURCE=..\..\src\engine\random.h
# End Source File
# Begin Source File

SOURCE=..\..\src\engine\refcount.h
# End Source File
# Begin Source File

SOURCE=..\..\src\engine\schedule.h
# End Source File
# Begin Source File

SOURCE=..\..\src\engine\signals.h
# End Source File
# Begin Source File

SOURCE=..\..\src\engine\sound_fx.h
# End Source File
# Begin Source File

SOURCE=..\..\src\engine\sprite.h
# End Source File
# Begin Source File

SOURCE=..\..\src\engine\sprite_types.h
# End Source File
# Begin Source File

SOURCE=..\..\src\engine\ui.h
# End Source File
# End Group
# Begin Group "sprites_h"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=..\..\src\sprites\bonus.h
# End Source File
# Begin Source File

SOURCE=..\..\src\sprites\sprites_autogen.h
# End Source File
# End Group
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
