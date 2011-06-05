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

// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Depthmap.h"

#include "MainFrm.h"
#include "GraphDoc.h"
#include "DepthmapView.h"
#include "PlotView.h"

#include "DepthmapOptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_LINELAYERS, OnViewLineLayers)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LINELAYERS, OnUpdateViewLineLayers)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_UPDATE_COMMAND_UI(ID_VIEW_SCATTERPLOT, OnUpdateWindowScatterplot)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_MAP, OnUpdateWindowMap)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_TABLE, OnUpdateViewTable)
	ON_COMMAND(ID_VIEW_COLOURRANGE, OnViewColourRange)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOURRANGE, OnUpdateViewColourRange)
	ON_WM_MOVE()
	//}}AFX_MSG_MAP
   ON_MESSAGE(WM_DMP_FOCUS_GRAPH, OnFocusGraph)
	ON_WM_CTLCOLOR()
   ON_COMMAND(ID_TOOLS_OPTIONS, &CMainFrame::OnToolsOptions)
   ON_COMMAND(ID_SHOW_RESEARCHTOOLBAR, &CMainFrame::OnShowResearchtoolbar)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
   ID_INDICATOR_POINTS,
   ID_INDICATOR_DIMENSIONS,
   ID_INDICATOR_POSITION
	//ID_INDICATOR_CAPS,
	//ID_INDICATOR_NUM,
	//ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	if (!m_wndToolBar.CreateEx(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
 
	if (!m_wndReBar.Create(this,RBS_AUTOSIZE|RBS_BANDBORDERS) ||
		!m_wndReBar.AddBar(&m_wndToolBar) )
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}
   // Tool tips

   m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);
/*
// removed as layer functionality no longer works

   if (GetApp()->ModuleLoaded(CDepthmapApp::AGENT_MODULE)) {
   	if (!m_wndAgentToolBar.CreateEx(this) ||
	   	 !m_wndAgentToolBar.LoadToolBar(IDR_AGENT_TOOLBAR) ||
          !m_wndReBar.AddBar(&m_wndAgentToolBar))
	   {
		   TRACE0("Failed to create research toolbar\n");
		   return -1;      // fail to create
	   }
      m_wndAgentToolBar.SetBarStyle(m_wndAgentToolBar.GetBarStyle() |
		   CBRS_TOOLTIPS | CBRS_FLYBY);
   }
*/
	if (!m_wndResearchToolBar.CreateEx(this) ||
   	 !m_wndResearchToolBar.LoadToolBar(IDR_RESEARCH_TOOLBAR) ||
       !m_wndReBar.AddBar(&m_wndResearchToolBar))
   {
	   TRACE0("Failed to create research toolbar\n");
	   return -1;      // fail to create
   }
   m_wndResearchToolBar.SetBarStyle(m_wndResearchToolBar.GetBarStyle() |
	   CBRS_TOOLTIPS | CBRS_FLYBY);

   ////////////////////////////////////////////////////////

   if (!m_wndSideBar.Create(this, IDD_SIDEBAR, CBRS_LEFT, ID_SIDEBAR))
	{
		TRACE0("Failed to create sidebar\n");
		return -1;		// fail to create
	}

   ///////////////////////////////////////////////////////////////

   if (!m_wndColourScale.Create(IDD_COLOUR_SCALE, this)) {
		TRACE0("Failed to create colour control dialog\n");
		return -1;		// fail to create
   }


	return 0;
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

