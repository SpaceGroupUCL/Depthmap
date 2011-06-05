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

// PlotView.cpp : implementation file
//

#include "stdafx.h"
#include "depthmap.h"

#include "GraphDoc.h"
#include "HoverWnd.h"
#include "PlotView.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PlotView

IMPLEMENT_DYNCREATE(CPlotView, CView)

CPlotView::CPlotView()
{
   m_x_axis = -1;
   m_y_axis = -1;

   m_queued_redraw = false;
   m_view_trend_line = false;
   m_view_rsquared = false;
   m_view_monochrome = false;
   m_view_origin = false;
   m_view_equation = false;

   // for drawing drag rect
   m_drawdragrect = false;
   m_selecting = false;

   // hover window
   m_hover_tick = 0;
   m_hover_wnd = NULL;
}

CPlotView::~CPlotView()
{
}

BEGIN_MESSAGE_MAP(CPlotView, CView)
	//{{AFX_MSG_MAP(CPlotView)
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_TIMER()
	ON_COMMAND(ID_VIEW_TREND_LINE, OnViewTrendLine)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TREND_LINE, OnUpdateViewTrendLine)
	ON_COMMAND(ID_VIEW_RSQUARED, OnViewRsquared)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RSQUARED, OnUpdateViewRsquared)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_VIEW_COLOR, OnViewColor)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR, OnUpdateViewColor)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_VIEW_ORIGIN, OnViewOrigin)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ORIGIN, OnUpdateViewOrigin)
	ON_COMMAND(ID_VIEW_EQUATION, OnViewEquation)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EQUATION, OnUpdateViewEquation)
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
   //
   ON_MESSAGE(WM_DMP_REDRAW_MESSAGE, OnRedraw)
   ON_WM_RBUTTONUP()
   ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlotView drawing

int CPlotView::screenX(double x)
{
   return m_screen_bounds.left + m_screen_bounds.Width() * (m_data_bounds.width() ? (x - m_data_bounds.bottom_left.x) / m_data_bounds.width() : 0.5);
}

int CPlotView::screenY(double y)
{
   return m_screen_bounds.top - (abs(m_screen_bounds.Height()) * (m_data_bounds.height() ? (y - m_data_bounds.bottom_left.y) / m_data_bounds.height(): 0.5));
}

double CPlotView::dataX(int x)
{
   return m_data_bounds.bottom_left.x + m_data_bounds.width() * double(x - m_screen_bounds.left) / double(m_screen_bounds.Width());
}

double CPlotView::dataY(int y)
{
   return m_data_bounds.bottom_left.y + m_data_bounds.height() * double(m_screen_bounds.top - y) / double(abs(m_screen_bounds.Height()));
}

void CPlotView::OnDraw(CDC* pDC)
{
	CGraphDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

   CRect rect;
   GetClientRect(rect);

   if (pDC->IsPrinting()) {

      PrintOutput(pDC, pDoc);

   }
   else {   // if (m_clear)

      if (m_drawdragrect) {
         
         pDC->DrawDragRect( m_drag_rect_a, CSize(1,1), m_drag_rect_b, CSize(1,1) );
         m_drag_rect_b = m_drag_rect_a;
         m_drawdragrect = false;

      }
      else {

         CBrush brush( GetApp()->m_background );
         CGdiObject *oldbrush = pDC->SelectObject( &brush );
         pDC->PatBlt(rect.left,rect.top,rect.Width(),rect.Height(),PATCOPY);
         pDC->SelectObject( oldbrush );
         // m_clear = false;

         Output(pDC, pDoc, true);
      }
   }
}

void CPlotView::OnEditCopy() 
{
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
      PrintOutput(pDC,GetDocument());
      dummyDC->DeleteDC();
      delete dummyDC;

      HENHMETAFILE hMF = pDC->CloseEnhanced();
      
      HANDLE h = SetClipboardData(CF_ENHMETAFILE, hMF);

      CloseClipboard();

      pDC->DeleteDC();
      delete pDC;
   }
}

void CPlotView::PrintOutput(CDC *pDC, CGraphDoc *pDoc)
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

   CGdiObject *oldpen   = pDC->SelectStockObject( NULL_PEN );
   CBrush brush( GetApp()->m_background );
   CGdiObject *oldbrush = pDC->SelectObject( &brush );

   pDC->Rectangle( rectin );

   pDC->SelectObject( oldbrush );
   pDC->SelectObject( oldpen );

   if (!Output( pDC, pDoc, false )) {
      AfxMessageBox(_T("Unable to print / copy screen at this time"));
      pDC->AbortDoc();
      return;
   }
}

