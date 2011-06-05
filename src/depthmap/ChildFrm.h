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

// ChildFrm.h : interface of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDFRM_H__71D5A06A_ED18_11D3_A8D5_00609787FC1E__INCLUDED_)
#define AFX_CHILDFRM_H__71D5A06A_ED18_11D3_A8D5_00609787FC1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ViewSelector.h"
#include "ViewSelectorSub.h"

class CDepthmapView;
class CTableView;

enum {
   ID_MAPBAR_ZOOM_ITEMS = 2,
   ID_MAPBAR_FILL_ITEMS = 8,
   ID_MAPBAR_DRAW_ITEMS = 10,
   ID_MAPBAR_ISOVIST_ITEMS = 12,
   ID_MAPBAR_JOIN_ITEMS = 15
};

enum {
   ID_MAPBAR_ITEM_SELECT = 0,
   ID_MAPBAR_ITEM_MOVE = 1,
   ID_MAPBAR_ITEM_ZOOM_IN = 2,
   ID_MAPBAR_ITEM_ZOOM_OUT = 3,
   ID_MAPBAR_ITEM_FINDLOC = 4,
   ID_MAPBAR_ITEM_CENTREVIEW = 5,
   ID_MAPBAR_ITEM_GRID = 6,
   ID_MAPBAR_ITEM_FILL = 7,
   ID_MAPBAR_ITEM_SEMIFILL = 8,
   ID_MAPBAR_ITEM_PENCIL = 9,
   ID_MAPBAR_ITEM_LINETOOL = 10,
   ID_MAPBAR_ITEM_POLYGON = 11,
   ID_MAPBAR_ITEM_ISOVIST = 12,
   ID_MAPBAR_ITEM_HALFISOVIST = 13,
   ID_MAPBAR_ITEM_AL2 = 14,
   ID_MAPBAR_ITEM_PD = 15,
   ID_MAPBAR_ITEM_JOIN = 16,
   ID_MAPBAR_ITEM_UNJOIN = 17
};

class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// Attributes
public:
   CReBar m_rebar;
   CToolBar m_map_bar;
   CToolBar m_view_editor;
   CToolBar m_view_editor2;
   //CViewSelector m_view_selector;
   CViewSelectorSub m_view_selector_sub_x;
   CViewSelectorSub m_view_selector_sub_y;
   //
   CImageList m_map_bar_imgs;
   //
   int m_zoom_tool;
   int m_fill_tool;
   int m_draw_tool;
   int m_isovist_tool;
   int m_link_tool;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, const RECT& rect = rectDefault, CMDIFrameWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

   bool MapBarSetup();

// Implementation
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CChildFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnViewTable();
	afx_msg void OnUpdateViewTable(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnMapbarZoomIn();
	afx_msg void OnUpdateMapbarZoomIn(CCmdUI* pCmdUI);
	afx_msg void OnMapbarZoomOut();
	afx_msg void OnUpdateMapbarZoomOut(CCmdUI* pCmdUI);
	afx_msg void OnMapbarFill();
	afx_msg void OnUpdateMapbarFill(CCmdUI* pCmdUI);
	afx_msg void OnMapbarSemifill();
	afx_msg void OnUpdateMapbarSemifill(CCmdUI* pCmdUI);
	afx_msg void OnMapbarJoin();
	afx_msg void OnUpdateMapbarJoin(CCmdUI* pCmdUI);
	afx_msg void OnMapbarUnjoin();
	afx_msg void OnUpdateMapbarUnjoin(CCmdUI* pCmdUI);
	afx_msg void OnMapbarHalfovist();
	afx_msg void OnUpdateMapbarHalfovist(CCmdUI* pCmdUI);
	afx_msg void OnMapbarIsovist();
	afx_msg void OnUpdateMapbarIsovist(CCmdUI* pCmdUI);
	afx_msg void OnMapbarLine();
	afx_msg void OnMapbarPolygon();
	afx_msg void OnUpdateMapbarLine(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMapbarPolygon(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnDropDown(NMHDR* pNotifyStruct, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRM_H__71D5A06A_ED18_11D3_A8D5_00609787FC1E__INCLUDED_)
