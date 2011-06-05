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

// 3DView.cpp : implementation file
//

#include "stdafx.h"
#include "depthmap.h"
#include "3DView.h"
#include "graphdoc.h"
#include "float.h"

#include <generic/xmlparse.h>

#include <gl/gl.h>
#include <gl/glu.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3DView

IMPLEMENT_DYNCREATE(C3DView, CView)

C3DView::C3DView()
{
   m_points = NULL;
   m_pointcount = 0;
   m_lock_draw = false;
   m_mouse_mode = ID_3D_ROT;
   m_mouse_mode_on = 0;
   m_key_mode_on = 0;
   m_quick_draw = false;
   m_animating = false;
   m_drawtrails = false;
   m_fill = true;
   m_track = 0.0;
   m_right_mouse = false;
}

C3DView::~C3DView()
{
}


BEGIN_MESSAGE_MAP(C3DView, CView)
	//{{AFX_MSG_MAP(C3DView)
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_VIEW_CENTREVIEW, OnRecentreView)
	ON_COMMAND(ID_3D_PAN, On3dPan)
	ON_UPDATE_COMMAND_UI(ID_3D_PAN, OnUpdateMode)
	ON_COMMAND(ID_3D_ROT, On3dRot)
	ON_COMMAND(ID_3D_ZOOM, On3dZoom)
	ON_COMMAND(ID_ADD_AGENT, OnAddAgent)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_AGENT_TRAILS, OnAgentTrails)
	ON_UPDATE_COMMAND_UI(ID_AGENT_TRAILS, OnUpdateAgentTrails)
	ON_UPDATE_COMMAND_UI(ID_ADD_AGENT, OnUpdateAddAgent)
	ON_COMMAND(ID_TOOLS_AGENT_LOAD_PROGRAM, OnToolsAgentLoadProgram)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_COMMAND(ID_3D_PLAY_LOOP, OnPlayLoop)
	ON_COMMAND(ID_3D_FILLED, On3dFilled)
	ON_UPDATE_COMMAND_UI(ID_3D_FILLED, OnUpdate3dFilled)
	ON_WM_MOUSEWHEEL()
	ON_UPDATE_COMMAND_UI(ID_3D_ZOOM, OnUpdateMode)
	ON_UPDATE_COMMAND_UI(ID_3D_ROT, OnUpdateMode)
	ON_UPDATE_COMMAND_UI(ID_3D_PLAY_LOOP, OnUpdateMode)
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
   ON_MESSAGE(WM_DMP_REDRAW_MESSAGE, OnRedraw)
   ON_COMMAND(ID_TOOLS_IMPORT_TRACES, &C3DView::OnToolsImportTraces)
   ON_UPDATE_COMMAND_UI(ID_TOOLS_AGENTS_PAUSE, &C3DView::OnUpdateToolsAgentsPause)
   ON_UPDATE_COMMAND_UI(ID_TOOLS_AGENTS_STOP, &C3DView::OnUpdateToolsAgentsStop)
   ON_COMMAND(ID_TOOLS_AGENTS_PAUSE, &C3DView::OnToolsAgentsPause)
   ON_COMMAND(ID_TOOLS_AGENTS_STOP, &C3DView::OnToolsAgentsStop)
   ON_UPDATE_COMMAND_UI(ID_TOOLS_AGENTS_PLAY, &C3DView::OnUpdateToolsAgentsPlay)
   ON_COMMAND(ID_TOOLS_AGENTS_PLAY, &C3DView::OnToolsAgentsPlay)
   ON_UPDATE_COMMAND_UI(ID_TOOLS_IMPORT_TRACES, &C3DView::OnUpdateToolsImportTraces)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3DView drawing

void C3DView::OnDraw(CDC* pDC)
{
   DrawScene();
}

LRESULT C3DView::OnRedraw(WPARAM wParam, LPARAM lParam)
{
   CGraphDoc *pDoc = GetDocument();

   int flag = pDoc->GetRedrawFlag(CGraphDoc::VIEW_3D);
   if (flag != CGraphDoc::REDRAW_DONE) {
      //
      while (!pDoc->SetRedrawFlag(CGraphDoc::VIEW_3D,CGraphDoc::REDRAW_DONE)) {
         // prefer waitformultipleobjects here
         Sleep(1);
      }
      //
      if (flag == CGraphDoc::REDRAW_TOTAL || flag == CGraphDoc::REDRAW_GRAPH) {
         if (wParam == CGraphDoc::NEW_LINESET) {
            // changed which lines are visible:
            ReloadLineData();
         }
         if (wParam == CGraphDoc::NEW_TABLE) {
            // changed which point map is visible:
            ReloadPointData();
         }
         m_quick_draw = false;
         Invalidate();
         UpdateWindow();
      }
   }

   return 0;
}


/////////////////////////////////////////////////////////////////////////////
// C3DView diagnostics

#ifdef _DEBUG
void C3DView::AssertValid() const
{
	CView::AssertValid();
}

