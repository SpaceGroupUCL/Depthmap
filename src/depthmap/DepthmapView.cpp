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

// DepthmapView.cpp : implementation of the CDepthmapView class
//

#include "stdafx.h"
#include "Depthmap.h"

#include "GraphDoc.h"
#include "HoverWnd.h"
#include "DepthmapView.h"
#include "FindLocDlg.h"
#include "SetAttrValueDlg.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDepthmapView

IMPLEMENT_DYNCREATE(CDepthmapView, CView)

BEGIN_MESSAGE_MAP(CDepthmapView, CView)
	//{{AFX_MSG_MAP(CDepthmapView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
   ON_COMMAND(ID_EDIT_FILL, OnEditFill)
	ON_COMMAND(ID_VIEW_ZOOM_IN, OnViewZoomIn)
	ON_COMMAND(ID_VIEW_ZOOM_OUT, OnViewZoomOut)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM_OUT, OnUpdateViewZoomOut)
	ON_COMMAND(ID_VIEW_MOVE, OnViewMove)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MOVE, OnUpdateViewMove)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT, OnUpdateEditSelect)
	ON_COMMAND(ID_EDIT_SELECT, OnEditSelect)
	ON_WM_KEYDOWN()
	ON_UPDATE_COMMAND_UI(ID_EDIT_FILL, OnUpdateEditFill)
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_KEYUP()
	ON_UPDATE_COMMAND_UI(ID_EDIT_PENCIL, OnUpdateEditPencil)
	ON_COMMAND(ID_EDIT_PENCIL, OnEditPencil)
	ON_WM_SETFOCUS()
	ON_WM_TIMER()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_SAVESCREEN, OnEditSave)
	ON_COMMAND(ID_MODE_JOIN, OnModeJoin)
	ON_UPDATE_COMMAND_UI(ID_MODE_JOIN, OnUpdateModeJoin)
	ON_COMMAND(ID_EDIT_ERASER, OnEditEraser)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ERASER, OnUpdateEditEraser)
	ON_COMMAND(ID_EDIT_LINETOOL, OnEditLineTool)
	ON_UPDATE_COMMAND_UI(ID_EDIT_LINETOOL, OnUpdateEditLineTool)
	ON_COMMAND(ID_TOOLS_AL2, OnToolsAxialMap)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_AL2, OnUpdateToolsAxialMap)
	ON_COMMAND(ID_VIEW_CENTREVIEW, OnViewCentreView)
	ON_COMMAND(ID_EDIT_SEMIFILL, OnEditSemiFill)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SEMIFILL, OnUpdateEditSemiFill)
	ON_WM_KILLFOCUS()
	ON_COMMAND(ID_MODE_ISOVIST, OnModeIsovist)
	ON_WM_MOUSEWHEEL()
	ON_UPDATE_COMMAND_UI(ID_MODE_ISOVIST, OnUpdateModeIsovist)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM_IN, OnUpdateViewZoomIn)
	ON_COMMAND(ID_MODE_UNJOIN, OnModeUnjoin)
	ON_UPDATE_COMMAND_UI(ID_MODE_UNJOIN, OnUpdateModeUnjoin)
	ON_COMMAND(ID_MODE_HALFOVIST, OnModeHalfovist)
	ON_UPDATE_COMMAND_UI(ID_MODE_HALFOVIST, OnUpdateModeHalfovist)
	ON_COMMAND(ID_EDIT_POLYGON, OnEditPolygon)
	ON_UPDATE_COMMAND_UI(ID_EDIT_POLYGON, OnUpdateEditPolygon)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
   //
   ON_MESSAGE(WM_DMP_REDRAW_MESSAGE, OnRedraw)
   ON_COMMAND(ID_VIEW_FINDLOC, &CDepthmapView::OnViewFindloc)
   ON_WM_LBUTTONDBLCLK()
   ON_COMMAND(ID_VIEW_ZOOMSEL, &CDepthmapView::OnViewZoomsel)
   ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMSEL, &CDepthmapView::OnUpdateViewZoomsel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

// Palette entries...
// A set of colours we would really like to have

const PALETTEENTRY g_palette_entries[] = {
   // some standard colours:
   {0xFF,0x00,0x00,NULL},  // red
   {0x00,0xFF,0x00,NULL},  // green
   {0x00,0x00,0xFF,NULL},  // blue
   // for Depthmap highlight:
   {0x77,0x77,0xFF,NULL},
   {0xFF,0xFF,0x77,NULL},
   {0x77,0xFF,0x77,NULL},
   {0x77,0xBB,0x77,NULL},
   {0x77,0x77,0xBB,NULL},  // 5
   // for the Axman-esqe colour scale and for the greyscale (20 colours)
   {0x33,0x33,0xDD,NULL}, {0x00,0x00,0x00,NULL}, // 0 (black / blue)
   {0x33,0x88,0xDD,NULL}, {0x44,0x44,0x44,NULL},
   {0x22,0xCC,0xDD,NULL}, {0x77,0x77,0x77,NULL},
   {0x22,0xCC,0xBB,NULL}, {0x99,0x99,0x99,NULL},
   {0x22,0xDD,0x88,NULL}, {0xAA,0xAA,0xAA,NULL},
   {0x88,0xDD,0x22,NULL}, {0xBB,0xBB,0xBB,NULL},
   {0xBB,0xCC,0x22,NULL}, {0xCC,0xCC,0xCC,NULL},
   {0xDD,0xCC,0x22,NULL}, {0xDD,0xDD,0xDD,NULL},
   {0xDD,0x88,0x33,NULL}, {0xEE,0xEE,0xEE,NULL},
   {0xDD,0x33,0x33,NULL}, {0xFF,0xFF,0xFF,NULL}, // 9 (white / red)
   // for the Depthmap colour scale (yellow and cyan)
   {0xFF,0xFF,0x00,NULL},
   {0x00,0xFF,0xFF,NULL},
   // red-green and blue-green mixtures (56 colours)
   {0xEE,0xFF,0x00,NULL}, {0xFF,0xEE,0x00,NULL}, {0x00,0xFF,0xEE,NULL}, {0x00,0xEE,0xFF,NULL},
   {0xDD,0xFF,0x00,NULL}, {0xFF,0xDD,0x00,NULL}, {0x00,0xFF,0xDD,NULL}, {0x00,0xDD,0xFF,NULL},
   {0xCC,0xFF,0x00,NULL}, {0xFF,0xCC,0x00,NULL}, {0x00,0xFF,0xCC,NULL}, {0x00,0xCC,0xFF,NULL},
   {0xBB,0xFF,0x00,NULL}, {0xFF,0xBB,0x00,NULL}, {0x00,0xFF,0xBB,NULL}, {0x00,0xBB,0xFF,NULL},
   {0xAA,0xFF,0x00,NULL}, {0xFF,0xAA,0x00,NULL}, {0x00,0xFF,0xAA,NULL}, {0x00,0xAA,0xFF,NULL},
   {0x99,0xFF,0x00,NULL}, {0xFF,0x99,0x00,NULL}, {0x00,0xFF,0x99,NULL}, {0x00,0x99,0xFF,NULL},
   {0x88,0xFF,0x00,NULL}, {0xFF,0x88,0x00,NULL}, {0x00,0xFF,0x88,NULL}, {0x00,0x88,0xFF,NULL},
   {0x77,0xFF,0x00,NULL}, {0xFF,0x77,0x00,NULL}, {0x00,0xFF,0x77,NULL}, {0x00,0x77,0xFF,NULL},
   {0x66,0xFF,0x00,NULL}, {0xFF,0x66,0x00,NULL}, {0x00,0xFF,0x66,NULL}, {0x00,0x66,0xFF,NULL},
   {0x55,0xFF,0x00,NULL}, {0xFF,0x55,0x00,NULL}, {0x00,0xFF,0x55,NULL}, {0x00,0x55,0xFF,NULL},
   {0x44,0xFF,0x00,NULL}, {0xFF,0x44,0x00,NULL}, {0x00,0xFF,0x44,NULL}, {0x00,0x44,0xFF,NULL},
   {0x33,0xFF,0x00,NULL}, {0xFF,0x33,0x00,NULL}, {0x00,0xFF,0x33,NULL}, {0x00,0x33,0xFF,NULL},
   {0x22,0xFF,0x00,NULL}, {0xFF,0x22,0x00,NULL}, {0x00,0xFF,0x22,NULL}, {0x00,0x22,0xFF,NULL},
   {0x11,0xFF,0x00,NULL}, {0xFF,0x11,0x00,NULL}, {0x00,0xFF,0x11,NULL}, {0x00,0x11,0xFF,NULL},
   // magenta mixtures (only part scale required: 14 colours)
   {0xFF,0x00,0x77,NULL}, {0x77,0x00,0xFF,NULL},
   {0xFF,0x00,0x66,NULL}, {0x66,0x00,0xFF,NULL},
   {0xFF,0x00,0x55,NULL}, {0x55,0x00,0xFF,NULL},
   {0xFF,0x00,0x44,NULL}, {0x44,0x00,0xFF,NULL},
   {0xFF,0x00,0x33,NULL}, {0x33,0x00,0xFF,NULL},
   {0xFF,0x00,0x22,NULL}, {0x22,0x00,0xFF,NULL},
   {0xFF,0x00,0x11,NULL}, {0x11,0x00,0xFF,NULL}
};

LOGPALETTE g_logpalette = {0, 98, (unsigned char) g_palette_entries};

CPalette *g_color_palette = NULL;
int g_ref_count = 0;

/////////////////////////////////////////////////////////////////////////////
// CDepthmapView construction/destruction

CDepthmapView::CDepthmapView()
{
   m_drag_rect_a.SetRectEmpty();
   m_drag_rect_b.SetRectEmpty();

   // Several screen drawing booleans:
   m_continue_drawing = false;
   m_drawing = false;
   m_queued_redraw = false;

   m_viewport_set = false;
   m_clear = false;
   m_redraw = false;

   m_redraw_all = false;
   m_redraw_no_clear = false;

   m_resize_viewport = false;

   m_invalidate = false; // our own invalidation

   m_right_mouse_drag = false;
   m_alt_mode = false;

   m_current_mode = NONE;

   if (g_ref_count == 0) {
      if (!g_color_palette) {
         g_color_palette = new CPalette;
         g_color_palette->CreatePalette(&g_logpalette);
      }
   }
   g_ref_count++;

   m_hover_tick = 0;
   m_hover_wnd = NULL;

   m_snap = false;

   m_repaint_tag = 0;

   m_showlinks = false;

   m_mouse_mode = SELECT;
   m_fillmode = FULLFILL;
   m_cursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);

   m_active_point_handle = -1;

   m_poly_points = 0;

   PafColor selcol(SALA_SELECTED_COLOR);
   m_selected_color = RGB(selcol.redb(),selcol.greenb(),selcol.blueb());
}

CDepthmapView::~CDepthmapView()
{
   g_ref_count--;
   if (g_ref_count == 0) {
      if (g_color_palette) {
         delete g_color_palette;
      }
      g_color_palette = NULL;
   }
}

BOOL CDepthmapView::PreCreateWindow(CREATESTRUCT& cs)
{
   // Register the class 
   // Byte align the client window --- it draws it faster
   cs.lpszClass = AfxRegisterWndClass(CS_BYTEALIGNCLIENT|CS_DBLCLKS); // <- add this if you want window to receive double clicks

	return CView::PreCreateWindow(cs);
}

BOOL CDepthmapView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
   // dwStyle |= WS_HSCROLL | WS_VSCROLL; <- scroll bars disabled after input from Ava -> use the hand instead from a right click menu

	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

/////////////////////////////////////////////////////////////////////////////
// CDepthmapView drawing

void CDepthmapView::InitViewport(const CRect& phys_bounds, CGraphDoc *pDoc)
{
   Region bounds = pDoc->m_meta_graph->getBoundingBox();
   m_unit = __max( bounds.width() / double(phys_bounds.Width()), 
                   bounds.height() / double(phys_bounds.Height()) );
   m_centre = bounds.getCentre();
   m_physical_centre = CSize(phys_bounds.Width() / 2, phys_bounds.Height() / 2);

   m_viewport_set = true;
}

Region CDepthmapView::LogicalViewport(const CRect& phys_bounds, CGraphDoc *pDoc)
{
   if (m_resize_viewport) {
      m_physical_centre = CSize(phys_bounds.Width() / 2, phys_bounds.Height() / 2);
      m_resize_viewport = false;
   }

   return Region( LogicalUnits(CPoint(phys_bounds.left,phys_bounds.bottom)), 
                  LogicalUnits(CPoint(phys_bounds.right,phys_bounds.top)) );
}

void CDepthmapView::OnDraw(CDC* pDC)
{
	CGraphDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

   if (pDC->IsPrinting()) {

      if (!pDoc->m_meta_graph->setLock(this)) {
         return;
      }

      PrintBaby(pDC, pDoc);

      pDoc->m_meta_graph->releaseLock(this);

      return;
   }

   SetTimer(DMP_TIMER_REDRAW,100,NULL);

   if (!pDoc->m_meta_graph->setLock(this)) {
      return;
   }

   m_drawing = true;

   // selected colour is used for picking up highlights in OnMouseMove
   m_selected_color = pDC->GetNearestColor(m_selected_color);

   CRect rect;
   int state = pDoc->m_meta_graph->getState();

   if (m_invalidate) {
      if (m_invalidate & DRAWLINE) {
         if ((m_repaint_tag & DRAWLINE) && (m_invalidate == DRAWLINE || m_invalidate == LINEOFF)) {
            DrawLine(pDC, CRect(PhysicalUnits(m_old_line.start()),PhysicalUnits(m_old_line.end())), false);
            m_repaint_tag &= ~DRAWLINE;
         }
         else if (m_invalidate == DRAWLINE || m_invalidate == LINEON) {
            DrawLine(pDC, CRect(PhysicalUnits(m_line.start()),PhysicalUnits(m_line.end())), true);
            m_old_line = m_line;
            m_repaint_tag |= DRAWLINE;
         }
         m_invalidate = 0;
      }
      else if (m_invalidate & SNAP) {
         if ((m_repaint_tag & SNAP) && (m_invalidate == SNAP || m_invalidate == SNAPOFF)) {
            DrawCross(pDC, PhysicalUnits(m_old_snap_point), false);
            m_repaint_tag &= ~SNAP;
         }
         if (m_invalidate == SNAP || m_invalidate == SNAPON) {
            DrawCross(pDC, PhysicalUnits(m_snap_point), true);
            m_repaint_tag |= SNAP;
         }
         m_old_snap_point = m_snap_point;
         m_invalidate = 0;
      }
      else {
         pDC->DrawDragRect( m_drag_rect_a, CSize(1,1), m_drag_rect_b, CSize(1,1) );
         m_drag_rect_b = m_drag_rect_a;
         m_invalidate = 0;
      }
   }
   else if (m_redraw_all || m_redraw_no_clear) {

      m_repaint_tag = 0;   // <- for things that remember pixels in last paint colour (e.g., DrawCross and DrawLine

      GetClientRect( rect );

      m_redraw = true;

      if (!m_viewport_set && state & (MetaGraph::LINEDATA | MetaGraph::SHAPEGRAPHS | MetaGraph::DATAMAPS)) {
         InitViewport(rect, pDoc);
      }
      if (m_redraw_all) {
         m_clear = true;
      }

      m_redraw_all = false;
      m_redraw_no_clear = false;
   }
   else if (!m_internal_redraw) {
      // Give up on this: it just doesn't work on NT
      // && DCB_RESET != pDC->GetBoundsRect(rect, DCB_RESET)) {

      GetClientRect( rect );
      
      m_clear = true;
      m_redraw = true;

   }
   m_internal_redraw = false;

   // if redraw signalled:
   if (m_redraw && (state & (MetaGraph::LINEDATA | MetaGraph::SHAPEGRAPHS | MetaGraph::DATAMAPS)) && m_viewport_set) {

      // note that the redraw rect is dependent on the cleared portion above
      // note you *must* check *state* before drawing, you cannot rely on view_class as it can be set up before the layer is ready to draw:
      if (state & MetaGraph::POINTMAPS && (!pDoc->m_meta_graph->getDisplayedPointMap().isProcessed() || pDoc->m_meta_graph->getViewClass() & (MetaGraph::VIEWVGA | MetaGraph::VIEWBACKVGA)) && !pDoc->m_communicator) {   // <- m_communicator because I'm having thread locking problems
         pDoc->m_meta_graph->getDisplayedPointMap().setScreenPixel( m_unit ); // only used by points (at the moment!)
         pDoc->m_meta_graph->getDisplayedPointMap().makeViewportPoints( LogicalViewport(rect, pDoc) );
      }
      if (state & MetaGraph::SHAPEGRAPHS && (pDoc->m_meta_graph->getViewClass() & (MetaGraph::VIEWAXIAL | MetaGraph::VIEWBACKAXIAL))) {
         pDoc->m_meta_graph->getDisplayedShapeGraph().makeViewportShapes( LogicalViewport(rect, pDoc) );
      }
      if (state & MetaGraph::DATAMAPS && (pDoc->m_meta_graph->getViewClass() & (MetaGraph::VIEWBACKDATA | MetaGraph::VIEWDATA))) {
         pDoc->m_meta_graph->getDisplayedDataMap().makeViewportShapes( LogicalViewport(rect, pDoc) );
      }
      if (state & MetaGraph::LINEDATA) {
         pDoc->m_meta_graph->SuperSpacePixel::makeViewportShapes( LogicalViewport(rect, pDoc) );
      }

      m_continue_drawing = true;
      m_redraw = false;
   }

   if (m_clear) {
      CBrush brush( GetApp()->m_background );
      CGdiObject *oldbrush = pDC->SelectObject( &brush );
      pDC->PatBlt(rect.left,rect.top,rect.Width(),rect.Height(),PATCOPY);
      pDC->SelectObject( oldbrush );
      m_clear = false;
   }

   // If the meta graph (at least) contains a DXF, draw it:
   if (m_continue_drawing && (state & (MetaGraph::LINEDATA | MetaGraph::SHAPEGRAPHS | MetaGraph::DATAMAPS)) && m_viewport_set) {

      if (Output(pDC, pDoc, true)) {

//         GetApp()->m_redraw = this;
         m_continue_drawing = true;
      }
      else {

         m_continue_drawing = false;
         // Finished: kill the timer:
         if (!m_queued_redraw) {
            KillTimer(DMP_TIMER_REDRAW);
         }
//         GetApp()->m_redraw = NULL;
      }
   }
   else {
      m_continue_drawing = false;   // can't draw for some reason
   }

   m_drawing = false;

   pDoc->m_meta_graph->releaseLock(this);

//   if (m_continue_drawing) {
//      AfxGetApp()->OnIdle(0); // <- I've seen Microsoft do this... give the idle loop a prod
//   }
}

