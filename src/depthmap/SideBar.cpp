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

// SideBar.cpp : implementation file
//

#include "stdafx.h"
#include "depthmap.h"
#include "mainfrm.h"
#include "SideBar.h"
#include "GraphDoc.h"
#include "ColumnPropertiesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const wstring editstatetext[] = {L"Not Editable", L"Editable Off", L"Editable On"};

/////////////////////////////////////////////////////////////////////////////
// CSideBar


CSideBar::CSideBar()
{
   m_resizing = false;
   m_treeDoc = NULL;
   m_topgraph = NULL;
   m_backgraph = NULL;
   m_old_cy = 0;
}

CSideBar::~CSideBar()
{
   GetApp()->WriteProfileInt(_T("Sidebar"),_T("Width"),m_sizeDefault.cx);
}


BEGIN_MESSAGE_MAP(CSideBar, CDialogBar)
	//{{AFX_MSG_MAP(CSideBar)
//	ON_WM_NCHITTEST()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_NOTIFY(TVN_SELCHANGING, IDC_SIDEBAR_TREE, OnSelchangingSidebarTree)
	ON_COMMAND(ID_POPUP_SIDEBAR_ATTR_DELETE, OnPopupSidebarAttrDelete)
	ON_COMMAND(ID_POPUP_SIDEBAR_ATTR_RENAME, OnPopupSidebarAttrRename)
	ON_COMMAND(ID_POPUP_SIDEBAR_ATTR_UPDATE, OnPopupSidebarAttrUpdate)
	ON_COMMAND(ID_POPUP_SIDEBAR_ATTR_PROPERTIES, OnPopupSidebarAttrProperties)
	//}}AFX_MSG_MAP
   ON_MESSAGE(WM_DMP_FOCUS_GRAPH, OnFocusGraph)
   ON_WM_NCHITTEST()
   ON_NOTIFY(NM_CLICK, IDC_ATTRIBUTE_LIST, &CSideBar::OnAttributeListNMClick)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSideBar message handlers

// This is the MSVC 6 version
//UINT CSideBar::OnNcHitTest(CPoint point) 
//{
//   CRect rect;
//   GetWindowRect(rect);
//
//   if (point.x > rect.right - 4) {	
//      return HTRIGHT;
//   }
//   else {
//	   return CDialogBar::OnNcHitTest(point);
//   }
//}

// CONVPAF -- this is the MSVC 8 version (seems to use LRESULT not UINT
LRESULT CSideBar::OnNcHitTest(CPoint point)
{
   CRect rect;
   GetWindowRect(rect);

   if (point.x > rect.right - 4) {
      // grab the border
      return HTRIGHT;
   }
   else {
      return CDialogBar::OnNcHitTest(point);
   }
}

void CSideBar::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
   if (nHitTest == HTRIGHT) {
      m_resizing = true;
      CRect rect;
      // complicated: client rect of m_pDockSite is misleading:
      GetWindowRect(m_dragrect);
      m_pDockSite->GetWindowRect(rect);
      m_dragrect.OffsetRect(-rect.left,-rect.top);
      m_dragrect.left = m_dragrect.right - 4;
      CWindowDC dc(m_pDockSite);
      CSize thickness(2,2);
      dc.DrawDragRect(m_dragrect, thickness, NULL, thickness);
      SetCapture();
   }
   else {   
	   CDialogBar::OnNcLButtonDown(nHitTest, point);
   }
}

void CSideBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
   if (m_resizing) {
      m_resizing = false;
      CWindowDC dc(m_pDockSite);
      CSize thickness(2,2);
      dc.DrawDragRect(m_dragrect, thickness, NULL, thickness);
      ReleaseCapture();
      m_sizeDefault.cx = point.x;
      if (m_sizeDefault.cx < 32) {
         m_sizeDefault.cx = 32;
      }
      m_pDockSite->RepositionBars(0,0xFFFF,AFX_IDW_PANE_FIRST);
   }
	
	CDialogBar::OnLButtonUp(nFlags, point);
}

void CSideBar::OnMouseMove(UINT nFlags, CPoint point) 
{
   if (m_resizing) {
      CWindowDC dc(m_pDockSite);
      CRect nextrect = m_dragrect;
      nextrect.left = point.x;
      if (nextrect.left < 32) {
         nextrect.left = 32;
      }
      nextrect.right = nextrect.left + 4;
      CSize thickness(2,2);
      dc.DrawDragRect(nextrect, thickness, m_dragrect, thickness);
      m_dragrect = nextrect;
   }
   
	CDialogBar::OnMouseMove(nFlags, point);
}

void CSideBar::OnSize(UINT nType, int cx, int cy) 
{
   bool grow = (cy > m_old_cy);
   m_old_cy = cy;
	CDialogBar::OnSize(nType, cx, cy);
   int middlebar = (cy-12) / 3;

   // move in order according to move:
   if (grow) {
      // grow any other controls first
      // then:
      // size the side bar tree
      CWnd *tree = GetDlgItem(IDC_SIDEBAR_TREE);
      if (tree && ::IsWindow(tree->m_hWnd)) {
         tree->MoveWindow(2,4,cx-4,middlebar-4);
      }
      CListCtrl *list = (CListCtrl *)GetDlgItem(IDC_ATTRIBUTE_LIST);
      if (list && ::IsWindow(list->m_hWnd)) {
         list->MoveWindow(2,middlebar+4,cx-4,cy-middlebar-8);
      }
   }
   else {
      // size the side bar tree
      CWnd *tree = GetDlgItem(IDC_SIDEBAR_TREE);
      if (tree && ::IsWindow(tree->m_hWnd)) {
         tree->MoveWindow(2,4,cx-4,middlebar-4);
      }
      CListCtrl *list = (CListCtrl *) GetDlgItem(IDC_ATTRIBUTE_LIST);
      if (list&& ::IsWindow(list->m_hWnd)) {
         list->MoveWindow(2,middlebar+4,cx-4,cy-middlebar-8);
      }
      // then reduce any other controls
   }
}

