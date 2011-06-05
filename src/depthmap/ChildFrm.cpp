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

// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "Depthmap.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "DepthmapView.h"
#include "GraphDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_VIEW_TABLE, OnViewTable)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TABLE, OnUpdateViewTable)
	ON_WM_SIZE()
	ON_COMMAND(ID_MAPBAR_ZOOM_IN, OnMapbarZoomIn)
	ON_UPDATE_COMMAND_UI(ID_MAPBAR_ZOOM_IN, OnUpdateMapbarZoomIn)
	ON_COMMAND(ID_MAPBAR_ZOOM_OUT, OnMapbarZoomOut)
	ON_UPDATE_COMMAND_UI(ID_MAPBAR_ZOOM_OUT, OnUpdateMapbarZoomOut)
	ON_COMMAND(ID_MAPBAR_FILL, OnMapbarFill)
	ON_UPDATE_COMMAND_UI(ID_MAPBAR_FILL, OnUpdateMapbarFill)
	ON_COMMAND(ID_MAPBAR_SEMIFILL, OnMapbarSemifill)
	ON_UPDATE_COMMAND_UI(ID_MAPBAR_SEMIFILL, OnUpdateMapbarSemifill)
	ON_COMMAND(ID_MAPBAR_JOIN, OnMapbarJoin)
	ON_UPDATE_COMMAND_UI(ID_MAPBAR_JOIN, OnUpdateMapbarJoin)
	ON_COMMAND(ID_MAPBAR_UNJOIN, OnMapbarUnjoin)
	ON_UPDATE_COMMAND_UI(ID_MAPBAR_UNJOIN, OnUpdateMapbarUnjoin)
	ON_COMMAND(ID_MAPBAR_HALFOVIST, OnMapbarHalfovist)
	ON_UPDATE_COMMAND_UI(ID_MAPBAR_HALFOVIST, OnUpdateMapbarHalfovist)
	ON_COMMAND(ID_MAPBAR_ISOVIST, OnMapbarIsovist)
	ON_UPDATE_COMMAND_UI(ID_MAPBAR_ISOVIST, OnUpdateMapbarIsovist)
	ON_WM_MDIACTIVATE()
	ON_COMMAND(ID_MAPBAR_LINE, OnMapbarLine)
	ON_COMMAND(ID_MAPBAR_POLYGON, OnMapbarPolygon)
	ON_UPDATE_COMMAND_UI(ID_MAPBAR_LINE, OnUpdateMapbarLine)
	ON_UPDATE_COMMAND_UI(ID_MAPBAR_POLYGON, OnUpdateMapbarPolygon)
	//}}AFX_MSG_MAP
	ON_NOTIFY(TBN_DROPDOWN, AFX_IDW_TOOLBAR, OnDropDown)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

void CChildFrame::OnClose() 
{
   CGraphDoc *pDoc = (CGraphDoc *) GetActiveDocument();

   if (pDoc && pDoc->m_communicator) {
      AfxMessageBox(_T("Depthmap is processing... please cancel process before continuing"));
   }
   else {
   	CMDIChildWnd::OnClose();
   }
}

