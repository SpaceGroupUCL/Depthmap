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

// GraphDoc.h : interface of the CGraphDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHDOC_H__71D5A06C_ED18_11D3_A8D5_00609787FC1E__INCLUDED_)
#define AFX_GRAPHDOC_H__71D5A06C_ED18_11D3_A8D5_00609787FC1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

UINT graphThread( LPVOID vparam );

class CMSCommunicator;

struct CFilePath {
   CString m_path;
   CString m_name;
   CString m_ext;
   CFilePath(const CString& pathname)
   {
      int dot   = pathname.ReverseFind('.');
      int slash = pathname.ReverseFind('\\');
      if (slash != -1) {
         m_path = pathname.Left(slash+1);
      }
      if (dot != -1) {
         m_name = pathname.Mid(slash+1,dot-slash-1);
         m_ext = pathname.Mid(dot+1);
         m_ext.MakeUpper();
      }
      else {
         m_name = pathname.Mid(slash+1);
      }
   }
};

class CGraphDoc : public CDocument
{
protected: // create from serialization only
	CGraphDoc();
	DECLARE_DYNCREATE(CGraphDoc)

// Attributes
public:
   CMSCommunicator *m_communicator;

   int m_make_algorithm;   // algorithm to make graph
   double m_make_maxdist;  // maximum distance you can see (set to -1.0 for infinite)

   MetaGraph *m_meta_graph;

   CString m_base_title;

   // Redraw commands
   enum {REDRAW_DONE, REDRAW_MARKEDSET, REDRAW_POINTS, REDRAW_GRAPH, REDRAW_TOTAL };
   // Redraw reasons
   enum {UNDECLARED, NEW_FOCUS, NEW_DEPTHMAPVIEW_SETUP,
                     NEW_VIEWER, NEW_LINESET, NEW_DATA,
                     NEW_SELECTION, NEW_TABLE, NEW_COLUMN, NEW_FILE, DELETED_TABLE };
   // Mainframe table changes:
   enum {CONTROLS_DESTROYALL,CONTROLS_LOADALL,
         CONTROLS_LOADGRAPH,CONTROLS_RELOADGRAPH,CONTROLS_LOADCONVERT,CONTROLS_LOADDRAWING,
         CONTROLS_LOADATTRIBUTES,CONTROLS_CHANGEATTRIBUTE};

   // Views attached (by viewtypes)
   enum {VIEW_ALL = 0, VIEW_MAP = 1, VIEW_SCATTER = 2, VIEW_TABLE = 3, VIEW_3D = 4, VIEW_TYPES = 5};
   CView *m_view[VIEW_TYPES];
   // now individual to each view
   bool m_flag_lock;
   int m_redraw_flag[VIEW_TYPES];
   bool SetRedrawFlag(int viewtype, int flag, int reason = UNDECLARED, CWnd *originator = NULL) // (almost) thread safe
   {
      if (!m_flag_lock) {
         m_flag_lock = true;
         if (viewtype) {
            // it's the view calling itself
            if (m_redraw_flag[viewtype] < flag) {
               m_view[viewtype]->PostMessage(WM_DMP_REDRAW_MESSAGE, (WPARAM) reason, (LPARAM) originator);
               m_redraw_flag[viewtype] = flag;
            }
            if (flag == REDRAW_DONE) {
               m_redraw_flag[viewtype] = flag;
            }
         }
         else {
            for (int i = 1; i < VIEW_TYPES; i++) {
               if (m_view[i] && m_redraw_flag[i] < flag) {
                  m_redraw_flag[i] = flag;
                  m_view[i]->PostMessage(WM_DMP_REDRAW_MESSAGE, (WPARAM) reason, (LPARAM) originator);
               }
            }
         }
         m_flag_lock = false;
         return true;
      }
      return false;
   }
   int GetRedrawFlag(int viewtype) const
   {
      return m_redraw_flag[viewtype];
   }
   bool m_remenu_flag[VIEW_TYPES];
   void SetRemenuFlag(int viewtype, bool on) {
      if (viewtype) {
         m_remenu_flag[viewtype] = on;
      }
      else {
         for (int i = 1; i < VIEW_TYPES; i++) {
            m_remenu_flag[i] = on;
         }
      }
   }
   bool GetRemenuFlag(int viewtype) const {
      return m_remenu_flag[viewtype];
   }
   void SetUpdateFlag(int type, bool modified = true)
   {
      switch (type) {
      case NEW_FILE:
         GetApp()->GetMainWnd()->PostMessage( WM_DMP_FOCUS_GRAPH, (WPARAM) this, CONTROLS_LOADALL );
         break;
      case NEW_DATA:
         GetApp()->GetMainWnd()->PostMessage( WM_DMP_FOCUS_GRAPH, (WPARAM) this, CONTROLS_LOADATTRIBUTES );
         break;
      case NEW_TABLE:
         GetApp()->GetMainWnd()->PostMessage( WM_DMP_FOCUS_GRAPH, (WPARAM) this, CONTROLS_LOADGRAPH );
         break;
      case DELETED_TABLE:
         GetApp()->GetMainWnd()->PostMessage( WM_DMP_FOCUS_GRAPH, (WPARAM) this, CONTROLS_RELOADGRAPH );
         break;
      }
      SetRemenuFlag(VIEW_ALL,true);     // Tell the views to update their menus
      if (modified) {
         SetModifiedFlag();
      }
   }
   Point2f m_position;  // Last known mouse position, in DXF units

