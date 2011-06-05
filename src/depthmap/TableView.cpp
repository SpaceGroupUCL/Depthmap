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

// TableView.cpp : implementation file
//

#include "stdafx.h"
#include "depthmap.h"
#include "GraphDoc.h"
#include "TableEdit.h"
#include "TableView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTableView

IMPLEMENT_DYNCREATE(CTableView, CListView)

CTableView::CTableView()
{
   m_cache = NULL;
   m_select_cache = NULL;
   m_from = -1;
   m_to = -1;
   m_cacherowwidth = 0;
   m_column_count = 0;
   m_edit = NULL;
   m_editing = IntPair(-1,-1);
   m_protect_edit = false;
}

CTableView::~CTableView()
{
   if (m_cache) {
      delete [] m_cache;
      m_cache = NULL;
   }
   if (m_select_cache) {
      delete [] m_select_cache;
      m_select_cache = NULL;
   }
   if (m_edit) {
      m_edit->DestroyWindow();
      delete m_edit;
      m_edit = NULL;
   }
}


BEGIN_MESSAGE_MAP(CTableView, CListView)
	//{{AFX_MSG_MAP(CTableView)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetDispInfo)
	ON_NOTIFY_REFLECT(LVN_ODCACHEHINT, OnOdCacheHint)
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemChanged)
	ON_WM_SETFOCUS()
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGING, OnItemChanging)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnClick)
	//}}AFX_MSG_MAP
   ON_MESSAGE(WM_DMP_REDRAW_MESSAGE, OnRedraw)
   ON_MESSAGE(WM_DMP_GRID_EDIT_MSG, OnGridEditMsg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTableView drawing

void CTableView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
   if (m_edit) {
      m_edit->BringWindowToTop();
   }
}

/////////////////////////////////////////////////////////////////////////////
// CTableView diagnostics

#ifdef _DEBUG
void CTableView::AssertValid() const
{
	CListView::AssertValid();
}

void CTableView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CGraphDoc* CTableView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGraphDoc)));
	return (CGraphDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTableView message handlers

BOOL CTableView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= LVS_OWNERDATA | LVS_REPORT | LVS_SINGLESEL | LVS_OWNERDRAWFIXED;

	return CListView::PreCreateWindow(cs);
}

void CTableView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();

   CListCtrl& ctrl = GetListCtrl();
   ctrl.SetExtendedStyle(LVS_EX_GRIDLINES);

   m_stateImageList.Create(IDB_STATEICONS2, 14, 1, RGB(255, 0, 255));
   ctrl.SetImageList(&m_stateImageList,LVSIL_STATE);

   CHeaderCtrl *headers = ctrl.GetHeaderCtrl();
   m_headerImageList.Create(IDB_COLUMNHEADERICONS, 14, 1, RGB(255, 0, 255));

   CGraphDoc *pDoc = GetDocument();

   // register with GraphDoc:
   pDoc->m_view[CGraphDoc::VIEW_TABLE] = this;

   // send initial redraw:
   pDoc->SetRemenuFlag(CGraphDoc::VIEW_TABLE,true);
   pDoc->SetRedrawFlag(CGraphDoc::VIEW_TABLE,CGraphDoc::REDRAW_TOTAL);
}

void CTableView::OnDestroy() 
{
   // deregister with GraphDoc
   GetDocument()->m_view[CGraphDoc::VIEW_TABLE] = NULL;

   CListView::OnDestroy();
}

void CTableView::OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

   if (pDispInfo->item.mask & LVIF_TEXT) {
      int item = pDispInfo->item.iItem;
      if (item != -1) {
         if (item >= m_from && item <= m_to) {
            // use cache
            wcscpy( pDispInfo->item.pszText, m_cache+CACHECOLWIDTH*pDispInfo->item.iSubItem+(item-m_from)*m_cacherowwidth );
         }
         else {
            // lookup
            const AttributeTable& table = GetDocument()->m_meta_graph->getAttributeTable();
            const AttributeIndex& index = table.m_display_index;
            if (pDispInfo->item.iSubItem == 0) {
               swprintf(pDispInfo->item.pszText,_T("%ld"),table.getRowKey(index[item].index));
            }
            else {
               swprintf(pDispInfo->item.pszText,_T("%g"),table.getValue(index[item].index,pDispInfo->item.iSubItem-1));
            }
         }
      }
   }

	*pResult = 0;
}