void CDepthmapView::PrintBaby(CDC *pDC, CGraphDoc *pDoc)
{
   CRect rectin, rectout;
   GetClientRect(rectin);

   int pixels;
   if (pDC->IsPrinting()) {
      pixels = __min( pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES) );

      if (rectin.Width() > rectin.Height()) {
         rectout.SetRect( CPoint(0,0), CPoint(pixels * rectin.Height() / rectin.Width(), pixels) );
      }
      else {
         rectout.SetRect( CPoint(0,0), CPoint(pixels, pixels * rectin.Width() / rectin.Height()) );
      }
      // Not sure what isotropic does for us
      pDC->SetMapMode( MM_ISOTROPIC );
   }
   else {
      // When 'copy'ing, the rectin and rectout are set one to one:
      rectout = rectin;
   }

   pDC->SetWindowExt( rectin.Size() );
   pDC->SetViewportExt( rectout.Size() );

   if (!m_viewport_set) {
      AfxMessageBox(_T("Can't print / copy as the Depthmap window is not initialised"));
      pDC->AbortDoc();
      return;
   }

   CGdiObject *oldpen   = pDC->SelectStockObject( NULL_PEN );
   CBrush brush( GetApp()->m_background );
   CGdiObject *oldbrush = pDC->SelectObject( &brush );

   pDC->Rectangle( rectin );

   pDC->SelectObject( oldbrush );
   pDC->SelectObject( oldpen );

   int state = pDoc->m_meta_graph->getState();

   if (state & MetaGraph::POINTMAPS && (!pDoc->m_meta_graph->getDisplayedPointMap().isProcessed() || pDoc->m_meta_graph->getViewClass() & (MetaGraph::VIEWVGA | MetaGraph::VIEWBACKVGA))) {
      pDoc->m_meta_graph->getDisplayedPointMap().setScreenPixel( m_unit ); // only used by points (at the moment!)
      pDoc->m_meta_graph->getDisplayedPointMap().makeViewportPoints( LogicalViewport(rectin, pDoc) );
   }
   if (state & MetaGraph::SHAPEGRAPHS && (pDoc->m_meta_graph->getViewClass() & (MetaGraph::VIEWBACKAXIAL | MetaGraph::VIEWAXIAL))) {
      pDoc->m_meta_graph->getDisplayedShapeGraph().makeViewportShapes( LogicalViewport(rectin, pDoc) );
   }
   if (state & MetaGraph::DATAMAPS && (pDoc->m_meta_graph->getViewClass() & (MetaGraph::VIEWBACKDATA | MetaGraph::VIEWDATA))) {
      pDoc->m_meta_graph->getDisplayedDataMap().makeViewportShapes( LogicalViewport(rectin, pDoc) );
   }
   if (state & MetaGraph::LINEDATA) {
      pDoc->m_meta_graph->SuperSpacePixel::makeViewportShapes( LogicalViewport(rectin, pDoc) );
   }

   Output( pDC, pDoc, false );
}

static COLORREF colorMerge(COLORREF color, COLORREF mergecolor)
{
   return (color & 0x006f6f6f) | (mergecolor & 0x00a0a0a0);
}

int CDepthmapView::GetSpacer(CGraphDoc *pDoc)
{
   int spacer = 1;
   int viewclass = pDoc->m_meta_graph->getViewClass();
   if (viewclass & (MetaGraph::VIEWVGA | MetaGraph::VIEWBACKVGA)) {
      spacer = int(ceil(5.0 * pDoc->m_meta_graph->getDisplayedPointMap().getSpacing() / (m_unit * 10.0) ));
   }
   else if (viewclass & MetaGraph::VIEWAXIAL) {
      spacer = int(ceil(pDoc->m_meta_graph->getDisplayedShapeGraph().getSpacing() / (m_unit * 10.0) ));
   }
   else if (viewclass & MetaGraph::VIEWDATA) {
      spacer = int(ceil(pDoc->m_meta_graph->getDisplayedDataMap().getSpacing() / (m_unit * 10.0) ));
   }
   return spacer;
}

bool CDepthmapView::Output(CDC *pDC, CGraphDoc *pDoc, bool screendraw) 
{
   DWORD ticks = ::GetTickCount();

   int state = pDoc->m_meta_graph->getState();

   bool b_continue = false;

   int spacer = GetSpacer(pDoc);

   if (!pDoc->m_communicator) {
      int viewclass = pDoc->m_meta_graph->getViewClass();
      if (viewclass & MetaGraph::VIEWVGA) {
         if (!b_continue && viewclass & MetaGraph::VIEWBACKAXIAL) {
            b_continue = DrawShapes(pDC, pDoc->m_meta_graph->getDisplayedShapeGraph(), true, spacer, ticks, screendraw);
         }
         if (!b_continue && viewclass & MetaGraph::VIEWBACKDATA) {
            b_continue = DrawShapes(pDC, pDoc->m_meta_graph->getDisplayedDataMap(), true, spacer, ticks, screendraw);
         }
         if (!b_continue) { 
            b_continue = DrawPoints(pDC, pDoc, spacer, ticks, screendraw);
         }
      }
      else if (!b_continue && pDoc->m_meta_graph->getViewClass() & MetaGraph::VIEWAXIAL) {
         if (viewclass & MetaGraph::VIEWBACKVGA) {
            b_continue = DrawPoints(pDC, pDoc, spacer, ticks, screendraw);
         }
         if (!b_continue && viewclass & MetaGraph::VIEWBACKDATA) {
            b_continue = DrawShapes(pDC, pDoc->m_meta_graph->getDisplayedDataMap(), true, spacer, ticks, screendraw);
         }
         if (!b_continue) {
            b_continue = DrawShapes(pDC, pDoc->m_meta_graph->getDisplayedShapeGraph(), false, spacer, ticks, screendraw);
         }
      }
      else if (!b_continue && pDoc->m_meta_graph->getViewClass() & MetaGraph::VIEWDATA) {
         if (viewclass & MetaGraph::VIEWBACKAXIAL) {
            b_continue = DrawShapes(pDC, pDoc->m_meta_graph->getDisplayedShapeGraph(), true, spacer, ticks, screendraw);
         }
         if (!b_continue && viewclass & MetaGraph::VIEWBACKVGA) {
            b_continue = DrawPoints(pDC, pDoc, spacer, ticks, screendraw);
         }
         if (!b_continue) {
            b_continue = DrawShapes(pDC, pDoc->m_meta_graph->getDisplayedDataMap(), false, spacer, ticks, screendraw);
         }
      }
   }

   if (!b_continue && state & MetaGraph::LINEDATA) {

      CPen pen;
      CGdiObject *oldpen = pDC->GetCurrentPen();
      bool nextlayer = false, first = true;

      // not written the colour setting yet:
      pen.CreatePen( PS_SOLID, spacer/20+1, GetApp()->m_foreground );
      pDC->SelectObject(&pen);

      while ( (b_continue = pDoc->m_meta_graph->SuperSpacePixel::findNextShape(nextlayer)) ) {

         /*
         Line l = pDoc->m_meta_graph->SuperSpacePixel::getNextLine();
         if (nextlayer || first) {
            PafColor color;
            color = pDoc->m_meta_graph->SuperSpacePixel::getLineColor();
            pDC->SelectObject(oldpen);
            pen.DeleteObject();
            if (color.alphab() != 0) {
               pen.CreatePen( PS_SOLID, spacer/20+1, color );
            }
            else {
               pen.CreatePen( PS_SOLID, spacer/20+1, GetApp()->m_foreground );
            }
            pDC->SelectObject(&pen);
            first = false;
            nextlayer = false;
         }*/
        
         const SalaShape& shape = pDoc->m_meta_graph->SuperSpacePixel::getNextShape();

         if (shape.isPoint()) {
         }
         else if (shape.isLine()) {
            Line line = shape.getLine();
            CPoint p1 = PhysicalUnits(line.start());
            CPoint p2 = PhysicalUnits(line.end());
            if (p1 != p2) {
               pDC->MoveTo( p1 );
               pDC->LineTo( p2 );
            }
         }
         else {
            pDC->MoveTo(PhysicalUnits(shape[0]));
            for (size_t i = 1; i < shape.size(); i++) {
               pDC->LineTo(PhysicalUnits(shape[i]));
            }
            if (shape.isClosed()) {
               pDC->LineTo(PhysicalUnits(shape[0]));
            }
         }
      }

      pDC->SelectObject(oldpen);
      pen.DeleteObject();
   }

   if (!b_continue && spacer > 1 && pDoc->m_meta_graph->viewingProcessedShapes() && pDoc->m_meta_graph->m_showtext) {
      // set up text formatting
      COLORREF oldforeground = pDC->SetTextColor(GetApp()->m_foreground);
      COLORREF oldbackground = pDC->SetBkColor(GetApp()->m_background);
      CFont font;
      font.CreatePointFont(spacer * 60,_T("Arial"),pDC);
      CGdiObject *oldfont = pDC->SelectObject( &font );
      pDC->SetTextAlign(TA_CENTER);
      // display the layer attribute data:
      /*
      // THIS NEEDS SORTING OUT
      ShapeMap& map = pDoc->m_meta_graph->getDisplayedDataMap();
      for (int i = 0; i < map.getObjectCount(); i++) {
         if (map.getDisplayedAttributeValue(i) > 0) {
            pstring text = map.getDisplayedAttributeText(i);
            CPoint p = PhysicalUnits(map.getCentroid(i));
            CSize sz = pDC->GetTextExtent(text.c_str());
            pDC->TextOut(p.x, p.y - (sz.cy/2), text.c_str());
         }
      }
      */
      // reset text formatting
      pDC->SetTextColor( oldforeground );
      pDC->SetBkColor( oldbackground );
      pDC->SelectObject( oldfont );
   }

   if (!b_continue && m_showlinks) {
      CBrush brush( GetApp()->m_foreground );
      CGdiObject *oldbrush = pDC->SelectObject( &brush );
      if (pDoc->m_meta_graph->getViewClass() & MetaGraph::VIEWVGA && pDoc->m_meta_graph->getDisplayedPointMap().isProcessed()) {
         PointMap& map = pDoc->m_meta_graph->getDisplayedPointMap();
         // merge lines
         CPen greenpen( PS_SOLID, spacer/10+1, 0x0000ff00 );
         pDC->SelectObject(&greenpen);
         while ( (b_continue = map.findNextMergeLine()) ) {
            Line line = map.getNextMergeLine();
            DrawLink(pDC, spacer, line);
         }
         pDC->SelectStockObject( NULL_PEN );
      }
      else if ((state & MetaGraph::SHAPEGRAPHS) && (pDoc->m_meta_graph->getViewClass() & MetaGraph::VIEWAXIAL)) {
         // link lines
         CPen greenpen( PS_SOLID, spacer/10+1, 0x0000ff00 );
         pDC->SelectObject(&greenpen);
         while ( (b_continue = pDoc->m_meta_graph->getDisplayedShapeGraph().findNextLinkLine()) ) {
            Line line = pDoc->m_meta_graph->getDisplayedShapeGraph().getNextLinkLine();
            DrawLink(pDC, spacer, line);
         }
         CPen redpen( PS_SOLID, spacer/10+1, 0x000000ff );
         pDC->SelectObject(&redpen);
         CBrush bgbrush( GetApp()->m_background );
         pDC->SelectObject( &bgbrush );

         while ( (b_continue = pDoc->m_meta_graph->getDisplayedShapeGraph().findNextUnlinkPoint()) ) {
            CPoint p = PhysicalUnits( pDoc->m_meta_graph->getDisplayedShapeGraph().getNextUnlinkPoint() );
            p -= CPoint(2+spacer/2,2+spacer/2);
            CRect rect(p, CSize(spacer+4,spacer+4));
            pDC->Ellipse( rect );
         }
         pDC->SelectStockObject( NULL_PEN );
      }
      pDC->SelectObject( oldbrush );
   }

   return b_continue;
}

bool CDepthmapView::DrawPoints(CDC *pDC, CGraphDoc *pDoc, int spacer, DWORD ticks, bool screendraw) 
{
   bool b_continue = false;

   PointMap& map = pDoc->m_meta_graph->getDisplayedPointMap();

   bool muted = (map.isProcessed() && pDoc->m_meta_graph->getViewClass() & MetaGraph::VIEWBACKVGA);
   if (m_showlinks) {
      if (!muted) {
         muted = true;
      }
      else {
         return b_continue; // too confusing!
      }
   }

   CPalette *oldpalette = pDC->SelectPalette(g_color_palette,0);
   pDC->RealizePalette();

   CGdiObject *oldpen;
   oldpen = pDC->SelectStockObject( NULL_PEN );

   if (pDoc->m_meta_graph->getViewClass() & (MetaGraph::VIEWBACKAXIAL | MetaGraph::VIEWBACKDATA)) {
      spacer /= 2;   // allow see through to axial lines
   }

   bool monochrome = (map.isProcessed() && map.getDisplayParams().colorscale == DisplayParams::MONOCHROME);
   CGdiObject *oldbrush;
   CBrush *standardbrush = NULL;   
   //if (monochrome) {
    
      standardbrush = new CBrush( GetApp()->m_foreground );
      oldbrush = pDC->SelectObject( standardbrush );
   //}

   while ( (b_continue = map.findNextPoint()) ) {

      Point2f logical = map.getNextPointLocation();

      PafColor color;
      color = map.getPointColor();

      if (color.alphab() != 0) { // alpha == 0 is transparent
         if (monochrome && !map.getPointSelected()) {
            CPoint p = PhysicalUnits(logical);
            int subspacer = (3 * color.blueb() * spacer) / 255;
            if (subspacer >= 1) {
               pDC->Ellipse( p.x - subspacer, p.y + subspacer, p.x + subspacer, p.y - subspacer);
            }
            else {
               pDC->SetPixel( p, GetApp()->m_foreground );
            }
         }
         else {
            COLORREF rgb = RGB(color.redb(),color.greenb(),color.blueb());
            if (muted && !map.getPointSelected()) { // keeps selected points bright yellow
               rgb = colorMerge( rgb, GetApp()->m_background );
            }
            CPoint p = PhysicalUnits(logical);
            if (spacer > 1) {
               // Temporary trial
               /*CBrush brush( rgb );
                 CBrush pen( rgb );
                 pDC->SelectObject( &brush );
                 pDC->SelectObject( &pen );
                 pDC->Rectangle(p.x - spacer - 1, p.y + spacer + 1, p.x + spacer + 1, p.y - spacer - 1);*/
               // Standard code
               pDC->FillSolidRect( CRect(p.x - spacer, p.y + spacer, p.x + spacer, p.y - spacer), rgb);
            }
            else {
               pDC->SetPixel( p, rgb );
            }
         }
      }
      if (screendraw && ::GetTickCount() - 200 > ticks) {
         break;
      }
   }

   //if (standardbrush) {
      pDC->SelectObject( oldbrush );
      delete standardbrush;
      standardbrush = NULL;
   //}

   pDC->SelectPalette(oldpalette,0);
   pDC->RealizePalette();

   if (!b_continue && pDoc->m_meta_graph->m_showgrid) {
      CPen pen( PS_SOLID, 1, colorMerge( GetApp()->m_foreground, GetApp()->m_background ) );
      pDC->SelectObject(&pen);
      if (pDoc->m_meta_graph->getViewClass() & MetaGraph::VIEWVGA) {
         // show grid as though points are filling the spaces
         while ( (b_continue = map.findNextRow()) ) {
            Line logical = map.getNextRow();
            pDC->MoveTo( PhysicalUnits( logical.start() ) );
            pDC->LineTo( PhysicalUnits( logical.end() ) );
         }
         while ( (b_continue = map.findNextCol()) ) {
            Line logical = map.getNextCol();
            pDC->MoveTo( PhysicalUnits( logical.start() ) );
            pDC->LineTo( PhysicalUnits( logical.end() ) );
         }
      }
      else {
         // show actual grid
         while ( (b_continue = map.findNextPointRow()) ) {
            Line logical = map.getNextPointRow();
            pDC->MoveTo( PhysicalUnits( logical.start() ) );
            pDC->LineTo( PhysicalUnits( logical.end() ) );
         }
         while ( (b_continue = map.findNextPointCol()) ) {
            Line logical = map.getNextPointCol();
            pDC->MoveTo( PhysicalUnits( logical.start() ) );
            pDC->LineTo( PhysicalUnits( logical.end() ) );
         }
      }
      pDC->SelectStockObject( NULL_PEN );
   }

   pDC->SelectObject(oldpen);

   return b_continue;
}

