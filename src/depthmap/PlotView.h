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

#if !defined(AFX_SCATTERVIEW_H__AE692009_1EFF_4FCF_9A74_6D6B66FDE9FA__INCLUDED_)
#define AFX_SCATTERVIEW_H__AE692009_1EFF_4FCF_9A74_6D6B66FDE9FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlotView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PlotView view

class CHoverWnd;

class CPlotView : public CView
{
protected:
	CPlotView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPlotView)

// Attributes
public:
	CGraphDoc* GetDocument();

   int m_x_axis;
   int m_y_axis;

   AttributeIndex idx_x;
   AttributeIndex idx_y;
   void RedoIndices();

   bool m_queued_redraw;

   bool m_view_origin;
   bool m_view_trend_line;
   bool m_view_equation;
   bool m_view_rsquared;
   bool m_view_monochrome;

   LinReg<float> m_regression;

   //
   double dataX(int x);
   int screenX(double x);
   double dataY(int x);
   int screenY(double x);
   //
   Region m_data_bounds;
   CRect m_screen_bounds;
   //
   CPoint m_mouse_point;
   CRect m_drag_rect_a;
   CRect m_drag_rect_b;
   bool m_selecting;
   bool m_drawdragrect;

// Operations
public:
   void SetAxis(int axis, int col, bool reset);
   //void SetYAxis(int col);
   //void ResetAxes(int xcol, int ycol);
   void ResetRegression();

   bool Output(CDC *pDC, CGraphDoc *pDoc, bool screendraw);
   void PrintOutput(CDC *pDC, CGraphDoc *pDoc);

   // this is a tells us how many 100ms ticks have passed since the mouse moved
   int m_hover_tick;
   // the point where the mouse is hovering
   CPoint m_hover_point;
   // this is a window created after mouse hovers for a period of time
   CHoverWnd *m_hover_wnd;
   // hover information
   void ResetHoverWnd(const CPoint& p = CPoint(-1,-1));
   void CreateHoverWnd();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlotView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPlotView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
   afx_msg LRESULT OnRedraw(WPARAM wParam, LPARAM lParam);
	//{{AFX_MSG(CPlotView)
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnViewTrendLine();
	afx_msg void OnUpdateViewTrendLine(CCmdUI* pCmdUI);
	afx_msg void OnViewRsquared();
	afx_msg void OnUpdateViewRsquared(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnViewColor();
	afx_msg void OnUpdateViewColor(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnViewOrigin();
	afx_msg void OnUpdateViewOrigin(CCmdUI* pCmdUI);
	afx_msg void OnViewEquation();
	afx_msg void OnUpdateViewEquation(CCmdUI* pCmdUI);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
public:
   afx_msg void OnKillFocus(CWnd* pNewWnd);
};

#ifndef _DEBUG  // debug version in CPlotView.cpp
inline CGraphDoc* CPlotView::GetDocument()
   { return (CGraphDoc*)m_pDocument; }
#endif


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCATTERVIEW_H__AE692009_1EFF_4FCF_9A74_6D6B66FDE9FA__INCLUDED_)