BOOL CSideBar::Create( CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID )
{
   BOOL retvar = CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);

   m_sizeDefault.cx = GetApp()->GetProfileInt(_T("Sidebar"),_T("Width"),120);

   if (retvar) {
      m_stateTreeImages.Create(IDB_SIDETREEICONS, 14, 0, RGB(255, 0, 255));
      GetTreeCtrl().SetImageList(&m_stateTreeImages, TVSIL_NORMAL);
      GetTreeCtrl().SetBkColor( RGB(255,255,255) );
      m_stateListImages.Create(IDB_SIDELISTICONS, 14, 0, RGB(255, 0, 255));
      GetListCtrl().SetImageList(&m_stateListImages, LVSIL_SMALL);
      GetListCtrl().SetBkColor( RGB(255,255,255) );
      GetListCtrl().InsertColumn(0,_T("Attribute"));
   }

   return retvar;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

LRESULT CSideBar::OnFocusGraph(WPARAM wParam, LPARAM lParam)
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

   // Replacement for m_linelayer_chooser is my tree ctrl:
   CTreeCtrl& tree = GetTreeCtrl();
   CListCtrl& list = GetListCtrl();
   if (::IsWindow(tree.m_hWnd)) {
      CGraphDoc *pDoc = (CGraphDoc *) wParam;
      if (lParam == CGraphDoc::CONTROLS_DESTROYALL && pDoc == m_treeDoc) {      // Lost graph
         m_topgraph = NULL;
         m_backgraph = NULL;
         tree.DeleteAllItems();
         list.DeleteAllItems();
         list.SetColumnWidth(0,0);
         m_treeDoc = NULL;
      }
      else if (lParam == CGraphDoc::CONTROLS_LOADALL && pDoc != m_treeDoc) {    // [Possible] change of window (sent on focus)
         m_treeDoc = pDoc;
         m_topgraph = NULL;
         m_backgraph = NULL;
         MakeTree();
      }
      else if (lParam == CGraphDoc::CONTROLS_LOADGRAPH && pDoc == m_treeDoc) {    // Force update if match current window
         m_topgraph = NULL;
         m_backgraph = NULL;
         MakeGraphTree();
         // also make drawing tree as this overrides layer visible status sometimes:
         MakeDrawingTree();
      }
      else if (lParam == CGraphDoc::CONTROLS_RELOADGRAPH && pDoc == m_treeDoc) {    // Force reload of graph tree if match current window
         m_topgraph = NULL;
         m_backgraph = NULL;
         ClearGraphTree();
         MakeGraphTree();
      }
      else if (lParam == CGraphDoc::CONTROLS_LOADDRAWING && pDoc == m_treeDoc) {    // Force update if match current window
         MakeDrawingTree();
      }
      else if (lParam == CGraphDoc::CONTROLS_LOADATTRIBUTES && pDoc == m_treeDoc) {    // Force update if match current window
         MakeAttributeList();
      }
      else if (lParam == CGraphDoc::CONTROLS_CHANGEATTRIBUTE && pDoc == m_treeDoc) {    // Force update if match current window
         SetAttributeChecks();
      }
      else if (lParam == CGraphDoc::CONTROLS_LOADCONVERT && pDoc == m_treeDoc) {
         m_topgraph = NULL;
         m_backgraph = NULL;
         MakeGraphTree();
         // conversions typically turn off drawing layers:
         SetDrawingTreeChecks();
      }
      if (m_treeDoc == NULL) {
         tree.EnableWindow(FALSE);
         // Stop some strange auto scroll property:
         SetTreeStyle(TVS_NOSCROLL, TRUE);
      }
      else {
         tree.EnableWindow(TRUE);
         // Stop some strange auto scroll property:
         SetTreeStyle(TVS_NOSCROLL, FALSE);
      }
   }

   return 0;
}


// Tree stuff

void CSideBar::MakeTree()
{
   CTreeCtrl& tree = GetTreeCtrl();

   tree.DeleteAllItems();
   m_treegraphmap.clear();
   m_treedrawingmap.clear();
   for (int i = 0; i < 5; i++) {
      m_treeroots[i] = NULL;
   }

   MetaGraph *graph = m_treeDoc->m_meta_graph;
   if (!graph) {
      return;
   }

   int state = graph->getState();
   int viewclass = graph->getViewClass();

   MakeGraphTree();
   MakeDrawingTree();
}

void CSideBar::SetTreeStyle(long lStyleMask, BOOL bSetBits)
{
   CTreeCtrl& tree = GetTreeCtrl();
	long  lStyleOld;
	lStyleOld = GetWindowLong(tree.m_hWnd, GWL_STYLE);
	lStyleOld &= ~lStyleMask;
	if (bSetBits)
		lStyleOld |= lStyleMask;
	SetWindowLong(tree.m_hWnd, GWL_STYLE, lStyleOld);
	tree.SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
}