bool CPlotView::Output(CDC *pDC, CGraphDoc *pDoc, bool screendraw) 
{
   // this is going to need a timer at somepoint, but for now, it's all very easy to start off:
   if (!pDoc->m_meta_graph->setLock(this)) {
      return false;
   }

   if (pDoc->m_communicator || !pDoc->m_meta_graph->viewingProcessed()) {
      pDoc->m_meta_graph->releaseLock(this);
      return false;
   }

   AttributeTable& table = pDoc->m_meta_graph->getAttributeTable();
   int displaycol = pDoc->m_meta_graph->getDisplayedAttribute();

   CRect rect;
   GetClientRect(rect);
   int mindim = __min(rect.Width(),rect.Height());
   int viscount = table.getVisibleRowCount();
   int spacer = floor(2.0 * sqrt(double(mindim)/double(viscount != 0 ? viscount : 1)));
   if (!screendraw && spacer < 4) {
      spacer = 4;
   }

   // text formatting
   int pointsize;
   if (mindim > 200) {
      pointsize = 100;
   }
   else {
      pointsize = 50+mindim/4;
   }
   CFont font;
   font.CreatePointFont(pointsize,_T("Arial"),pDC);
   CFont vfont;
   LOGFONT logfont;
   font.GetLogFont(&logfont);
   logfont.lfEscapement = 900;
   logfont.lfOrientation = 900;
   vfont.CreateFontIndirect(&logfont);

   CGdiObject *oldfont = pDC->SelectObject( &font );
   COLORREF oldforeground = pDC->SetTextColor(GetApp()->m_foreground);
   COLORREF oldbackground = pDC->SetBkColor(GetApp()->m_background);

   CPen pen( PS_SOLID, 1, GetApp()->m_foreground );
   CPen *oldpen = pDC->SelectObject( &pen );

   float minx, miny, maxx, maxy;
   CString str_minx, str_miny, str_maxx, str_maxy; 

   minx = m_data_bounds.bottom_left.x = m_view_origin ? 0.0 : table.getVisibleMinValue(m_x_axis);
   miny = m_data_bounds.bottom_left.y = m_view_origin ? 0.0 : table.getVisibleMinValue(m_y_axis);
   maxx = m_data_bounds.top_right.x = table.getVisibleMaxValue(m_x_axis);
   maxy = m_data_bounds.top_right.y = table.getVisibleMaxValue(m_y_axis);

   str_minx.Format(_T("%g"), minx);
   str_miny.Format(_T("%g"), miny);
   str_maxx.Format(_T("%g"), maxx);
   str_maxy.Format(_T("%g"), maxy);

   // now work out the drawing window for 
   CSize sizex = pDC->GetTextExtent(str_maxx);
   CSize sizey = pDC->GetTextExtent(str_miny);
   CSize sizey2 = pDC->GetTextExtent(str_maxy);
   if (sizey2.cx > sizey.cx) {
      sizey.cx = sizey2.cx;
   }
   // doesn't matter which, just want a height:
   int texth = sizex.cy;

   int xaxis_pos = 99*rect.Height()/100 - 2 * texth;
   int yaxis_pos = rect.Width()/100 + sizey.cx + texth;
   int miny_pos = m_screen_bounds.top = xaxis_pos - texth;
   int maxy_pos = m_screen_bounds.bottom = rect.Height()/100 + texth / 2;
   int minx_pos = m_screen_bounds.left = yaxis_pos + texth;
   int maxx_pos = m_screen_bounds.right = 99*rect.Width()/100 - sizex.cx / 2;

   int width = maxx_pos -minx_pos;
   if (minx == maxx) {
      minx_pos = maxx_pos = m_screen_bounds.left = m_screen_bounds.right = minx_pos + width / 2;
      width = 0;
   }
   int height = maxy_pos - miny_pos;
   if (miny == maxy) {
      miny_pos = maxy_pos = m_screen_bounds.bottom = m_screen_bounds.top = miny_pos + height / 2;
      height = 0;
   }

   pDC->SetTextAlign(TA_CENTER);
   pDC->TextOut(minx_pos, 99*rect.Height()/100 - texth, str_minx);
   if (minx != maxx) {
      pDC->TextOut(maxx_pos, 99*rect.Height()/100 - texth, str_maxx);
   }
   
   pDC->SetTextAlign(TA_RIGHT);
   pDC->TextOut(yaxis_pos-texth, miny_pos - texth/2, str_miny);
   if (miny != maxy) {
      pDC->TextOut(yaxis_pos-texth, maxy_pos - texth/2, str_maxy);
   }

   pDC->MoveTo(yaxis_pos-texth/2,miny_pos);
   pDC->LineTo(yaxis_pos,miny_pos);
   if (miny != maxy) {
      pDC->LineTo(yaxis_pos,maxy_pos);
      pDC->LineTo(yaxis_pos-texth/2,maxy_pos);
   }

   pDC->MoveTo(minx_pos,xaxis_pos+texth/2);
   pDC->LineTo(minx_pos,xaxis_pos);
   if (minx != maxx) {
      pDC->LineTo(maxx_pos,xaxis_pos);
      pDC->LineTo(maxx_pos,xaxis_pos+texth/2);
   }

   int sel_parity = 0;
   CPen *pen2 = NULL;
   for (int i = 0; i < table.getRowCount(); i++) {
      if (!table.isVisible(i)) {
         continue;
      }
      float x = table.getValue(i,m_x_axis);
      float y = table.getValue(i,m_y_axis);
      if (!_finite(x) || !_finite(y) || x == -1.0f || y == -1.0f) {
         continue;
      }
      COLORREF rgb;
      if (m_view_monochrome) {
         rgb = GetApp()->m_foreground;
      }
      else {
         PafColor color = table.getDisplayColor(i);
         rgb = RGB(color.redb(),color.greenb(),color.blueb());
      }
      int tempspacer = spacer;
      if (table.isSelected(i)) {
         tempspacer = (spacer + 1) * 2 - 1;
         if (m_view_monochrome) {
            rgb = RGB(0xff,0,0);
         }
      }
      if (tempspacer == 0) {
         tempspacer = 1;
      }
      if (!m_view_monochrome) {
         if (pen2 != NULL) {
            delete pen2;
         }
         pen2 = new CPen();
         pen2->CreatePen(PS_SOLID, spacer, rgb);
      }
      else if (sel_parity != (table.isSelected(i) ? 1 : -1)) {
         if (pen2 != NULL) {
            delete pen2;
         }
         pen2 = new CPen();
         pen2->CreatePen(PS_SOLID, spacer, rgb);
         sel_parity = (table.isSelected(i) ? 1 : -1);
      }
      if (pen2 != NULL) {
         pDC->SelectObject(pen2);
      }
      //
      CPoint point(screenX(x),screenY(y));
      pDC->MoveTo(point.x-tempspacer,point.y-tempspacer);
      pDC->LineTo(point.x+tempspacer+1,point.y+tempspacer+1);
      pDC->MoveTo(point.x-tempspacer,point.y+tempspacer);
      pDC->LineTo(point.x+tempspacer+1,point.y-tempspacer-1);
      pDC->SelectObject(&pen);
/*
      if (tempspacer < 1) {
         pDC->SetPixel(point, rgb);
      }
      else if (tempspacer < 4) {
         pDC->SetPixel(point, rgb);
         pDC->SetPixel(point.x-1, point.y, rgb);
         pDC->SetPixel(point.x+1, point.y, rgb);
         pDC->SetPixel(point.x, point.y-1, rgb);
         pDC->SetPixel(point.x, point.y+1, rgb);
      }
      else  if (tempspacer < 6) {
         // Temporary trial
         //CBrush colorBrush( rgb );
         //pDC->SelectObject( &colorBrush );
         //pDC->Rectangle(point.x-tempspacer/2,point.y-tempspacer/2,point.x+tempspacer/2,point.y+tempspacer/2);
         // Standard code
         pDC->FillSolidRect(point.x-tempspacer/2+1,point.y-tempspacer/2+1,tempspacer-1,tempspacer-1,rgb);
      }
      else {
         CBrush colorBrush( rgb );
         pDC->SelectObject( &colorBrush );
         pDC->Ellipse(point.x-tempspacer/2,point.y-tempspacer/2,point.x+tempspacer/2,point.y+tempspacer/2);
      }*/
   }
   if (pen2) {
      delete pen2;
   }

   //pDC->SelectObject( oldbrush );
   //if (spacer < 6) {
    //pDC->SelectObject(&pen);
    // don't know why, but text background color can also be lost:
    pDC->SetBkColor(GetApp()->m_background);

   // trend line if reqd
   if (m_view_trend_line) {
      CPoint bl, tr;
      CString string;
      if (m_regression.model(m_data_bounds.bottom_left.x) < m_data_bounds.bottom_left.y) {
         // check line is on page
         if (m_regression.model(m_data_bounds.top_right.x) > m_data_bounds.bottom_left.y) {
            bl = CPoint(screenX(m_regression.invmodel(m_data_bounds.bottom_left.y)),m_screen_bounds.top);
            if (m_regression.model(m_data_bounds.top_right.x) < m_data_bounds.top_right.y) {
               tr = CPoint(m_screen_bounds.right, screenY(m_regression.model(m_data_bounds.top_right.x)));
            }
            else {
               tr = CPoint(screenX(m_regression.invmodel(m_data_bounds.top_right.y)),m_screen_bounds.bottom);
            }
            pDC->MoveTo(bl);
            pDC->LineTo(tr);
         }
      }
      else if (m_regression.model(m_data_bounds.bottom_left.x) > m_data_bounds.top_right.y) {
         // check line is on page
         if (m_regression.model(m_data_bounds.top_right.x) < m_data_bounds.top_right.y) {
            bl = CPoint(screenX(m_regression.invmodel(m_data_bounds.bottom_left.x)),m_screen_bounds.bottom);
            if (m_regression.model(m_data_bounds.top_right.x) > m_data_bounds.bottom_left.x) {
               tr = CPoint(m_screen_bounds.right, screenY(m_regression.model(m_data_bounds.top_right.x)));
            }
            else {
               tr = CPoint(screenX(m_regression.invmodel(m_data_bounds.top_right.y)),m_screen_bounds.top);
            }
            pDC->MoveTo(bl);
            pDC->LineTo(tr);
         }
      }
      else {
         bl = CPoint(m_screen_bounds.left,screenY(m_regression.model(m_data_bounds.bottom_left.x)));
         double trv = m_regression.model(m_data_bounds.top_right.x);
         if (trv >= m_data_bounds.bottom_left.y && trv <= m_data_bounds.top_right.y) {
            string += " v1";
            tr = CPoint(m_screen_bounds.right,screenY(trv));
         }
         else if (m_regression.b() > 0) {   // upward inclined
            string += " v2";
            tr = CPoint(screenX(m_regression.invmodel(m_data_bounds.top_right.y)),m_screen_bounds.bottom);
         }
         else { // downward inclined
            string += " v3";
            tr = CPoint(screenX(m_regression.invmodel(m_data_bounds.bottom_left.y)),m_screen_bounds.top);
         }
         pDC->MoveTo(bl);
         pDC->LineTo(tr);
      }
   }

   CString string;
   int textpos = rect.Height()/100;
   if (m_view_rsquared) {
      // set text formating
      // hope ascii superscript 2 in place!
      string.Format(_T("R\xb2 = %g"), sqr(m_regression.r()));
      pDC->SetTextAlign(TA_RIGHT);
      pDC->TextOut(99*rect.Width()/100, textpos, string);
      textpos += texth;
   }
   if (m_view_equation) {
      if (m_regression.a() >= 0) {
         string.Format(_T("y = %g x + %g"), m_regression.b(), m_regression.a());
      }
      else {
         string.Format(_T("y = %g x - %g"), m_regression.b(), fabs(m_regression.a()));
      }
      pDC->SetTextAlign(TA_RIGHT);
      pDC->TextOut(99*rect.Width()/100, textpos, string);
   }

   pDC->SetTextAlign(TA_CENTER);
   CString xlabel(table.getColumnName(m_x_axis).c_str());
   pDC->TextOut(width/2+minx_pos,99*rect.Height()/100-texth,xlabel);

   // this is last to avoid switch between hfont and vfont
   pDC->SelectObject( &vfont );

   CString ylabel(table.getColumnName(m_y_axis).c_str());
   pDC->TextOut(rect.Width()/100,height/2+miny_pos,ylabel);

   // reset text formatting
   pDC->SetTextColor( oldforeground );
   pDC->SetBkColor( oldbackground );
   pDC->SelectObject( oldfont );
   pDC->SelectObject( oldpen );

   pDoc->m_meta_graph->releaseLock(this);

   return true;
}