bool CDepthmapView::DrawShapes(CDC *pDC, ShapeMap& map, bool muted, int spacer, DWORD ticks, bool screendraw) 
{
   bool b_continue = false;

   if (m_showlinks) {
      if (!muted) {
         muted = true;
      }
      else {
         return b_continue; // too confusing!
      }
   }

   bool monochrome = (map.getDisplayParams().colorscale == DisplayParams::MONOCHROME);

   if (screendraw && !muted) {
      // i.e., at the front... implies can select... allow this draw to overwrite any existing point handle locations:
      m_point_handles.clear();
   }

   CGdiObject *oldbrush = pDC->GetCurrentBrush();
   CGdiObject *oldpen = pDC->GetCurrentPen();
   CPen pen;
   pen.CreatePen(PS_SOLID, spacer/20+1, GetApp()->m_foreground );
   bool dummy;
   int count = 0;
   while ( (b_continue = map.findNextShape(dummy)) ) {

      count++;

      PafColor color;
      // n.b., MONOCHROME settings only for line thickness...
      color = map.getShapeColor();
      bool selected = map.getShapeSelected();

      // n.b., getNextShape clears polygon ready for next, so get polygon color and selected attribute before this:
      const SalaShape& poly = map.getNextShape();
      CPoint *points = NULL;
      int drawable = 0;
      if (!poly.isPoint() && !poly.isLine()) {
         points = new CPoint [poly.size()];
         for (size_t i = 0; i < poly.size(); i++) {
            points[drawable] = PhysicalUnits(poly[i]);
            if (i == 0 || points[drawable] != points[drawable-1]) {
               drawable++;
            }
         }
      }
      //
      CBrush brush;
      CPen pen2;
      COLORREF rgb;
      int tempspacer = selected ? spacer * 3: spacer;
      if (!monochrome || selected) {
         rgb = RGB(color.redb(),color.greenb(),color.blueb());
         if (muted && !selected) {
            rgb = colorMerge( rgb, GetApp()->m_background );
         }
         if (poly.isClosed() || poly.isPoint()) {
            brush.CreateSolidBrush( rgb );
         }
         else {
            pen2.CreatePen( PS_SOLID, tempspacer/10+1, rgb );
         }
      }
      else {
         int thickness = (spacer * color.blueb()) / 255;
         if (thickness < 1) {
            // note, monochrome excludes lines below 'thin' threshold
            continue;
         }
         if (poly.isClosed()) {
            rgb = GetApp()->m_background;
            brush.CreateSolidBrush( rgb );
         }
         rgb = GetApp()->m_foreground;
         // note, monochrome uses fixed widths
         pen2.CreatePen( PS_SOLID, thickness, rgb );
      }
      if (poly.isClosed()) {
         if (drawable > 1) {
            if (!map.m_show_lines) {
               pDC->SelectStockObject(NULL_PEN);
            }
            else if (monochrome && !selected) {
               pDC->SelectObject(&pen2);
            }
            else {
               pDC->SelectObject(&pen);
            }
            if (!map.m_show_fill) {
               pDC->SelectStockObject(NULL_BRUSH);
            }
            else {
               pDC->SelectObject(&brush);
            }
            pDC->Polygon( points, drawable );
            //
            if (map.m_show_centroids) {
               Point2f p = poly.getCentroid();
               pDC->SetPixel( PhysicalUnits(p), RGB(255,255,0) );
            }
         }
         else {
            pDC->SetPixel(points[0],rgb);
         }
      }
      else {
         if (poly.isPoint()) {
            CPoint point = PhysicalUnits(poly.getPoint());
            if (tempspacer < 2) {
               pDC->SetPixel(point,rgb);
            }
            else {
               CRect rect(point.x-tempspacer/2, point.y-tempspacer/2, point.x+tempspacer/2, point.y+tempspacer/2);
               if (tempspacer < 4) {
                  pDC->FillSolidRect(rect,rgb);
               }
               else {
                  pDC->SelectObject(&pen);
                  pDC->SelectObject(&brush);
                  pDC->Ellipse( rect );
               }
            }
         }
         else if (poly.isLine()) {
            pDC->SelectObject(&pen);
            pDC->SelectObject(&pen2);
            Line l = poly.getLine();
            CPoint start = PhysicalUnits(l.start());
            CPoint end = PhysicalUnits(l.end());
            if (start != end) {
               pDC->MoveTo( start );
               pDC->LineTo( end );
            }
            if (selected && screendraw && !muted && map.getSelCount() == 1 && map.isEditable()) {
               m_point_handles.push_back(l.start());
               m_point_handles.push_back(l.end());
            }
         }
         else {
            if (drawable > 1) {
               pDC->SelectObject(&pen2);
               pDC->Polyline( points, drawable );
            }
            else {
               pDC->SetPixel(points[0],rgb);
            }
         }
      }
      if (points) {
         delete [] points;
      }

      if (screendraw && ::GetTickCount() - 200 > ticks) {
         break;
      }
   }

   if (screendraw && !muted) {
      // i.e., at the front... implies can select... allow this draw to overwrite to draw point handle locations:
      for (size_t i = 0; i < m_point_handles.size(); i++) {
         DrawPointHandle(pDC,PhysicalUnits(m_point_handles[i]));
      }
   }

   pDC->SelectObject(oldbrush);
   pDC->SelectObject(oldpen);

   return b_continue;
}

void CDepthmapView::DrawLink(CDC *pDC, int spacer, const Line& logical) 
{
   spacer += 4;
   CPoint p1 = PhysicalUnits( logical.start() );
   CPoint p2 = PhysicalUnits( logical.end() );
   pDC->MoveTo( p1 );
   pDC->LineTo( p2 );
   p1 -= CPoint(spacer/2,spacer/2);
   p2 -= CPoint(spacer/2,spacer/2);
   CRect rect1(p1, CSize(spacer,spacer));
   CRect rect2(p2, CSize(spacer,spacer));
   pDC->Ellipse( rect1 );
   pDC->Ellipse( rect2 );
}

void CDepthmapView::DrawPointHandle(CDC *pDC, CPoint pt) 
{
   CRect rect(pt.x-7, pt.y-7, pt.x+7, pt.y+7);
   pDC->FillSolidRect(rect,GetApp()->m_foreground);
   rect.DeflateRect(1,1,1,1);
   pDC->FillSolidRect(rect,GetApp()->m_background);
   rect.DeflateRect(1,1,1,1);
   pDC->FillSolidRect(rect,m_selected_color);
}

void CDepthmapView::OutputEPS( ofstream& stream, CGraphDoc *pDoc )
{
   // This output EPS is a copy of the standard output... obviously, if you change
   // standard output, remember to change this one too!
   
   // now the two are a little out of synch

   if (!m_viewport_set) {
      AfxMessageBox(_T("Can't save screen as the Depthmap window is not initialised"));
      return;
   }

   CRect clrect, rect;
   GetClientRect( clrect );

   stream << "%!PS-Adobe-3.0 EPSF-3.0\n"
          << "%%BoundingBox: 0 0 " << clrect.Width() << " " << clrect.Height() << "\n"
          << "%%Creator: Depthmap " << DEPTHMAP_VERSION << "." << DEPTHMAP_MINOR_VERSION << endl;

   // temporarily inflate resolution for EPS draw
   rect = CRect(clrect.left * 10, clrect.top * 10, clrect.right * 10, clrect.bottom * 10);
   CPoint oldcentre = m_physical_centre;
   double oldunit = m_unit;
   m_physical_centre = CSize(rect.Width() / 2, rect.Height() / 2);
   m_unit /= 10;

   int bg = GetApp()->m_background;
   float bgr = float(GetRValue(bg))/255.0f;
   float bgg = float(GetGValue(bg))/255.0f;
   float bgb = float(GetBValue(bg))/255.0f;

   int fg = GetApp()->m_foreground;
   float fgr = float(GetRValue(fg))/255.0f;
   float fgg = float(GetGValue(fg))/255.0f;
   float fgb = float(GetBValue(fg))/255.0f;
   
   stream << "/M {moveto} def" << endl;
   stream << "/L {lineto} def" << endl;
   stream << "/R {rlineto} def" << endl;
   stream << "/C {setrgbcolor} def" << endl;
   stream << "/W {setlinewidth} def" << endl;

   stream << "newpath\n"
          << 0 << " " << 0 << " M\n"
          << clrect.Width() << " " << 0 << " R\n"
          << 0 << " " << clrect.Height() << " R\n"
          << -clrect.Width() << " " << 0 << " R\n"
          << "closepath\n"
          << bgr << " " << bgg << " " << bgb << " C\n"
          << "fill" << endl;

   int state = pDoc->m_meta_graph->getState();

   Region logicalviewport = LogicalViewport(rect, pDoc);

   if (state & MetaGraph::POINTMAPS) {
      pDoc->m_meta_graph->getDisplayedPointMap().setScreenPixel( m_unit ); // only used by points (at the moment!)
      pDoc->m_meta_graph->getDisplayedPointMap().makeViewportPoints( logicalviewport );
   }
   if (state & MetaGraph::SHAPEGRAPHS) {
      pDoc->m_meta_graph->getDisplayedShapeGraph().makeViewportShapes( logicalviewport );
   }
   if (state & MetaGraph::DATAMAPS) {
      pDoc->m_meta_graph->getDisplayedDataMap().makeViewportShapes( logicalviewport );
   }
   if (state & MetaGraph::LINEDATA) {
      pDoc->m_meta_graph->SuperSpacePixel::makeViewportShapes( logicalviewport );
   }

   double spacer = GetSpacer(pDoc) / 10.0;
   if (spacer < 0.1) {
      spacer = 0.1;
   }

   if (state & MetaGraph::POINTMAPS && pDoc->m_meta_graph->getViewClass() & MetaGraph::VIEWVGA) {

      // Define EPS box using spacer dimensions:
      stream << "/bx\n"
             << " { newpath M\n" 
             << "   " <<  2 * spacer << " " << 0 << " R\n"
             << "   " <<  0 << " " << 2 * spacer << " R\n"
             << "   " <<  2 * -spacer << " " << 0 << " R\n" 
             << "   closepath } def" << endl;
      stream << "/fbx\n"
             << " { C fill } def" << endl;

      PointMap& map = pDoc->m_meta_graph->getDisplayedPointMap();

      while ( map.findNextPoint() ) {

         Point2f logical = map.getNextPointLocation();

         PafColor color = map.getPointColor();

         if (color.alphab() != 0) { // alpha == 0 is transparent

            CPoint p = PhysicalUnits(logical);

            // Now do EPS box... remember the coordinate system is the right way up!
            stream << p.x / 10.0 - spacer << " " << (rect.Height() - p.y) / 10.0 - spacer << " bx" << endl;
            stream << color.redf() << " " << color.greenf() << " " << color.bluef() << " fbx" << endl;
         }
      }
   }

   if (state & MetaGraph::SHAPEGRAPHS && pDoc->m_meta_graph->getViewClass() & MetaGraph::VIEWAXIAL) {

      ShapeMap& map = pDoc->m_meta_graph->getDisplayedShapeGraph();

      OutputEPSMap(stream, map, logicalviewport, rect, spacer);
   }

   if (state & MetaGraph::DATAMAPS && pDoc->m_meta_graph->getViewClass() & MetaGraph::VIEWDATA) {

      ShapeMap& map = pDoc->m_meta_graph->getDisplayedDataMap();

      OutputEPSMap(stream, map, logicalviewport, rect, spacer);
   }

   if (state & MetaGraph::LINEDATA) {

      stream << "newpath" << endl;
      bool nextlayer = false;
      bool first = true;
      int style = 0;

      while ( pDoc->m_meta_graph->SuperSpacePixel::findNextShape(nextlayer) ) {

         const SalaShape& shape = pDoc->m_meta_graph->SuperSpacePixel::getNextShape();

         Line l;
         if (shape.isPoint()) {
         }
         else if (shape.isLine()) {
            Line line = shape.getLine();
            OutputEPSLine(stream, line, spacer, logicalviewport, rect);
         }
         else {
            OutputEPSPoly(stream, shape, spacer, logicalviewport, rect);
         }
      }

      int fg = GetApp()->m_foreground;
      float fgr = float(GetRValue(fg))/255.0f;
      float fgg = float(GetGValue(fg))/255.0f;
      float fgb = float(GetBValue(fg))/255.0f;
      if (style == 1) {
         stream << "[" << spacer/10+1 << "]";
      }
      else {
         stream << "[]";
      }
      stream << " 0 setdash" << endl;
      stream << fgr << " " << fgg << " " << fgb << " C" << endl;
      stream << spacer/10+1 << " W" << endl;
      stream << "stroke" << endl;
   }

   // loaded paths
   if (pDoc->m_evolved_paths.size()) {

      stream << "newpath" << endl;

      for (size_t i = 0; i < pDoc->m_evolved_paths.size(); i++) {

         const prefvec<Point2f>& path = pDoc->m_evolved_paths[i];

         if (path.size() > 1) {

            CPoint last = PhysicalUnits(path[0]);
            stream << (last.x - spacer/40) / 10.0 << " " << (rect.Height() - last.y + spacer/40) / 10.0 << " M\n" << endl;

            for (size_t j = 1; j < path.size(); j++) {
               CPoint next = PhysicalUnits(path[j]);
               stream << (next.x - last.x - spacer/40) / 10.0 << " " << (last.y - next.y - spacer/40) / 10.0 << " L" << endl;
               last = next;
            }

            stream << fgr << " " << fgg << " " << fgb << " C" << endl;
            stream << spacer/20+1 << " W" << endl;
            stream << "stroke" << endl;
         }
      }
   }
/*
   if (pDoc->m_agent_manager && pDoc->m_agent_manager->isPaused())
   {
      if (pDoc->m_agent_manager->isEcomorphic()) {
         // draw the ecomorphic artworks
         Ecomorph& eco = pDoc->m_agent_manager->getEcomorph();
         for (int i = 0; i < eco.arts().size(); i++)
         {
            PixelRef pos = eco.arts()[i].getPos();
            Point2f logical = pDoc->m_meta_graph->getDisplayedPointMap().depixelate(pos);
            // this is in units of logical is unit based so this actually works okay:
            CPoint p = PhysicalUnits(Point2f(logical.x, logical.y));
            CPoint bottomleft = PhysicalUnits(Point2f(logical.x - 0.5, logical.y - 0.5));
            // Now do EPS box... remember the coordinate system is the right way up!
            stream << (p.x - spacer) / 10.0 << " " << (rect.Height() - p.y - spacer) / 10.0 << " bx" << endl;
            stream << bgr << " " << bgg << " " << bgb << " fbx" << endl;
            // And cover with line:
            stream << (bottomleft.x - spacer/20) / 10.0 << " " << (rect.Height() - bottomleft.y + spacer/20) / 10.0 << " M\n"
                   << (spacer * 2 - spacer/20) / 10.0 << " " << 0 << " L\n"
                   << 0 << " " << (spacer * 2 - spacer/20) / 10.0 << " L\n"
                   << (-spacer * 2 + spacer/20) / 10.0 << " " << 0 << " L\n"
                   << 0 << " " << (-spacer * 2 + spacer/20) / 10.0 << " L" << endl;
            stream << fgr << " " << fgg << " " << fgb << " setrgbcolor" << endl;
            stream << spacer/10+1 << " setlinewidth" << endl;
            stream << "stroke" << endl;
         }
      }
   }
*/

   // add the scale to the bottom lefthand corner
   double logicalwidth = m_unit * rect.Width();
   if (logicalwidth > 10) {
      int workingwidth = floor(log10(logicalwidth/2)*2.0);
      int barwidth = (int) pow(10.0,(double)(workingwidth/2)) * ((workingwidth%2 == 0) ? 1 : 5);
      double physicalbar = double(barwidth) / m_unit;
      stream << "newpath" << endl;
      stream << "0 0 M 0 18 R" << endl;
      stream << physicalbar / 10.0 << " 0 R 0 -18 R closepath" << endl;
      stream << bgr << " " << bgg << " " << bgb << " C" << endl;
      stream << "fill newpath" << endl;
      stream << fgr << " " << fgg << " " << fgb << " C" << endl;
      stream << "0 12 M" << endl;
      stream << physicalbar / 10.0 << " 0 R" << endl;
      stream << "3 W stroke" << endl;
      stream << "0 6 M 0 7.5 R" << endl;
      stream << physicalbar / 10.0 << " 6 M 0 7.5 R" << endl;
      stream << "1.5 W stroke" << endl;
      stream << "/Arial findfont 12 scalefont setfont" << endl;
      // assume metres!
      if (barwidth > 1000) {
         stream << "(" << (barwidth / 1000) << "km) stringwidth pop 2 div" << endl;
      }
      else {
         stream << "(" << barwidth << "m) stringwidth pop 2 div" << endl;
      }
      stream << physicalbar / 20.0 << " exch sub" << endl;
      stream << "0 M" << endl;
      stream << "(" << barwidth << "m) show" << endl;
   }

   stream << "showpage" << endl;

   // undo temporary unit setting
   m_unit = oldunit;
   m_physical_centre = oldcentre;
}