void CSideBar::OnSelchangingSidebarTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
   
   switch (pNMTreeView->action) {
   case TVC_UNKNOWN: case TVC_BYMOUSE: case TVC_BYKEYBOARD:
      break;
   default:
      // shouldn't have received this, ignore completely:
      *pResult = TRUE;
      return;
   }

   CTreeCtrl& tree = GetTreeCtrl();
   HTREEITEM hItem = pNMTreeView->itemNew.hItem;

   // since this may happen at any time, should really check for anything going badly wrong,
   // e.g., no current graph document:
   CMDIFrameWnd *pFrame = (CMDIFrameWnd*) AfxGetApp()->m_pMainWnd;
   CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();
   CDocument *pDoc = pChild->GetActiveDocument();
   if (pDoc != m_treeDoc) {
      return;
   }
   MetaGraph *graph = m_treeDoc->m_meta_graph;

   // look it up in the table to see what to do:
   size_t n = m_treegraphmap.searchindex(hItem);
   if (n != paftl::npos) {
      CTreeEntry entry = m_treegraphmap.value(n);
      bool remenu = false;
      if (entry.m_cat != -1) {
         if (entry.m_subcat == -1) {
            switch (entry.m_type) {
            case 0:
               if (graph->getViewClass() & MetaGraph::VIEWVGA) {
                  if (graph->getDisplayedPointMapRef() == entry.m_cat) {
                     graph->setViewClass(MetaGraph::SHOWHIDEVGA);
                  }
                  else {
                     graph->setDisplayedPointMapRef(entry.m_cat);
                  }
               }
               else {
                  graph->setDisplayedPointMapRef(entry.m_cat);
                  graph->setViewClass(MetaGraph::SHOWVGATOP);
               }
               remenu = true;
               break;
            case 1:
               if (graph->getViewClass() & MetaGraph::VIEWAXIAL) {
                  if (graph->getShapeGraphs().getDisplayedMapRef() == entry.m_cat) {
                     graph->setViewClass(MetaGraph::SHOWHIDEAXIAL);
                  }
                  else {
                     graph->getShapeGraphs().setDisplayedMapRef(entry.m_cat);
                  }
               }
               else {
                  graph->getShapeGraphs().setDisplayedMapRef(entry.m_cat);
                  graph->setViewClass(MetaGraph::SHOWAXIALTOP);
               }
               remenu = true;
               break;
            case 2:
               if (graph->getViewClass() & MetaGraph::VIEWDATA) {
                  if (graph->getDataMaps().getDisplayedMapRef() == entry.m_cat) {
                     graph->setViewClass(MetaGraph::SHOWHIDESHAPE);
                  }
                  else {
                     graph->getDataMaps().setDisplayedMapRef(entry.m_cat);
                  }
               }
               else {
                  graph->getDataMaps().setDisplayedMapRef(entry.m_cat);
                  graph->setViewClass(MetaGraph::SHOWSHAPETOP);
               }
               remenu = true;
               break;
            case 4:
               // slightly different for this one
               break;
            }
            if (remenu) {
               SetGraphTreeChecks();
               m_treeDoc->SetRemenuFlag(CGraphDoc::VIEW_ALL, true);
               GetApp()->GetMainWnd()->PostMessage( WM_DMP_FOCUS_GRAPH, (WPARAM) m_treeDoc, CGraphDoc::CONTROLS_CHANGEATTRIBUTE );
            }
            m_treeDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_TABLE );
         }
         else if (entry.m_subcat == -2) {
            // hit editable box
            if (SetEditable(entry)) {
               // update required (but probably not a redraw of the views)
               SetGraphTreeChecks();
            }
         }
         else {
            // They've clicked on the displayed layers
            bool update = false;
            if (entry.m_type == 1) {
               bool show = graph->getShapeGraphs().getMap(entry.m_cat).isLayerVisible(entry.m_subcat);
               graph->getShapeGraphs().getMap(entry.m_cat).setLayerVisible(entry.m_subcat, !show);
               update = true;
            }
            else if (entry.m_type == 2) {
               bool show = graph->getDataMaps().getMap(entry.m_cat).isLayerVisible(entry.m_subcat);
               graph->getDataMaps().getMap(entry.m_cat).setLayerVisible(entry.m_subcat, !show);
               update = true;
            }
            if (update) {
               // Update required for the check boxes
               SetGraphTreeChecks();
               // Depending on if the map is displayed you may have to redraw -- I'm just going to redraw *anyway*
               // (it may be worth switching it to topmost when they do click here)
               m_treeDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_TABLE );
            }
         }
      }
   }
   else {
      size_t n = m_treedrawingmap.searchindex(hItem);
      if (n != paftl::npos) {
         CTreeEntry entry = m_treedrawingmap.value(n);
         if (entry.m_subcat != -1) {
            if (graph->getLineLayer(entry.m_cat,entry.m_subcat).isShown()) {
               graph->getLineLayer(entry.m_cat,entry.m_subcat).setShow(false);
               graph->PointMaps::redoBlockLines();
               graph->resetBSPtree();
               tree.SetItemImage(hItem,13,13);
            }
            else {
               graph->getLineLayer(entry.m_cat,entry.m_subcat).setShow(true);
               graph->PointMaps::redoBlockLines();
               graph->resetBSPtree();
               tree.SetItemImage(hItem,12,12);
            }
         }
         m_treeDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_LINESET );
      }
   }

   // this doesn't appear to work (at least not always)...
   SwitchFocusBack();

   // we handled it... don't let it select:
	*pResult = TRUE;
}