void C3DView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGraphDoc* C3DView::GetDocument() // non-debug version is inline
{
   ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGraphDoc)));
   return (CGraphDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// C3DView message handlers

BOOL C3DView::OnEraseBkgnd(CDC* pDC) 
{
   return FALSE;
   
//	return CView::OnEraseBkgnd(pDC);
}

void C3DView::OnTimer(UINT_PTR nIDEvent) 
{
   if (m_mouse_mode == ID_3D_PLAY_LOOP) {
      PlayLoop();
      SetModelMat();
      m_quick_draw = true;
      if (!m_animating) {  // if animating will redraw below
         DrawScene();
      }
   }
   else if (m_key_mode_on) {
      CSize diff(0,0);
      switch (m_keydown) {
      case 37: diff.cx = -2; break;
      case 38: diff.cy = -2; break; // up arrow: note, as per MS reversed y
      case 39: diff.cx = 2; break;
      case 40: diff.cy = 2;
      }
      switch (m_key_mode_on) {
      case ID_3D_ROT:
         Rot(diff);
         break;
      case ID_3D_PAN:
         Pan(diff);
         break;
      case ID_3D_ZOOM:
         Zoom(diff);
         break;
      }
      SetModelMat();
      m_quick_draw = true;
      if (!m_animating) {  // if animating will redraw below
         DrawScene();
      }
   }

	CGraphDoc* pDoc = GetDocument();

   if (m_animating && !pDoc->m_communicator && pDoc->m_meta_graph && pDoc->m_meta_graph->viewingProcessedPoints()) {
      PointMap& pointmap = pDoc->m_meta_graph->getDisplayedPointMap();
      m_animating = false;
      for (size_t i = 0; i < m_mannequins.size(); i++) {
         m_mannequins[i].frame();
         if (m_mannequins[i].m_frame == 0) {
            if (m_mannequins[i].m_playback) {
               int j = m_mannequins[i].m_trace_id;
               Trace& trace = m_traces[j];
               if (m_mannequins[i].m_time >= trace.starttime && m_mannequins[i].m_time <= trace.endtime) {
                  // active zone:
                  m_mannequins[i].m_active = true;
                  size_t k = 0;
                  while (k < m_traces[j].events.size() && m_traces[j].events[k].t < m_mannequins[i].m_time) {
                     k++;
                  }
                  if (k < m_traces[j].events.size()) {
                     Point2f p = (Point2f) m_traces[j].events[k];
                     PixelRef pix = pointmap.pixelate(p); // note, take the pix before you scale!
                     p.normalScale(m_region);
                     m_mannequins[i].advance(p);
                     size_t x = m_pixels.searchindex(pix);
                     if (x != paftl::npos) {
                        if (m_pixels[x].m_value < 10) {
                           m_pixels[x].m_value += 1;
                        }
                     }
                  }
               }
               else {
                  m_mannequins[i].m_active = false;
               }
            }
            else {
               int j = m_mannequins[i].m_agent_id;
               m_agents[j].onMove();
               Point2f p = m_agents[j].getLocation();
               p.normalScale(m_region);
               m_mannequins[i].advance(p);
               //
               // pretty coloured pixels
               PixelRef pix = m_agents[j].getNode();
               size_t x = m_pixels.searchindex(pix);
               if (x != paftl::npos) {
                  if (m_pixels[x].m_value < 10) {
                     m_pixels[x].m_value += 1;
                  }
               }
            }
         }
      }
      m_animating = true;
      DrawScene();
   }

	CView::OnTimer(nIDEvent);
}

BOOL C3DView::PreCreateWindow(CREATESTRUCT& cs) 
{
   // An OpenGL window must be created with the following flags and must not
   // include CS_PARENTDC for the class style. Refer to SetPixelFormat
   // documentation in the "Comments" section for further information.
   
   cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	
	return CView::PreCreateWindow(cs);
}

int C3DView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
   Init(); // initialize OpenGL
   
   ReloadLineData();
   
   // register with GraphDoc:
   CGraphDoc *pDoc = GetDocument();
   pDoc->m_view[CGraphDoc::VIEW_3D] = this;

   return 0;
}

void C3DView::OnDestroy() 
{
   // deregister with GraphDoc
   GetDocument()->m_view[CGraphDoc::VIEW_3D] = NULL;
	
   HGLRC   hrc;
   
   if (m_nTimerID)
      KillTimer(m_nTimerID);
   
   hrc	 = ::wglGetCurrentContext();
   
   ::wglMakeCurrent(NULL,  NULL);
   
   if (hrc)
      ::wglDeleteContext(hrc);
   
   CPalette    palDefault;
   
   // Select our palette out of the dc
   palDefault.CreateStockObject(DEFAULT_PALETTE);
   m_pDC->SelectPalette(&palDefault, FALSE);
   
   if (m_pDC)
      delete m_pDC;
   
   if (m_points) {
      delete [] m_points;
      m_points = NULL;
   }

   CView::OnDestroy();
}

void C3DView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
   if (cy > 0)
   {    
      glViewport(0, 0, cx, cy);
      
      if ((m_oldRect.right > cx) || (m_oldRect.bottom > cy))
         RedrawWindow();
      
      m_oldRect.right = cx;
      m_oldRect.bottom = cy;

      Reshape(cx,cy);
   }	
}

void C3DView::Init()
{
   PIXELFORMATDESCRIPTOR pfd;
   int         n;
   HGLRC		hrc;

   m_pDC = new CClientDC(this);
   
   ASSERT(m_pDC != NULL);
   
   if (!SetupPixelFormat())
      return;
   
   n = ::GetPixelFormat(m_pDC->GetSafeHdc());
   ::DescribePixelFormat(m_pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);
   
   CreateRGBPalette();
   
   // create a rendering context and make it current
   
   hrc = wglCreateContext(m_pDC->GetSafeHdc());
   wglMakeCurrent(m_pDC->GetSafeHdc(), hrc);
   
   GetClientRect(&m_oldRect);

   glClearDepth(1.0f);
   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_FLAT);
   glEnableClientState(GL_VERTEX_ARRAY);
   
   Reshape(m_oldRect.right, m_oldRect.bottom);
   OnRecentreView();

   // 42 for ideal 24 fps
   m_nTimerID = SetTimer(DMP_TIMER_3DVIEW, 20, NULL);
   
   ASSERT(m_nTimerID != NULL);
}

void C3DView::CreateRGBPalette()
{
   PIXELFORMATDESCRIPTOR pfd;
   WORD n;

   n = ::GetPixelFormat(m_pDC->GetSafeHdc());
   ::DescribePixelFormat(m_pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);
  
   if (pfd.dwFlags & PFD_NEED_PALETTE)
   {
      AfxMessageBox(_T("Problem setting up 3D view: Need palette"));
   }
}