void CDepthmapView::OutputEPSMap(ofstream& stream, ShapeMap& map, Region& logicalviewport, CRect& rect, float spacer)
{
   bool monochrome = (map.getDisplayParams().colorscale == DisplayParams::MONOCHROME);
   double thickness = 1.0, oldthickness = 1.0;
   bool closed, oldclosed = false;
   PafColor color, oldcolor;

   int fg = GetApp()->m_foreground;
   float fgr = float(GetRValue(fg))/255.0f;
   float fgg = float(GetGValue(fg))/255.0f;
   float fgb = float(GetBValue(fg))/255.0f;

   stream << "newpath" << endl;
   stream << fgr << " " << fgg << " " << fgb << " C" << endl;
   bool dummy;
   while ( map.findNextShape(dummy) ) {

      // note: getNextLine clears current line, so getLineColor before line
      color = map.getShapeColor();
      const SalaShape& shape = map.getNextShape();
      closed = shape.isClosed();

      if (monochrome) {
         thickness = 3.0 * (color.blueb() * spacer) / 255.0;
         // note: anything below 'thin' threshold in monochrome note drawn
         if (thickness < 0.25) {
            continue;
         }
         if (thickness != oldthickness || closed != oldclosed) {
            stream << oldthickness << " W" << endl;
            stream << (oldclosed ? "fill" : "stroke") << endl;
            oldthickness = thickness;
            oldclosed = closed;
         }
      }
      else if (color != oldcolor || closed != oldclosed) {
         stream << oldcolor.redf() << " " << oldcolor.greenf() << " " << oldcolor.bluef() << " C" << endl;
         stream << (oldclosed ? "fill" : "stroke") << endl;
         oldcolor = color;
         oldclosed = closed;
      }

      Line l;
      if (shape.isPoint()) {
      }
      else if (shape.isLine()) {
         Line line = shape.getLine();
         OutputEPSLine(stream, line, spacer, logicalviewport, rect);
      }
      else {
         OutputEPSPoly(stream, shape, spacer, logicalviewport, rect);
      }
   }
   stream << thickness << " W" << endl;
   if (!monochrome) {
      stream << color.redf() << " " << color.greenf() << " " << color.bluef() << " C" << endl;
   }
   stream << (closed ? "fill" : "stroke") << endl;
}

void CDepthmapView::OutputEPSLine(ofstream& stream, Line& line, int spacer, Region& logicalviewport, CRect& rect)
{
   bool drewit = false;
   if (line.crop(logicalviewport)) {
      CPoint start = PhysicalUnits(line.start());
      CPoint end = PhysicalUnits(line.end());
      // 10 units corresponds to 1 pixel on the screen
      if (dist(start,end) > 5.0) {
         stream << (start.x / 10.0) << " " << (rect.Height() - start.y) / 10.0 << " M ";
         stream << (end.x / 10.0) << " " << (rect.Height() - end.y) / 10.0 << " L" << endl;
      }
   }
}

void CDepthmapView::OutputEPSPoly(ofstream& stream, const SalaShape& shape, int spacer, Region& logicalviewport, CRect& rect)
{
   bool starter = true;
   Point2f lastpoint = shape[0];
   int count = shape.isClosed() ? shape.size() + 1 : shape.size();
   int size = shape.size();
   for (size_t i = 1; i < count; i++) {
      Line line(lastpoint,shape[i%size]);
      if (line.crop(logicalviewport)) {
         // note: use t_start and t_end so that this line moves in the correct direction
         CPoint start = PhysicalUnits(line.t_start());
         CPoint end = PhysicalUnits(line.t_end());
         // 5.0 is about 1/2 pixel width
         if (dist(start,end) > 5.0) {
            if (starter) {
               stream << start.x / 10.0 << " " << (rect.Height() - start.y) / 10.0 << " M ";
            }
            stream << end.x / 10.0 << " " << (rect.Height() - end.y) / 10.0 << " L" << endl;
            // note: you must use t_end (true end) so that it takes the end point from the shape[i] end:
            lastpoint = line.t_end();
            starter = false;
         }
      }
      else {
         lastpoint = shape[i];
         starter = true;
      }
   }
}

static string SVGColor(COLORREF color)
{
   stringstream text;
   int r = GetRValue(color);
   int g = GetGValue(color);
   int b = GetBValue(color);
   text << setfill('0') << "#" << setw(2) << std::hex << r << setw(2) << std::hex << g<< setw(2) << std::hex << b;
   return text.str();
}

static string SVGColor(PafColor color)
{
   stringstream text;
   int r = color.redb();
   int g = color.greenb();
   int b = color.blueb();
   text << setfill('0') << "#" << setw(2) << std::hex << r << setw(2) << std::hex << g<< setw(2) << std::hex << b;
   return text.str();
}

static CPoint SVGPhysicalUnits(const Point2f& p, const Region& r, int h)
{
   // converts to a 4800 unit wide region
   return CPoint(
      int(4800.0 * ((p.x-r.bottom_left.x)/r.width())),h - int(4800.0 * ((p.y-r.bottom_left.y)/r.width()))
      );
}

void CDepthmapView::OutputSVG( ofstream& stream, CGraphDoc *pDoc )
{
   // This output SVG is a copy of the standard output... obviously, if you change
   // standard output, remember to change this one too!
   
   // also out of synch with EPS!

   // now the two are a little out of synch

   if (!m_viewport_set) {
      AfxMessageBox(_T("Can't save screen as the Depthmap window is not initialised"));
      return;
   }

   CRect rect;
   GetClientRect( rect );

   // we'll make this 24cm wide whatever, and base the height on it:
   int h = (4800 * rect.Height()) / rect.Width();

   stream << "<?xml version=\"1.0\" standalone=\"no\"?>" << endl;
   stream << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"" << endl;
   stream << "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">" << endl;
   stream << "<svg width=\"24cm\" height=\"" << (h/200) << "cm\" viewBox=\"0 0 4800 " << h << "\"" << endl;
   stream << "xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" version=\"1.1\">" << endl;
   stream << "<desc>Depthmap " << DEPTHMAP_VERSION << "." << DEPTHMAP_MINOR_VERSION << "</desc>" << endl;

   // note, SVG draw completely overrides standard draw physical units to achieve hi-res output
   // (EPS should probably follow this model too)
   Region logicalviewport = LogicalViewport(rect, pDoc);

   stream << "<rect x=\"0\" y=\"0\" width=\"4800\" height=\"" << h << "\" "
          << "fill=\"" << SVGColor(GetApp()->m_background) << "\" stroke=\"none\" stroke-width=\"0\" />" << endl;

   int state = pDoc->m_meta_graph->getState();

   if (state & MetaGraph::POINTMAPS) {
      pDoc->m_meta_graph->getDisplayedPointMap().setScreenPixel( m_unit ); // only used by points (at the moment!)
      pDoc->m_meta_graph->getDisplayedPointMap().makeViewportPoints( logicalviewport );
   }
   if (state & MetaGraph::SHAPEGRAPHS) {
      pDoc->m_meta_graph->getDisplayedShapeGraph().makeViewportShapes( logicalviewport );
   }
   if (state & MetaGraph::DATAMAPS) {
      pDoc->m_meta_graph->getDisplayedDataMap().makeViewportShapes( logicalviewport );
   }
   if (state & MetaGraph::LINEDATA) {
      pDoc->m_meta_graph->SuperSpacePixel::makeViewportShapes( logicalviewport );
   }

   if (state & MetaGraph::POINTMAPS && pDoc->m_meta_graph->getViewClass() & MetaGraph::VIEWVGA) {

      PointMap& map = pDoc->m_meta_graph->getDisplayedPointMap();

      double spacing = map.getSpacing();
      double spacer = 4800.0 * (spacing/logicalviewport.width()) / 2.0;

      stream << "<g stroke=\"none\">" << endl;

      stream << "<defs><rect id=\"a\" width=\"" << 2 * spacer << "\" height=\"" << 2 * spacer << "\" /></defs>" << endl;

      while ( map.findNextPoint() ) {

         Point2f logical = map.getNextPointLocation();
         PafColor color = map.getPointColor();
         
         if (color.alphab() != 0) { // alpha == 0 is transparent

            CPoint p = SVGPhysicalUnits(logical,logicalviewport,h);

            stream << "<use fill=\"" << SVGColor(color) << "\" x=\"" << p.x - spacer << "\" y=\"" << p.y - spacer << "\" xlink:href=\"#a\" />" << endl;
         }
      }
      stream << "</g>" << endl;
   }

   if (state & MetaGraph::SHAPEGRAPHS && pDoc->m_meta_graph->getViewClass() & MetaGraph::VIEWAXIAL) {

      ShapeMap& map = pDoc->m_meta_graph->getDisplayedShapeGraph();

      OutputSVGMap(stream, map, logicalviewport, h);
   }

   if (state & MetaGraph::DATAMAPS && pDoc->m_meta_graph->getViewClass() & MetaGraph::VIEWDATA) {

      ShapeMap& map = pDoc->m_meta_graph->getDisplayedDataMap();

      OutputSVGMap(stream, map, logicalviewport, h);
   }

   if (state & MetaGraph::LINEDATA) {
      // arbitrary stroke width for now
      stream << "<g stroke-width=\"4\" fill=\"none\" stroke=\"" << SVGColor(GetApp()->m_foreground) << "\">" << endl;
      bool nextlayer = false;
      while ( pDoc->m_meta_graph->SuperSpacePixel::findNextShape(nextlayer) ) {
         const SalaShape& shape = pDoc->m_meta_graph->SuperSpacePixel::getNextShape();
         Line l;
         if (shape.isPoint()) {
         }
         else if (shape.isLine()) {
            Line line = shape.getLine();
            OutputSVGLine(stream, line, logicalviewport, h);
         }
         else {
            OutputSVGPoly(stream, shape, logicalviewport, h);
         }
      }
      stream << "</g>" << endl;
   }

   stream << "</svg>" << endl;
}

void CDepthmapView::OutputSVGMap(ofstream& stream, ShapeMap& map, Region& logicalviewport, int h)
{
   bool monochrome = (map.getDisplayParams().colorscale == DisplayParams::MONOCHROME);
   // monochrome not implemented yet!
   if (monochrome) {
      AfxMessageBox(_T("This map is displaying in monochrome, which is not yet supported.  Please note your SVG file will be empty.  Sorry for the inconvenience."));
      return;
   }

   // I haven't implemented all of these, but I hope I will get there:
   bool showlines, showfill, showcentroids;
   map.getPolygonDisplay(showlines,showfill,showcentroids);

   // arbitrary stroke width for now
   stream << "<g stroke-width=\"4\">" << endl;

   PafColor color, oldcolor;
   bool closed, oldclosed;

   bool first = true;
   bool dummy;
   while ( map.findNextShape(dummy) ) {

      // note: getNextLine clears current line, so getLineColor before line
      color = map.getShapeColor();
      const SalaShape& shape = map.getNextShape();
      closed = shape.isClosed();

      if (first || color != oldcolor || closed != oldclosed) {
         if (!first) {
            stream << "</g>" << endl;
         }
         else {
            first = false;
         }
         if (closed) {
            if (showlines) {
               stream << "<g fill=\"" << SVGColor(color) << "\" stroke=\"" << SVGColor(GetApp()->m_foreground) << "\">" << endl;
            }
            else {
               stream << "<g fill=\"" << SVGColor(color) << "\" stroke=\"none\">" << endl;
            }
         }
         else {
            stream << "<g fill=\"none\" stroke=\"" << SVGColor(color) << "\">" << endl;
         }
         oldcolor = color;
         oldclosed = closed;
      }

      Line l;
      if (shape.isPoint()) {
      }
      else if (shape.isLine()) {
         Line line = shape.getLine();
         OutputSVGLine(stream, line, logicalviewport, h);
      }
      else {
         OutputSVGPoly(stream, shape, logicalviewport, h);
      }
   }
   if (!first) {
      stream << "</g>" << endl;
   }

   stream << "</g>" << endl;
}

void CDepthmapView::OutputSVGLine(ofstream& stream, Line& line, Region& logicalviewport, int h)
{
   bool drewit = false;
   if (line.crop(logicalviewport)) {
      CPoint start = SVGPhysicalUnits(line.start(),logicalviewport,h);
      CPoint end = SVGPhysicalUnits(line.end(),logicalviewport,h);
      // 2.0 is about 0.1mm in a standard SVG output size
      if (dist(start,end) >= 2.4f) {
         stream << "<line x1=\"" << start.x << "\" y1=\"" << start.y << "\""
                << " x2=\"" << end.x << "\" y2=\"" << end.y << "\" />" << endl;
      }
   }
}