void CSideBar::SetGraphTreeChecks()
{
   CTreeCtrl& tree = GetTreeCtrl();
   MetaGraph *graph = m_treeDoc->m_meta_graph;
   int viewclass = graph->getViewClass();
   for (size_t i = 0; i < m_treegraphmap.size(); i++) {
      HTREEITEM key = m_treegraphmap.key(i);
      CTreeEntry entry = m_treegraphmap[i];
      int checkstyle = 7;
      if (entry.m_cat != -1) {
         if (entry.m_subcat == -1) {
            // this is the main type box hit
            switch (entry.m_type) {
               case 0:
                  if (viewclass & MetaGraph::VIEWVGA && graph->getDisplayedPointMapRef() == entry.m_cat) {
                     checkstyle = 5;
                     m_topgraph = key;
                  }
                  else if (viewclass & MetaGraph::VIEWBACKVGA && graph->getDisplayedPointMapRef() == entry.m_cat) {
                     checkstyle = 6;
                     m_backgraph = key;
                  }
                  break;
               case 1:
                  if (viewclass & MetaGraph::VIEWAXIAL && graph->getShapeGraphs().getDisplayedMapRef() == entry.m_cat) {
                     checkstyle = 5;
                     m_topgraph = key;
                  }
                  else if (viewclass & MetaGraph::VIEWBACKAXIAL && graph->getShapeGraphs().getDisplayedMapRef() == entry.m_cat) {
                     checkstyle = 6;
                     m_backgraph = key;
                  }
                  break;
               case 2:
                  if (viewclass & MetaGraph::VIEWDATA && graph->getDataMaps().getDisplayedMapRef() == entry.m_cat) {
                     checkstyle = 5;
                     m_topgraph = key;
                  }
                  else if (viewclass & MetaGraph::VIEWBACKDATA && graph->getDataMaps().getDisplayedMapRef() == entry.m_cat) {
                     checkstyle = 6;
                     m_backgraph = key;
                  }
                  break;
            }
            tree.SetItemImage(key,checkstyle,checkstyle);
         }
         else if (entry.m_subcat == -2) {
            // the editable box
            int editable = MetaGraph::NOT_EDITABLE;
            switch (entry.m_type) {
               case 0:
                  if (graph->PointMaps::at(entry.m_cat).isProcessed()) {
                     editable = MetaGraph::NOT_EDITABLE;
                  }
                  else {
                     editable = MetaGraph::EDITABLE_ON;
                  }
                  break;
               case 1:
                  {
                     int type = graph->getShapeGraphs().getMap(entry.m_cat).getMapType();
                     if (type == ShapeMap::SEGMENTMAP || type == ShapeMap::ALLLINEMAP) {
                        editable = MetaGraph::NOT_EDITABLE;
                     }
                     else {
                        editable = graph->getShapeGraphs().getMap(entry.m_cat).isEditable() ? MetaGraph::EDITABLE_ON : MetaGraph::EDITABLE_OFF;
                     }
                  }
                  break;
               case 2:
                  editable = graph->getDataMaps().getMap(entry.m_cat).isEditable() ? MetaGraph::EDITABLE_ON : MetaGraph::EDITABLE_OFF;
                  break;
            }
            switch (editable) {
               case MetaGraph::NOT_EDITABLE:
                  tree.SetItemImage(key,16,16);
                  tree.SetItemText(key,editstatetext[0].c_str());
                  break;
               case MetaGraph::EDITABLE_OFF:
                  tree.SetItemImage(key,15,15);
                  tree.SetItemText(key,editstatetext[1].c_str());
                  break;
               case MetaGraph::EDITABLE_ON:
                  tree.SetItemImage(key,14,14);
                  tree.SetItemText(key,editstatetext[2].c_str());
               break;
            }
         }
         else {
            // the displayed layers (note that VGA graphs (type 0)
            // do not currently have layers supported
            bool show = false;
            if (entry.m_type == 1) {
               show = graph->getShapeGraphs().getMap(entry.m_cat).isLayerVisible(entry.m_subcat);
            }
            else if (entry.m_type == 2) {
               show = graph->getDataMaps().getMap(entry.m_cat).isLayerVisible(entry.m_subcat);
            }
            if (show) {
               tree.SetItemImage(key,12,12);
            }
            else {
               tree.SetItemImage(key,13,13);
            }
         }
      }
   }
   MakeAttributeList();
}

void CSideBar::SetDrawingTreeChecks()
{
   CTreeCtrl& tree = GetTreeCtrl();
   MetaGraph *graph = m_treeDoc->m_meta_graph;
   int viewclass = graph->getViewClass();
   for (size_t i = 0; i < m_treedrawingmap.size(); i++) {
      CTreeEntry entry = m_treedrawingmap[i];
      if (entry.m_subcat != -1) {
         if (graph->getLineLayer(entry.m_cat,entry.m_subcat).isShown()) {
            tree.SetItemImage(m_treedrawingmap.key(i),12,12);
         }
         else {
            tree.SetItemImage(m_treedrawingmap.key(i),13,13);
         }
      }
   }
}

// clear the graph tree (not the drawing tree) but also clear the attribute list

void CSideBar::ClearGraphTree()
{
   CListCtrl& list = GetListCtrl();
   list.DeleteAllItems();
   m_attribute_locked.clear();

   CTreeCtrl& tree = GetTreeCtrl();

   for (int i = 2; i >= 0; i--) {
      if (m_treeroots[i]) {
         tree.DeleteItem(m_treeroots[i]);
         m_treeroots[i] = NULL;
      }
   }
   m_treegraphmap.clear();
}