   // Paths for the March 05 evolved agents
   // (loaded from file using the test button)
   prefvec<pvecpoint> m_evolved_paths;

// Operations
public:
   void OnFillPoints( const Point2f& p, int semifilled = 0 );
   void OnMakeIsovist(const Point2f& seed, double angle = -1.0);
   void OnToolsAxialMap( const Point2f& seed );
   //
   bool CheckMemory(const CString& filename = CString());
   //
   bool ViewHandler(int nCode, void *pExtra, int viewing, int layer = -1);
   //
   int RenameColumn(AttributeTable *tab, int col);
   bool ReplaceColumnContents(PointMap* pointmap, ShapeMap *shapemap, int col);
   bool SelectByQuery(PointMap* pointmap, ShapeMap *shapemap);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void OnCloseDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE);
	//}}AFX_VIRTUAL
   // my version
   BOOL OnSaveDocument(LPCTSTR lpszPathName, int version);

// Implementation
public:
	virtual ~CGraphDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:

   // my own:
   void OnToolsRunImportedAnalysis(CImportedModule& module, int analysis);
   afx_msg void OnUpdatePosition(CCmdUI* pCmdUI);
   afx_msg void OnUpdateDimensions(CCmdUI* pCmdUI);
   afx_msg void OnUpdatePoints(CCmdUI* pCmdUI);
	//{{AFX_MSG(CGraphDoc)
	afx_msg void OnFileImport();
	afx_msg void OnFileExport();
	afx_msg void OnUpdateFileExport(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsMakeGraph(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsRun(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileImport(CCmdUI* pCmdUI);
	afx_msg void OnToolsMakeGraph();
	afx_msg void OnEditClear();
	afx_msg void OnUpdateEditClear(CCmdUI* pCmdUI);
	afx_msg void OnVGAOptions();
	afx_msg void OnUpdateVGAOptions(CCmdUI* pCmdUI);
	afx_msg void OnToolsRun();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateToolsPD(CCmdUI* pCmdUI);
	afx_msg void OnToolsPD();
	afx_msg void OnToolsAxialLines();
	afx_msg void OnUpdateToolsAxialLines(CCmdUI* pCmdUI);
	afx_msg void OnAddGate();
	afx_msg void OnPushToLayer();
	afx_msg void OnEditGrid();
	afx_msg void OnUpdateEditGrid(CCmdUI* pCmdUI);
	afx_msg void OnEditFixgrid();
	afx_msg void OnUpdateEditFixgrid(CCmdUI* pCmdUI);
	afx_msg void OnEditFixFill();
	afx_msg void OnUpdateEditFixFill(CCmdUI* pCmdUI);
	afx_msg void OnToolsMPD();
	afx_msg void OnUpdateToolsMPD(CCmdUI* pCmdUI);
	afx_msg void OnTestButton();
	afx_msg void OnEvoAgent();
	afx_msg void OnBinDisplay();
	afx_msg void OnToolsMakeFewestLineMap();
	afx_msg void OnUpdateToolsMakeFLM(CCmdUI* pCmdUI);
	afx_msg void OnToolsRunAxa();
	afx_msg void OnUpdateToolsRunAxa(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePushToLayer(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddGate(CCmdUI* pCmdUI);
	afx_msg void OnToolsRunSeg();
	afx_msg void OnUpdateToolsRunSeg(CCmdUI* pCmdUI);
	afx_msg void OnAddColumn();
	afx_msg void OnUpdateAddColumn(CCmdUI* pCmdUI);
	afx_msg void OnRemoveColumn();
	afx_msg void OnUpdateRemoveColumn(CCmdUI* pCmdUI);
	afx_msg void OnMagiMif();
	afx_msg void OnUpdateMagiMif(CCmdUI* pCmdUI);
	afx_msg void OnBinDistances();
	afx_msg void OnShowBinDistances();
	afx_msg void OnFileProperties();
	afx_msg void OnToolsAPD();
	afx_msg void OnUpdateToolsAPD(CCmdUI* pCmdUI);
	afx_msg void OnViewShowGrid();
	afx_msg void OnUpdateViewShowGrid(CCmdUI* pCmdUI);
	afx_msg void OnViewShowText();
	afx_msg void OnUpdateViewShowText(CCmdUI* pCmdUI);
	afx_msg void OnToolsBoundaryToAxial();
	afx_msg void OnUpdateToolsBoundaryToAxial(CCmdUI* pCmdUI);
	afx_msg void OnViewSummary();
	afx_msg void OnUpdateViewSummary(CCmdUI* pCmdUI);
	afx_msg void OnSwapColours();
	afx_msg void OnUpdateSwapColours(CCmdUI* pCmdUI);
	afx_msg void OnRedButton();
	afx_msg void OnUpdateRedButton(CCmdUI* pCmdUI);
	afx_msg void OnToolsAxialClearLinks();
	afx_msg void OnUpdateToolsAxialClearLinks(CCmdUI* pCmdUI);
	afx_msg void OnUpdateColumn();
	afx_msg void OnUpdateUpdateColumn(CCmdUI* pCmdUI);
	afx_msg void OnToolsPointConvShapeMap();
	afx_msg void OnUpdateToolsPointConvShapeMap(CCmdUI* pCmdUI);
	afx_msg void OnToolsAxialConvShapeMap();
	afx_msg void OnUpdateToolsAxialConvShapeMap(CCmdUI* pCmdUI);
	afx_msg void OnViewScatterplot();
	afx_msg void OnUpdateViewScatterplot(CCmdUI* pCmdUI);
	afx_msg void OnWindowMap();
	afx_msg void OnUpdateWindowMap(CCmdUI* pCmdUI);
	afx_msg void OnViewTable();
	afx_msg void OnUpdateViewTable(CCmdUI* pCmdUI);
	afx_msg void OnWindow3dView();
	afx_msg void OnUpdateWindow3dView(CCmdUI* pCmdUI);
	afx_msg void OnToolsLineLoadUnlinks();
	afx_msg void OnUpdateToolsLineLoadUnlinks(CCmdUI* pCmdUI);
	afx_msg void OnToolsAgentRun();
	afx_msg void OnUpdateToolsAgentRun(CCmdUI* pCmdUI);
	afx_msg void OnRedtest();
	afx_msg void OnGreentest();
	afx_msg void OnRenameColumn();
	afx_msg void OnUpdateRenameColumn(CCmdUI* pCmdUI);
	afx_msg void OnEditQuery();
	afx_msg void OnUpdateEditQuery(CCmdUI* pCmdUI);
	afx_msg void OnColumnProperties();
	afx_msg void OnUpdateColumnProperties(CCmdUI* pCmdUI);
	afx_msg void OnPinktest();
	afx_msg void OnLayerNew();
	afx_msg void OnUpdateLayerNew(CCmdUI* pCmdUI);
	afx_msg void OnLayerDelete();
	afx_msg void OnUpdateLayerDelete(CCmdUI* pCmdUI);
	afx_msg void OnLayerConvert();
	afx_msg void OnUpdateLayerConvert(CCmdUI* pCmdUI);
	afx_msg void OnLayerConvertDrawing();
	afx_msg void OnUpdateLayerConvertDrawing(CCmdUI* pCmdUI);
	//}}AFX_MSG
   afx_msg void OnEditSelectToLayer();
   afx_msg void OnUpdateEditSelectToLayer(CCmdUI *pCmdUI);
   afx_msg void OnToolsIsovistpath();
   afx_msg void OnUpdateToolsIsovistpath(CCmdUI *pCmdUI);
   afx_msg void OnFileSave();
   afx_msg void OnFileSaveAs();
   //
	DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnConvertMapShapes();
public:
   afx_msg void OnUpdateConvertMapShapes(CCmdUI *pCmdUI);
public:
   afx_msg void OnToolsTopomet();
public:
   afx_msg void OnUpdateToolsTopomet(CCmdUI *pCmdUI);
public:
   afx_msg void OnToolsTPD();
public:
   afx_msg void OnUpdateToolsTPD(CCmdUI *pCmdUI);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHDOC_H__71D5A06C_ED18_11D3_A8D5_00609787FC1E__INCLUDED_)
