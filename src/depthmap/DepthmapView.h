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

// DepthmapView.h : interface of the CDepthmapView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEPTHMAPVIEW_H__71D5A06E_ED18_11D3_A8D5_00609787FC1E__INCLUDED_)
#define AFX_DEPTHMAPVIEW_H__71D5A06E_ED18_11D3_A8D5_00609787FC1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGraphDoc;
class CHoverWnd;

// CPoint dist
inline float dist(const CPoint a, const CPoint b)
{
   return (float) sqrt(sqr(a.x-b.x)+sqr(a.y-b.y));
}

class CDepthmapView : public CView
{
protected: // create from serialization only
	CDepthmapView();
	DECLARE_DYNCREATE(CDepthmapView)

// Attributes
public:
	CGraphDoc* GetDocument();

   HCURSOR m_cursor;

   CPoint m_mouse_point;
   Point2f m_mouse_location;

   int m_current_mode;

   // Selectable mouse modes
   enum {NONE = 0x0000, SELECT = 0x10000, FILL = 0x0002, SEEDAXIAL = 0x0004, 
         LINETOOL = 0x0008, POLYGONTOOL = 0x0010,
         ZOOMORDRAG  = 0x0300, 
         DRAG = 0x0101, 
         ZOOM = 0x0200, ZOOM_IN = 0x0202, ZOOM_OUT = 0x0204, 
         GENERICPENCIL = 0x0800, PENCIL = 0x0801, ERASE = 0x0802,
         SNAP = 0x1000, SNAPON = 0x1001, SNAPOFF = 0x1002, // snap modes are not used outside invalidate -- '1000' used to test snap
         DRAWLINE = 0x2000, LINEON = 0x2001, LINEOFF = 0x2002,
         GENERICISOVIST = 0x4000, SEEDISOVIST = 0x4001, SEEDHALFOVIST = 0x4002,
         OVERHANDLE = 0x8000,
         GENERICJOIN = 0x20000, JOINB = 0x00400, JOIN = 0x20001, UNJOIN = 0x20002

   };
   int m_mouse_mode;
   enum {FULLFILL = 0, SEMIFILL};
   int m_fillmode;

   CRect m_drag_rect_a;
   CRect m_drag_rect_b;

   COLORREF m_selected_color;

   bool ModeOk();

   // lots of screen drawing booleans... to be tidied up...
   bool m_drawing;
   bool m_continue_drawing;
   //
   int m_invalidate; // <- includes the mode
   bool m_queued_redraw;
   bool m_internal_redraw;
   bool m_clear;
   bool m_redraw;

   bool m_resize_viewport;
   bool m_viewport_set;
   bool m_redraw_all;
   bool m_redraw_no_clear;

   bool m_right_mouse_drag;
   bool m_alt_mode;

   // logical units:
   Point2f m_centre;
   double m_unit;
   // keep tabs on the screen size:
   CSize m_physical_centre;

   // record any previous find location loc:
   Point2f m_lastfindloc;

   bool m_showgrid;
   bool m_showtext;
   bool m_showlinks;

   // this is a tells us how many 100ms ticks have passed since the mouse moved
   int m_hover_tick;
   // the point where the mouse is hovering
   CPoint m_hover_point;
   // this is a window created after mouse hovers for a period of time
   CHoverWnd *m_hover_wnd;

   // Snap to control (must be in map units for accuracy)
   Point2f m_snap_point;
   Point2f m_old_snap_point;
   COLORREF m_cross_pixels[18];
   bool m_snap;
   int m_repaint_tag;

   // Line start and end (must be in map units in case you zoom out / pan while you're drawing!
   Line m_line;
   Line m_old_line;
   pvector<COLORREF> m_line_pixels;
   prefvec<Point2f> m_point_handles;
   int m_active_point_handle;

   // polygon drawing utilities
   Point2f m_poly_start;
   int m_poly_points;


// Operations
public:
   Point2f LogicalUnits( const CPoint& p );
   CPoint PhysicalUnits( const Point2f& p );

   // hover information
   void ResetHoverWnd(const CPoint& p = CPoint(-1,-1));
   void CreateHoverWnd();

   void ZoomIn(double ratio, const Point2f& point);
   void ZoomOut();

   void InitViewport(const CRect& phys_bounds, CGraphDoc *pDoc);
   Region LogicalViewport(const CRect& phys_bounds, CGraphDoc *pDoc);

   int GetSpacer(CGraphDoc *pDoc);
   void PrintBaby(CDC *pDC, CGraphDoc *pDoc);
   bool Output(CDC *pDC, CGraphDoc *pDoc, bool screendraw);
   bool DrawPoints(CDC *pDC, CGraphDoc *pDoc, int spacer, DWORD ticks, bool screendraw);
   bool DrawAxial(CDC *pDC, CGraphDoc *pDoc, int spacer, DWORD ticks, bool screendraw);
//   bool DrawShapes(CDC *pDC, CGraphDoc *pDoc, int spacer, DWORD ticks, bool screendraw);
   bool DrawShapes(CDC *pDC, ShapeMap& map, bool muted, int spacer, DWORD ticks, bool screendraw);