void CSideBar::MakeGraphTree()
{
   CTreeCtrl& tree = GetTreeCtrl();
   MetaGraph *graph = m_treeDoc->m_meta_graph;

   int state = graph->getState();
   int viewclass = graph->getViewClass();

   if (state & MetaGraph::POINTMAPS) {
      if (!m_treeroots[0]) {
         HTREEITEM root = tree.InsertItem(_T("Visibility Graphs"),TVI_ROOT,TVI_FIRST);
         tree.SetItemImage(root,0,0);
         CTreeEntry entry(0,-1,-1);
         m_treegraphmap.add(root,entry);
         m_treeroots[0] = root;
      }
      HTREEITEM hItem = tree.GetChildItem(m_treeroots[0]);
      for (size_t i = 0; i < m_treeDoc->m_meta_graph->PointMaps::size(); i++) {
         CString name = CString(m_treeDoc->m_meta_graph->PointMaps::at(i).getName().c_str());
         if (hItem == NULL) {
            hItem = tree.InsertItem(name,m_treeroots[0]);
            CTreeEntry entry(0,(short)i,-1);
            m_treegraphmap.add(hItem,entry);
            {
               HTREEITEM hNewItem;
               CTreeEntry newentry = CTreeEntry(0,(short)i,-2);
               hNewItem = tree.InsertItem(_T("Editable"),hItem);
               m_treegraphmap.add(hNewItem,newentry);
            }
         }
         else if (tree.GetItemText(hItem) != name) {
            tree.SetItemText(hItem,name);
         }
         hItem = tree.GetNextSiblingItem(hItem);
      }
      while (hItem != NULL) {
         HTREEITEM hItemDestroy = hItem;
         hItem = tree.GetNextSiblingItem(hItem);
         tree.DeleteItem(hItemDestroy);
         m_treegraphmap.remove(hItemDestroy);
      }
      tree.Expand(m_treeroots[0], TVE_EXPAND);
   }
   else if (m_treeroots[0]) {
      tree.DeleteItem(m_treeroots[0]);
      m_treegraphmap.remove(m_treeroots[0]);
      m_treeroots[0] = NULL;
   }

   if (state & MetaGraph::SHAPEGRAPHS) {
      if (!m_treeroots[1]) {
         HTREEITEM root = tree.InsertItem(_T("Shape Graphs"),TVI_ROOT,m_treeroots[0]?m_treeroots[0]:TVI_FIRST);
         tree.SetItemImage(root,1,1);
         CTreeEntry entry(1,-1,-1);
         m_treegraphmap.add(root,entry);
         m_treeroots[1] = root;
      }
      HTREEITEM hItem = tree.GetChildItem(m_treeroots[1]);
      for (size_t i = 0; i < m_treeDoc->m_meta_graph->getShapeGraphs().getMapCount(); i++) {
         CString name = CString(m_treeDoc->m_meta_graph->getShapeGraphs().getMap(i).getName().c_str());
         if (hItem == NULL) {
            hItem = tree.InsertItem(name,m_treeroots[1]);
            CTreeEntry entry(1,(short)i,-1);
            m_treegraphmap.add(hItem,entry);
            {
               HTREEITEM hNewItem;
               CTreeEntry newentry = CTreeEntry(1,(short)i,-2);
               hNewItem = tree.InsertItem(_T("Editable"),hItem);
               m_treegraphmap.add(hNewItem,newentry);
            }
         }
         else if (tree.GetItemText(hItem) != name) {
            tree.SetItemText(hItem,name);
         }
         HTREEITEM hNewItem = tree.GetChildItem(hItem);
         // skip the editable node
         hNewItem = tree.GetNextSiblingItem(hNewItem);
         // now add layers
         AttributeTable& table = m_treeDoc->m_meta_graph->getShapeGraphs().getMap(i).getAttributeTable();
         for (int j = 0; j < table.getLayerCount(); j++) {
            CString name = CString(table.getLayerName(j).c_str());
            if (hNewItem == NULL) {
               hNewItem = tree.InsertItem(name,hItem);
               CTreeEntry entry(1,(short)i,j);
               m_treegraphmap.add(hNewItem,entry);
            }
            else if (tree.GetItemText(hNewItem) != name) {
               tree.SetItemText(hNewItem,name);
            }
            hNewItem = tree.GetNextSiblingItem(hNewItem);
         }
         hItem = tree.GetNextSiblingItem(hItem);
      }
      while (hItem != NULL) {
         HTREEITEM hItemDestroy = hItem;
         hItem = tree.GetNextSiblingItem(hItem);
         tree.DeleteItem(hItemDestroy);
      }
      tree.Expand(m_treeroots[1], TVE_EXPAND);
   }
   else if (m_treeroots[1]) {
      tree.DeleteItem(m_treeroots[1]);
      m_treegraphmap.remove(m_treeroots[1]);
      m_treeroots[1] = NULL;
   }

   if (state & MetaGraph::DATAMAPS) {
      if (!m_treeroots[2]) {
         HTREEITEM root = tree.InsertItem(_T("Data Maps"),TVI_ROOT,m_treeroots[1]?m_treeroots[1]:(m_treeroots[0]?m_treeroots[0]:TVI_FIRST));
         tree.SetItemImage(root,2,2);
         CTreeEntry entry(2,-1,-1);
         m_treegraphmap.add(root,entry);
         m_treeroots[2] = root;
      }
      HTREEITEM hItem = tree.GetChildItem(m_treeroots[2]);
      for (size_t i = 0; i < m_treeDoc->m_meta_graph->getDataMaps().getMapCount(); i++) {
         CString name = CString(m_treeDoc->m_meta_graph->getDataMaps().getMap(i).getName().c_str());
         if (hItem == NULL) {
            hItem = tree.InsertItem(name,m_treeroots[2]);
            CTreeEntry entry(2,(short)i,-1);
            m_treegraphmap.add(hItem,entry);
            {
               HTREEITEM hNewItem;
               CTreeEntry newentry = CTreeEntry(2,(short)i,-2);
               hNewItem = tree.InsertItem(_T("Editable"),hItem);
               m_treegraphmap.add(hNewItem,newentry);
            }
         }
         else if (tree.GetItemText(hItem) != name) {
            tree.SetItemText(hItem,name);
         }
         HTREEITEM hNewItem = tree.GetChildItem(hItem);
         // skip the editable node
         hNewItem = tree.GetNextSiblingItem(hNewItem);
         // now add layers
         AttributeTable& table = m_treeDoc->m_meta_graph->getDataMaps().getMap(i).getAttributeTable();
         for (int j = 0; j < table.getLayerCount(); j++) {
            CString name = CString(table.getLayerName(j).c_str());
            if (hNewItem == NULL) {
               hNewItem = tree.InsertItem(name,hItem);
               CTreeEntry entry(2,(short)i,j);
               m_treegraphmap.add(hNewItem,entry);
            }
            else if (tree.GetItemText(hNewItem) != name) {
               tree.SetItemText(hNewItem,name);
            }
            hNewItem = tree.GetNextSiblingItem(hNewItem);
         }
         hItem = tree.GetNextSiblingItem(hItem);
      }
      while (hItem != NULL) {
         HTREEITEM hItemDestroy = hItem;
         hItem = tree.GetNextSiblingItem(hItem);
         tree.DeleteItem(hItemDestroy);
      }
      tree.Expand(m_treeroots[2], TVE_EXPAND);
   }
   else if (m_treeroots[2]) {
      tree.DeleteItem(m_treeroots[2]);
      m_treegraphmap.remove(m_treeroots[2]);
      m_treeroots[2] = NULL;
   }

   SetGraphTreeChecks();
}

