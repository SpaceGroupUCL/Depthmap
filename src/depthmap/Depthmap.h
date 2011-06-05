// Depthmap.h : main header file for the DEPTHMAP application
//
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

#if !defined(AFX_DEPTHMAP_H__71D5A064_ED18_11D3_A8D5_00609787FC1E__INCLUDED_)
#define AFX_DEPTHMAP_H__71D5A064_ED18_11D3_A8D5_00609787FC1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "splashdlg.h"
#include "importedmodules.h"

/////////////////////////////////////////////////////////////////////////////
// CDepthmapApp:
// See Depthmap.cpp for the implementation of this class
//

class CDepthmapView;
class CAgentManager;
class CGraphDoc;

const wchar_t * getAuthors();

class CDepthmapApp : public CWinApp
{
public:
	CDepthmapApp();

   //int m_licence_code;

   int m_process_count;

   COLORREF m_foreground;
   COLORREF m_background;
   COLORREF m_selected_color;
   bool m_show_researchtoolbar;

   CString m_formula_cache; // for derived columns (useful it to be remembered)

   // menus for imported modules
   pvector<CMenu *> m_modulemenus;

   void CheckAlerts();

   CString m_manual_location; // the web address for the online manual

   DWORD m_dwAgentPause;

   CMultiDocTemplate* m_pMapTemplate;
   CMultiDocTemplate* m_pScatterTemplate;
   CMultiDocTemplate* m_pTableTemplate;
   CMultiDocTemplate* m_p3dViewTemplate;

   // Agent loads
   CMenu m_agent_menu;
   void LoadAgentMenu();

   // DLL loads
   int m_next_module_id;
   pmap<int,CImportedModule> m_imported_modules;
   int m_next_imported_menu_id;
   pmap<UINT,CImportedModuleMenuItem> m_imported_menu_items;
   void LoadImportedModules();
   void UnloadModule(int ref);
   int ImportModule(const CString& path);
   // this has module filename / error
   pqmap<CString,CString> m_dll_load_errors;

   // Graph analysis options
   Options m_options;

	DWORD m_dwAgentTime;
	DWORD m_dwSplashTime;
   CSplashDlg m_splash;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDepthmapApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	virtual int ExitInstance();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CDepthmapApp)
	afx_msg void OnAppAbout();
	afx_msg void OnHelpManual();
	afx_msg void OnWindowForeground();
	afx_msg void OnWindowBackground();
	afx_msg void OnToolsImport();
	afx_msg void OnHelpTutorials();
	afx_msg void OnHelpAlerts();
	afx_msg void OnHelpSalaManual();
	//}}AFX_MSG

	bool AgreeLicence();
   void TriggerGraphRedraw();
   void RemoveProfileValue(LPCTSTR lpszSection, LPCTSTR lpszEntry);
   void ListProfileEntries(LPCTSTR lpszSection, pqvector<CString>& subkeys);  

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEPTHMAP_H__71D5A064_ED18_11D3_A8D5_00609787FC1E__INCLUDED_)