BOOL C3DView::SetupPixelFormat()
{
   static PIXELFORMATDESCRIPTOR pfd =
   {
      sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
         1,                           // version number
         PFD_DRAW_TO_WINDOW |         // support window
         PFD_SUPPORT_OPENGL |         // support OpenGL
         PFD_DOUBLEBUFFER,            // double buffered
         PFD_TYPE_RGBA,               // RGBA type
         24,                          // 24-bit color depth
         0, 0, 0, 0, 0, 0,            // color bits ignored
         0,                           // no alpha buffer
         0,                           // shift bit ignored
         0,                           // no accumulation buffer
         0, 0, 0, 0,                  // accum bits ignored
         16,                          // 16-bit z-buffer
         0,                           // no stencil buffer
         0,                           // no auxiliary buffer
         PFD_MAIN_PLANE,              // main layer
         0,                           // reserved
         0, 0, 0                      // layer masks ignored
   };
   
   int pixelformat;
   
   if ((pixelformat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd)) == 0)
   {
      AfxMessageBox(_T("Problem setting up 3D view: ChoosePixelFormat failed"));
      return FALSE;
   }
   
   if (SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd) == FALSE)
   {
      AfxMessageBox(_T("Problem setting up 3D view: SetPixelFormat failed"));
      return FALSE;
   }
   
   return TRUE;
}

void C3DView::DrawScene()
{
	CGraphDoc* pDoc = GetDocument();

   if (m_lock_draw) {
      return;
   }
   m_lock_draw = true;
   
   COLORREF bg = GetApp()->m_background;
   COLORREF fg = GetApp()->m_foreground;
   glClearColor((GLfloat)GetRValue(bg)/255.0f,(GLfloat)GetGValue(bg)/255.0f,(GLfloat)GetBValue(bg)/255.0f, 0.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glColor3f(1.0f,0.0f,0.0f);
   if (m_male_template.m_init) {
      for (size_t i = 0; i < m_mannequins.size(); i++) {
         if (m_mannequins[i].m_active) {
            if (i % 2 == 0) {
               m_mannequins[i].draw(m_male_template, m_drawtrails, m_fill);
            }
            else {
               m_mannequins[i].draw(m_female_template, m_drawtrails, m_fill);
            }
         }
      }
   }

   if (m_fill) {
      if (!m_animating) {
         if (pDoc->m_meta_graph && pDoc->m_meta_graph->viewingProcessedPoints()) {
            // okay, you can go for it and draw all the squares in cutesy 3d:
            PointMap& pointmap = pDoc->m_meta_graph->getDisplayedPointMap();
            AttributeTable& table = pointmap.getAttributeTable();
            for (int i = 0; i < table.getRowCount(); i++) {
               PixelRef pix = table.getRowKey(i);
               PafColor color;
               int col = pointmap.getDisplayedAttribute();
               float value = table.getNormValue(i,col);
               if (value != -1.0f) {
                  color.makeAxmanesque(value);
                  glColor3f(color.redf(),color.greenf(),color.bluef());
                  Point2f p = pointmap.depixelate(pix);
                  p.normalScale(m_region);
                  glPushMatrix();
                  glTranslatef(p.x,p.y,0.0f);
                  glVertexPointer(3, GL_FLOAT, 0, m_rect);
                  glDrawArrays(GL_QUADS, 0, 4);
                  glPopMatrix();
               }
            }
         }
      }
      else {
         for (size_t i = 0; i < m_pixels.size(); i++) {
            int& value = m_pixels[i].m_value;
            if (value != -1) {
               if (pafrand() % 10000 == 0) {
                  value--;
               }
               PafColor color;
               color.makeAxmanesque(float(value)/10.0f);
               glColor3f(color.redf(),color.greenf(),color.bluef());
               Point2f& p = m_pixels[i].m_point;
               glPushMatrix();
               glTranslatef(p.x,p.y,0.0f);
               glVertexPointer(3, GL_FLOAT, 0, m_rect);
               glDrawArrays(GL_QUADS, 0, 4);
               glPopMatrix();
            }
         }
      }
   }

   glColor3f((GLfloat)GetRValue(fg)/255.0f,(GLfloat)GetGValue(fg)/255.0f,(GLfloat)GetBValue(fg)/255.0f);
   if (m_pointcount) {
      glVertexPointer(3, GL_FLOAT, 0, m_points);
      glDrawArrays(GL_LINES, 0, m_pointcount);
   }

   glFlush();

   SwapBuffers(wglGetCurrentDC());
   
   m_lock_draw = false;
}

void C3DView::Reshape(int x, int y)
{
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0f, (GLfloat) x / (GLfloat) y, 0.1f, 3.0f);
   // leave matrix mode in model view:
   glMatrixMode(GL_MODELVIEW);
}

void C3DView::OnRecentreView() 
{
   m_panx = m_pany = m_rotx = m_roty = 0.0f;
   m_zoom = 1.0f;
   m_roty = 60.0f;

   SetModelMat();

   Invalidate();
   UpdateWindow();
}

void C3DView::SetModelMat()
{
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef(-m_panx, m_pany, -m_zoom);
   glRotatef(-m_roty, 1.0f, 0.0f, 0.0f);
   glRotatef(-m_rotx, 0.0f, 0.0f, 1.0f);
   glTranslatef(-0.5f, -0.5f, 0.0f);
}