void CTableView::OnOdCacheHint(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVCACHEHINT* pCacheHint = (NMLVCACHEHINT*)pNMHDR;

   if (m_cache && pCacheHint->iFrom < m_from || pCacheHint->iTo > m_to) {
      PrepareCache(pCacheHint->iFrom, pCacheHint->iTo);
   }

   // strange place for this, but this notify is called when you scroll!
   if (m_edit && !m_protect_edit) {
      m_edit->DestroyWindow();
      delete m_edit;
      m_edit = NULL;
      Invalidate();
      UpdateWindow();
   }

	*pResult = 0;
}

void CTableView::PrepareCache(int from, int to) 
{
   CGraphDoc *pDoc = GetDocument();
   const AttributeTable& table = pDoc->m_meta_graph->getAttributeTable();
   const AttributeIndex& index = table.m_display_index;
   m_from = from;
   m_to = to;
   if (m_to - m_from > CACHEROWCOUNT) {
      m_to = m_from + CACHEROWCOUNT;
   }
   int pos = 0;
   for (int i = m_from; i <= m_to; i++) {
      swprintf(m_cache+pos,_T("%ld"),table.getRowKey(index[i].index));
      pos += CACHECOLWIDTH;
      for (int j = 0; j < table.getColumnCount(); j++) {
         swprintf(m_cache+pos,_T("%g"),table.getValue(index[i].index,j));
         pos += CACHECOLWIDTH;
      }
      m_select_cache[i-m_from] = table.isSelected(index[i].index);
   }
}

LRESULT CTableView::OnRedraw(WPARAM wParam, LPARAM lParam)
{
   CGraphDoc *pDoc = GetDocument();

   bool done = false;
   if (pDoc->GetRemenuFlag(CGraphDoc::VIEW_TABLE)) {
      pDoc->SetRemenuFlag(CGraphDoc::VIEW_TABLE,false);
      // this is big: start from scratch...
      RedoTable(pDoc);
      done = true;
   }
   int flag = pDoc->GetRedrawFlag(CGraphDoc::VIEW_TABLE);
   if (flag != CGraphDoc::REDRAW_DONE) {
      //
      while (!pDoc->SetRedrawFlag(CGraphDoc::VIEW_TABLE,CGraphDoc::REDRAW_DONE)) {
         // prefer waitformultipleobjects here
         Sleep(1);
      }
      //
      if (!done && lParam != (LPARAM) this) {
         if (wParam == CGraphDoc::NEW_TABLE) {
            // this is big start from scratch...
            RedoTable(pDoc);
            done = true;
         }
         else {
            // redo the cache and redisplay
            PrepareCache(m_from,m_to);
            Invalidate();
            UpdateWindow();
         }
      }
   }

   return 0;
}

LRESULT CTableView::OnGridEditMsg(WPARAM wParam, LPARAM lParam)
{
   // kill it...
   if (!m_protect_edit) {
      CGraphDoc *pDoc = GetDocument();
      m_protect_edit = true;
      if (wParam == 1) {
         CString edittext;
         m_edit->GetWindowText(edittext);
         wchar_t *endptr;
         // first reset contents:
         double value = wcstod(edittext,&endptr); // AT: Unicode conversion -- this doesn't look safe -- edittext is a CString!
         if (endptr == edittext) {
            AfxMessageBox(_T("Cannot convert text to number"));
            m_protect_edit = false;
            return 0;
         }
         MetaGraph *graph = pDoc->m_meta_graph;
         if (graph && graph->viewingProcessed()) {
            // go for the change:
            AttributeTable &table = graph->getAttributeTable();
            double value2 = table.getValue(table.m_display_index[m_editing.a].index,m_editing.b-1);
            if (value2 == 0 || fabs((value / value2) - 1.0) > 1e-5) {
               table.changeValue(table.m_display_index[m_editing.a].index,m_editing.b-1,value);
               pDoc->SetModifiedFlag(TRUE);
            }
            // bit horrible for the moment, until I change the interface, this goes to the next row down for next input:
            if (m_editing.a + 1 < GetListCtrl().GetItemCount()) {
               pvecint x;
               x.push_back(pDoc->m_meta_graph->getAttributeTable().getRowKey(table.m_display_index[m_editing.a + 1].index));
               pDoc->m_meta_graph->setSelSet(x);
            }
            RedoTable(pDoc);
            // note: this as caller will prevent us from redrawing ourself:
            // could be either new data or new selection, just go for a big redraw:
            pDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL,CGraphDoc::REDRAW_GRAPH,CGraphDoc::NEW_DATA,this);
         }
      }
      if (m_edit) {
         m_edit->DestroyWindow();
         delete m_edit;
         m_edit = NULL;
      }

      if (wParam == 1) {
         //
         CListCtrl& ctrl = GetListCtrl();
         if (m_editing.a + 1 < ctrl.GetItemCount()) {
            if (!ctrl.EnsureVisible(m_editing.a + 1,FALSE)) {
               CRect rect;
               ctrl.GetItemRect(m_editing.a + 1,rect,LVIR_BOUNDS);
               ctrl.Scroll(CSize(0,rect.Height()));
            }
            m_edit = new CTableEdit;
            CRect rect;
            ctrl.GetSubItemRect(m_editing.a + 1,m_editing.b,LVIR_BOUNDS,rect);
            m_edit->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_BORDER|ES_AUTOHSCROLL,rect,this,1001);
            m_edit->SetFont(ctrl.GetFont());
            m_edit->SetFocus();
            m_edit->SetWindowText(ctrl.GetItemText(m_editing.a + 1,m_editing.b));
            m_edit->SetSel(0,-1);
            m_editing = IntPair(m_editing.a + 1,m_editing.b);
         }
      }
      m_protect_edit = false;
   }

   return 0;
}