BOOL CChildFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CMDIFrameWnd* pParentWnd, CCreateContext* pContext) 
{
   CRect newRect;
   if (pParentWnd == NULL || pParentWnd->IsKindOf(RUNTIME_CLASS(CMainFrame))) {
      CMainFrame *pFrame = (CMainFrame *) AfxGetApp()->m_pMainWnd;
      pFrame->GetActualClientRect( newRect );
   }
   else {
      pParentWnd->GetClientRect( newRect );
   }

   newRect.DeflateRect(10,10);

	m_map_bar_imgs.Create(IDB_MAP_BAR_HOT, 16, 0, RGB(255, 0, 255));

	return CMDIChildWnd::Create(lpszClassName, lpszWindowName, dwStyle, newRect, pParentWnd, pContext);
}

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	MDICREATESTRUCT* lpmcs;
	lpmcs = (MDICREATESTRUCT*)lpCreateStruct->lpCreateParams;
	CCreateContext* pContext = (CCreateContext*)lpmcs->lParam;
   
   if (pContext->m_pNewDocTemplate == GetApp()->m_pMapTemplate) {
      if (!MapBarSetup()) {
		   TRACE0("Failed to create map toolbar\n");
		   return -1;      // fail to create
      }
      // push tooltips band over to left
      m_rebar.GetReBarCtrl().MaximizeBand(1);
   }
   else if (pContext->m_pNewDocTemplate == GetApp()->m_pScatterTemplate) {
      m_view_selector_sub_y.m_axis = 1;   // y axis for y selector
      if (!m_view_selector_sub_x.Create(IDD_DIALOG_BAR1, this))
      {
		   TRACE0("Failed to create view selector sub\n");
		   return -1;      // fail to create
      }
      if (!m_view_selector_sub_y.Create(IDD_DIALOG_BAR1, this))
      {
		   TRACE0("Failed to create view selector sub\n");
		   return -1;      // fail to create
      }
      if (!m_view_editor.CreateEx(this) ||
          !m_view_editor.LoadToolBar(IDR_COLUMN_EDITOR_TOOLBAR2)) {
		   TRACE0("Failed to create tool bar\n");
		   return -1;      // fail to create
      }
	   if (!m_rebar.Create(this,RBS_AUTOSIZE|RBS_FIXEDORDER|RBS_BANDBORDERS) ||
		    !m_rebar.AddBar(&m_view_selector_sub_x) ||
		    !m_rebar.AddBar(&m_view_selector_sub_y) ||
          !m_rebar.AddBar(&m_view_editor))
	   {
		   TRACE0("Failed to create rebar\n");
		   return -1;      // fail to create
	   }
      // enable tooltips
   	m_view_editor.SetBarStyle(m_view_editor.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
      // push tooltips band over to left
      m_rebar.GetReBarCtrl().MaximizeBand(1);
   }
   else if (pContext->m_pNewDocTemplate == GetApp()->m_pTableTemplate) {
      /*if (!m_view_editor.CreateEx(this) ||
          !m_view_editor.LoadToolBar(IDR_COLUMN_EDITOR_TOOLBAR1)) {
		   TRACE0("Failed to create tool bar\n");
		   return -1;      // fail to create
      }
	   if (!m_rebar.Create(this,RBS_AUTOSIZE|RBS_FIXEDORDER|RBS_BANDBORDERS) ||
          !m_rebar.AddBar(&m_view_editor))
	   {
		   TRACE0("Failed to create rebar\n");
		   return -1;      // fail to create
	   }
      // enable tooltips
   	m_view_editor.SetBarStyle(m_view_editor.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);*/
   }
   else if (pContext->m_pNewDocTemplate == GetApp()->m_p3dViewTemplate) {
      if (!m_view_editor.CreateEx(this) ||
          !m_view_editor.LoadToolBar(IDR_3D_NAV)) {
		   TRACE0("Failed to create tool bar\n");
		   return -1;      // fail to create
      }
	   if (!m_rebar.Create(this,RBS_AUTOSIZE|RBS_FIXEDORDER|RBS_BANDBORDERS) ||
          !m_rebar.AddBar(&m_view_editor))
	   {
		   TRACE0("Failed to create rebar\n");
		   return -1;      // fail to create
	   }
      // enable tooltips
   	m_view_editor.SetBarStyle(m_view_editor.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
   }

	return 0;
}

bool CChildFrame::MapBarSetup()
{
   if (!m_rebar.Create(this,RBS_AUTOSIZE|RBS_FIXEDORDER|RBS_BANDBORDERS)) {
      TRACE0("Failed to create rebar\n");
		return false;
   }
   if (!m_map_bar.CreateEx(this)) {
      TRACE0("Failed to create tool bar\n");
		return false;
   }

   // and extras...
	m_map_bar.GetToolBarCtrl().SetButtonWidth(16, 16);
	m_map_bar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);

	m_map_bar.GetToolBarCtrl().SetImageList(&m_map_bar_imgs);
// img.Detach();
//	img.Create(IDB_MAP_BAR_COLD, 16, 0, RGB(255, 0, 255));
//	m_map_bar.GetToolBarCtrl().SetImageList(&img);
//	img.Detach();
	m_map_bar.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT);
	m_map_bar.SetButtons(NULL, 16);

	// set up each toolbar button
	m_map_bar.SetButtonInfo(0, ID_EDIT_SELECT, TBSTYLE_BUTTON, ID_MAPBAR_ITEM_SELECT);
	m_map_bar.SetButtonInfo(1, ID_VIEW_MOVE, TBSTYLE_BUTTON, ID_MAPBAR_ITEM_MOVE);
	m_map_bar.SetButtonInfo(ID_MAPBAR_ZOOM_ITEMS, ID_VIEW_ZOOM_IN, TBSTYLE_BUTTON | TBSTYLE_DROPDOWN, ID_MAPBAR_ITEM_ZOOM_IN);
	m_map_bar.SetButtonInfo(3, -1, TBBS_SEPARATOR, -1);
	m_map_bar.SetButtonInfo(4, ID_VIEW_ZOOMSEL, TBSTYLE_BUTTON, ID_MAPBAR_ITEM_FINDLOC);
	m_map_bar.SetButtonInfo(5, ID_VIEW_CENTREVIEW, TBSTYLE_BUTTON, ID_MAPBAR_ITEM_CENTREVIEW);
	m_map_bar.SetButtonInfo(6, -1, TBBS_SEPARATOR, -1);
	m_map_bar.SetButtonInfo(7, ID_EDIT_GRID, TBSTYLE_BUTTON, ID_MAPBAR_ITEM_GRID);
	m_map_bar.SetButtonInfo(ID_MAPBAR_FILL_ITEMS, ID_EDIT_FILL, TBSTYLE_BUTTON | TBSTYLE_DROPDOWN, ID_MAPBAR_ITEM_FILL);
	m_map_bar.SetButtonInfo(9, ID_EDIT_PENCIL, TBSTYLE_BUTTON, ID_MAPBAR_ITEM_PENCIL);
	m_map_bar.SetButtonInfo(ID_MAPBAR_DRAW_ITEMS, ID_EDIT_LINETOOL, TBSTYLE_BUTTON | TBSTYLE_DROPDOWN, ID_MAPBAR_ITEM_LINETOOL);
	m_map_bar.SetButtonInfo(11, -1, TBBS_SEPARATOR, -1);
	m_map_bar.SetButtonInfo(ID_MAPBAR_ISOVIST_ITEMS, ID_MODE_ISOVIST, TBSTYLE_BUTTON | TBSTYLE_DROPDOWN, ID_MAPBAR_ITEM_ISOVIST);
	m_map_bar.SetButtonInfo(13, ID_TOOLS_AL2, TBSTYLE_BUTTON, ID_MAPBAR_ITEM_AL2);
	m_map_bar.SetButtonInfo(14, ID_TOOLS_PD, TBSTYLE_BUTTON, ID_MAPBAR_ITEM_PD);
	m_map_bar.SetButtonInfo(ID_MAPBAR_JOIN_ITEMS, ID_MODE_JOIN, TBSTYLE_BUTTON | TBSTYLE_DROPDOWN, ID_MAPBAR_ITEM_JOIN);

   // default zoom, edit and link tools:
   m_zoom_tool = ID_VIEW_ZOOM_IN;
   m_fill_tool = ID_EDIT_FILL;
   m_isovist_tool = ID_MODE_ISOVIST;
   m_link_tool = ID_MODE_JOIN;

	// set up toolbar button sizes
	CRect rectToolBar;
	m_map_bar.GetItemRect(0, &rectToolBar);
	m_map_bar.SetSizes(rectToolBar.Size(), CSize(16,16));

   // enable tooltips
   m_map_bar.SetBarStyle(m_map_bar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED);

   if (!m_rebar.AddBar(&m_map_bar,NULL,NULL,RBBS_NOGRIPPER)) {
		TRACE0("Failed to add map bar to rebar\n");
		return false;
	}

   return true;
}


BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	return CMDIChildWnd::OnCreateClient(lpcs,pContext);
}

void CChildFrame::OnViewTable() 
{
   CGraphDoc *pDoc = (CGraphDoc *) GetActiveDocument();
   /*
   if (!m_pTableView) {
      m_pDepthmapView = (CDepthmapView *) GetActiveView();
      m_pTableView = new CTableView;
      m_pTableView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, rectDefault, this, AFX_IDW_PANE_FIRST + 1, NULL);
      pDoc->AddView(m_pTableView);
      m_pTableView->OnInitialUpdate();
      CRect rect,rebar;
      GetClientRect(rect);
      m_rebar.GetWindowRect(rebar);
      m_pTableView->SetWindowPos(NULL,rect.left,rect.top+rebar.Height(),150,rect.Height()-rebar.Height(),SWP_NOZORDER);
      m_pDepthmapView->SetWindowPos(NULL,rect.left+150,rect.top+rebar.Height(),rect.Width()-150,rect.Height()-rebar.Height(),SWP_NOZORDER);
   }
   else
   {
      SetActiveView(m_pDepthmapView);
      pDoc->RemoveView(m_pTableView);
      m_pTableView->DestroyWindow();
      //delete m_pTableView;      
      m_pTableView = NULL;
      CRect rect,rebar;
      GetClientRect(rect);
      m_rebar.GetWindowRect(rebar);
      m_pDepthmapView->SetWindowPos(NULL,rect.left,rect.top+rebar.Height(),rect.Width(),rect.Height()-rebar.Height(),SWP_NOZORDER);
   }

   pDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_VIEWER, this);
   */
}