void C3DView::ReloadLineData()
{
   if (m_points) {
      delete [] m_points;
      m_points = NULL;
   }
   m_region = Region();

   CGraphDoc *pDoc = GetDocument();

   if (pDoc->m_meta_graph && pDoc->m_meta_graph->getState() & MetaGraph::LINEDATA) {
      // should really check communicator is not open...
      pDoc->m_meta_graph->setLock(this);
      while (m_lock_draw) {
         Sleep(1);
      }
      m_lock_draw = true;

      SuperSpacePixel& superspacepix = *(pDoc->m_meta_graph);

      prefvec<Line> lines;
      for (int i = 0; i < superspacepix.size(); i++) {
         for (int j = 0; j < superspacepix.at(i).size(); j++) {
            if (superspacepix.at(i).at(j).isShown()) {
               if (m_region.isNull()) {
                  m_region = superspacepix.at(i).at(j).getRegion();
               }
               else {
                  m_region = runion(m_region,superspacepix.at(i).at(j).getRegion());
               }
               for (int k = 0; k < superspacepix.at(i).at(j).getAllShapes().size(); k++) {
                  SalaShape& shape = superspacepix.at(i).at(j).getAllShapes().at(k);
                  if (shape.isLine()) {
                     lines.push_back(shape.getLine());
                  }
                  else if (shape.isPolyLine() || shape.isPolygon()) {
                     for (int n = 0; n < shape.size() - 1; n++) {
                        lines.push_back(Line(shape[n],shape[n+1]));
                     }
                     if (shape.isPolygon()) {
                        lines.push_back(Line(shape.tail(),shape.head()));
                     }
                  }
               }
            }
         }
      }

      m_pointcount = lines.size() * 2;
      if (m_pointcount) {
         // now scale up to a nice square region around midpoint:
         if (m_region.width() > m_region.height()) {
            double oldheight = m_region.height();
            m_region.bottom_left.y -= (m_region.width()-oldheight)/2;
            m_region.top_right.y += (m_region.width()-oldheight)/2;
         }
         else {
            double oldwidth = m_region.width();
            m_region.bottom_left.x -= (m_region.height()-oldwidth)/2;
            m_region.top_right.x += (m_region.height()-oldwidth)/2;
         }

         m_points = new GLfloat [m_pointcount*3];
         for (int i = 0; i < m_pointcount; i++) {
            Point2f p;
            if (i % 2 == 0) {
               p = lines[i/2].start();
            }
            else {
               p = lines[i/2].end();
            }
            p.normalScale(m_region);
            m_points[i*3+0] = p.x;
            m_points[i*3+1] = p.y;
            m_points[i*3+2] = 0.0;
         }
      }

      m_lock_draw = false;
      pDoc->m_meta_graph->releaseLock(this);
   }

   // note: as affects region, will also affect point data:
   ReloadPointData();
}

const GLfloat g_rect[][3] = 
{
   {-0.5f,-0.5f,-0.05f},
   {0.5f,-0.5f,-0.05f},
   {0.5f,0.5f,-0.05f},
   {-0.5f,0.5f,-0.05f}
};

void C3DView::ReloadPointData()
{
   CGraphDoc *pDoc = GetDocument();

   m_mannequins.clear();
   m_agents.clear();
   m_animating = false;

   if (pDoc->m_meta_graph && pDoc->m_meta_graph->viewingProcessedPoints()) {
      //
      if (!m_region.isNull()) {
         GLfloat unit = pDoc->m_meta_graph->getDisplayedPointMap().getSpacing() / m_region.width();
         m_male_template.Init(unit, true);
         m_female_template.Init(unit, false);
         for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 3; j++) {
               m_rect[i][j] = unit * g_rect[i][j];
            }
         }
      }
      else {
         m_male_template.Destroy();
         m_female_template.Destroy();
      }
      //
      m_pixels.clear();
      PointMap& map = pDoc->m_meta_graph->getDisplayedPointMap();
      AttributeTable& table = map.getAttributeTable();
      for (int i = 0; i < table.getRowCount(); i++) {
         PixelRef pix = table.getRowKey(i);
         Point2f p = map.depixelate(pix);
         p.normalScale(m_region);
         m_pixels.add(pix,C3DPixelData(p));
      }
   }
   else {
      m_male_template.Destroy();
      m_female_template.Destroy();
   }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void C3DView::OnUpdateMode(CCmdUI* pCmdUI) 
{
   if (pCmdUI->m_nID == m_mouse_mode) {
      pCmdUI->SetCheck(1);
   }
   else {
      pCmdUI->SetCheck(0);
   }
}

void C3DView::OnUpdateAddAgent(CCmdUI* pCmdUI) 
{
   if (!GetDocument()->m_meta_graph || !GetDocument()->m_meta_graph->viewingProcessedPoints()) {
      if (m_mouse_mode == ID_ADD_AGENT) {
         m_mouse_mode = ID_3D_ROT;
      }
      pCmdUI->Enable(FALSE);
   }
   else {
      pCmdUI->Enable(TRUE);
      if (m_mouse_mode == ID_ADD_AGENT) {
         pCmdUI->SetCheck(1);
      }
      else {
         pCmdUI->SetCheck(0);
      }
   }
}

void C3DView::OnUpdateToolsImportTraces(CCmdUI *pCmdUI)
{
   if (GetDocument()->m_meta_graph && GetDocument()->m_meta_graph->viewingProcessedPoints()) {
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE);
   }
}

void C3DView::On3dPan() 
{
   m_mouse_mode = ID_3D_PAN;
}

void C3DView::On3dRot()
{
   m_mouse_mode = ID_3D_ROT;
}

void C3DView::On3dZoom() 
{
   m_mouse_mode = ID_3D_ZOOM;
}

void C3DView::OnPlayLoop() 
{
   m_mouse_mode = ID_3D_PLAY_LOOP;
}

void C3DView::OnAddAgent() 
{
   m_mouse_mode = ID_ADD_AGENT;
}