void CDepthmapView::OutputSVGPoly(ofstream& stream, const SalaShape& shape, Region& logicalviewport, int h)
{
   CPoint bl = SVGPhysicalUnits(shape.getBoundingBox().bottom_left,logicalviewport,h);
   CPoint tr = SVGPhysicalUnits(shape.getBoundingBox().top_right,logicalviewport,h);
   if (dist(bl,tr) < 2.0f) {
      // 2.0 is about 0.1mm in standard SVG output size -- if this is too small, we won't bother trying to draw it at all
      return;
   }
   if (shape.isOpen()) {
      // open lines are fairly easy: simply chop lines as they enter and exit
      stream << "<polyline points=\"";
      bool starter = true, drawn = false;
      Point2f lastpoint = shape.head();
      for (size_t i = 1; i < shape.size(); i++) {
         Line line(lastpoint,shape[i]);
         if (line.crop(logicalviewport)) {
            // note: use t_start and t_end so that this line moves in the correct direction
            CPoint start = SVGPhysicalUnits(line.t_start(),logicalviewport,h);
            CPoint end = SVGPhysicalUnits(line.t_end(),logicalviewport,h); 
            // always draw either from the first point or whenever you enter the viewport:
            if (starter) {
               stream << start.x << "," << start.y << " ";
               starter = false;
            }
            // 2.0 is about 0.1mm in a standard SVG output size
            if (dist(start,end) >= 2.0f || i == shape.size() - 1) { // also, always draw the very last point regardless of distance
               stream << end.x << "," << end.y << " ";
               drawn = true;
            }
         }
         else {
            starter = true;
            drawn = true;
         }
         if (drawn) {
            lastpoint = shape[i];
            drawn = false;
         }
      }
      stream << "\" />" << endl;
   }
   else {
      // polygons are hard... have to work out entry and exit points to the clipping frame
      // and wind according to their direction
      stream << "<polygon points=\"";
      pqvector<SalaEdgeU> eus = shape.getClippingSet(logicalviewport);
      if (eus.size() == 0) {
         // this should be a shape that is entirely within the viewport:
         CPoint last = SVGPhysicalUnits(shape[0],logicalviewport,h);
         stream << last.x << "," << last.y << " ";
         for (size_t i = 1; i < shape.size(); i++) {
            CPoint next = SVGPhysicalUnits(shape[i],logicalviewport,h);
            if (dist(last,next) >= 2.0f) {
               stream << next.x << "," << next.y << " ";
               last = next;
            }
         }
      }
      else if (eus.size() == 1) {
         // dummy: getClippingSet deliberately adds a single empty EdgeU if the polygon is completely outside the frame
         // (this can happen when a polygon wraps around the frame)
      }
      else if (eus.size() >= 2) {
         int entry = eus[0].entry ? 0 : 1;
         // this can get very messy (sometimes have to split into separate polys)... here's hoping for the best
         while (entry < (int)eus.size()) {
            int exit = int((entry + 1) % eus.size());
            Point2f pt = logicalviewport.getEdgeUPoint(eus[entry]);
            CPoint last = SVGPhysicalUnits(pt,logicalviewport,h);
            CPoint next;
            stream << last.x << "," << last.y << " ";
            for (int i = eus[entry].index + 1; i != eus[exit].index; i++) {
               if (i >= shape.size()) {
                  i = 0;
               }
               next = SVGPhysicalUnits(shape[i],logicalviewport,h);
               if (dist(last,next) >= 2.0f) {
                  stream << next.x << "," << next.y << " ";
                  last = next;
               }
            }
            pt = logicalviewport.getEdgeUPoint(eus[exit]);
            last = SVGPhysicalUnits(pt,logicalviewport,h);
            stream << last.x << "," << last.y << " ";
            bool breakup = false;
            if (entry + 2 < (int)eus.size() && ccwEdgeU(eus[entry],eus[entry+1],eus[entry+2]) != shape.isCCW()) {
               breakup = true;
            }
            EdgeU& nextentry = breakup ? eus[entry] : eus[(exit+1)%eus.size()];
            if (shape.isCCW()) {
               if (nextentry.edge != eus[exit].edge || nextentry.u < eus[exit].u) {
                  int edge = eus[exit].edge;
                  do {
                     edge++;
                     if (edge > 3) {
                        edge = 0;
                     }
                     next = SVGPhysicalUnits( logicalviewport.getEdgeUPoint(EdgeU(edge,0)),logicalviewport,h);
                     stream << next.x << "," << next.y << " ";
                  }
                  while (edge != nextentry.edge);
               }
            }
            else {
               if (nextentry.edge != eus[exit].edge || nextentry.u > eus[exit].u) {
                  int edge = eus[exit].edge;
                  do {
                     edge--;
                     if (edge < 0) {
                        edge = 3;
                     }
                     next = SVGPhysicalUnits( logicalviewport.getEdgeUPoint(EdgeU(edge,1)),logicalviewport,h);
                     stream << next.x << "," << next.y << " ";
                  }
                  while (edge != nextentry.edge);
               }
            }
            if (breakup) {
            //if (entry + 2 < eus.size() && ccwEdgeU(eus[entry],eus[entry+1],eus[entry+2]) != shape.isCCW()) {
               stream << "\" />" << endl;
               stream << "<polygon points=\"";
            }
            entry += 2;
         }
      }
      stream << "\" />" << endl;
   }
}

/////////////////////////////////////////////////////////////////////////////

void CDepthmapView::FillLocation(CDC *pDC, CPoint& p, int spacer, unsigned int blocked, COLORREF color)
{
   CBrush colorBrush( color );
   if (spacer > 6) {
      for (int i = 0; i < 16; i++) {
         if (blocked & (1 << i)) {
            pDC->FillRect( CRect(p.x + ((i % 4) - 2) * spacer / 2, 
                                 p.y + (1 - (i / 4)) * spacer / 2,
                                 p.x + ((i % 4) - 1) * spacer / 2, 
                                 p.y + (2 - (i / 4)) * spacer / 2), &colorBrush); 
         }
      }
   }
   else if (spacer > 3) {
      if (blocked & 0x3300)   // tl
         pDC->FillRect( CRect(p.x - spacer, p.y, p.x, p.y - spacer), &colorBrush); 
      if (blocked & 0xCC00)   // tr
         pDC->FillRect( CRect(p.x, p.y, p.x + spacer, p.y - spacer), &colorBrush); 
      if (blocked & 0x0033)   // bl
         pDC->FillRect( CRect(p.x - spacer, p.y + spacer, p.x, p.y), &colorBrush); 
      if (blocked & 0x00CC)   // br
         pDC->FillRect( CRect(p.x, p.y + spacer, p.x + spacer, p.y), &colorBrush);  
   }
   else if (spacer > 1) {
      pDC->FillRect( CRect(p.x - spacer, p.y + spacer, p.x + spacer, p.y - spacer), &colorBrush);
   }
   else {
      pDC->SetPixel( p, color );
   }
}

/////////////////////////////////////////////////////////////////////////////

void CDepthmapView::DrawCross(CDC *pDC, CPoint& centre, bool drawit)
{
   if (drawit) {
      for (int i = 0; i < 9; i++) {
         CPoint pos(centre.x-4+i,centre.y);
         m_cross_pixels[i] = pDC->GetPixel(pos);
         pDC->SetPixel(pos,GetApp()->m_foreground);
      }
      for (int j = 0; j < 9; j++) {
         if (j != 4) {
            CPoint pos(centre.x,centre.y-4+j);
            m_cross_pixels[9+j] = pDC->GetPixel(pos);
            pDC->SetPixel(pos,GetApp()->m_foreground);
         }
      }
   }
   else { // erase it
      for (int i = 0; i < 9; i++) {
         CPoint pos(centre.x-4+i,centre.y);
         pDC->SetPixel(pos,m_cross_pixels[i]);
      }
      for (int j = 0; j < 9; j++) {
         if (j != 4) {
            CPoint pos(centre.x,centre.y-4+j);
            pDC->SetPixel(pos,m_cross_pixels[9+j]);
         }
      }
   }
}

void CDepthmapView::DrawLine(CDC *pDC, CRect& line, bool drawit)
{
   if (line.Width() == 0 && line.Height() == 0) {
      return;
   }
   if (drawit) {
      m_line_pixels.clear();
   }
   else if (!m_line_pixels.size()) {
      return; // shouldn't happen, but does appear to
   }
   bool wide = (abs(line.Width()) > abs(line.Height()));
   if (wide) {
      int step = (line.Width() > 0) ? 1 : -1;
      for (int i = 0; (i * step) < abs(line.Width()); i += step) {
         CPoint thispixel(i, int((float(line.Height())/float(line.Width())) * float(i)));
         if (drawit) {
            m_line_pixels.push_back(pDC->GetPixel(line.TopLeft() + thispixel));
            pDC->SetPixel(line.TopLeft() + thispixel, GetApp()->m_foreground);
         }
         else {
            pDC->SetPixel(line.TopLeft() + thispixel, m_line_pixels[step * i]);
         }
      }
   }
   else {
      int step = (line.Height() > 0) ? 1 : -1;
      for (int i = 0; (i * step) < abs(line.Height()); i += step) {
         CPoint thispixel(int((float(line.Width())/float(line.Height())) * float(i)), i);
         if (drawit) {
            m_line_pixels.push_back(pDC->GetPixel(line.TopLeft() + thispixel));
            pDC->SetPixel(line.TopLeft() + thispixel, GetApp()->m_foreground);
         }
         else {
            pDC->SetPixel(line.TopLeft() + thispixel, m_line_pixels[step * i]);
         }
      }
   }
   if (!drawit) {
      m_line_pixels.clear();
   }
}

/////////////////////////////////////////////////////////////////////////////

void CDepthmapView::OnTimer(UINT_PTR nIDEvent) 
{
   if (nIDEvent == DMP_TIMER_REDRAW) {

      if (m_queued_redraw) {

         // Internal own redraw
         OnRedraw(0,0);

      }
      else if (m_continue_drawing) {

         if (!m_drawing) {

            m_internal_redraw = true;
            Invalidate(FALSE);
            UpdateWindow();

         }

      }
   }
   else if (nIDEvent == DMP_TIMER_HOVER && !m_continue_drawing && !m_queued_redraw && !m_drawing) {
      if (!(m_mouse_mode & DRAWLINE)) {
         m_hover_tick++;
      }
      // on the sixth tick, create a hover window (if there's a selection)
      if (m_hover_tick == 6) {
         CreateHoverWnd();
      }
   }
   
	CView::OnTimer(nIDEvent);
}

void CDepthmapView::ResetHoverWnd(const CPoint& p) // = CPoint(-1,-1);
{
   if (p == CPoint(-1,-1) || m_hover_point != p) {
      KillTimer(DMP_TIMER_HOVER);
      m_hover_tick = 0;
      m_hover_point = p;
      // kill hover window if it exists
      if (m_hover_wnd) {
         m_hover_wnd->DestroyWindow();
         delete m_hover_wnd;
         m_hover_wnd = NULL;
      }
      SetTimer(DMP_TIMER_HOVER,100,NULL);
   }
}

void CDepthmapView::CreateHoverWnd()
{
   KillTimer(DMP_TIMER_HOVER);

   if (!GetDocument()->m_communicator && !m_hover_wnd && m_hover_point != CPoint(-1,-1)) {
      // check point in window
      CPoint point;
      GetCursorPos(&point);
      ScreenToClient(&point);
      CRect rect;
      GetClientRect(rect);
      if (rect.PtInRect(point)) {
         // first, check you have a meta graph
         CGraphDoc *pDoc = GetDocument();
         if (pDoc->m_meta_graph) {
            prefvec<CString> strings;
            if (pDoc->m_meta_graph->viewingProcessed() && pDoc->m_meta_graph->getSelCount() > 1) {
               float val = pDoc->m_meta_graph->getSelAvg();
               int count = pDoc->m_meta_graph->getSelCount();
               if (val == -1.0f) {
                  m_hover_wnd = new CHoverWnd;
                  strings.push_back("Null selection");
                  m_hover_wnd->Create(this, strings, m_hover_point);
               }
               else if (val != -2.0f) {
                  strings.push_back(_T("Selection"));
                  strings.push_back(CString());
                  strings.tail().Format(_T("Average: %g"),val);
                  strings.push_back(CString());
                  strings.tail().Format(_T("Count: %d"),count);
                  m_hover_wnd = new CHoverWnd;
                  m_hover_wnd->Create(this, strings, m_hover_point);
               }
            }
            else if (pDoc->m_meta_graph->viewingProcessed()) {
               // and that it has an appropriate state to display a hover wnd
               float val = pDoc->m_meta_graph->getLocationValue( LogicalUnits(m_hover_point) );
               if (val == -1.0f) {
                  m_hover_wnd = new CHoverWnd;
                  strings.push_back(_T("No value"));
                  m_hover_wnd->Create(this, strings, m_hover_point);
               }
               else if (val != -2.0f) {
                  strings.push_back(CString());
                  strings.tail().Format(_T("%g"),val);
                  m_hover_wnd = new CHoverWnd;
                  m_hover_wnd->Create(this, strings, m_hover_point);
               }
            }
         }
      }
   }
}

LRESULT CDepthmapView::OnRedraw(WPARAM wParam, LPARAM lParam)
{
   CGraphDoc *pDoc = GetDocument();

   if (pDoc->GetRemenuFlag(CGraphDoc::VIEW_MAP)) {
      pDoc->SetRemenuFlag(CGraphDoc::VIEW_MAP,false);
      // redo the menus for this *view* directly:
      //((CChildFrame*) GetParentFrame())->m_view_selector.RedoMenu( *pDoc->m_meta_graph );
   }
   if (pDoc->GetRedrawFlag(CGraphDoc::VIEW_MAP) != CGraphDoc::REDRAW_DONE) {

      if (!pDoc->m_communicator) {

         m_queued_redraw = false;

         switch (pDoc->GetRedrawFlag(CGraphDoc::VIEW_MAP)) {

            case CGraphDoc::REDRAW_POINTS:

               if (pDoc->m_meta_graph->viewingProcessedLines()) {
                  // Axial lines are thicker on selection, so background needs clearing
                  m_redraw_all = true;
               }
               else {
                  m_redraw_no_clear = true;
               }
               
               break;

            case CGraphDoc::REDRAW_GRAPH:

               m_redraw_all = true;
               break;

            case CGraphDoc::REDRAW_TOTAL:
         
               m_viewport_set = false;
               m_redraw_all = true;
               break;
         }

         pDoc->SetRedrawFlag(CGraphDoc::VIEW_MAP,CGraphDoc::REDRAW_DONE);
         
         Invalidate(FALSE);
         UpdateWindow();
      }
      else {

         KillTimer(DMP_TIMER_REDRAW);
         SetTimer(DMP_TIMER_REDRAW,100,NULL);
         m_queued_redraw = true;

      }
   }
   
   return 0;
}


void CDepthmapView::OnSize(UINT nType, int cx, int cy) 
{
   m_redraw_all = true;
   m_resize_viewport = true;

	CView::OnSize(nType, cx, cy);
}

   // If selecting, draw selection box
   /*
   if (m_selecting && m_end_point != m_current_point) {
      CRect *rect = new CRect( m_start_point, m_current_point );
      static CRect *lastrect = NULL;
      pDC->DrawDragRect( rect, CSize(1,1), lastrect, CSize(1,1) );
      if (lastrect) {
         delete lastrect;
      }
      lastrect = rect;
      m_end_point = m_current_point;
   }
   */

/////////////////////////////////////////////////////////////////////////////
// CDepthmapView printing