/*
      // not required any more
      // 0 is no edit allowed, 1 edit allowed but off, 2 edit allowed and on
      // int editstate = graph->isEditable();

// code for setting initial editstate
               hItem = tree.InsertItem(editstatetext[editstate].c_str(),root);
               CTreeEntry entry(i,mapref,-2);
               m_attributemap.add(hItem,entry);
*/

void CSideBar::MakeDrawingTree()
{
   CTreeCtrl& tree = GetTreeCtrl();
   MetaGraph *graph = m_treeDoc->m_meta_graph;

   int state = graph->getState();

   if (state & MetaGraph::LINEDATA) {
      if (m_treeroots[4]) {
         tree.DeleteItem(m_treeroots[4]);
         m_treeroots[4] = NULL;
         m_treedrawingmap.clear();
      }
      // we'll do all of these if it works...
      HTREEITEM root = tree.InsertItem(_T("Drawing Layers"));
      tree.SetItemImage(root,4,4);
      CTreeEntry entry(4,0,-1);
      m_treedrawingmap.add(root,entry);
      m_treeroots[4] = root;
        
      for (int i = 0; i < m_treeDoc->m_meta_graph->getLineFileCount(); i++) {
         HTREEITEM subroot = tree.InsertItem(CString(m_treeDoc->m_meta_graph->getLineFileName(i).c_str()),root);
         tree.SetItemImage(subroot,8,8);
         CTreeEntry entry(4,i,-1);
         m_treedrawingmap.add(subroot,entry);

         for (int j = 0; j < m_treeDoc->m_meta_graph->getLineLayerCount(i); j++) {
            CString name(m_treeDoc->m_meta_graph->getLineLayer(i,j).getName().c_str());
            HTREEITEM hItem = tree.InsertItem(name, subroot);
            if (m_treeDoc->m_meta_graph->getLineLayer(i,j).isShown()) {
               tree.SetItemImage(hItem,12,12);
            }
            else {
               tree.SetItemImage(hItem,13,13);
            }
            CTreeEntry entry(4,i,j);
            m_treedrawingmap.add(hItem,entry);
         }
      }
   }
}

void CSideBar::MakeAttributeList()
{
   MetaGraph *graph = m_treeDoc->m_meta_graph;
   if (graph == NULL) {
      return;
   }
   if (graph->setLock(this)) {

      // just doing this the simple way to start off with
      // (when you add new attributes, list is cleared and re
      CListCtrl& list = GetListCtrl();
      m_attribute_locked.clear();
      list.DeleteAllItems();

      int cx = 0;
      // LVITEM item; // no longer required?
      CString name;
      if (graph->viewingProcessed()) {
         const AttributeTable& table = graph->getAttributeTable();
         list.InsertItem(0,_T("Ref Number"));
         m_attribute_locked.push_back(true);
         for (int i = 0; i < table.getColumnCount(); i++) {
            name = CString(table.getColumnName(i).c_str());
            int w = list.GetStringWidth(name) + 22; // + 14 is icon width + 8 arbitrary spacer
            if (w > cx) {
               cx = w;
            }
            list.InsertItem(i+1,name);
            m_attribute_locked.push_back(table.isColumnLocked(i));
         }
      }
      list.SetColumnWidth(0,cx);

      graph->releaseLock(this);
   }


   SetAttributeChecks();
}