void C3DView::OnToolsAgentLoadProgram() 
{
   CFileDialog openDialog(TRUE,NULL,NULL,
						  OFN_EXPLORER|OFN_HIDEREADONLY,
						  _T("Text files (*.txt)|*.txt|All files (*.*)|*.*||"),
						  NULL);
   openDialog.m_ofn.lpstrTitle = _T("Import");

   if (openDialog.DoModal() != IDOK) {
      return;
   }  

   pstring filename = openDialog.GetPathName();
   
   if (!filename.empty()) {
      m_animating = false;
      while (m_lock_draw) {
         Sleep(1);
      }
      m_lock_draw = true;
      m_agents.clear();
      m_mannequins.clear();
      m_lock_draw = false;
      if (!m_agent_program.open(filename)) {
         AfxMessageBox(_T("Unable to understand agent program"));
      }
   }
   else {
      AfxMessageBox(_T("No file selected"));
   }
}

void C3DView::On3dFilled() 
{
   if (m_fill) {
      m_fill = false;
   }
   else {
      m_fill = true;
   }
   Invalidate();
   UpdateWindow();
}

void C3DView::OnUpdate3dFilled(CCmdUI* pCmdUI) 
{
   if (m_fill) {
      pCmdUI->SetCheck(1);
   }
   else {
      pCmdUI->SetCheck(0);
   }
}

void C3DView::OnAgentTrails() 
{
   if (m_drawtrails) {
      m_drawtrails = false;
   }
   else {
      m_drawtrails = true;
   }
}

void C3DView::OnUpdateAgentTrails(CCmdUI* pCmdUI) 
{
   if (m_drawtrails) {
      pCmdUI->SetCheck(1);
   }
   else {
      pCmdUI->SetCheck(0);
   }
}

/////////////////////////////////////////////////////////////////////////////////////////

void C3DView::OnLButtonDown(UINT nFlags, CPoint point) 
{
   m_right_mouse = false;

   while (m_lock_draw) {
      Sleep(1);
   }

   SetCapture();

   switch (m_mouse_mode) {
   case ID_3D_PAN: case ID_3D_ROT: case ID_3D_ZOOM:
      m_mouse_mode_on = m_mouse_mode;
      m_mouse_origin = point;
      break;
   case ID_ADD_AGENT:
      {
         if (m_male_template.m_init) {
            CreateAgent(point);
         }
      }
      break;
   }

	CView::OnLButtonDown(nFlags, point);
}

void C3DView::OnLButtonUp(UINT nFlags, CPoint point) 
{
   ReleaseCapture();

	m_mouse_mode_on = 0;

   m_quick_draw = false;
   Invalidate();
   UpdateWindow();

	CView::OnLButtonUp(nFlags, point);
}

void C3DView::OnRButtonDown(UINT nFlags, CPoint point) 
{
   m_right_mouse = true;

   while (m_lock_draw) {
      Sleep(1);
   }

   SetCapture();

   m_mouse_origin = point;

	CView::OnRButtonDown(nFlags, point);
}

void C3DView::OnRButtonUp(UINT nFlags, CPoint point) 
{
   ReleaseCapture();

   m_right_mouse = false;

   m_quick_draw = false;
   Invalidate();
   UpdateWindow();
	
	CView::OnRButtonUp(nFlags, point);
}


void C3DView::OnMouseMove(UINT nFlags, CPoint point) 
{
   if (m_mouse_mode_on && point != m_mouse_origin) {
      CSize diff = m_mouse_origin - point;
      switch (m_mouse_mode) {
      case ID_3D_PAN: 
         Pan(diff);
         m_mouse_origin = point;
         break;
      case ID_3D_ROT:
         Rot(diff);
         m_mouse_origin = point;
         break;
      case ID_3D_ZOOM:
         Zoom(diff);
         m_mouse_origin = point;
         break;
      }
      SetModelMat();
      m_quick_draw = true;
      Invalidate();
      UpdateWindow();
   }
   else if (m_right_mouse && point != m_mouse_origin) {
      // always pan with right mouse
      CSize diff = m_mouse_origin - point;
      Pan(diff);
      m_mouse_origin = point;
      SetModelMat();
      m_quick_draw = true;
      Invalidate();
      UpdateWindow();
   }
   
	CView::OnMouseMove(nFlags, point);
}

void C3DView::Pan(CSize diff) 
{
   m_panx += 0.005f * (diff.cx);
   if (m_panx < -1.0f) {
      m_panx = -1.0f;
   }
   else if (m_panx > 1.0f) {
      m_panx = 1.0f;
   }
   m_pany += 0.005f * (diff.cy);
   if (m_pany < -1.0f) {
      m_pany = -1.0f;
   }
   else if (m_pany > 1.0f) {
      m_pany = 1.0f;
   }
}

void C3DView::Rot(CSize diff) 
{
   m_rotx += 0.5f * (diff.cx);
   if (m_rotx < -180.0f) {
      m_rotx = 180.0f;
   }
   else if (m_rotx > 180.0f) {
      m_rotx = -180.0f;
   }
   m_roty += 0.5f * (diff.cy);
   if (m_roty < 0.0f) {
      m_roty = 0.0f;
   }
   else if (m_roty > 90.0f) {
      m_roty = 90.0f;
   }
}

void C3DView::Zoom(CSize diff) 
{
   m_zoom += 0.005f * (diff.cy);
   if (m_zoom < 0.02f) {
      m_zoom = 0.02f;
   }
   else if (m_zoom > 2.5f) {
      m_zoom = 2.5f;
   }
}