/////////////////////////////////////////////////////////////////////////////
// CPlotView printing

void CPlotView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	CView::OnBeginPrinting(pDC, pInfo);
}

void CPlotView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	CView::OnEndPrinting(pDC, pInfo);
}

BOOL CPlotView::OnPreparePrinting(CPrintInfo* pInfo) 
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CPlotView diagnostics

#ifdef _DEBUG
void CPlotView::AssertValid() const
{
	CView::AssertValid();
}

void CPlotView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGraphDoc* CPlotView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGraphDoc)));
	return (CGraphDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPlotView message handlers

BOOL CPlotView::PreCreateWindow(CREATESTRUCT& cs) 
{
   // Byte align for faster redraw
   cs.lpszClass = AfxRegisterWndClass(CS_BYTEALIGNCLIENT); // |CS_DBLCLKS); No double clicks required (I don't think)
	
	return CView::PreCreateWindow(cs);
}

LRESULT CPlotView::OnRedraw(WPARAM wParam, LPARAM lParam)
{
   CGraphDoc *pDoc = GetDocument();

   if (pDoc->GetRemenuFlag(CGraphDoc::VIEW_SCATTER)) {
      pDoc->SetRemenuFlag(CGraphDoc::VIEW_SCATTER,false);
      // redo the menus:
      // really ought to do this from the doc directly... bit silly setting off all onredraw
      ((CChildFrame*) GetParentFrame())->m_view_selector_sub_x.RedoMenu( *pDoc->m_meta_graph, false );
      ((CChildFrame*) GetParentFrame())->m_view_selector_sub_y.RedoMenu( *pDoc->m_meta_graph, true );
   }
   if (pDoc->GetRedrawFlag(CGraphDoc::VIEW_SCATTER) != CGraphDoc::REDRAW_DONE) {

      if (!pDoc->m_communicator) {

         m_queued_redraw = false;

         while (!pDoc->SetRedrawFlag(CGraphDoc::VIEW_SCATTER,CGraphDoc::REDRAW_DONE)) {
            // prefer waitformultipleobjects here
            Sleep(1);
         }
         
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

void CPlotView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();

   GetDocument()->SetRemenuFlag(CGraphDoc::VIEW_SCATTER,true);

   // register with GraphDoc:
   GetDocument()->m_view[CGraphDoc::VIEW_SCATTER] = this;
}

void CPlotView::RedoIndices()
{
   if (GetDocument()->m_meta_graph && GetDocument()->m_meta_graph->viewingProcessed()) {
      AttributeTable& table = GetDocument()->m_meta_graph->getAttributeTable();
      idx_x.makeIndex(table, m_x_axis, false);
      idx_y.makeIndex(table, m_y_axis, false);
   }
}

void CPlotView::OnDestroy() 
{
   // deregister with GraphDoc
   GetDocument()->m_view[CGraphDoc::VIEW_SCATTER] = NULL;

	CView::OnDestroy();
}

void CPlotView::OnSetFocus(CWnd* pOldWnd) 
{
	CView::OnSetFocus(pOldWnd);
	
   GetApp()->GetMainWnd()->PostMessage(WM_DMP_FOCUS_GRAPH, (WPARAM) GetDocument(), CGraphDoc::CONTROLS_LOADALL);
   GetDocument()->SetRedrawFlag(CGraphDoc::VIEW_SCATTER,CGraphDoc::REDRAW_GRAPH);

   ResetHoverWnd();
}

void CPlotView::OnTimer(UINT_PTR nIDEvent) 
{
   if (nIDEvent == DMP_TIMER_REDRAW) {

      if (m_queued_redraw) {

         // Internal own redraw
         OnRedraw(0,0);

      }
   }
   else if (nIDEvent == DMP_TIMER_HOVER) {
      m_hover_tick++;
      // on the sixth tick, create a hover window (if there's a selection)
      if (m_hover_tick == 6) {
         CreateHoverWnd();
      }
   }

	CView::OnTimer(nIDEvent);
}

void CPlotView::OnViewTrendLine() 
{
   if (m_view_trend_line) {
      m_view_trend_line = false;
   }	
   else {
      m_view_trend_line = true;
   }
   Invalidate(FALSE);
   UpdateWindow();
}

void CPlotView::OnUpdateViewTrendLine(CCmdUI* pCmdUI) 
{
   if (m_view_trend_line) {
      pCmdUI->SetCheck(1);
   }
   else {
      pCmdUI->SetCheck(0);
   }
}

void CPlotView::OnViewRsquared() 
{
   if (m_view_rsquared) {
      m_view_rsquared = false;
   }
   else {
      m_view_rsquared = true;
   }
   Invalidate(FALSE);
   UpdateWindow();
}

void CPlotView::OnUpdateViewRsquared(CCmdUI* pCmdUI) 
{
   if (m_view_rsquared) {
      pCmdUI->SetCheck(1);
   }
   else {
      pCmdUI->SetCheck(0);
   }
}

void CPlotView::OnViewColor() 
{
   if (m_view_monochrome) {
      m_view_monochrome = false;
   }
   else {
      m_view_monochrome = true;
   }
   Invalidate(FALSE);
   UpdateWindow();
}

void CPlotView::OnUpdateViewColor(CCmdUI* pCmdUI) 
{
   if (m_view_monochrome) {
      pCmdUI->SetCheck(1);
   }
   else {
      pCmdUI->SetCheck(0);
   }
}


void CPlotView::SetAxis(int axis, int col, bool reset)
{
   if (axis == 0) {
      if (m_x_axis != col) {
         m_x_axis = col;
      }
   }
   else {
      if (m_y_axis != col) {
         m_y_axis = col;
      }
   }
   if (reset) {
      RedoIndices();
      ResetRegression();
   }
}
/*
void CPlotView::SetYAxis(int col)
{
}

void CPlotView::ResetAxes(int xcol, int ycol)
{
   m_x_axis = xcol;
   m_y_axis = ycol;
   ResetRegression();
}
*/
void CPlotView::ResetRegression()
{
   m_regression.clear();
   if (GetDocument()->m_meta_graph && GetDocument()->m_meta_graph->viewingProcessed()) {
      AttributeTable& table = GetDocument()->m_meta_graph->getAttributeTable();
      for (int i = 0; i < table.getRowCount(); i++) {
         if (table.isVisible(i)) {
            float x = table.getValue(i,m_x_axis);
            float y = table.getValue(i,m_y_axis);
            if (_finite(x) && _finite(y) && x != -1.0f && y != -1.0f) {
               m_regression.add(x,y);
            }
         }
      }
   }
}

void CPlotView::OnLButtonDown(UINT nFlags, CPoint point) 
{
   m_mouse_point = point;
   m_drag_rect_a.SetRectEmpty();
   m_drag_rect_b.SetRectEmpty();
   m_selecting = true;
   m_drawdragrect = true;
   Invalidate(FALSE);
   UpdateWindow();

   SetCapture();
	
	CView::OnLButtonDown(nFlags, point);
}

void CPlotView::OnMouseMove(UINT nFlags, CPoint point) 
{
   ResetHoverWnd(point);

   if (m_selecting) {
      m_drag_rect_a = CRect( m_mouse_point, point );
      m_drag_rect_a.NormalizeRect();

      m_drawdragrect = true;
      Invalidate(FALSE);
      UpdateWindow();
   }

   CView::OnMouseMove(nFlags, point);
}

void CPlotView::OnLButtonUp(UINT nFlags, CPoint point) 
{
   ReleaseCapture();

   m_selecting = false;

   size_t xfloor = idx_x.searchfloorindex(ValuePair(-1,dataX(m_drag_rect_a.left-2)));
   size_t xceil = idx_x.searchceilindex(ValuePair(-1,dataX(m_drag_rect_a.right+2)));
   size_t yfloor = idx_y.searchfloorindex(ValuePair(-1,dataY(m_drag_rect_a.bottom+2)));
   size_t yceil = idx_y.searchceilindex(ValuePair(-1,dataY(m_drag_rect_a.top-2)));

   // Stop drag rect...
   m_drag_rect_a.SetRectEmpty();
   m_drawdragrect = true;
   Invalidate(FALSE);
   UpdateWindow();

   CGraphDoc *pDoc = GetDocument();
   AttributeTable& table = pDoc->m_meta_graph->getAttributeTable();

   // work out selection
   pvecint xkeys;
   for (size_t i = xfloor + 1; i < xceil; i++) {
      int index = idx_x[i].index;
      xkeys.add(index);
   }
   pvecint finalkeys;
   for (size_t j = yfloor + 1; j < yceil; j++) {
      int index = idx_y[j].index;
      if (xkeys.searchindex(index) != paftl::npos) {
         finalkeys.push_back( table.getRowKey(index) );
      }
   }
   
   // redraw selection set
   bool add = false;
   if (nFlags & MK_SHIFT) {
      add = true;
   }
   pDoc->m_meta_graph->setSelSet(finalkeys,add);
   pDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL,CGraphDoc::REDRAW_POINTS, CGraphDoc::NEW_SELECTION, this);

	CView::OnLButtonUp(nFlags, point);
}


void CPlotView::OnRButtonUp(UINT nFlags, CPoint point)
{
   // may well be used for drag later, but now, just clear any selection:
   CGraphDoc *pDoc = GetDocument();
   pDoc->m_meta_graph->clearSel();
   pDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL,CGraphDoc::REDRAW_POINTS, CGraphDoc::NEW_SELECTION, this);

   CView::OnRButtonUp(nFlags, point);
}

void CPlotView::OnViewOrigin() 
{
   m_view_origin = !m_view_origin;
   Invalidate(FALSE);
   UpdateWindow();
}

void CPlotView::OnUpdateViewOrigin(CCmdUI* pCmdUI) 
{
   if (m_view_origin) {
      pCmdUI->SetCheck(1);
   }
   else {
      pCmdUI->SetCheck(0);
   }	
}

void CPlotView::OnViewEquation() 
{
   m_view_equation = !m_view_equation;
   Invalidate(FALSE);
   UpdateWindow();
}

void CPlotView::OnUpdateViewEquation(CCmdUI* pCmdUI) 
{
   if (m_view_equation) {
      pCmdUI->SetCheck(1);
   }
   else {
      pCmdUI->SetCheck(0);
   }	
}

BOOL CPlotView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
   BOOL bResult = CView::OnSetCursor(pWnd, nHitTest, message);

   ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));

	return bResult;
}

