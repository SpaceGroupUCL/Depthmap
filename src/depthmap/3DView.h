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

#if !defined(AFX_3DVIEW_H__225CD53B_5914_45A2_99AB_228D329EA91A__INCLUDED_)
#define AFX_3DVIEW_H__225CD53B_5914_45A2_99AB_228D329EA91A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 3DView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// C3DView view

#include <gl/gl.h>

#include <Sala/nagent.h>

/////////////////////////////////////////////////////////////////////////////

// CScreenAgent

struct CMannequinTemplate
{
   bool m_init;
   GLfloat m_unit;
   bool m_male;
   GLfloat m_points[10][3];
   CMannequinTemplate();
   virtual ~CMannequinTemplate();
   void Init(GLfloat unit, bool male);
   void Destroy();
};

struct CMannequin
{
   GLfloat m_zrot;       // facing direction
   Point2f m_startloc;
   Point2f m_lastloc;
   Point2f m_nextloc;
   int m_frame;
   bool m_left;
   bool m_paused;
   // these are used when replaying a trace
   bool m_active;
   bool m_playback;
   double m_time;
   // 
   int m_agent_id;
   int m_trace_id;
   //
   CMannequin(const Point2f& lastloc = Point2f(), int id = -1, bool playback = false);
   CMannequin(const CMannequin& man);
   CMannequin& operator = (const CMannequin& man);
   void draw(CMannequinTemplate& templ, bool drawtrails, bool highlight);
   void frame();
   void advance(const Point2f& nextloc);
   // big long array to record trails!
   GLfloat m_points[150];
   int m_pointstart;
   int m_pointcount;
};

struct C3DPixelData
{
   Point2f m_point;
   int m_value;
   C3DPixelData(const Point2f& p = Point2f())
   { m_point = p; m_value = -1; } 
};

/////////////////////////////////////////////////////////////////////////////

class C3DView : public CView
{
protected:
	C3DView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(C3DView)

// Attributes
public:
	CGraphDoc* GetDocument();

   UINT_PTR m_nTimerID;
	CRect m_oldRect;
	float m_fRadius;
   CClientDC *m_pDC;
   //
   bool m_quick_draw;
   bool m_lock_draw;
   bool m_animating;
   bool m_drawtrails;
   bool m_fill;
   double m_track; // camera track -- used in playloop
   //
   //
   Region m_region;
   GLfloat *m_points;
   GLfloat m_rect[4][3];
   pqmap<int,C3DPixelData> m_pixels;
   //
   int m_pointcount;
   //
   int m_mouse_mode;
   int m_mouse_mode_on;
   bool m_right_mouse;
   int m_key_mode_on;
   int m_keydown;
   CPoint m_mouse_origin;
   //
   GLfloat m_panx;
   GLfloat m_pany;
   GLfloat m_rotx;
   GLfloat m_roty;
   GLfloat m_zoom;
   //
   // use to initialise mannequintemplate:
   CMannequinTemplate m_male_template;
   CMannequinTemplate m_female_template;
   prefvec<CMannequin> m_mannequins;
   prefvec<Agent> m_agents;
   prefvec<Trace> m_traces;
   AgentProgram m_agent_program;
   //
   // used to keep track of internal time for all agents
   double m_global_time;
   //
// Operations
public:
   void Init();
   BOOL SetupPixelFormat();
   void CreateRGBPalette();
   void Reshape(int x, int y);
   void DrawScene();
   void ReloadLineData();
   void ReloadPointData();
   void SetModelMat();
   void CreateAgent(CPoint point);

   void Rot(CSize diff);
   void Pan(CSize diff);
   void Zoom(CSize diff);
   void PlayLoop();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C3DView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~C3DView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// My message map functions
protected:
   LRESULT OnRedraw(WPARAM wParam, LPARAM lParam);
	// Generated message map functions
protected:
	//{{AFX_MSG(C3DView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRecentreView();
	afx_msg void On3dPan();
	afx_msg void OnUpdateMode(CCmdUI* pCmdUI);
	afx_msg void On3dRot();
	afx_msg void On3dZoom();
	afx_msg void OnAddAgent();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnAgentTrails();
	afx_msg void OnUpdateAgentTrails(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddAgent(CCmdUI* pCmdUI);
	afx_msg void OnToolsAgentLoadProgram();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPlayLoop();
	afx_msg void On3dFilled();
	afx_msg void OnUpdate3dFilled(CCmdUI* pCmdUI);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnToolsImportTraces();
public:
   afx_msg void OnUpdateToolsAgentsPause(CCmdUI *pCmdUI);
public:
   afx_msg void OnUpdateToolsAgentsStop(CCmdUI *pCmdUI);
public:
   afx_msg void OnToolsAgentsPause();
public:
   afx_msg void OnToolsAgentsStop();
public:
   afx_msg void OnUpdateToolsAgentsPlay(CCmdUI *pCmdUI);
public:
   afx_msg void OnToolsAgentsPlay();
public:
   afx_msg void OnUpdateToolsImportTraces(CCmdUI *pCmdUI);
};

#ifndef _DEBUG
inline CGraphDoc* C3DView::GetDocument()
    { return (CGraphDoc*) m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_3DVIEW_H__225CD53B_5914_45A2_99AB_228D329EA91A__INCLUDED_)