void C3DView::PlayLoop() 
{
   m_rotx += 0.05f;
   if (m_rotx < -180.0f) {
      m_rotx = 180.0f;
   }
   else if (m_rotx > 180.0f) {
      m_rotx = -180.0f;
   }
   m_track += 0.001 * M_PI;
   if (m_track > 2.0 * M_PI) {
      m_track -= 2.0 * M_PI;
   }
   m_roty += 0.05f * cosf(m_track);
   if (m_roty < 0.0f) {
      m_roty = 0.0f;
   }
   else if (m_roty > 75.0f) {
      m_roty = 75.0f;
   }
   m_zoom -= 0.001f * sinf(m_track);
   if (m_zoom < 0.02f) {
      m_zoom = 0.02f;
   }
   else if (m_zoom > 2.5f) {
      m_zoom = 2.5f;
   }
}

void C3DView::CreateAgent(CPoint point)
{
   if (!m_male_template.m_init) {
      return;
   }

   while (m_lock_draw) {
      Sleep(1);
   }
   m_lock_draw = true;

   bool animating = m_animating;
   m_animating = false;

   // click test
   GLint viewport[4];
   GLdouble mvmatrix[16], projmatrix[16];

   glGetIntegerv(GL_VIEWPORT, viewport);
   glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
   glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);

   GLint realy = viewport[3] - point.y;

   GLdouble wx1, wy1, wz1, wx2, wy2, wz2;
   gluUnProject((GLdouble) point.x, (GLdouble) realy, 0.0,
                mvmatrix, projmatrix, viewport, &wx1, &wy1, &wz1);
   gluUnProject((GLdouble) point.x, (GLdouble) realy, 1.0,
                mvmatrix, projmatrix, viewport, &wx2, &wy2, &wz2);

   // 0 plane has to lie between wz1 and wz2:
   if (_finite(wz1) && _finite(wz2) && wz1 > 0 && wz2 < 0) {
      double scaling = wz1/(wz2-wz1);
      Point2f p(wx1-scaling*(wx2-wx1),wy1-scaling*(wy2-wy1));

      CGraphDoc *pDC = GetDocument();
      if (pDC->m_meta_graph && pDC->m_meta_graph->viewingProcessedPoints()) {
         // okay, you can go for it and add an agent:
         PointMap& pointmap = pDC->m_meta_graph->getDisplayedPointMap();
         p.denormalScale(m_region);
         PixelRef pix = pointmap.pixelate(p);
         if (pointmap.getPoint(pix).filled()) {
            m_agents.push_back( Agent(&m_agent_program, &pointmap) );
            m_agents.tail().onInit(pix);
            Point2f p2 = m_agents.tail().getLocation();
            p2.normalScale(m_region);
            m_mannequins.push_back( CMannequin(p2, m_agents.size()-1) );
            m_agents.tail().onMove();
            p2 = m_agents.tail().getLocation();
            p2.normalScale(m_region);
            m_mannequins.tail().advance(p2);

            m_animating = true;
         }
      }
   }

   m_animating |= animating;
   m_lock_draw = false;
}

//////////////////////////////////////////////////////////////////////////////////////

const GLfloat g_male_mannequin_points[][3] = 
{
   {0.0f,0.0f,0.3f},    //  0 top head
   {0.0f,0.0f,0.0f},    //  1 top spine
   {0.0f,0.0f,-0.7f},   //  2 base spine
   {-0.25f,0.0f,0.0f},  //  3 left shoulder
   {-0.15f,0.0f,-0.7f},  //  4 left hip
   {0.25f,0.0f,0.0f},   //  5 right shoulder
   {0.15f,0.0f,-0.7f},   //  6 right hip
   {-0.25f,0.0f,-0.8f}, //  7 bottom left arm (hung from top spine)
   {-0.15f,0.0f,0.0f},   //  8 top left leg (hung from base spine)
   {-0.15f,0.0f,-1.0f}   //  9 bottom left leg (hung from base spine)
};

const GLfloat g_female_mannequin_points[][3] = 
{
   {0.0f,0.0f,0.3f},    //  0 top head 
   {0.0f,0.0f,0.0f},    //  1 top spine
   {0.0f,0.0f,-0.7f},   //  2 base spine
   {-0.2f,0.0f,0.0f},   //  3 left shoulder
   {-0.2f,0.0f,-0.7f}, //  4 left hip
   {0.2f,0.0f,0.0f},    //  5 right shoulder
   {0.2f,0.0f,-0.7f},  //  6 right hip
   {-0.25f,0.0f,-0.8f}, //  7 bottom left arm (hung from top spine)
   {-0.2f,0.0f,0.0f},  //  8 top left leg (hung from base spine)
   {-0.1f,0.0f,-1.0f}   //  9 bottom left leg (hung from base spine)
};

CMannequinTemplate::CMannequinTemplate()
{
   m_init = false;
   m_unit = 0.0f;
}

CMannequinTemplate::~CMannequinTemplate()
{
}

void CMannequinTemplate::Init(GLfloat unit, bool male)
{
   m_unit = unit;
   m_male = male;

   for (int i = 0; i < 10; i++) {
      for (int j = 0; j < 3; j++) {
         m_points[i][j] = m_unit * (male ? g_male_mannequin_points[i][j] : g_female_mannequin_points[i][j]);
      }
   }

   m_init = true;
}

void CMannequinTemplate::Destroy()
{
   m_init = false;
   m_unit = 0.0f;
}

CMannequin::CMannequin(const Point2f& startloc, int id, bool playback)
{
   m_left = true;
   m_paused = false;
   m_frame = 0;
   m_zrot = 0.0f;
   m_startloc = startloc;
   m_nextloc = startloc;
   m_pointcount = 0;
   m_pointstart = 0;
   m_time = 0.0;
   m_active = true;
   m_playback = playback;
   if (m_playback) {
      m_trace_id = id;
   }
   else {
      m_agent_id = id;
   }
}