   void DrawLink(CDC *pDC, int spacer, const Line& logical);
   void DrawPointHandle(CDC *pDC, CPoint pt);

   //
   void OutputEPS( ofstream& stream, CGraphDoc *pDoc );
   void OutputEPSMap(ofstream& stream, ShapeMap& map, Region& logicalviewport, CRect& rect, float spacer);
   void OutputEPSLine(ofstream& stream, Line& line, int spacer, Region& logicalviewport, CRect& rect);
   void OutputEPSPoly(ofstream& stream, const SalaShape& shape, int spacer, Region& logicalviewport, CRect& rect);

   void OutputSVG( ofstream& stream, CGraphDoc *pDoc );
   void OutputSVGMap(ofstream& stream, ShapeMap& map, Region& logicalviewport, int h);
   void OutputSVGLine(ofstream& stream, Line& line, Region& logicalviewport, int h);
   void OutputSVGPoly(ofstream& stream, const SalaShape& shape, Region& logicalviewport, int h);

   void FillLocation(CDC *pDC, CPoint& p, int spacer, unsigned int blocked, COLORREF color);
   void DrawLine(CDC *pDC, CRect& line, bool drawit);
   void DrawCross(CDC *pDC, CPoint& centre, bool drawit);

   void SetCursor(int mode);
   void AltMode();
   void BeginJoin();
   void BeginDrag(CPoint point);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDepthmapView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDepthmapView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Message map functions
protected:
   void OnUpdateGenericDrawingTools(CCmdUI* pCmdUI, int mode);
   void OnUpdateGeneric(CCmdUI* pCmdUI, int mode, int fillmode = 0, bool allowgraph = true);
   void OnUpdateGenericJoin(CCmdUI* pCmdUI, int mode);
   afx_msg LRESULT OnRedraw(WPARAM wParam, LPARAM lParam);
// Generated:
   //{{AFX_MSG(CDepthmapView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnEditFill();
	afx_msg void OnViewZoomIn();
	afx_msg void OnViewZoomOut();
	afx_msg void OnUpdateViewZoomOut(CCmdUI* pCmdUI);
	afx_msg void OnViewMove();
	afx_msg void OnUpdateViewMove(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnUpdateEditSelect(CCmdUI* pCmdUI);
	afx_msg void OnEditSelect();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnUpdateEditFill(CCmdUI* pCmdUI);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnUpdateEditPencil(CCmdUI* pCmdUI);
	afx_msg void OnEditPencil();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEditCopy();
	afx_msg void OnEditSave();
	afx_msg void OnModeJoin();
	afx_msg void OnUpdateModeJoin(CCmdUI* pCmdUI);
	afx_msg void OnEditEraser();
	afx_msg void OnUpdateEditEraser(CCmdUI* pCmdUI);
	afx_msg void OnEditLineTool();
	afx_msg void OnUpdateEditLineTool(CCmdUI* pCmdUI);
	afx_msg void OnToolsAxialMap();
	afx_msg void OnUpdateToolsAxialMap(CCmdUI* pCmdUI);
	afx_msg void OnViewCentreView();
	afx_msg void OnEditSemiFill();
	afx_msg void OnUpdateEditSemiFill(CCmdUI* pCmdUI);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnModeIsovist();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnUpdateModeIsovist(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewZoomIn(CCmdUI* pCmdUI);
	afx_msg void OnModeUnjoin();
	afx_msg void OnUpdateModeUnjoin(CCmdUI* pCmdUI);
	afx_msg void OnModeHalfovist();
	afx_msg void OnUpdateModeHalfovist(CCmdUI* pCmdUI);
	afx_msg void OnEditPolygon();
	afx_msg void OnUpdateEditPolygon(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnViewFindloc();
public:
   afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
public:
   afx_msg void OnViewZoomsel();
public:
   afx_msg void OnUpdateViewZoomsel(CCmdUI *pCmdUI);
};

inline Point2f CDepthmapView::LogicalUnits( const CPoint& p )
{
   return m_centre + Point2f(m_unit * double(p.x - m_physical_centre.cx), 
                             m_unit * double(m_physical_centre.cy - p.y));
}

inline CPoint CDepthmapView::PhysicalUnits( const Point2f& p )
{
   return CPoint( m_physical_centre.cx + int((p.x - m_centre.x) / m_unit + 0.4999), 
                  m_physical_centre.cy - int((p.y - m_centre.y) / m_unit + 0.4999) );
}


inline int PixelDist(CPoint a, CPoint b)
{
   return (int)sqrt(double((b.x-a.x)*(b.x-a.x)+(b.y-a.y)*(b.y-a.y)));
}

#ifndef _DEBUG  // debug version in DepthmapView.cpp
inline CGraphDoc* CDepthmapView::GetDocument()
   { return (CGraphDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEPTHMAPVIEW_H__71D5A06E_ED18_11D3_A8D5_00609787FC1E__INCLUDED_)