BOOL CDepthmapView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDepthmapView::OnBeginPrinting(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDepthmapView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDepthmapView diagnostics

#ifdef _DEBUG
void CDepthmapView::AssertValid() const
{
	CView::AssertValid();
}

void CDepthmapView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGraphDoc* CDepthmapView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGraphDoc)));
	return (CGraphDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDepthmapView message handlers

void CDepthmapView::OnViewMove() 
{
   m_mouse_mode = DRAG;
   SetCursor(DRAG);
}

void CDepthmapView::OnViewZoomIn() 
{
   m_mouse_mode = ZOOM_IN;
   SetCursor(ZOOM_IN);
}

void CDepthmapView::OnViewZoomOut() 
{
   m_mouse_mode = ZOOM_OUT;
   SetCursor(ZOOM_OUT);
}

void CDepthmapView::OnEditSelect() 
{
   m_mouse_mode = SELECT;
   SetCursor(SELECT);
   if (m_showlinks) {
      m_showlinks = false;
      GetDocument()->SetRedrawFlag(CGraphDoc::VIEW_MAP, CGraphDoc::REDRAW_POINTS, CGraphDoc::NEW_DEPTHMAPVIEW_SETUP, this);
   }
}

void CDepthmapView::OnModeIsovist() 
{
   m_mouse_mode = SEEDISOVIST;
   SetCursor(SEEDISOVIST);
   if (m_showlinks) {
      m_showlinks = false;
      GetDocument()->SetRedrawFlag(CGraphDoc::VIEW_MAP,CGraphDoc::REDRAW_POINTS, CGraphDoc::NEW_DEPTHMAPVIEW_SETUP, this);
   }
}

void CDepthmapView::OnModeHalfovist() 
{
   m_mouse_mode = SEEDHALFOVIST;
   SetCursor(SEEDHALFOVIST);
   if (m_showlinks) {
      m_showlinks = false;
      GetDocument()->SetRedrawFlag(CGraphDoc::VIEW_MAP,CGraphDoc::REDRAW_POINTS, CGraphDoc::NEW_DEPTHMAPVIEW_SETUP, this);
   }
}

void CDepthmapView::OnToolsAxialMap() 
{
   m_mouse_mode = SEEDAXIAL;
   SetCursor(SEEDAXIAL);
   if (m_showlinks) {
      m_showlinks = false;
      GetDocument()->SetRedrawFlag(CGraphDoc::VIEW_MAP,CGraphDoc::REDRAW_POINTS, CGraphDoc::NEW_DEPTHMAPVIEW_SETUP, this);
   }
}

void CDepthmapView::OnEditFill() 
{
   m_mouse_mode = FILL;
   m_fillmode = FULLFILL;
   SetCursor(FILL);
}

void CDepthmapView::OnEditSemiFill() 
{
   m_mouse_mode = FILL;
   m_fillmode = SEMIFILL;
   SetCursor(FILL);
}

void CDepthmapView::OnEditPencil() 
{
   m_mouse_mode = PENCIL;
   SetCursor(PENCIL);
}

void CDepthmapView::OnEditEraser() 
{
   m_mouse_mode = ERASE;
   SetCursor(ERASE);
}

void CDepthmapView::OnEditLineTool() 
{
   m_mouse_mode = LINETOOL;
   SetCursor(LINETOOL);
   if (m_showlinks) {
      m_showlinks = false;
      GetDocument()->SetRedrawFlag(CGraphDoc::VIEW_MAP,CGraphDoc::REDRAW_POINTS, CGraphDoc::NEW_DEPTHMAPVIEW_SETUP, this);
   }
}

void CDepthmapView::OnEditPolygon() 
{
   m_mouse_mode = POLYGONTOOL;
   SetCursor(POLYGONTOOL);
   if (m_showlinks) {
      m_showlinks = false;
      GetDocument()->SetRedrawFlag(CGraphDoc::VIEW_MAP,CGraphDoc::REDRAW_POINTS, CGraphDoc::NEW_DEPTHMAPVIEW_SETUP, this);
   }
}

void CDepthmapView::OnModeJoin() 
{
   if (GetDocument()->m_meta_graph->getState() & (MetaGraph::POINTMAPS | MetaGraph::SHAPEGRAPHS)) {
      m_mouse_mode = JOIN;
      if (!GetDocument()->m_meta_graph->isSelected()) {
         SetCursor(m_mouse_mode);
      }
      else {
         BeginJoin();
      }
      // Redraw scene
      m_showlinks = true;
      GetDocument()->SetRedrawFlag(CGraphDoc::VIEW_MAP,CGraphDoc::REDRAW_POINTS, CGraphDoc::NEW_DEPTHMAPVIEW_SETUP, this);
   }
}

void CDepthmapView::OnModeUnjoin() 
{
   if (GetDocument()->m_meta_graph->getState() & (MetaGraph::POINTMAPS | MetaGraph::SHAPEGRAPHS)) {
      m_mouse_mode = UNJOIN;
      if (!GetDocument()->m_meta_graph->isSelected()) {
         SetCursor(m_mouse_mode);
      }
      else {
         if (GetDocument()->m_meta_graph->viewingProcessedPoints()) {
            GetDocument()->m_meta_graph->clearSel();
         }
         else {
            m_mouse_mode |= JOINB;
            SetCursor(m_mouse_mode);
         }
      }
      // Redraw scene
      m_showlinks = true;
      GetDocument()->SetRedrawFlag(CGraphDoc::VIEW_MAP,CGraphDoc::REDRAW_POINTS, CGraphDoc::NEW_DEPTHMAPVIEW_SETUP, this);
   }
}

void CDepthmapView::BeginJoin()
{
   if (GetDocument()->m_meta_graph->getSelCount() > 1 && GetDocument()->m_meta_graph->viewingProcessedPoints()) {
      Region r = GetDocument()->m_meta_graph->getDisplayedPointMap().getSelBounds();
      CRect rect(PhysicalUnits(Point2f(r.bottom_left.x,r.top_right.y)),PhysicalUnits(Point2f(r.top_right.x,r.bottom_left.y)));
      int spacer = int(ceil(5.0 * GetDocument()->m_meta_graph->getDisplayedPointMap().getSpacing() / (m_unit * 10.0) ));
      GetCursorPos(&m_mouse_point);
      m_drag_rect_a = CRect(-rect.Width()-spacer/2,-rect.Height()-spacer/2,spacer/2,spacer/2);
      m_drag_rect_a.OffsetRect(m_mouse_point);
   }
   else {
      m_drag_rect_a.SetRectEmpty();
   }
   m_drag_rect_b.SetRectEmpty();

   m_mouse_mode |= JOINB;
   SetCursor(m_mouse_mode);
}

// invert any bi-mode with the alt key
LRESULT CDepthmapView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
   if (wParam == 18 && (m_mouse_mode & (ZOOM | JOINB | FILL)) != 0) {
      if (message == WM_SYSKEYDOWN) {
         if (!m_alt_mode) {
            m_alt_mode = true;
            // signal the button...
            AltMode();
         }
      }
      else if (message == WM_SYSKEYUP) {
         if (m_alt_mode) {
            m_alt_mode = false;
            // signal the button...
            AltMode();
         }
      }
      return 0;
   }

   return CView::WindowProc(message, wParam, lParam);
}

// set the inverse of any one mode:
void CDepthmapView::AltMode()
{
   switch (m_mouse_mode) {
   case ZOOM_IN:
      OnViewZoomOut();
      break;
   case ZOOM_OUT:
      OnViewZoomIn();
      break;
   case FILL:
      if (m_fillmode == FULLFILL) {
         OnEditSemiFill();
      }
      else {
         OnEditFill();
      }
   case JOIN:
   case JOIN | JOINB:
      OnModeUnjoin();
      break;
   case UNJOIN:
   case UNJOIN | JOINB:
      OnModeJoin();
      break;
   }
}

//////////////////////////////////////////////////////////////////////

void CDepthmapView::SetCursor(int mode)
{
   switch (mode)
   {
   case SELECT:
      m_cursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
      break;
   case SELECT | OVERHANDLE:
      m_cursor = AfxGetApp()->LoadCursor(IDC_MOVE_CURSOR);
      break;
   case DRAG:
      m_cursor = AfxGetApp()->LoadCursor(IDC_HAND_CURSOR);
      break;
   case ZOOM_IN:
      m_cursor = AfxGetApp()->LoadCursor(IDC_ZOOM_IN);
      break;
   case ZOOM_OUT:
      m_cursor = AfxGetApp()->LoadCursor(IDC_ZOOM_OUT);
      break;
   case SEEDISOVIST:
   case SEEDHALFOVIST:
   case SEEDAXIAL:
      m_cursor = AfxGetApp()->LoadCursor(IDC_SEED_CURSOR);
      break;
   case FILL:
      m_cursor = AfxGetApp()->LoadCursor(IDC_FILL_CURSOR);
      break;
   case PENCIL:
      m_cursor = AfxGetApp()->LoadCursor(IDC_PENCIL);
      break;
   case LINETOOL:
   case LINETOOL | DRAWLINE:
   case POLYGONTOOL:
   case POLYGONTOOL | DRAWLINE:
   case SELECT | DRAWLINE:
      m_cursor = AfxGetApp()->LoadCursor(IDC_CROSSCURSOR);
      break;
   case ERASE:
      m_cursor = AfxGetApp()->LoadCursor(IDC_ERASE);
      break;
   case JOIN:
   case UNJOIN:
      m_cursor = AfxGetApp()->LoadCursor(IDC_LINKCURSOR);
      break;
   case JOIN | JOINB:
      m_cursor = AfxGetApp()->LoadCursor(IDC_LINKCURSOR2);
      break;
   case UNJOIN | JOINB:
      m_cursor = AfxGetApp()->LoadCursor(IDC_LINKCURSOR1);
      break;
   default:
      m_cursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
      break;
   }
   ::SetCursor(m_cursor);
}

/////////////////////////////////////////////////////////////////////

void CDepthmapView::OnUpdateViewMove(CCmdUI* pCmdUI) 
{
   OnUpdateGeneric(pCmdUI, DRAG );
}

void CDepthmapView::OnUpdateViewZoomIn(CCmdUI* pCmdUI) 
{
   OnUpdateGeneric(pCmdUI, ZOOM_IN );
}

void CDepthmapView::OnUpdateViewZoomOut(CCmdUI* pCmdUI) 
{
   OnUpdateGeneric(pCmdUI, ZOOM_OUT );
}

void CDepthmapView::OnUpdateEditSelect(CCmdUI* pCmdUI) 
{
   OnUpdateGeneric(pCmdUI, SELECT );
}

void CDepthmapView::OnUpdateEditFill(CCmdUI* pCmdUI) 
{
   OnUpdateGeneric(pCmdUI, FILL, FULLFILL, false );
}

void CDepthmapView::OnUpdateEditSemiFill(CCmdUI* pCmdUI) 
{
   OnUpdateGeneric(pCmdUI, FILL, SEMIFILL, false );
}

///////////////////////////////////////////////////////////////////////////////////

void CDepthmapView::OnUpdateModeIsovist(CCmdUI* pCmdUI) 
{
   int state = GetDocument()->m_meta_graph->getState();
   if (~state & MetaGraph::LINEDATA) {
      pCmdUI->Enable(FALSE);
   }
   else {
      pCmdUI->Enable(TRUE);
   }
   if (m_mouse_mode == SEEDISOVIST) {
      pCmdUI->SetCheck(1);
   }
   else {
      pCmdUI->SetCheck(0);
   }
}

void CDepthmapView::OnUpdateModeHalfovist(CCmdUI* pCmdUI) 
{
   int state = GetDocument()->m_meta_graph->getState();
   if (~state & MetaGraph::LINEDATA) {
      pCmdUI->Enable(FALSE);
   }
   else {
      pCmdUI->Enable(TRUE);
   }
   if (m_mouse_mode == SEEDHALFOVIST) {
      pCmdUI->SetCheck(1);
   }
   else {
      pCmdUI->SetCheck(0);
   }
}


void CDepthmapView::OnUpdateToolsAxialMap(CCmdUI* pCmdUI) 
{
   int state = GetDocument()->m_meta_graph->getState();
   if (~state & MetaGraph::LINEDATA) {
      pCmdUI->Enable(FALSE);
   }
   else {
      pCmdUI->Enable(TRUE);
   }
   if (m_mouse_mode == SEEDAXIAL) {
      pCmdUI->SetCheck(1);
   }
   else {
      pCmdUI->SetCheck(0);
   }
}

///////////////////////////////////////////////////////////////////////////////////

void CDepthmapView::OnUpdateModeJoin(CCmdUI* pCmdUI) 
{
   OnUpdateGenericJoin(pCmdUI, JOIN);
}

void CDepthmapView::OnUpdateModeUnjoin(CCmdUI* pCmdUI) 
{
   OnUpdateGenericJoin(pCmdUI, UNJOIN);
}

void CDepthmapView::OnUpdateGenericJoin(CCmdUI* pCmdUI, int mode) 
{
   CGraphDoc *pDoc = GetDocument();
   if (( ( (pDoc->m_meta_graph->getViewClass() & MetaGraph::VIEWVGA) && 
           (pDoc->m_meta_graph->getDisplayedPointMap().isProcessed())) || 
         ( (pDoc->m_meta_graph->getViewClass() & MetaGraph::VIEWAXIAL) && 
           (pDoc->m_meta_graph->getState() & MetaGraph::SHAPEGRAPHS)) && 
           (!pDoc->m_meta_graph->getDisplayedShapeGraph().isSegmentMap()) ) ) {
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE);
      // turn off unavailable mode
      if (m_mouse_mode & GENERICJOIN) {
         OnEditSelect();
      }
   }
   if ((m_mouse_mode & mode) == mode) {
      m_showlinks = true;
      pCmdUI->SetCheck(1);
   }
   else {
      pCmdUI->SetCheck(0);
   }
}

void CDepthmapView::OnUpdateEditPencil(CCmdUI* pCmdUI) 
{
   OnUpdateGeneric(pCmdUI, PENCIL, 0, false );
}

void CDepthmapView::OnUpdateEditEraser(CCmdUI* pCmdUI) 
{
   OnUpdateGeneric(pCmdUI, ERASE, 0, false );	
}

void CDepthmapView::OnUpdateEditLineTool(CCmdUI* pCmdUI) 
{
   OnUpdateGenericDrawingTools(pCmdUI, LINETOOL);
}

void CDepthmapView::OnUpdateEditPolygon(CCmdUI* pCmdUI) 
{
   OnUpdateGenericDrawingTools(pCmdUI, POLYGONTOOL);
}

void CDepthmapView::OnUpdateGenericDrawingTools(CCmdUI* pCmdUI, int mode) 
{
   MetaGraph *meta_graph = ((CGraphDoc *)GetDocument())->m_meta_graph;
   int type = meta_graph->getDisplayedMapType();
   if ((type == ShapeMap::DATAMAP && meta_graph->getDisplayedDataMap().isEditable()) ||
       ((type == ShapeMap::AXIALMAP || type == ShapeMap::CONVEXMAP || type == ShapeMap::PESHMAP) && meta_graph->getDisplayedShapeGraph().isEditable())) {
      if (type == ShapeMap::AXIALMAP && mode == POLYGONTOOL) {
         ((CChildFrame *)GetParentFrame())->PostMessage(WM_COMMAND,ID_MAPBAR_LINE);
         return;
      }
      else if (type == ShapeMap::CONVEXMAP && mode == LINETOOL) {
         ((CChildFrame *)GetParentFrame())->PostMessage(WM_COMMAND,ID_MAPBAR_POLYGON);
         return;
      }
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE);
      if (m_mouse_mode & mode) {
         OnEditSelect();
      }
   }
   if (m_mouse_mode & mode) {
      pCmdUI->SetCheck(1);
   }
   else {
      pCmdUI->SetCheck(0);
   }
}

void CDepthmapView::OnUpdateGeneric(CCmdUI* pCmdUI, int mode, int fillmode /* = 0 */, bool allowgraph /* = true */ ) 
{
   if (allowgraph || GetDocument()->m_meta_graph->viewingUnprocessedPoints()) {
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE);
      // if the mode is unavailable for some reason, switch to select mode (excluding select mode!)
      if (m_mouse_mode == mode && m_mouse_mode != SELECT) {
         pCmdUI->SetCheck(0);
         OnEditSelect();
         return;
      }
   }
   if (m_mouse_mode == mode) {
      if (mode == FILL) {
         if (m_fillmode == fillmode) {
            pCmdUI->SetCheck(1);
         }
         else {
            pCmdUI->SetCheck(0);
         }
      }
      else {
         pCmdUI->SetCheck(1);
      }
   }
   else {
      pCmdUI->SetCheck(0);
   }
}

bool CDepthmapView::ModeOk()
{
   int state = GetDocument()->m_meta_graph->getState();

   if (!m_mouse_mode) {
      return true;
   }
   else if (!state) {
      return false;
   }
   else
   {
      int mode = m_mouse_mode;
      if ((mode == FILL || mode == PENCIL) && !GetDocument()->m_meta_graph->viewingUnprocessedPoints()) {
         return false;
      }
   }
   return true;
}

BOOL CDepthmapView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
   BOOL bResult = CView::OnSetCursor(pWnd, nHitTest, message);

   if (ModeOk()) {
      ::SetCursor(m_cursor);
   }
   else {
      ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
   }

	return bResult;
}

/////////////////////////////////////////////////////////////////////////////

void CDepthmapView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
   if (m_current_mode == NONE) {
      switch (nChar) {
      case VK_ADD:         // Numeric '+'
      case 187:            // Standard '+'
         ZoomIn(2.0, m_centre);
         break;
      case VK_SUBTRACT:    // Numeric '+'
      case 189:            // Standard '-'
         ZoomOut();
         break;
      case VK_LEFT:
      case VK_UP:
      case VK_RIGHT:
      case VK_DOWN:
         m_current_mode = DRAG;
         GetClientRect(m_drag_rect_a);
         m_drag_rect_b.SetRectEmpty();
         m_invalidate = DRAG;
         Invalidate(FALSE);
         UpdateWindow();
         break;
      }
   }
   else if (m_current_mode == DRAG) {
      bool handled = false;
      switch (nChar) {
      case VK_LEFT:
         m_drag_rect_a.OffsetRect(-10,0);
         m_centre.x += double(10) * m_unit;
         handled = true;
         break;
      case VK_UP:
         m_drag_rect_a.OffsetRect(0,-10);
         m_centre.y += double(-10) * m_unit;
         handled = true;
         break;
      case VK_RIGHT:
         m_drag_rect_a.OffsetRect(10,0);
         m_centre.x += double(-10) * m_unit;
         handled = true;
         break;
      case VK_DOWN:
         m_drag_rect_a.OffsetRect(0,10);
         m_centre.y += double(10) * m_unit;
         handled = true;
         break;
      }      
      if (handled) {
         m_invalidate = DRAG;
         Invalidate(FALSE);
         UpdateWindow();
      }
   }

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CDepthmapView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
   if (m_current_mode == DRAG) {
      switch (nChar) {
      case VK_LEFT:
      case VK_UP:
      case VK_RIGHT:
      case VK_DOWN:
         // Stop drag rect...
         m_drag_rect_a.SetRectEmpty();
         m_invalidate = DRAG;
         Invalidate(FALSE);
         UpdateWindow();
         // Redraw scene
         m_redraw_all = true;
         Invalidate(FALSE);
         UpdateWindow();         
         m_current_mode = NONE;
         break;
      }

   }

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CDepthmapView::ZoomIn(double ratio, const Point2f& point)
{
   m_centre = point;
   m_unit /= ratio;

   // Animation
   GetClientRect(m_drag_rect_a);
   m_drag_rect_a.DeflateRect(30,30);
   m_drag_rect_b.SetRectEmpty();
   for (int i = 0; i < 30; i++) {
      m_invalidate = ZOOM_IN;
      Invalidate(FALSE);
      UpdateWindow();
      m_drag_rect_a.InflateRect(1,1);
      Sleep(10);
   }
   m_drag_rect_a.SetRectEmpty();
   m_invalidate = ZOOM_IN;
   Invalidate(FALSE);
   UpdateWindow();

   // Redraw
   m_redraw_all = true;
   Invalidate(FALSE);
   UpdateWindow();
}

