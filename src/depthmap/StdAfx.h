// Depthmap - spatial network analysis platform
// Copyright (C) 2000-2010 University College London, Alasdair Turner 

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__71D5A066_ED18_11D3_A8D5_00609787FC1E__INCLUDED_)
#define AFX_STDAFX_H__71D5A066_ED18_11D3_A8D5_00609787FC1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Windows ME/2000/XP/2003
#define WINVER 0x0500

#define WM_DMP_REDRAW_MESSAGE     (WM_USER + 100)
#define WM_DMP_PROGRESS_MESSAGE   (WM_USER + 101)
#define WM_DMP_FINISHED_MESSAGE   (WM_USER + 102)
#define WM_DMP_ADD_AGENT          (WM_USER + 103)
#define WM_DMP_ADD_STATIC_AGENT   (WM_USER + 104)
#define WM_DMP_MOVE_AGENT         (WM_USER + 105)
#define WM_DMP_REDO_MENU          (WM_USER + 106)
#define WM_DMP_FOCUS_GRAPH        (WM_USER + 107)
#define WM_DMP_PROCESS_BACKGROUND (WM_USER + 108)
#define WM_DMP_INITIALISE         (WM_USER + 109)
#define WM_DMP_RUN                (WM_USER + 110)
#define WM_DMP_GRID_EDIT_MSG      (WM_USER + 111)

#define DMP_TIMER_SPLASH      1
#define DMP_TIMER_REDRAW      2
#define DMP_TIMER_HOVER       3
#define DMP_TIMER_3DVIEW      4

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>        // MFC core and standard components
#include <afxext.h>        // MFC extensions
#include <afxdisp.h>       // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#include <afxmt.h>         // MFC support for threads
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxcview.h>      // MFC support for ListView

// My standard stuff:
#include <iomanip>
#include <sstream>
#include <math.h>
#include <float.h>
#include <generic/paftl.h>
#include <generic/comm.h>

#include "hyperlink.h"  // hyperlinks
#include "buildinfo.h"  // easy place for me to store the build information

// Sala
#include <strstream>
#include <Sala/mgraph.h>

#include <idepthmap.h>    // dll interface

class CDepthmapApp;
inline CDepthmapApp *GetApp()
   { return (CDepthmapApp *) AfxGetApp(); }

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__71D5A066_ED18_11D3_A8D5_00609787FC1E__INCLUDED_)