/////////////////////////////////////////////////////////////////////////////

void CPlotView::ResetHoverWnd(const CPoint& p) // = CPoint(-1,-1);
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

void CPlotView::CreateHoverWnd()
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
            if (pDoc->m_meta_graph->viewingProcessed()) {

               AttributeTable& table = pDoc->m_meta_graph->getAttributeTable();

               size_t xfloor = idx_x.searchfloorindex(ValuePair(-1,dataX(point.x-2)));
               size_t xceil = idx_x.searchceilindex(ValuePair(-1,dataX(point.x+2)));
               size_t yfloor = idx_y.searchfloorindex(ValuePair(-1,dataY(point.y+2)));
               size_t yceil = idx_y.searchceilindex(ValuePair(-1,dataY(point.y-2)));

               // work out anything near this point...
               pvecint xkeys;
               for (size_t i = xfloor + 1; i < xceil; i++) {
                  int index = idx_x[i].index;
                  xkeys.add(index);
               }
               pvecint finalkeys;
               for (size_t j = yfloor + 1; j < yceil; j++) {
                  size_t index = idx_y[j].index;
                  if (xkeys.searchindex(index) != paftl::npos) {
                     finalkeys.push_back( table.getRowKey(index) );
                  }
               }

               if (finalkeys.size()) {

                  AttributeTable& table = pDoc->m_meta_graph->getAttributeTable();

                  // and that it has an appropriate state to display a hover wnd
                  float val = table.getValue( finalkeys[0], pDoc->m_meta_graph->getDisplayedAttribute() );

                  if (val == -1.0f) {
                     m_hover_wnd = new CHoverWnd;
                     strings.push_back("No value");
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
}


void CPlotView::OnKillFocus(CWnd* pNewWnd)
{
   CView::OnKillFocus(pNewWnd);

   // this destroys the hover window if it exists
   ResetHoverWnd();
   // and stops the timer
   KillTimer(DMP_TIMER_HOVER);
}