void CDepthmapView::ZoomOut()
{
   m_unit *= 2.0;

   // Animation
   GetClientRect(m_drag_rect_a);
   m_drag_rect_b.SetRectEmpty();
   for (int i = 0; i < 30; i++) {
      m_invalidate = ZOOM_OUT;
      Invalidate(FALSE);
      UpdateWindow();
      m_drag_rect_a.DeflateRect(1,1);
      Sleep(10);
   }
   m_drag_rect_a.SetRectEmpty();
   m_invalidate = ZOOM_OUT;
   Invalidate(FALSE);
   UpdateWindow(); 
   
   // Redraw scene
   m_redraw_all = true;
   Invalidate(FALSE);
   UpdateWindow();
}

void CDepthmapView::OnLButtonDown(UINT nFlags, CPoint point) 
{
   if (m_snap) {
      point = PhysicalUnits(m_snap_point);
   }

   m_mouse_point = point;

   m_mouse_location = m_snap ? m_snap_point : LogicalUnits(point);

   // Just reset?

   m_current_mode = NONE;

   if (m_current_mode == NONE) {

      m_current_mode = m_mouse_mode;

      switch (m_mouse_mode) {
      case DRAG:
         {
            m_mouse_point = point;

            GetClientRect(m_drag_rect_a);
            m_drag_rect_b.SetRectEmpty();
            m_invalidate = DRAG;
            Invalidate(FALSE);
            UpdateWindow();

            SetCapture();
         }
         break;
      case SELECT:
      case ZOOM_IN:
      case JOIN:
      case UNJOIN:
         {
            m_mouse_point = point;
            m_drag_rect_a.SetRectEmpty();
            m_drag_rect_b.SetRectEmpty();

            m_invalidate = SELECT;
            Invalidate(FALSE);
            UpdateWindow();

            SetCapture();
         }
         break;
      case PENCIL:
         {
            m_current_mode = PENCIL;
            // Fill the point
            GetDocument()->m_meta_graph->getDisplayedPointMap().fillPoint(LogicalUnits(point),true);
            m_mouse_point = point;
            // Redraw scene
            m_redraw_no_clear = true;
            Invalidate(FALSE);
            UpdateWindow();
         }
         break;
      case ERASE:
         // no longer used
         break;
      default:
         break;
      }
   }
	CView::OnLButtonDown(nFlags, point);
}

void CDepthmapView::OnRButtonDown(UINT nFlags, CPoint point) 
{
   if (m_snap) {
      point = PhysicalUnits(m_snap_point);
   }

   m_mouse_point = point;

	CView::OnRButtonDown(nFlags, point);
}

void CDepthmapView::OnLButtonUp(UINT nFlags, CPoint point) 
{
   ReleaseCapture();
   CGraphDoc *pDoc = GetDocument();

   if (m_snap) {
      point = PhysicalUnits(m_snap_point);
   }
   Point2f location = m_snap ? m_snap_point : LogicalUnits( point );

   switch (m_current_mode) {
   case ZOOM_IN:
      if (m_drag_rect_a.Width() > 2 && m_drag_rect_a.Height() > 2) {
         CRect rect;
         GetClientRect(rect);
         double ratio = __min( double(rect.Height() / double(m_drag_rect_a.Height()) ),
                               double(rect.Width()) / double(m_drag_rect_a.Width()) );
         ZoomIn(ratio, LogicalUnits(m_drag_rect_a.CenterPoint()) );
      }
      else {
         ZoomIn(2.0, m_centre);
      }
      break;
   case ZOOM_OUT:
      {
         ZoomOut();
      }
      break;
   case DRAG:
      {   
         // Stop drag rect...
         m_drag_rect_a.SetRectEmpty();
         m_invalidate = DRAG;
         Invalidate(FALSE);
         UpdateWindow();

         // Redraw scene
         m_redraw_all = true;
         Invalidate(FALSE);
         UpdateWindow();
      }
      break;
   case UNJOIN:
   case SELECT: 
   case JOIN:
      {
         Region r;
         if (m_drag_rect_a.IsRectEmpty()) {
            r = Region( LogicalUnits(m_mouse_point), LogicalUnits(m_mouse_point) );
         }
         else {
            r = Region( LogicalUnits( CPoint(m_drag_rect_a.left, m_drag_rect_a.bottom) ), 
                        LogicalUnits( CPoint(m_drag_rect_a.right, m_drag_rect_a.top) ) );
         }

         // Stop drag rect...
         m_drag_rect_a.SetRectEmpty();
         m_invalidate = SELECT;
         Invalidate(FALSE);
         UpdateWindow();

         if (!pDoc->m_communicator) {
            // After checking that processing isn't occurring...
            // Do the selection (might take a while if someone selects the lot...)
            if (nFlags & MK_SHIFT) {
               pDoc->m_meta_graph->setCurSel( r, true ); // <- add to current sel
            }
            else {
               pDoc->m_meta_graph->setCurSel( r, false ); // <- reset current sel
            }
            // Redraw scene
            pDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL,CGraphDoc::REDRAW_POINTS, CGraphDoc::NEW_SELECTION, this);
         }
      }
      break;
   case JOIN | JOINB:
      {
         m_current_mode = NONE;
         // now get on with join:
         bool ok = false; 
         bool clearcursor = false;
         if (GetDocument()->m_meta_graph->getViewClass() & MetaGraph::VIEWVGA) {
            ok = GetDocument()->m_meta_graph->getDisplayedPointMap().mergePoints( LogicalUnits(point) );
         }
         else if (GetDocument()->m_meta_graph->getViewClass() & MetaGraph::VIEWAXIAL) {
            if (GetDocument()->m_meta_graph->getSelCount() == 1) {
               ok = GetDocument()->m_meta_graph->getDisplayedShapeGraph().linkShapes( LogicalUnits(point) );
            }
            else {
               // oops: you are only allowed to join lines one to one:
               GetDocument()->m_meta_graph->clearSel();
               clearcursor = true;
            }
         }
         if (clearcursor || ok) {
            m_mouse_mode = JOIN;
            SetCursor(JOIN);
            m_drag_rect_a.SetRectEmpty();
            if (ok) {
               GetDocument()->SetModifiedFlag();
               GetDocument()->SetRedrawFlag(CGraphDoc::VIEW_ALL,CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_DATA, this);
            }
            else {
               m_redraw_all = true;
               Invalidate(FALSE);
               UpdateWindow();
            }
         }
      }
      break;
   case UNJOIN | JOINB:
      {
         m_current_mode = NONE;
         // now get on with unjoin:
         bool ok = false;
         bool clearcursor = false;
         if (GetDocument()->m_meta_graph->getViewClass() & MetaGraph::VIEWAXIAL) {
            if (GetDocument()->m_meta_graph->getSelCount() == 1) {
               ok = GetDocument()->m_meta_graph->getDisplayedShapeGraph().unlinkShapes( LogicalUnits(point) );
            }
            else {
               // oops: you are only allowed to join lines one to one:
               GetDocument()->m_meta_graph->clearSel();
               clearcursor = true;
            }
         }
         if (clearcursor || ok) {
            m_mouse_mode = UNJOIN;
            SetCursor(UNJOIN);
            m_drag_rect_a.SetRectEmpty();
            if (ok) {
               GetDocument()->SetModifiedFlag();
               GetDocument()->SetRedrawFlag(CGraphDoc::VIEW_ALL,CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_DATA, this);
            }
            else {
               m_redraw_all = true;
               Invalidate(FALSE);
               UpdateWindow();
            }
         }
      }
      break;
   case SELECT | DRAWLINE:
      {
         m_current_mode = NONE;
         m_mouse_mode &= ~DRAWLINE;
         m_invalidate = LINEOFF;
         SetCursor(m_mouse_mode);
         Invalidate(FALSE);
         UpdateWindow();
         if (pDoc->m_meta_graph->moveSelShape(Line(m_line.t_start(),location))) {
            GetDocument()->SetModifiedFlag();
            GetDocument()->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_DATA, this );
         }
      }
      break;
   case SELECT | OVERHANDLE:

      BeginDrag(point);
      break;

   case LINETOOL: case POLYGONTOOL:
      {
         m_current_mode = NONE;
         m_mouse_mode |= DRAWLINE;
         m_line = Line(location, location);
         m_invalidate = LINEON;
         Invalidate(FALSE);
         UpdateWindow();
      }
      break;
   case LINETOOL | DRAWLINE:
      {
         m_current_mode = NONE;
         m_mouse_mode &= ~DRAWLINE;
         m_invalidate = LINEOFF;
         Invalidate(FALSE);
         UpdateWindow();
         if (pDoc->m_meta_graph->makeShape(Line(m_line.t_start(),location))) {
            GetDocument()->SetModifiedFlag();
            GetDocument()->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_DATA );
         }
      } 
      break;
   case POLYGONTOOL | DRAWLINE:
      {
         m_current_mode = NONE;
         m_mouse_mode &= ~DRAWLINE;
         m_invalidate = LINEOFF;
         Invalidate(FALSE);
         UpdateWindow();
         // if it's the first part, just make it a line:
         if (m_poly_points == 0) {
            pDoc->m_meta_graph->polyBegin(Line(m_line.t_start(),location));
            m_poly_start = m_line.t_start();
            m_poly_points += 2;
            m_mouse_mode |= DRAWLINE;
            m_line = Line(location, location);
            m_invalidate = LINEON;
            Invalidate(FALSE);
            UpdateWindow();
         }
         else if (m_poly_points > 2 && PixelDist(point,PhysicalUnits(m_poly_start)) < 6) {
            // check to see if it's back to the original start point, if so, close off
            pDoc->m_meta_graph->polyClose();
            m_poly_points = 0;
         }
         else {
            pDoc->m_meta_graph->polyAppend(location);
            m_poly_points += 1;
            m_mouse_mode |= DRAWLINE;
            m_line = Line(location, location);
            m_invalidate = LINEON;
            Invalidate(FALSE);
            UpdateWindow();
         }
         GetDocument()->SetModifiedFlag();
         GetDocument()->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_DATA );
      }
      break;
   case FILL:
      GetDocument()->OnFillPoints( location, m_fillmode );
      break;
   case SEEDISOVIST:
      m_current_mode = NONE;
      GetDocument()->OnMakeIsovist( location );
      break;
   case SEEDHALFOVIST:
      m_current_mode = NONE;
      m_mouse_mode |= DRAWLINE;
      m_line = Line(location,location);
      m_invalidate = LINEON;
      Invalidate(FALSE);
      UpdateWindow();
      break;
   case SEEDHALFOVIST | DRAWLINE:
      {
         m_current_mode = NONE;
         m_mouse_mode &= ~DRAWLINE;
         m_invalidate = LINEOFF;
         Invalidate(FALSE);
         UpdateWindow();
         Point2f vec = m_line.vector();
         vec.normalise();
         GetDocument()->OnMakeIsovist( m_line.t_start(), vec.angle() );
      }
      break;
   case SEEDAXIAL:
      GetDocument()->OnToolsAxialMap( location );
      // switch to select mode (stops you accidently pressing twice)
      OnEditSelect();
      break;
   }

   if (m_mouse_mode == JOIN) {
      if (GetDocument()->m_meta_graph->isSelected()) {
         BeginJoin();
      }
   }
   else if (m_mouse_mode == UNJOIN) {
      if (GetDocument()->m_meta_graph->isSelected()) {
         if (GetDocument()->m_meta_graph->viewingProcessedPoints()) {
            if (GetDocument()->m_meta_graph->getDisplayedPointMap().unmergePoints()) {
               GetDocument()->SetModifiedFlag();
               GetDocument()->SetRedrawFlag(CGraphDoc::VIEW_ALL,CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_DATA, this);
            }
         }
         else {
            m_mouse_mode |= JOINB;
            SetCursor(m_mouse_mode);
         }
      }
   }

	CView::OnLButtonUp(nFlags, point);
}

void CDepthmapView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
   if (m_mouse_mode == SELECT && !GetDocument()->m_communicator) {
      // This allows the user to edit the value of the clicked shape
      // One final check: you need something selected to edit the value
      // (note, this can work with either single or multiple selections)
      MetaGraph *graph = GetDocument()->m_meta_graph;
      if (graph->viewingProcessed() && graph->isSelected()) {
         CSetAttrValueDlg dlg;
         float oldval = graph->getSelAvg();
         if (graph->getSelCount() == 1) {
            // actually the "average" is simply the single row:
            dlg.m_value.Format(_T("%g"),oldval);
         }
         else {
            // in the case of multiple rows, show something that indicates values may vary:
            dlg.m_value = _T("#.#");
         }
         AttributeTable& table = graph->getAttributeTable();
         int col = table.getDisplayColumn();
         dlg.m_locked_column = table.isColumnLocked(col);
         dlg.m_pos = point;
         ClientToScreen(&(dlg.m_pos));
         if (IDOK == dlg.DoModal()) {
            if (!dlg.m_locked_column) {
               // set overidden value
               float newval = _wtof(dlg.m_value);
               if (graph->getSelCount() != 1 || newval != oldval) {
                  table.changeSelValues(col,newval);
                  // force re-evaluate of colours:
                  graph->setDisplayedAttribute(col);
                  GetDocument()->SetModifiedFlag();
                  GetDocument()->SetRedrawFlag(CGraphDoc::VIEW_ALL,CGraphDoc::REDRAW_GRAPH,CGraphDoc::NEW_DATA);
               }
            }
         }
      }
   }

   CView::OnLButtonDblClk(nFlags, point);
}


void CDepthmapView::BeginDrag(CPoint point)
{
   m_current_mode = NONE;
   int handle = -1;
   for (size_t i = 0; i < m_point_handles.size(); i++) {
      CPoint pt = PhysicalUnits(m_point_handles[i]);
      if (abs(pt.x-point.x) < 7 && abs(pt.y-point.y) < 7) {
         handle = (int)i;
         m_mouse_mode |= OVERHANDLE;
         SetCursor(m_mouse_mode);
         break;
      }
   }
   m_active_point_handle = handle;
   // for now, only two handles exist:
   if (m_active_point_handle != -1) {
      if (m_active_point_handle == 0) {
         m_line = Line(m_point_handles[1],m_point_handles[0]);
      }
      else {
         m_line = Line(m_point_handles[0],m_point_handles[1]);
      }
      m_mouse_mode |= DRAWLINE;
      m_mouse_mode &= ~OVERHANDLE;
      SetCursor(m_mouse_mode);
      m_invalidate = LINEON;
      Invalidate(FALSE);
      UpdateWindow();
   }
}


/*
void CDepthmapView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
   if (m_snap) {
      point = PhysicalUnits(m_snap_point);
   }

   if (m_current_mode == LINETOOL) {

      if (GetDocument()->m_meta_graph->endLine()) {

         // Get rid of temporary line
         m_drag_rect_a.SetRectEmpty();
         m_invalidate = LINETOOL;
         Invalidate(FALSE);
         UpdateWindow();
         m_line_drawing = false;

         m_current_mode = NONE;

         // Redraw scene
         m_redraw_no_clear = true;
         Invalidate(FALSE);
         UpdateWindow();            
      }

   }
	
	CView::OnLButtonDblClk(nFlags, point);
}
*/