void CMainFrame::GetActualClientRect(CRect& rect)
{
   GetClientRect(rect);
   CRect temprect;
   m_wndSideBar.GetWindowRect(temprect);
   rect.right -= temprect.Width();
   m_wndReBar.GetWindowRect(temprect);
   rect.bottom -= temprect.Height();
   m_wndStatusBar.GetWindowRect(temprect);
   rect.bottom -= temprect.Height();
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnClose() 
{
   if (GetApp()->m_process_count) {
      AfxMessageBox(_T("Depthmap is processing... please cancel process before continuing"));
   }
   else {
   	CMDIFrameWnd::OnClose();
   }
}

BOOL CMainFrame::DestroyWindow() 
{

   return CMDIFrameWnd::DestroyWindow();
}

/*
void CMainFrame::OnEditPencil() 
{
   // Test code... 1, 2, 3... destroy the toolbar arrrrgghghghghghgghghgh!
   
   m_wndToolBar.DestroyWindow();

	if (!m_wndToolBar.CreateEx(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
	}

   m_wndReBar.AddBar(&m_wndToolBar);
}
*/

void CMainFrame::OnViewLineLayers() 
{
   /*
   if (m_linelayer_chooser) {
      m_linelayer_chooser->DestroyWindow();
      delete m_linelayer_chooser;
      m_linelayer_chooser = NULL;
   }
   else {
      m_linelayer_chooser = new CLayerChooser(this);

      CMDIChildWnd *pChild = MDIGetActive();
      if (pChild) {
         CDepthmapView *pView = (CDepthmapView *) pChild->GetActiveView();
         if (pView) {
            m_linelayer_chooser->PostMessage(WM_DMP_FOCUS_GRAPH, (UINT) pView->GetDocument(), 1);
         }
      }
   }
   */
}

void CMainFrame::OnUpdateViewLineLayers(CCmdUI* pCmdUI) 
{
   /*
   if (m_linelayer_chooser) {
      pCmdUI->SetCheck(1);
   }
   else {
      pCmdUI->SetCheck(0);
   }
   */
}

void CMainFrame::OnViewColourRange() 
{
   if (m_wndColourScale.IsWindowVisible()) {
      m_wndColourScale.ShowWindow(SW_HIDE);
   }
   else {
      CRect recta,rectb;
      GetWindowRect(recta);
      m_wndColourScale.GetWindowRect(rectb);
      m_wndColourScale.SetWindowPos(NULL, recta.right - 7 - rectb.Width(), recta.top + 68, -1, -1, 
         SWP_SHOWWINDOW | SWP_NOOWNERZORDER | SWP_NOSENDCHANGING | SWP_NOSIZE );
      m_wndColourScale.m_docked = true;
   }
}

void CMainFrame::OnUpdateViewColourRange(CCmdUI* pCmdUI) 
{
   if (m_wndColourScale.IsWindowVisible()) {
      pCmdUI->SetCheck(1);
   }
   else {
      pCmdUI->SetCheck(0);
   }
}


LRESULT CMainFrame::OnFocusGraph(WPARAM wParam, LPARAM lParam)
{
   /*
   if (m_linelayer_chooser) {
      if (lParam == 9) {
         m_linelayer_chooser->DestroyWindow();
         delete m_linelayer_chooser;
         m_linelayer_chooser = NULL;
      }
      else {
         m_linelayer_chooser->PostMessage(WM_DMP_FOCUS_GRAPH, wParam, lParam);
      }
   }
   */

   m_wndColourScale.PostMessage(WM_DMP_FOCUS_GRAPH, wParam, lParam);
   return m_wndSideBar.PostMessage(WM_DMP_FOCUS_GRAPH, wParam, lParam);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIFrameWnd::OnSize(nType, cx, cy);

   // This keeps a "docked" colour scale dialog in the same relative position:
   if (m_wndColourScale.IsWindowVisible() && m_wndColourScale.m_docked) {
      CRect recta,rectb;
      GetWindowRect(recta);
      m_wndColourScale.GetWindowRect(rectb);
      m_wndColourScale.SetWindowPos(NULL, recta.right - 7 - rectb.Width(), recta.top + 68, -1, -1, 
         SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOSENDCHANGING | SWP_NOSIZE );
      m_wndColourScale.m_docked = true;
   }

   // I like rebar bands left aligned - this code does that for you
   m_wndReBar.GetReBarCtrl().MaximizeBand( m_wndReBar.GetReBarCtrl().GetBandCount() - 1);
}

// if no graph is shown, uncheck these items:

void CMainFrame::OnUpdateWindowMap(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(0);
}

void CMainFrame::OnUpdateWindowScatterplot(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(0);
}

void CMainFrame::OnUpdateViewTable(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(0);
}



void CMainFrame::OnMove(int x, int y) 
{
   // This keeps a "docked" colour scale dialog in the same relative position:
   if (m_wndColourScale.IsWindowVisible() && m_wndColourScale.m_docked) {
      CRect recta,rectb;
      GetWindowRect(recta);
      m_wndColourScale.GetWindowRect(rectb);
      m_wndColourScale.SetWindowPos(NULL, recta.right - 7 - rectb.Width(), recta.top + 68, -1, -1, 
         SWP_NOACTIVATE | SWP_NOSENDCHANGING | SWP_NOOWNERZORDER | SWP_NOSIZE );
      m_wndColourScale.m_docked = true;
   }

   CMDIFrameWnd::OnMove(x, y);
}

void CMainFrame::OnToolsOptions()
{
   CDepthmapOptionsDlg dlg;

   if (GetApp()->m_show_researchtoolbar) {
      dlg.m_show_research_toolbar = true;
   }

   if (IDOK == dlg.DoModal()) {
      if (dlg.m_show_research_toolbar) {
         GetApp()->m_show_researchtoolbar = true;
      }
      else {
         GetApp()->m_show_researchtoolbar = false;
      }
      OnShowResearchtoolbar();
   }
}

void CMainFrame::OnShowResearchtoolbar()
{
   if (GetApp()->m_show_researchtoolbar) {
      m_wndResearchToolBar.ShowWindow(SW_SHOW);
   }
   else {
      m_wndResearchToolBar.ShowWindow(SW_HIDE);
   }
}
