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

// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__71D5A068_ED18_11D3_A8D5_00609787FC1E__INCLUDED_)
#define AFX_MAINFRM_H__71D5A068_ED18_11D3_A8D5_00609787FC1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "sidebar.h"
#include "colourscaledlg.h"

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:
   void GetActualClientRect(CRect& rect);
   bool m_sidebarExpanded;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
   CToolBar    m_wndAgentToolBar;
   CToolBar    m_wndResearchToolBar;
	CReBar      m_wndReBar;
   CSideBar    m_wndSideBar;
   CColourScaleDlg m_wndColourScale;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewLineLayers();
	afx_msg void OnUpdateViewLineLayers(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnUpdateWindowScatterplot(CCmdUI* pCmdUI);
	afx_msg void OnUpdateWindowMap(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewTable(CCmdUI* pCmdUI);
	afx_msg void OnViewColourRange();
	afx_msg void OnUpdateViewColourRange(CCmdUI* pCmdUI);
	afx_msg void OnMove(int x, int y);
	//}}AFX_MSG
   LRESULT OnFocusGraph(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnToolsOptions();
public:
   afx_msg void OnShowResearchtoolbar();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__71D5A068_ED18_11D3_A8D5_00609787FC1E__INCLUDED_)