void CSideBar::SetAttributeChecks()
{
   MetaGraph *graph = m_treeDoc->m_meta_graph;
   if (graph == NULL) {
      return;
   }
   CListCtrl& list = GetListCtrl();

   if (graph->viewingProcessed()) {
            
      int displayed_attribute = graph->getDisplayedAttribute();

      LVITEM item;
      for (int i = 0; i < list.GetItemCount(); i++) {
         item.iItem = i;
         item.iSubItem = 0;
         item.iImage = 0;
         item.mask = LVIF_IMAGE;
         list.GetItem(&item);
         int image = item.iImage;
         if ((i-1) == displayed_attribute) {
            if (!m_attribute_locked[i]) {
               image = 0;
            }
            else {
               image = 2;
            }
         }
         else {
            if (!m_attribute_locked[i]) {
               image = 1;
            }
            else {
               image = 3;
            }
         }
         if (item.iImage != image) {
            item.iImage = image;
            list.SetItem(&item);
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////////

bool CSideBar::SetEditable(CTreeEntry entry)
{
   MetaGraph *graph = m_treeDoc->m_meta_graph;
   CTreeCtrl& tree = GetTreeCtrl();

   bool update = false;

   switch (entry.m_type) {
      case 0:
         // nothing -- it can't be changed!
         break;
      case 1:
         {
            int type = graph->getShapeGraphs().getMap(entry.m_cat).getMapType();
            if (type != ShapeMap::SEGMENTMAP && type != ShapeMap::ALLLINEMAP) {
               bool editable = graph->getShapeGraphs().getMap(entry.m_cat).isEditable();
               graph->getShapeGraphs().getMap(entry.m_cat).setEditable(!editable);
               update = true;
            }
         }
         break;
      case 2:
         {
            bool editable = graph->getDataMaps().getMap(entry.m_cat).isEditable();
            graph->getDataMaps().getMap(entry.m_cat).setEditable(!editable);
            update = true;
         }
         break;
   }
   return update;
}

///////////////////////////////////////////////////////////////////////////////////

void CSideBar::OnContextMenu(CWnd* pWnd, CPoint point) 
{
   // Need to check level really (oo, yes, whoopy doo! -- use this to allow colour changes to the line layers!)
   CTreeCtrl& tree = GetTreeCtrl();
   CListCtrl& list = GetListCtrl();

   CPoint cpoint = point;
   tree.ScreenToClient(&cpoint);

   TVHITTESTINFO HitTestInfo;
   HitTestInfo.pt = cpoint;
   tree.HitTest(&HitTestInfo);

   if (HitTestInfo.flags & TVHT_ONITEM) {

      // look it up in the table to see what to do:
      size_t n = m_treegraphmap.searchindex(HitTestInfo.hItem);
      if (n != paftl::npos) {
         /*
         DISABLE FOR TIME BEING: REINSERT FOR RENAME / DELETION OF MAPS
         m_popup_entry = m_treegraphmap.value(n);
         if (m_popup_entry.m_cat != -1 && m_popup_entry.m_subcat == -1) {
            // Context menu for this item (map renamer / deleter)
            CMenu menu;
            VERIFY(menu.LoadMenu(IDR_POPUP_SIDEBAR));
            CMenu *pPopup = menu.GetSubMenu(0);
            ASSERT(pPopup != NULL);
            pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
         }
         */
      }
   }

   cpoint = point;
   list.ScreenToClient(&cpoint);

   LVHITTESTINFO HitTestInfoB;
   HitTestInfoB.pt = cpoint;
   list.HitTest(&HitTestInfoB);

   if (HitTestInfoB.flags & (LVHT_ONITEMICON|LVHT_ONITEMLABEL)) {
      int n = HitTestInfoB.iItem;
      m_popup_attribute = n - 1;
      // Context menu for this item (map renamer / deleter)
      CMenu menu;
      VERIFY(menu.LoadMenu(IDR_POPUP_SIDEBAR));
      CMenu *pPopup;
      if (!m_attribute_locked[n]) {
         pPopup = menu.GetSubMenu(1);
      }
      else {
         pPopup = menu.GetSubMenu(2);
      }
      ASSERT(pPopup != NULL);
      pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
   }
}

void CSideBar::OnPopupSidebarAttrDelete() 
{
   CTreeCtrl& tree = GetTreeCtrl();
   // since this may happen at any time, should really check for anything going badly wrong,
   // e.g., no current graph document:
   CMDIFrameWnd *pFrame = (CMDIFrameWnd*) AfxGetApp()->m_pMainWnd;
   CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();
   CDocument *pDoc = pChild->GetActiveDocument();
   if (pDoc != m_treeDoc) {
      return;
   }
   MetaGraph *graph = m_treeDoc->m_meta_graph;
   if (graph->viewingProcessed()) {
      AttributeTable& table = graph->getAttributeTable();
      if (table.isColumnLocked(m_popup_attribute)) {
         AfxMessageBox(_T("Cannot remove locked column"));
      }
      else if (IDYES == AfxMessageBox(CString("Are you sure you want to delete the ") + table.getColumnName(m_popup_attribute).c_str() + CString(" column?"), MB_YESNO)) {
         bool displayed = graph->getDisplayedAttribute() == m_popup_attribute;
         // note this -1 simply means shift back one
         if (displayed) {
            // note, cannot delete the ref column, so minimum is display -1
            graph->setDisplayedAttribute(m_popup_attribute - 1);
         }
         int view_class = graph->getViewClass() & (MetaGraph::VIEWVGA | MetaGraph::VIEWAXIAL | MetaGraph::VIEWDATA);
         switch (view_class) {
         case MetaGraph::VIEWVGA: case MetaGraph::VIEWBACKVGA:
            graph->PointMaps::getDisplayedPointMap().removeAttribute(m_popup_attribute);
            break;
         case MetaGraph::VIEWAXIAL: case MetaGraph::VIEWBACKAXIAL:
            graph->getShapeGraphs().getDisplayedMap().removeAttribute(m_popup_attribute);
            break;
         case MetaGraph::VIEWDATA: case MetaGraph::VIEWBACKDATA:
            graph->getDataMaps().getDisplayedMap().removeAttribute(m_popup_attribute);
            break;
         }
         m_treeDoc->SetUpdateFlag(CGraphDoc::NEW_DATA);
         // some views need to react to this for their column titles:
         m_treeDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_COLUMN );
      }
   }

   SwitchFocusBack();
}

void CSideBar::OnPopupSidebarAttrRename() 
{
   CTreeCtrl& tree = GetTreeCtrl();
   // since this may happen at any time, should really check for anything going badly wrong,
   // e.g., no current graph document:
   CMDIFrameWnd *pFrame = (CMDIFrameWnd*) AfxGetApp()->m_pMainWnd;
   CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();
   CDocument *pDoc = pChild->GetActiveDocument();
   if (pDoc != m_treeDoc) {
      return;
   }
   MetaGraph *graph = m_treeDoc->m_meta_graph;

   bool displayed = (graph->getDisplayedAttribute() == m_popup_attribute);

   AttributeTable *tab = NULL;

   int view_class = graph->getViewClass() & (MetaGraph::VIEWVGA | MetaGraph::VIEWAXIAL | MetaGraph::VIEWDATA);

   switch (view_class) {
   case MetaGraph::VIEWVGA: case MetaGraph::VIEWBACKVGA:
      tab = &(graph->PointMaps::getDisplayedPointMap().getAttributeTable());
      break;
   case MetaGraph::VIEWAXIAL: case MetaGraph::VIEWBACKAXIAL:
      tab = &(graph->getShapeGraphs().getDisplayedMap().getAttributeTable());
      break;
   case MetaGraph::VIEWDATA: case MetaGraph::VIEWBACKDATA:
      tab = &(graph->getDataMaps().getDisplayedMap().getAttributeTable());
      break;
   }

   if (tab != NULL) {
      int newcol = m_treeDoc->RenameColumn(tab, m_popup_attribute);
      if (newcol != -1) {
         m_treeDoc->SetUpdateFlag(CGraphDoc::NEW_DATA);
         graph->setDisplayedAttribute(newcol);
         m_treeDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_COLUMN );
      }
   }

   SwitchFocusBack();
}

void CSideBar::OnPopupSidebarAttrUpdate() 
{
   // NB!  This needs some work!

   CTreeCtrl& tree = GetTreeCtrl();
   // since this may happen at any time, should really check for anything going badly wrong,
   // e.g., no current graph document:
   CMDIFrameWnd *pFrame = (CMDIFrameWnd*) AfxGetApp()->m_pMainWnd;
   CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();
   CDocument *pDoc = pChild->GetActiveDocument();
   if (pDoc != m_treeDoc) {
      return;
   }
   if (m_popup_attribute == -1) {
      AfxMessageBox(_T("Cannot replace values for the reference number column"));
      return;
   }

   MetaGraph *graph = m_treeDoc->m_meta_graph;

   PointMap *pointmap = NULL;
   ShapeMap *shapemap = NULL;
   // Need to check what this does
   GetItemMap(graph,pointmap,shapemap);

   if (m_treeDoc->ReplaceColumnContents(pointmap, shapemap, m_popup_attribute)) {
      graph->setDisplayedAttribute(m_popup_attribute);
      m_treeDoc->SetUpdateFlag(CGraphDoc::NEW_DATA);
      m_treeDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_DATA );
   }

   SwitchFocusBack();
}

void CSideBar::OnPopupSidebarAttrProperties() 
{
   CTreeCtrl& tree = GetTreeCtrl();
   // since this may happen at any time, should really check for anything going badly wrong,
   // e.g., no current graph document:
   CMDIFrameWnd *pFrame = (CMDIFrameWnd*) AfxGetApp()->m_pMainWnd;
   CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();
   CDocument *pDoc = pChild->GetActiveDocument();
   if (pDoc != m_treeDoc) {
      return;
   }
   MetaGraph *graph = m_treeDoc->m_meta_graph;
   if (graph->viewingProcessed()) {
      AttributeTable& table = graph->getAttributeTable();
      CColumnPropertiesDlg dlg(&table, m_popup_attribute);
      dlg.DoModal();
   }

   SwitchFocusBack();
}

void CSideBar::GetItemMap(MetaGraph *graph, PointMap* &pointmap, ShapeMap* &shapemap)
{
   int view_class = graph->getViewClass() & (MetaGraph::VIEWVGA | MetaGraph::VIEWAXIAL | MetaGraph::VIEWDATA);

   switch (view_class) {
   case MetaGraph::VIEWVGA: case MetaGraph::VIEWBACKVGA:
      pointmap = &(graph->getDisplayedPointMap());
      break;
   case MetaGraph::VIEWAXIAL: case MetaGraph::VIEWBACKAXIAL:
      shapemap = &(graph->getShapeGraphs().getDisplayedMap());
      break;
   case MetaGraph::VIEWDATA: case MetaGraph::VIEWBACKDATA:
      shapemap = &(graph->getDataMaps().getDisplayedMap());
      break;
   }
}

void CSideBar::SwitchFocusBack()
{
   CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;

   // Get the active MDI child window.
   CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();

   // Get the active view attached to the active MDI child
   // window.
   CView *pView = pChild->GetActiveView();

   pView->SetFocus();
}


void CSideBar::OnAttributeListNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;   

   if (pNMListView->iItem != -1) {
      MetaGraph *graph = m_treeDoc->m_meta_graph;
      if (graph->viewingProcessed()) {
         graph->setDisplayedAttribute((pNMListView->iItem)-1);
      }
      m_treeDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_FOCUS );
      GetApp()->GetMainWnd()->PostMessage( WM_DMP_FOCUS_GRAPH, (WPARAM) m_treeDoc, CGraphDoc::CONTROLS_CHANGEATTRIBUTE );
   }

   // this *does* work here (but only if they click on a valid attribute):
   SwitchFocusBack();

   *pResult = TRUE;
}