void CTableView::RedoTable(CGraphDoc *pDoc)
{
   if (m_cache) {
      delete [] m_cache;
      m_cache = NULL;
      m_from = -1;
      m_to = -1;
   }
   if (m_select_cache) {
      delete [] m_select_cache;
      m_select_cache = NULL;
   }
   CListCtrl& ctrl = GetListCtrl();
   ctrl.DeleteAllItems();
   for (int i = m_column_count - 1; i >= 0; i--) {
      ctrl.DeleteColumn(i);
   }
   if (pDoc->m_meta_graph->viewingProcessed()) {
      const AttributeTable& table = pDoc->m_meta_graph->getAttributeTable();
      int col = pDoc->m_meta_graph->getDisplayedAttribute();
      ctrl.InsertColumn(0,_T("Ref Number"),LVCFMT_LEFT,100);
      for (int i = 0; i < table.getColumnCount(); i++) {
         ctrl.InsertColumn(i+1,CString(table.getColumnName(i).c_str()),LVCFMT_LEFT,100);
      }
      CHeaderCtrl *headers = NULL;
      headers = ctrl.GetHeaderCtrl();
      headers->SetImageList(&m_headerImageList);
      for (int j = -1; j < table.getColumnCount(); j++) {
         if (col == j) {
            HD_ITEM curitem;
            curitem.mask= HDI_IMAGE | HDI_FORMAT;
            curitem.iImage= 0;
            curitem.fmt= HDF_LEFT | HDF_BITMAP_ON_RIGHT | HDF_IMAGE | HDF_STRING;
            headers->SetItem(j+1, &curitem);
         }
      }
      ctrl.SetItemCount(table.getRowCount());
      m_column_count = table.getColumnCount() + 1;
      m_cacherowwidth = m_column_count * CACHECOLWIDTH;
      m_cache = new wchar_t [CACHEROWCOUNT * m_cacherowwidth];
      m_select_cache = new bool [CACHEROWCOUNT];
   }
}

void CTableView::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
   if (m_edit) {
      m_edit->DestroyWindow();
      delete m_edit;
      m_edit = NULL;
   }

	CPoint point;
   CListCtrl& ctrl = GetListCtrl();
	GetCursorPos(&point);
	ctrl.ScreenToClient(&point);
	//int item = ctrl.HitTest(point, &uFlags);
   LVHITTESTINFO info;
   info.pt = point;
   ctrl.SubItemHitTest(&info);

   if (info.flags & LVHT_ONITEMLABEL) {
      CGraphDoc *pDoc = GetDocument();
      if (pDoc->m_meta_graph && info.iItem >= 0 && info.iItem < pDoc->m_meta_graph->getAttributeTable().getRowCount()) {
         // bit horrible for the moment, until I change the interface
         pvecint x;
         AttributeTable& table = pDoc->m_meta_graph->getAttributeTable();
         AttributeIndex& index = table.m_display_index;
         x.push_back(table.getRowKey(index[info.iItem].index));
         pDoc->m_meta_graph->setSelSet(x);
         if (m_from != -1 && m_to != -1) {
            PrepareCache(m_from,m_to);
         }
         Invalidate(TRUE);
         // Redraw scene
         GetDocument()->SetRedrawFlag(CGraphDoc::VIEW_ALL,CGraphDoc::REDRAW_POINTS, CGraphDoc::NEW_SELECTION, this);
         // +1 for ref number column
         if (info.iSubItem > 0 && info.iSubItem < pDoc->m_meta_graph->getAttributeTable().getColumnCount() + 1) {
            // don't let them edit a locked attribute
            if (pDoc->m_meta_graph->getAttributeTable().isColumnLocked(info.iSubItem-1)) {
               AfxMessageBox(_T("This column is locked and cannot be edited"));
            }
            else {
               m_protect_edit = true;
               ctrl.SetItemState(info.iItem,LVIS_SELECTED,LVIS_SELECTED);
               m_edit = new CTableEdit;
               CRect rect;
               ctrl.GetSubItemRect(info.iItem,info.iSubItem,LVIR_BOUNDS,rect);
               m_edit->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_BORDER|ES_AUTOHSCROLL,rect,this,1001);
               m_edit->SetFont(ctrl.GetFont());
               m_edit->SetFocus();
               m_edit->SetWindowText(ctrl.GetItemText(info.iItem,info.iSubItem));
               m_edit->SetSel(0,-1);
               m_editing = IntPair(info.iItem,info.iSubItem);
               m_protect_edit = false;
            }
         }
      }
   }

   *pResult = 0;
}