void CDepthmapView::OnRButtonUp(UINT nFlags, CPoint point) 
{
   ReleaseCapture();
   CGraphDoc *pDoc = GetDocument();

   if (m_snap) {
      point = PhysicalUnits(m_snap_point);
   }

   // Right click now only acts as a cancel,
   // (and because I personally am used to right-click to zoom out, zoom out!

   if (!m_right_mouse_drag) {
      // cancel any tool which uses drawline:
      if (m_mouse_mode & DRAWLINE) {
         m_mouse_mode &= ~DRAWLINE;
         if (m_mouse_mode & POLYGONTOOL && m_poly_points > 0) {
            m_poly_points = 0;
            pDoc->m_meta_graph->polyCancel();
            GetDocument()->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_DATA );
         }
         else {
            m_invalidate = LINEOFF;
            Invalidate(FALSE);
            UpdateWindow();
         }
      }
      // cancel other tools:
      switch (m_mouse_mode) {
         case ZOOM_IN:
            ZoomOut();
            break;
         case JOIN | JOINB:
         case UNJOIN | JOINB:
            m_mouse_mode &= ~JOINB;
            SetCursor(m_mouse_mode);
            // drop through intentional
         case SELECT:
            if (pDoc->m_meta_graph->isSelected()) {
               pDoc->m_meta_graph->clearSel();
               // Redraw scene
               m_redraw_no_clear = true;
               Invalidate(FALSE);
               UpdateWindow();
            }
            break;
         case PENCIL:
            {
               GetDocument()->m_meta_graph->getDisplayedPointMap().fillPoint(LogicalUnits(point),false);
               m_redraw_all = true;
               Invalidate(FALSE);
               UpdateWindow();
            }
            break;
      }
   }
   else {
      m_right_mouse_drag = false;
      SetCursor(m_mouse_mode);

      // Stop drag rect...
      m_drag_rect_a.SetRectEmpty();
      m_invalidate = DRAG;
      Invalidate(FALSE);
      UpdateWindow();

       // Redraw scene
      m_redraw_all = true;
      Invalidate(FALSE);
      UpdateWindow();
   }
	
	CView::OnRButtonUp(nFlags, point);
}

void CDepthmapView::OnMouseMove(UINT nFlags, CPoint point) 
{
   ResetHoverWnd(point);

   if (nFlags & MK_CONTROL) { 
      if (nFlags & MK_SHIFT) {
         // CTRL and SHIFT key down together, snap to displayed drawing layers
         // (note, don't use SHIFT on it's own, because that's already used for multiple select)
         MetaGraph *graph = GetDocument()->m_meta_graph;
         Point2f p = LogicalUnits(point);
         double d = -1.0;
         for (int i = 0; i < graph->getLineFileCount(); i++) {
            for (int j = 0; j < graph->getLineLayerCount(i); j++) {
               ShapeMap& map = graph->getLineLayer(i,j);
               if (map.isShown()) {
                  Point2f px = map.getClosestVertex(p);
                  if (!px.isNull() && (d == -1 || dist(p,px) < d)) {
                     d = dist(p,px);
                     m_snap_point = px;
                  }
               }
            }
         }
         if (d != -1) {
            point = PhysicalUnits(m_snap_point);
            if (!m_snap) {
               m_invalidate = SNAPON;
               m_snap = true;
               Invalidate(FALSE);
               UpdateWindow();
            }
            else if (m_old_snap_point != m_snap_point) {
               m_invalidate = SNAP;
               m_snap = true;
               Invalidate(FALSE);
               UpdateWindow();
            }
         }
      }
      else {
         // If only CTRL key down, snap to grid
         if (GetDocument()->m_meta_graph->getViewClass() & (MetaGraph::VIEWVGA | MetaGraph::VIEWBACKVGA)) {
            PointMap& map = GetDocument()->m_meta_graph->getDisplayedPointMap();
            if (GetDocument()->m_meta_graph->getDisplayedPointMap().getSpacing() / m_unit > 20) {
               // hi-res snap when zoomed in
               m_snap_point = map.depixelate(map.pixelate(LogicalUnits(point),false,2),0.5);
            }
            else {
               m_snap_point = map.depixelate(map.pixelate(LogicalUnits(point)));
            }
            point = PhysicalUnits(m_snap_point);
            if (!m_snap) {
               m_invalidate = SNAPON;
               m_snap = true;
               Invalidate(FALSE);
               UpdateWindow();
            }
            else if (m_old_snap_point != m_snap_point) {
               m_invalidate = SNAP;
               m_snap = true;
               Invalidate(FALSE);
               UpdateWindow();
            }
         }
      }
   }
   else if (m_snap) {
      m_invalidate = SNAPOFF;
      Invalidate(FALSE);
      UpdateWindow();
      m_snap = false;
   }

   // Left button down...
   if (nFlags & MK_LBUTTON) {
      if (m_current_mode == DRAG) {

         point -= m_mouse_point;
         m_drag_rect_a.OffsetRect(point);
         m_centre.x -= double(point.x) * m_unit;
         m_centre.y += double(point.y) * m_unit;
         m_mouse_point += point;

         m_invalidate = DRAG;
         Invalidate(FALSE);
         UpdateWindow();
      }
      else if (m_current_mode == SELECT || m_current_mode == ZOOM_IN || m_current_mode == JOIN || m_current_mode == UNJOIN) {

         m_drag_rect_a = CRect( m_mouse_point, point + CPoint(-1,-1) ); // CPoint(-1,-1) is just to take it off the snap cursor if you have one
         m_drag_rect_a.NormalizeRect();

         m_invalidate = SELECT;
         Invalidate(FALSE);
         UpdateWindow();

      }
      // NB. deliberatle mouse mode not current mode
      else if (m_mouse_mode == (SELECT | OVERHANDLE)) {
         // ideally, I would enforce click once, move to location, and click again, but this
         // at least allows novices to use it easily:
         if (abs(point.x-m_mouse_point.x)+abs(point.y-m_mouse_point.y)>3) {
            BeginDrag(m_mouse_point);
            m_current_mode = SELECT | DRAWLINE;
         }
      }
      // NB. deliberatle mouse mode not current mode
      else if (m_mouse_mode == LINETOOL) {
         // ideally, I would enforce click once, move to location, and click again, but this
         // at least allows novices to use it easily:
         if (PixelDist(point,m_mouse_point) > 6) {
            m_mouse_mode |= DRAWLINE;
            m_current_mode = m_mouse_mode;
            m_line = Line(m_mouse_location, LogicalUnits(point));
            m_invalidate = LINEON;
            Invalidate(FALSE);
            UpdateWindow();
         }
      }
      else if (m_current_mode == PENCIL) {
         if (m_mouse_point != point &&
               GetDocument()->m_meta_graph->getDisplayedPointMap().pixelate(LogicalUnits(point)) !=
               GetDocument()->m_meta_graph->getDisplayedPointMap().pixelate(LogicalUnits(m_mouse_point))) {
            GetDocument()->m_meta_graph->getDisplayedPointMap().fillPoint(LogicalUnits(point),true);
            m_redraw_no_clear = true;
            m_mouse_point = point;
            // Redraw scene
            Invalidate(FALSE);
            UpdateWindow();
         }
      }
      else if (m_current_mode == ERASE) {
         if (m_mouse_point != point &&
               GetDocument()->m_meta_graph->getDisplayedPointMap().pixelate(LogicalUnits(point)) !=
               GetDocument()->m_meta_graph->getDisplayedPointMap().pixelate(LogicalUnits(m_mouse_point))) {
            GetDocument()->m_meta_graph->getDisplayedPointMap().fillPoint(LogicalUnits(point),false);
            m_redraw_no_clear = true;
            m_mouse_point = point;
            // Redraw scene
            Invalidate(FALSE);
            UpdateWindow();
         }
      }
   }
   else if (nFlags & MK_RBUTTON) {
      if (!m_right_mouse_drag && abs(point.x-m_mouse_point.x)+abs(point.y-m_mouse_point.y)>3) {
         // begin right mouse drag:
         SetCursor(DRAG);
         m_mouse_point = point;

         GetClientRect(m_drag_rect_a);
         m_drag_rect_b.SetRectEmpty();
         m_invalidate = DRAG;
         Invalidate(FALSE);
         UpdateWindow();

         SetCapture();
         //
         m_right_mouse_drag = true;
      }
      else if (m_right_mouse_drag) {
         // continue right mouse drag
         point -= m_mouse_point;
         m_drag_rect_a.OffsetRect(point);
         m_centre.x -= double(point.x) * m_unit;
         m_centre.y += double(point.y) * m_unit;
         m_mouse_point += point;
         m_invalidate = DRAG;
         Invalidate(FALSE);
         UpdateWindow();
      }
   }
   else if ((m_mouse_mode & SELECT) && !(m_mouse_mode & DRAWLINE) && !m_right_mouse_drag) {
      // in select mode, might be over a point handle -- if so, change the cursor:
      if (m_point_handles.size()) {
         bool found = false;
         for (size_t i = 0; i < m_point_handles.size(); i++) {
            CPoint pt = PhysicalUnits(m_point_handles[i]);
            if (abs(pt.x-point.x) < 7 && abs(pt.y-point.y) < 7) {
               found = true;
               m_mouse_mode |= OVERHANDLE;
               SetCursor(m_mouse_mode);
               break;
            }
         }
         if (!found && (m_mouse_mode & OVERHANDLE)) {
            m_mouse_mode &= ~OVERHANDLE;
            SetCursor(m_mouse_mode);
         }
      }
      else if (m_mouse_mode & OVERHANDLE) {
         m_mouse_mode &= ~OVERHANDLE;
         SetCursor(m_mouse_mode);
      }
   }

   if (!m_right_mouse_drag) {
      if (m_mouse_mode & DRAWLINE) {
         m_line = Line(m_line.t_start(), m_snap ? m_snap_point : LogicalUnits(point));
         if (m_line.t_end() != m_old_line.t_end()) {
            m_invalidate = DRAWLINE;
            Invalidate(FALSE);
            UpdateWindow();
         }
      }
      else if (m_mouse_mode & JOINB) {
         point -= m_mouse_point;
         m_drag_rect_a.OffsetRect(point);
         m_mouse_point += point;
         // Redraw scene
         m_invalidate = JOINB;
         Invalidate(FALSE);
         UpdateWindow();
      }
   }

   GetDocument()->m_position = m_snap ? m_snap_point : LogicalUnits( point );

	CView::OnMouseMove(nFlags, point);
}

void CDepthmapView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();

   // set the remenu flag up:   
   GetDocument()->SetRemenuFlag(CGraphDoc::VIEW_MAP,true);

   // register with GraphDoc:
   GetDocument()->m_view[CGraphDoc::VIEW_MAP] = this;
}

void CDepthmapView::OnDestroy() 
{
   GetDocument()->m_view[CGraphDoc::VIEW_MAP] = NULL;

   KillTimer(DMP_TIMER_REDRAW);
   /*
   if (GetApp()->m_redraw == this) {
      GetApp()->m_redraw = NULL;
   }
   */
	CView::OnDestroy();
}

void CDepthmapView::OnSetFocus(CWnd* pOldWnd) 
{
	CView::OnSetFocus(pOldWnd);
	
   GetApp()->GetMainWnd()->PostMessage(WM_DMP_FOCUS_GRAPH, (WPARAM) GetDocument(), CGraphDoc::CONTROLS_LOADALL);
   GetDocument()->SetRedrawFlag(CGraphDoc::VIEW_MAP,CGraphDoc::REDRAW_GRAPH);

   ResetHoverWnd();
}

void CDepthmapView::OnKillFocus(CWnd* pNewWnd) 
{
	CView::OnKillFocus(pNewWnd);
	
   // end any line drawing tool
   m_mouse_mode &= ~DRAWLINE;

   // note, this doesn't actually delete the polyline drawn up until now,
   // just resets so it'll start a new shape
   m_poly_points = 0;

   // this destroys the hover window
   ResetHoverWnd();
   // and stops the timer
   KillTimer(DMP_TIMER_HOVER);

   // set the cursor back?
}

/////////////////////////////////////////////////////////////////////////////

void CDepthmapView::OnEditCopy() 
{
   if (GetDocument()->m_communicator) {
      AfxMessageBox(_T("Another Depthmap process is running, please wait until it completes"));
      return;
   }

   // Copy to Clipboard

   if (OpenClipboard() && EmptyClipboard()) {

      CMetaFileDC *pDC = new CMetaFileDC;

      CRect rect;
      GetClientRect(rect);
      CDC *dummyDC = new CDC;
      dummyDC->CreateCompatibleDC(GetDC());
      int oldmm = dummyDC->SetMapMode(MM_HIMETRIC);
      CSize size(rect.Width(),rect.Height());
      dummyDC->DPtoLP(&size);
      rect.SetRect(0,0,size.cx,size.cy);
      dummyDC->SetMapMode(oldmm);
      pDC->CreateEnhanced(dummyDC,NULL,rect,_T("Depthmap\0Graph\0\0"));
      pDC->SetAttribDC(dummyDC->m_hAttribDC);
      PrintBaby(pDC,GetDocument());
      dummyDC->DeleteDC();
      delete dummyDC;

      HENHMETAFILE hMF = pDC->CloseEnhanced();
      
      HANDLE h = SetClipboardData(CF_ENHMETAFILE, hMF);

      CloseClipboard();

      pDC->DeleteDC();
      delete pDC;
   }
}

void CDepthmapView::OnEditSave() 
{
   // Very similar to copy to clipboard, only writes an EPS instead of a WMF
   CGraphDoc *pDoc = GetDocument();

   if (pDoc->m_communicator) {
      AfxMessageBox(_T("Another Depthmap process is running, please wait until it completes"));
      return;
   }

   CString saveas;
   CFilePath path(pDoc->GetPathName());
   saveas = path.m_path + (path.m_name.IsEmpty() ? pDoc->GetTitle() : path.m_name);

   CFileDialog saveDialog(FALSE,_T("eps"),saveas,
						  OFN_EXPLORER|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
						  _T("Encapsulated Postscript (*.eps)|*.eps|Scalable Vector Graphics (*.svg)|*.svg|All files (*.*)|*.*||"), this);
   saveDialog.m_ofn.lpstrTitle = _T("Save Screen As");

   if (saveDialog.DoModal() == IDOK) {

      ofstream stream( saveDialog.GetPathName() );
      if (stream.fail()) {
         AfxMessageBox( CString("Sorry, unable to open ") + saveDialog.GetPathName() + CString(" for writing") );
         return;
      }

      CFilePath newpath(saveDialog.GetPathName());

      CString ext = newpath.m_ext.MakeLower();
      if (ext == "svg") {
         OutputSVG( stream, pDoc );
      }
      else {
         // Set up complete... run the .eps outputter (copied from standard output!)
         OutputEPS( stream, pDoc );
      }

      stream.close();
   }
}

void CDepthmapView::OnViewCentreView() 
{
   GetDocument()->SetRedrawFlag(CGraphDoc::VIEW_MAP, CGraphDoc::REDRAW_TOTAL, CGraphDoc::NEW_DEPTHMAPVIEW_SETUP, this);
}

BOOL CDepthmapView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
   if (zDelta < 0) {
      ZoomOut();
   }
   else {
      CPoint pta = pt;
      ScreenToClient(&pta);
      ZoomIn(1.0 + double(zDelta) / double(WHEEL_DELTA), LogicalUnits(pta));
   }
	
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

// 11-Apr-2010 I have removed this button from the toolbar, as it 
// is a cack-handed way to find things.  Instead, I've included a 
// zoom to selection (see below)
void CDepthmapView::OnViewFindloc()
{
   CFindLocDlg dlg;
	CGraphDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

   if (pDoc->m_meta_graph) {
      dlg.m_bounds = pDoc->m_meta_graph->getBoundingBox();
      if (dlg.m_bounds.isNull()) {
         AfxMessageBox(_T("Depthmap cannot find a location within an empty map"));
      }
      else {
         if (m_lastfindloc != Point2f()) {
            dlg.m_x = m_lastfindloc.x;
            dlg.m_y = m_lastfindloc.y;
         }
         if (dlg.DoModal() == IDOK) {
            m_centre = Point2f(dlg.m_x,dlg.m_y);
            // Redraw scene
            m_redraw_all = true;
            Invalidate(FALSE);
            UpdateWindow();            
         }
      }
   }
}

// Zoom to Selection
void CDepthmapView::OnViewZoomsel()
{
   CFindLocDlg dlg;
	CGraphDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

   if (pDoc->m_meta_graph && pDoc->m_meta_graph->isSelected()) {
      Region sel_bounds = pDoc->m_meta_graph->getSelBounds();
      // select a suitable zoom factor based on bounding box dimensions:
      m_centre = sel_bounds.getCentre();
      CRect phys_bounds;
      GetClientRect( phys_bounds );
      if (sel_bounds.area() > 1e-9) {
         // base area on selection area
         m_unit =  1.1 * __max( sel_bounds.width() / double(phys_bounds.Width()), 
                               sel_bounds.height() / double(phys_bounds.Height()) );
      }
      else {
         // base area on some arbitrary zoom into the map
         Region map_bounds = pDoc->m_meta_graph->getBoundingBox();
         m_unit = 0.01 * __max( map_bounds.width() / double(phys_bounds.Width()), 
                                map_bounds.height() / double(phys_bounds.Height()) );
      }
      // Redraw scene
      m_redraw_all = true;
      Invalidate(FALSE);
      UpdateWindow();            
   }
}

void CDepthmapView::OnUpdateViewZoomsel(CCmdUI *pCmdUI)
{
	CGraphDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

   if (pDoc->m_meta_graph && pDoc->m_meta_graph->isSelected()) {
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE);
   }
}