CMannequin::CMannequin(const CMannequin& man)
{
   m_left = man.m_left;
   m_paused = man.m_paused;
   m_frame = man.m_frame;
   m_zrot = man.m_zrot;
   m_startloc = man.m_startloc;
   m_lastloc = man.m_lastloc;
   m_nextloc = man.m_nextloc;
   m_pointcount = man.m_pointcount;
   m_pointstart = man.m_pointstart;
   memcpy(m_points+m_pointstart,man.m_points+man.m_pointstart*3,m_pointcount*sizeof(GLfloat)*3);
   m_time = man.m_time;
   m_active = man.m_active;
   m_playback = man.m_playback;
   m_trace_id = man.m_trace_id;
   m_agent_id = man.m_agent_id;
}

CMannequin& CMannequin::operator = (const CMannequin& man)
{
   if (&man != this) {
      m_left = man.m_left;
      m_paused = man.m_paused;
      m_frame = man.m_frame;
      m_zrot = man.m_zrot;
      m_startloc = man.m_startloc;
      m_lastloc = man.m_lastloc;
      m_nextloc = man.m_nextloc;
      m_pointcount = man.m_pointcount;
      m_pointstart = man.m_pointstart;
      memcpy(m_points+m_pointstart,man.m_points+man.m_pointstart*3,m_pointcount*sizeof(GLfloat)*3);
      m_time = man.m_time;
      m_active = man.m_active;
      m_playback = man.m_playback;
      m_trace_id = man.m_trace_id;
      m_agent_id = man.m_agent_id;
   }
   return *this;
}

void CMannequin::frame()
{
   m_frame++;
   if (m_frame >= 24) {
      m_frame = 0;
      m_time += 1.0;
      m_left = m_left ? false : true;
   }
}

void CMannequin::advance(const Point2f& nextloc)
{
   m_lastloc = m_nextloc;
   m_nextloc = nextloc;
   if (m_nextloc == m_lastloc) {
      m_paused = true;
   }
   else {
      Point2f vec = m_nextloc - m_lastloc;
      vec.normalise();
      m_zrot = 90.0 + 180.0 * vec.angle() / M_PI;
      m_paused = false;
      //
      m_pointcount++;
      if (m_pointcount > 25) {
         m_pointstart++;
         m_pointcount = 25;
         if (m_pointstart > 25) {
            memcpy(m_points,m_points + (m_pointstart-1)*3, (m_pointcount-1)*sizeof(GLfloat)*3);
            m_pointstart = 0;
            m_pointcount = 25;
         }
      }
      int base = (m_pointstart+m_pointcount-1) * 3;
      m_points[base] = m_lastloc.x;
      m_points[base+1] = m_lastloc.y;
      m_points[base+2] = 0.0f;
   }
}

void CMannequin::draw(CMannequinTemplate& templ, bool drawtrails, bool highlight)
{
   if (!highlight) {
      if (templ.m_male) {
         glColor3f(0.4f,0.4f,0.8f);
      }
      else {
         glColor3f(0.7f,0.4f,0.6f);
      }
   }
   else {
      if (templ.m_male) {
         glColor3f(0.8f,0.8f,1.0f);
      }
      else {
         glColor3f(1.0f,0.8f,0.9f);
      }
   }

   glPushMatrix();

   // based on 24 frames per second:
   GLfloat framef = GLfloat(m_frame % 24) / 24.0f;
   if (m_paused) {
      framef = 0.0f;
   }
   glTranslatef( framef * m_nextloc.x + (1.0f - framef) * m_lastloc.x, 
                framef * m_nextloc.y + (1.0f - framef) * m_lastloc.y, 0.0f );
   glRotatef(m_zrot,0.0f,0.0f,1.0f);

   // now use framef as a swing:
   framef *= 2.0f;
   if (framef > 1.0f) {
      framef = 2.0f - framef;
   }
   GLfloat swing = framef * 30.0f * (m_left ? -1.0f : 1.0f);

   GLfloat h = cos(M_PI*0.16667f*framef) + 0.7f;  // 2 * M_PI * 30.0f * framef / 360.0f = M_PI * 0.16667 * framef

   glTranslatef( 0.0f, 0.0f, h * templ.m_unit);

   glBegin(GL_LINES);
      // head
      glVertex3fv(templ.m_points[0]);
      glVertex3fv(templ.m_points[1]);
      // spine
      glVertex3fv(templ.m_points[1]);
      glVertex3fv(templ.m_points[2]);
      // shoulders
      glVertex3fv(templ.m_points[3]);
      glVertex3fv(templ.m_points[5]);
      // hips
      glVertex3fv(templ.m_points[4]);
      glVertex3fv(templ.m_points[6]);
   glEnd();

   glPushMatrix();
   glRotatef(swing,1.0f,0.0f,0.0f);

   glBegin(GL_LINES);
      // left arm 
      glVertex3fv(templ.m_points[3]);
      glVertex3fv(templ.m_points[7]);
   glEnd();

   glRotatef(180.0f,0.0f,0.0f,1.0f);
   glRotatef(2.0*swing,1.0f,0.0f,0.0f);

   glBegin(GL_LINES);
      // right arm (n.b., reuse same point pair)
      glVertex3fv(templ.m_points[3]);
      glVertex3fv(templ.m_points[7]);
   glEnd();

   glPopMatrix();

   glTranslatef(0.0f,0.0f,-0.7f*templ.m_unit);
   glRotatef(-swing,1.0f,0.0f,0.0f);

   glBegin(GL_LINES);
      // left leg 
      glVertex3fv(templ.m_points[8]);
      glVertex3fv(templ.m_points[9]);
   glEnd();

   glRotatef(180.0f,0.0f,0.0f,1.0f);
   glRotatef(2.0*-swing,1.0f,0.0f,0.0f);

   glBegin(GL_LINES);
      // right leg (n.b., reuse same point pair)
      glVertex3fv(templ.m_points[8]);
      glVertex3fv(templ.m_points[9]);
   glEnd();

   glPopMatrix();

   // trails...
   if (drawtrails && m_pointcount > 1) {
      glVertexPointer(3, GL_FLOAT, 0, m_points);
      glDrawArrays(GL_LINE_STRIP, m_pointstart, m_pointcount);
   }
}