void CTableView::OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

   // Don't let them bloody select it! (this still leaves the border, but at least it's half way there)
   if (pNMListView->uNewState & LVIS_SELECTED) {
      GetListCtrl().SetItemState(pNMListView->iItem,0,LVIS_SELECTED);
   }
   *pResult = 0;
}

void CTableView::OnSetFocus(CWnd* pOldWnd) 
{
	CListView::OnSetFocus(pOldWnd);
	
   GetApp()->GetMainWnd()->PostMessage(WM_DMP_FOCUS_GRAPH, (WPARAM) GetDocument(), CGraphDoc::CONTROLS_LOADALL);
}

void CTableView::OnItemChanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CTableView::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CListCtrl& ctrl = GetListCtrl();
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rcItem(lpDrawItemStruct->rcItem);
	int item = lpDrawItemStruct->itemID;

	static _TCHAR szBuff[MAX_PATH];
	LV_ITEM lvi;
	lvi.mask = LVIF_TEXT;
	lvi.iItem = item;
	lvi.iSubItem = 0;
	lvi.pszText = szBuff;
	lvi.cchTextMax = sizeof(szBuff);
	ctrl.GetItem(&lvi);

   // handle our own icons state, since the OWNERDATA can't do it for us:
   CImageList *pImageList = ctrl.GetImageList(LVSIL_STATE);
   if (pImageList)
   {
      if (item >= m_from && item <= m_to) {
         // use cache
         pImageList->Draw(pDC, m_select_cache[item-m_from] ? 1 : 0, CPoint(rcItem.left, rcItem.top), ILD_TRANSPARENT);
      }
      else {
         // look it up
         const AttributeTable& table = GetDocument()->m_meta_graph->getAttributeTable();
         const AttributeIndex& index = table.m_display_index;
         pImageList->Draw(pDC, table.isSelected(index[item].index) ? 1 : 0, CPoint(rcItem.left, rcItem.top), ILD_TRANSPARENT);
      }
   }

	ctrl.GetItemRect(item, rcItem, LVIR_LABEL);
	pDC->DrawText(lvi.pszText,-1,rcItem, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);

	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH;

	for (int i = 1; ctrl.GetColumn(i, &lvc); i++)
	{
		rcItem.left = rcItem.right;
		rcItem.right += lvc.cx;

      if (ctrl.GetItemText(item, i, szBuff, sizeof(szBuff))) {
   		pDC->DrawText(lvi.pszText, -1, rcItem, DT_RIGHT | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);
      }
	}
}

void CTableView::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

   if (pNMListView->iSubItem - 1 != GetDocument()->m_meta_graph->getDisplayedAttribute()) {
      GetDocument()->m_meta_graph->setDisplayedAttribute(pNMListView->iSubItem - 1);
      GetApp()->GetMainWnd()->PostMessage(WM_DMP_FOCUS_GRAPH, (WPARAM) GetDocument(), CGraphDoc::CONTROLS_CHANGEATTRIBUTE);
      GetDocument()->SetRemenuFlag(CGraphDoc::VIEW_ALL, true);
      GetDocument()->SetRedrawFlag(CGraphDoc::VIEW_ALL,CGraphDoc::REDRAW_GRAPH,CGraphDoc::NEW_DATA);
   }

	*pResult = 0;
}