void CChildFrame::OnUpdateViewTable(CCmdUI* pCmdUI) 
{
   /*
   if (m_pTableView) {
      pCmdUI->SetCheck(1);
   }
   else {
      pCmdUI->SetCheck(0);
   }
   */
}

void CChildFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIChildWnd::OnSize(nType, cx, cy);
	
   if (m_rebar.m_hWnd) {
      m_rebar.GetReBarCtrl().MaximizeBand(m_rebar.GetReBarCtrl().GetBandCount()-1);
   }
}

void CChildFrame::OnDropDown(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	// this function handles the dropdown menus from the toolbar
	NMTOOLBAR* pNMToolBar = (NMTOOLBAR*)pNotifyStruct;
	CRect rect;

   // check we have actually hit the map bar and not another random bar...
   if (pNMToolBar->hdr.hwndFrom == m_map_bar.m_hWnd) {
	   // translate the current toolbar item rectangle into screen coordinates
	   // so that we'll know where to pop up the menu
	   m_map_bar.GetToolBarCtrl().GetRect(pNMToolBar->iItem, &rect);
	   rect.top = rect.bottom;
	   ::ClientToScreen(pNMToolBar->hdr.hwndFrom, &rect.TopLeft());
      int submenu = -1;
	   if(pNMToolBar->iItem == ID_VIEW_ZOOM_IN || pNMToolBar->iItem == ID_VIEW_ZOOM_OUT) {
         submenu = 0;
	   }
	   else if(pNMToolBar->iItem == ID_EDIT_FILL || pNMToolBar->iItem == ID_EDIT_SEMIFILL) {
         submenu = 1;
	   }
	   else if(pNMToolBar->iItem == ID_EDIT_LINETOOL || pNMToolBar->iItem == ID_EDIT_POLYGON) {
         // check the current type ->
         int type = ((CGraphDoc *)GetActiveDocument())->m_meta_graph->getDisplayedMapType();
         if (type == ShapeMap::DATAMAP) {
            submenu = 2;
         }
      }
	   else if(pNMToolBar->iItem == ID_MODE_ISOVIST || pNMToolBar->iItem == ID_MODE_HALFOVIST) {
         submenu = 3;
      }
	   else if(pNMToolBar->iItem == ID_MODE_JOIN || pNMToolBar->iItem == ID_MODE_UNJOIN) {
         submenu = 4;
      }
      if (submenu != -1) {
		   CMenu menu;
		   CMenu* pPopup;
		   menu.LoadMenu(IDR_POPUP_MAPBAR);
		   pPopup = menu.GetSubMenu(submenu);
		   pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left, rect.top + 1, AfxGetMainWnd());
      }
   }
	*pResult = TBDDRET_DEFAULT;
}

