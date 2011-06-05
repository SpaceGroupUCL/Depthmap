# Microsoft Developer Studio Project File - Name="sala" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=sala - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "sala.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "sala.mak" CFG="sala - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "sala - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "sala - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "sala - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "_WINDOWS" /D DEPTHMAP_BUILD=1 /D "PIREQUIRED" /D "NDEBUG" /D "WIN32" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"\alasdair\apps\lib\sala.lib"

!ELSEIF  "$(CFG)" == "sala - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Z7 /Od /D "_DEBUG" /D "_WINDOWS" /D DEPTHMAP_BUILD=1 /D "WIN32" /YX /FD /c
# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"\alasdair\apps\lib\sala_d.lib"

!ENDIF 

# Begin Target

# Name "sala - Win32 Release"
# Name "sala - Win32 Debug"
# Begin Source File

SOURCE=.\attributes.cpp
# End Source File
# Begin Source File

SOURCE=..\include\Sala\attributes.h
# End Source File
# Begin Source File

SOURCE=.\axialmap.cpp
# End Source File
# Begin Source File

SOURCE=..\include\Sala\axialmap.h
# End Source File
# Begin Source File

SOURCE=.\connector.cpp
# End Source File
# Begin Source File

SOURCE=..\include\Sala\connector.h
# End Source File
# Begin Source File

SOURCE=.\datalayer.cpp
# End Source File
# Begin Source File

SOURCE=..\include\Sala\datalayer.h
# End Source File
# Begin Source File

SOURCE=..\include\Sala\fileproperties.h
# End Source File
# Begin Source File

SOURCE=.\gmlmap.cpp
# End Source File
# Begin Source File

SOURCE=.\gmlmap.h
# End Source File
# Begin Source File

SOURCE=.\isovist.cpp
# End Source File
# Begin Source File

SOURCE=.\isovist.h
# End Source File
# Begin Source File

SOURCE=.\MapInfoData.cpp
# End Source File
# Begin Source File

SOURCE=.\MapInfoData.h
# End Source File
# Begin Source File

SOURCE=.\mgraph.cpp
# End Source File
# Begin Source File

SOURCE=..\include\Sala\mgraph.h
# End Source File
# Begin Source File

SOURCE=.\nagent.cpp
# End Source File
# Begin Source File

SOURCE=..\include\Sala\nagent.h
# End Source File
# Begin Source File

SOURCE=.\ngraph.cpp
# End Source File
# Begin Source File

SOURCE=..\include\Sala\ngraph.h
# End Source File
# Begin Source File

SOURCE=.\ntfp.cpp
# End Source File
# Begin Source File

SOURCE=.\ntfp.h
# End Source File
# Begin Source File

SOURCE=.\pointdata.cpp
# End Source File
# Begin Source File

SOURCE=..\include\Sala\pointdata.h
# End Source File
# Begin Source File

SOURCE=.\salaprogram.cpp
# End Source File
# Begin Source File

SOURCE=..\include\Sala\salaprogram.h
# End Source File
# Begin Source File

SOURCE=.\shapemap.cpp
# End Source File
# Begin Source File

SOURCE=..\include\Sala\shapemap.h
# End Source File
# Begin Source File

SOURCE=.\spacepix.cpp
# End Source File
# Begin Source File

SOURCE=..\include\Sala\spacepix.h
# End Source File
# Begin Source File

SOURCE=.\sparksieve2.cpp
# End Source File
# Begin Source File

SOURCE=.\sparksieve2.h
# End Source File
# Begin Source File

SOURCE=.\tigerp.cpp
# End Source File
# Begin Source File

SOURCE=.\tigerp.h
# End Source File
# Begin Source File

SOURCE=.\vertex.cpp
# End Source File
# Begin Source File

SOURCE=..\Include\Sala\vertex.h
# End Source File
# End Target
# End Project