void C3DView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
   switch (nChar) {
   case 37: case 38: case 39: case 40:
      m_key_mode_on = m_mouse_mode;
      m_keydown = nChar;
      break;
   case 33: // Page Up
      switch (m_mouse_mode)
      {
      case ID_ADD_AGENT: m_mouse_mode = ID_3D_ZOOM; break;
      case ID_3D_ROT: m_mouse_mode = ID_ADD_AGENT; break;
      case ID_3D_PAN: m_mouse_mode = ID_3D_ROT; break;
      case ID_3D_ZOOM: m_mouse_mode = ID_3D_PAN; break;
      }
      break;
   case 34: // Page Down
      switch (m_mouse_mode)
      {
      case ID_ADD_AGENT: m_mouse_mode = ID_3D_ROT; break;
      case ID_3D_ROT: m_mouse_mode = ID_3D_PAN; break;
      case ID_3D_PAN: m_mouse_mode = ID_3D_ZOOM; break;
      case ID_3D_ZOOM: m_mouse_mode = ID_ADD_AGENT; break;
      }
      break;
   case 'T':
      OnAgentTrails();
      break;
   case 'A':
      {
         CPoint point;
         ::GetCursorPos(&point);
	      ScreenToClient(&point);
         CreateAgent(point);
      }
      break;
   }

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void C3DView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
   m_key_mode_on = 0;

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

BOOL C3DView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
   CSize diff(0,-zDelta/5);
   Zoom(diff);
   SetModelMat();
   m_quick_draw = true;
   Invalidate();
   UpdateWindow();
   
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void C3DView::OnToolsImportTraces()
{
   CFileDialog openDialog(TRUE,NULL,NULL,
						  OFN_EXPLORER|OFN_HIDEREADONLY,
						  _T("XML files (*.xml)|*.xml|Text files (*.txt)|*.txt|All files (*.*)|*.*||"),
						  NULL);
   openDialog.m_ofn.lpstrTitle = _T("Import Traces");

   if (openDialog.DoModal() != IDOK) {
      return;
   }  

   pstring filename = openDialog.GetPathName();
   
   if (!filename.empty()) {
      m_animating = false;
      while (m_lock_draw) {
         Sleep(1);
      }
      m_lock_draw = true;
      m_agents.clear();
      m_traces.clear();
      m_mannequins.clear();
      m_lock_draw = false;
      //
      ifstream file(filename.c_str());
      // Eva's XMLs do not have the header yet:
      xmlelement traceset;
      string elementname;
      while (file && elementname != "traceset") {
         traceset.parse(file,false);
         elementname = tolower(traceset.name.c_str());
      }
      while (file) {
         xmlelement trace;
         trace.parse(file,true);
         elementname = tolower(trace.name.c_str());
         if (elementname == "trace") {
            m_traces.push_back(Trace());
            bool firstevent = true;
            for (int j = 0; j < trace.subelements.size(); j++) {
               // these should be events:
               xmlelement& traceevent = trace.subelements[j];
               if (traceevent.name == "event") {
                  double x = atof(traceevent.attributes["x"].c_str());
                  double y = atof(traceevent.attributes["y"].c_str());
                  double t = atof(traceevent.attributes["t"].c_str());
                  m_traces.tail().events.push_back(Event2f(x,y,t));
                  if (firstevent) {
                     m_traces.tail().starttime = t;
                     firstevent = false;
                  }
               }
            }
            if (m_traces.tail().events.size() >= 1) {
               m_traces.tail().endtime = m_traces.tail().events.tail().t;
               Point2f p = m_traces.tail().events[0];
               p.normalScale(m_region);
               m_mannequins.push_back( CMannequin(p,m_traces.size()-1,true) );
               m_mannequins.tail().m_active = false;
            }
         }
      }
   }
   else {
      AfxMessageBox(_T("No file selected"));
   }
}

void C3DView::OnUpdateToolsAgentsPlay(CCmdUI *pCmdUI)
{
   if (m_mannequins.size()) {
      pCmdUI->Enable(TRUE);
      if (m_animating) {
         pCmdUI->SetCheck(TRUE);
      }
      else {
         pCmdUI->SetCheck(FALSE);
      }
   }
   else {
      pCmdUI->SetCheck(FALSE);
      pCmdUI->Enable(FALSE);
   }
}

void C3DView::OnUpdateToolsAgentsPause(CCmdUI *pCmdUI)
{
   if (m_mannequins.size()) {
      pCmdUI->Enable(TRUE);
      if (!m_animating) {
         pCmdUI->SetCheck(TRUE);
      }
      else {
         pCmdUI->SetCheck(FALSE);
      }
   }
   else {
      pCmdUI->SetCheck(FALSE);
      pCmdUI->Enable(FALSE);
   }
}

void C3DView::OnUpdateToolsAgentsStop(CCmdUI *pCmdUI)
{
   if (m_mannequins.size()) {
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE);
   }
}

void C3DView::OnToolsAgentsPause()
{
   m_animating = false;
}

void C3DView::OnToolsAgentsStop()
{
   m_animating = false;
   for (int i = 0; i < m_mannequins.size(); i++) {
      if (m_mannequins[i].m_playback) {
         m_mannequins[i].m_active = false;
         m_mannequins[i].m_time = 0.0;
         m_mannequins[i].m_nextloc = m_mannequins[i].m_startloc;
      }
   }
   for (int j = 0; j < m_pixels.size(); j++) {
      m_pixels[j].m_value = -1;
   }
}

void C3DView::OnToolsAgentsPlay()
{
   m_animating = true;
}

