# Microsoft Developer Studio Project File - Name="genlib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=genlib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "genlib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "genlib.mak" CFG="genlib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "genlib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "genlib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "genlib - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "_LIB" /D "NDEBUG" /D "MT" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"\alasdair\apps\lib\genlib.lib"

!ELSEIF  "$(CFG)" == "genlib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GX /Z7 /Od /D "_LIB" /D "_DEBUG" /D "WIN32" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"\alasdair\apps\lib\genlib_d.lib"

!ENDIF 

# Begin Target

# Name "genlib - Win32 Release"
# Name "genlib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\dxfp.cpp

!IF  "$(CFG)" == "genlib - Win32 Release"

# ADD CPP /D "WIN32"
# SUBTRACT CPP /D "MT"

!ELSEIF  "$(CFG)" == "genlib - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\p2dpoly.cpp

!IF  "$(CFG)" == "genlib - Win32 Release"

# ADD CPP /D "WIN32"
# SUBTRACT CPP /D "MT"

!ELSEIF  "$(CFG)" == "genlib - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\pafmath.cpp

!IF  "$(CFG)" == "genlib - Win32 Release"

# ADD CPP /D "WIN32"
# SUBTRACT CPP /D "MT"

!ELSEIF  "$(CFG)" == "genlib - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\include\generic\comm.h
# End Source File
# Begin Source File

SOURCE=..\include\generic\dxfp.h
# End Source File
# Begin Source File

SOURCE=..\include\generic\p2dpoly.h
# End Source File
# Begin Source File

SOURCE=..\include\generic\pafmath.h
# End Source File
# Begin Source File

SOURCE=..\include\generic\paftl.h
# End Source File
# End Group
# End Target
# End Project