void CChildFrame::OnMapbarZoomIn() 
{
   if (m_zoom_tool != ID_VIEW_ZOOM_IN) {
   	m_map_bar.SetButtonInfo(ID_MAPBAR_ZOOM_ITEMS, ID_VIEW_ZOOM_IN, TBSTYLE_BUTTON | TBSTYLE_DROPDOWN, ID_MAPBAR_ITEM_ZOOM_IN);
      m_zoom_tool = ID_VIEW_ZOOM_IN;
   }
   ((CDepthmapView *)GetActiveView())->SendMessage(WM_COMMAND,ID_VIEW_ZOOM_IN);
}
void CChildFrame::OnUpdateMapbarZoomIn(CCmdUI* pCmdUI) 
{
   if (m_zoom_tool == ID_VIEW_ZOOM_IN) {
      pCmdUI->SetCheck(TRUE);
   }
}
void CChildFrame::OnMapbarZoomOut() 
{
   if (m_zoom_tool != ID_VIEW_ZOOM_OUT) {
   	m_map_bar.SetButtonInfo(ID_MAPBAR_ZOOM_ITEMS, ID_VIEW_ZOOM_OUT, TBSTYLE_BUTTON | TBSTYLE_DROPDOWN, ID_MAPBAR_ITEM_ZOOM_OUT);
      m_zoom_tool = ID_VIEW_ZOOM_OUT;
   }
   ((CDepthmapView *)GetActiveView())->SendMessage(WM_COMMAND,ID_VIEW_ZOOM_OUT);
}
void CChildFrame::OnUpdateMapbarZoomOut(CCmdUI* pCmdUI) 
{
   if (m_zoom_tool == ID_VIEW_ZOOM_OUT) {
      pCmdUI->SetCheck(TRUE);
   }
}
void CChildFrame::OnMapbarFill() 
{
   if (m_fill_tool != ID_EDIT_FILL) {
   	m_map_bar.SetButtonInfo(ID_MAPBAR_FILL_ITEMS, ID_EDIT_FILL, TBSTYLE_BUTTON | TBSTYLE_DROPDOWN, ID_MAPBAR_ITEM_FILL);
      m_fill_tool = ID_EDIT_FILL;
   }
   ((CDepthmapView *)GetActiveView())->SendMessage(WM_COMMAND,ID_EDIT_FILL);
}
void CChildFrame::OnUpdateMapbarFill(CCmdUI* pCmdUI) 
{
   if (m_fill_tool == ID_EDIT_FILL) {
      pCmdUI->SetCheck(TRUE);
   }
}
void CChildFrame::OnMapbarSemifill() 
{
   if (m_fill_tool != ID_EDIT_SEMIFILL) {
   	m_map_bar.SetButtonInfo(ID_MAPBAR_FILL_ITEMS, ID_EDIT_SEMIFILL, TBSTYLE_BUTTON | TBSTYLE_DROPDOWN, ID_MAPBAR_ITEM_SEMIFILL);
      m_fill_tool = ID_EDIT_SEMIFILL;
   }
   ((CDepthmapView *)GetActiveView())->SendMessage(WM_COMMAND,ID_EDIT_SEMIFILL);
}
void CChildFrame::OnUpdateMapbarSemifill(CCmdUI* pCmdUI) 
{
   if (m_fill_tool == ID_EDIT_SEMIFILL) {
      pCmdUI->SetCheck(TRUE);
   }
}
void CChildFrame::OnMapbarLine() 
{
   if (m_draw_tool != ID_EDIT_LINETOOL) {
   	m_map_bar.SetButtonInfo(ID_MAPBAR_DRAW_ITEMS, ID_EDIT_LINETOOL, TBSTYLE_BUTTON | TBSTYLE_DROPDOWN, ID_MAPBAR_ITEM_LINETOOL);
      m_draw_tool = ID_EDIT_LINETOOL;
   }
   ((CDepthmapView *)GetActiveView())->SendMessage(WM_COMMAND,ID_EDIT_LINETOOL);
}
void CChildFrame::OnUpdateMapbarLine(CCmdUI* pCmdUI) 
{
   if (m_draw_tool == ID_EDIT_LINETOOL) {
      pCmdUI->SetCheck(TRUE);
   }
}
void CChildFrame::OnMapbarPolygon() 
{
   if (m_draw_tool != ID_EDIT_POLYGON) {
   	m_map_bar.SetButtonInfo(ID_MAPBAR_DRAW_ITEMS, ID_EDIT_POLYGON, TBSTYLE_BUTTON | TBSTYLE_DROPDOWN, ID_MAPBAR_ITEM_POLYGON);
      m_draw_tool = ID_EDIT_POLYGON;
   }
   ((CDepthmapView *)GetActiveView())->SendMessage(WM_COMMAND,ID_EDIT_POLYGON);
}
void CChildFrame::OnUpdateMapbarPolygon(CCmdUI* pCmdUI) 
{
   if (m_draw_tool == ID_EDIT_POLYGON) {
      pCmdUI->SetCheck(TRUE);
   }
}
void CChildFrame::OnMapbarIsovist() 
{
   if (m_isovist_tool != ID_MODE_ISOVIST) {
   	m_map_bar.SetButtonInfo(ID_MAPBAR_ISOVIST_ITEMS, ID_MODE_ISOVIST, TBSTYLE_BUTTON | TBSTYLE_DROPDOWN, ID_MAPBAR_ITEM_ISOVIST);
      m_isovist_tool = ID_MODE_ISOVIST;
   }
   ((CDepthmapView *)GetActiveView())->SendMessage(WM_COMMAND,ID_MODE_ISOVIST);
}
void CChildFrame::OnUpdateMapbarIsovist(CCmdUI* pCmdUI) 
{
   if (m_isovist_tool == ID_MODE_ISOVIST) {
      pCmdUI->SetCheck(TRUE);
   }
}
void CChildFrame::OnMapbarHalfovist() 
{
   if (m_isovist_tool != ID_MODE_HALFOVIST) {
   	m_map_bar.SetButtonInfo(ID_MAPBAR_ISOVIST_ITEMS, ID_MODE_HALFOVIST, TBSTYLE_BUTTON | TBSTYLE_DROPDOWN, ID_MAPBAR_ITEM_HALFISOVIST);
      m_isovist_tool = ID_MODE_HALFOVIST;
   }
   ((CDepthmapView *)GetActiveView())->SendMessage(WM_COMMAND,ID_MODE_HALFOVIST);
}
void CChildFrame::OnUpdateMapbarHalfovist(CCmdUI* pCmdUI) 
{
   if (m_isovist_tool == ID_MODE_HALFOVIST) {
      pCmdUI->SetCheck(TRUE);
   }
}
void CChildFrame::OnMapbarJoin() 
{
   if (m_link_tool != ID_MODE_JOIN) {
   	m_map_bar.SetButtonInfo(ID_MAPBAR_JOIN_ITEMS, ID_MODE_JOIN, TBSTYLE_BUTTON | TBSTYLE_DROPDOWN, ID_MAPBAR_ITEM_JOIN);
      m_link_tool = ID_MODE_JOIN;
   }
   ((CDepthmapView *)GetActiveView())->SendMessage(WM_COMMAND,ID_MODE_JOIN);
}
void CChildFrame::OnUpdateMapbarJoin(CCmdUI* pCmdUI) 
{
   if (m_link_tool == ID_MODE_JOIN) {
      pCmdUI->SetCheck(TRUE);
   }
}
void CChildFrame::OnMapbarUnjoin() 
{
   if (m_link_tool != ID_MODE_UNJOIN) {
   	m_map_bar.SetButtonInfo(ID_MAPBAR_JOIN_ITEMS, ID_MODE_UNJOIN, TBSTYLE_BUTTON | TBSTYLE_DROPDOWN, ID_MAPBAR_ITEM_UNJOIN);
      m_link_tool = ID_MODE_UNJOIN;
   }
   ((CDepthmapView *)GetActiveView())->SendMessage(WM_COMMAND,ID_MODE_UNJOIN);
}
void CChildFrame::OnUpdateMapbarUnjoin(CCmdUI* pCmdUI) 
{
   if (m_link_tool == ID_MODE_UNJOIN) {
      pCmdUI->SetCheck(TRUE);
   }
}

