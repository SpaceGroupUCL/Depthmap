# Microsoft Developer Studio Project File - Name="Depthmap" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Depthmap - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Depthmap.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Depthmap.mak" CFG="Depthmap - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Depthmap - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Depthmap - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Depthmap - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "_AFXDLL" /D "NDEBUG" /D "_DEPTHMAP" /D DEPTHMAP_BUILD=1 /D "_MBCS" /D "WIN32" /D "_WINDOWS" /D "PIREQUIRED" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 sala.lib opengl32.lib glu32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Depthmap - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_AFXDLL" /D "_DEBUG" /D "_DEPTHMAP" /D DEPTHMAP_BUILD=1 /D "_MBCS" /D "WIN32" /D "_WINDOWS" /D "PIREQUIRED" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 sala_d.lib opengl32.lib glu32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Depthmap - Win32 Release"
# Name "Depthmap - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\3DView.cpp
# End Source File
# Begin Source File

SOURCE=.\AgentAnalysisDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AttributeChooserDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AttributeSummary.cpp
# End Source File
# Begin Source File

SOURCE=.\AxialAnalysisOptionsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ColourScaleDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ColumnPropertiesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Depthmap.cpp
# End Source File
# Begin Source File

SOURCE=.\Depthmap.rc
# End Source File
# Begin Source File

SOURCE=.\DepthmapAlert.cpp
# End Source File
# Begin Source File

SOURCE=.\DepthmapAtom.cpp
# End Source File
# Begin Source File

SOURCE=.\DepthmapView.cpp
# End Source File
# Begin Source File

SOURCE=.\EditConnectionsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FewestLineOptionsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FilePropertiesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\GridDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\HoverWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\HyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\idepthmap.cpp
# End Source File
# Begin Source File

SOURCE=.\InsertColumnDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LayerChooserDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LicenceDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MakeLayerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MakeOptionsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NewLayerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\optionsdlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PromptReplace.cpp
# End Source File
# Begin Source File

SOURCE=.\PushDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\RenameColumnDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RichTabEditCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ScatterView.cpp
# End Source File
# Begin Source File

SOURCE=.\SegmentAnalysisDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SideBar.cpp
# End Source File
# Begin Source File

SOURCE=.\SplashDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TableEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\TableView.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewSelector.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewSelectorSub.cpp
# End Source File
# Begin Source File

SOURCE=.\WaitDialog.cpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\accept1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\agent_to.bmp
# End Source File
# Begin Source File

SOURCE=.\arrow.cur
# End Source File
# Begin Source File

SOURCE=.\res\bitmap.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00007.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00008.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00009.bmp
# End Source File
# Begin Source File

SOURCE=.\res\column_e.bmp
# End Source File
# Begin Source File

SOURCE=.\res\columnhe.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00002.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00003.cur
# End Source File
# Begin Source File

SOURCE=.\Res\cur00004.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00005.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00006.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00007.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\dephmapsplash2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\dephmapsplash3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Depthmap.ico
# End Source File
# Begin Source File

SOURCE=.\res\Depthmap.rc2
# End Source File
# Begin Source File

SOURCE=.\res\depthmapsplash_SSL3_Lg.bmp
# End Source File
# Begin Source File

SOURCE=.\res\fill_cur.cur
# End Source File
# Begin Source File

SOURCE=.\res\GraphDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\hand_cur.cur
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_grap.ico
# End Source File
# Begin Source File

SOURCE=.\Res\idr_main.ico
# End Source File
# Begin Source File

SOURCE=.\res\linkcurs.cur
# End Source File
# Begin Source File

SOURCE=.\res\map_bar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\map_bar_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sideicon.bmp
# End Source File
# Begin Source File

SOURCE=.\res\splash.bmp
# End Source File
# Begin Source File

SOURCE=.\res\splash1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\SPLASH2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\stateico.bmp
# End Source File
# Begin Source File

SOURCE=.\res\stochata.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\UCLlogo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\zoom_in1.cur
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\3DView.h
# End Source File
# Begin Source File

SOURCE=.\AttributeChooserDlg.h
# End Source File
# Begin Source File

SOURCE=..\include\Sala\attributes.h
# End Source File
# Begin Source File

SOURCE=.\AttributeSummary.h
# End Source File
# Begin Source File

SOURCE=.\AxialAnalysisOptionsDlg.h
# End Source File
# Begin Source File

SOURCE=.\buildinfo.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\ColourScaleDlg.h
# End Source File
# Begin Source File

SOURCE=.\ColumnPropertiesDlg.h
# End Source File
# Begin Source File

SOURCE=.\Depthmap.h
# End Source File
# Begin Source File

SOURCE=.\DepthmapAlert.h
# End Source File
# Begin Source File

SOURCE=.\DepthmapAtom.h
# End Source File
# Begin Source File

SOURCE=.\DepthmapView.h
# End Source File
# Begin Source File

SOURCE=.\EditConnectionsDlg.h
# End Source File
# Begin Source File

SOURCE=.\FewestLineOptionsDlg.h
# End Source File
# Begin Source File

SOURCE=.\FilePropertiesDlg.h
# End Source File
# Begin Source File

SOURCE=.\GraphDoc.h
# End Source File
# Begin Source File

SOURCE=.\GridDialog.h
# End Source File
# Begin Source File

SOURCE=.\HoverWnd.h
# End Source File
# Begin Source File

SOURCE=.\HyperLink.h
# End Source File
# Begin Source File

SOURCE=.\importedmodules.h
# End Source File
# Begin Source File

SOURCE=.\InsertColumnDlg.h
# End Source File
# Begin Source File

SOURCE=.\LayerChooserDlg.h
# End Source File
# Begin Source File

SOURCE=.\LicenceDialog.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MakeLayerDlg.h
# End Source File
# Begin Source File

SOURCE=.\MakeOptionsDlg.h
# End Source File
# Begin Source File

SOURCE=..\include\Sala\mgraph.h
# End Source File
# Begin Source File

SOURCE=.\NewLayerDlg.h
# End Source File
# Begin Source File

SOURCE=.\optionsdlg.h
# End Source File
# Begin Source File

SOURCE=.\PromptReplace.h
# End Source File
# Begin Source File

SOURCE=.\PushDialog.h
# End Source File
# Begin Source File

SOURCE=.\RenameColumnDlg.h
# End Source File
# Begin Source File

SOURCE=.\RichTabEditCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ScatterView.h
# End Source File
# Begin Source File

SOURCE=.\SegmentAnalysisDlg.h
# End Source File
# Begin Source File

SOURCE=.\SideBar.h
# End Source File
# Begin Source File

SOURCE=.\SplashDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TableEdit.h
# End Source File
# Begin Source File

SOURCE=.\TableView.h
# End Source File
# Begin Source File

SOURCE=.\ViewSelector.h
# End Source File
# Begin Source File

SOURCE=.\ViewSelectorSub.h
# End Source File
# Begin Source File

SOURCE=.\WaitDialog.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Depthmap.reg
# End Source File
# End Target
# End Project
