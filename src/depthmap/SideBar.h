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

#if !defined(AFX_SIDEBAR_H__8845DCE5_90D7_490C_8129_C0C2EE586CC3__INCLUDED_)
#define AFX_SIDEBAR_H__8845DCE5_90D7_490C_8129_C0C2EE586CC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SideBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSideBar window

struct CTreeEntry
{
   char m_type;
   short m_cat;
   short m_subcat;
   CTreeEntry()
   { m_type = -1; m_cat = -1; m_subcat = -1; }
   CTreeEntry(char t, short c, short sc)
   { m_type = t; m_cat = c; m_subcat = sc; }
};

class CSideBar : public CDialogBar
{
// Construction
public:
	CSideBar();

// Attributes
public:
   bool m_resizing;
   CPoint m_point;
   CRect m_dragrect;
   int m_old_cy;

   CImageList m_stateTreeImages;
   CImageList m_stateListImages;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSideBar)
	public:
   virtual BOOL Create( CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID );
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSideBar();

   // quick access to the tree ctrl:
   CTreeCtrl& GetTreeCtrl()
   { return *((CTreeCtrl *)GetDlgItem(IDC_SIDEBAR_TREE)); }
   // quick access to the list ctrl:
   CListCtrl& GetListCtrl()
   { return *((CListCtrl *)GetDlgItem(IDC_ATTRIBUTE_LIST)); }
   // and memory for it:
   CGraphDoc *m_treeDoc;
   HTREEITEM m_treeroots[5];
   pmap<HTREEITEM,CTreeEntry> m_treegraphmap;
   pmap<HTREEITEM,CTreeEntry> m_treedrawingmap;
   HTREEITEM m_topgraph;
   HTREEITEM m_backgraph;
   int m_view_selection;
   pvector<bool> m_attribute_locked;
   void MakeTree();
   void ClearGraphTree();
   void MakeGraphTree();
   void MakeDrawingTree();
   void SetTreeStyle(long lStyleMask, BOOL bSetBits);
   void SetGraphTreeChecks();
   void SetDrawingTreeChecks();
   void MakeAttributeList();
   void SetAttributeChecks();
   int SetDisplayedAttribute(HTREEITEM hItem, CTreeEntry entry);
   bool SetEditable(CTreeEntry entry);

   // popup stuff
   CTreeEntry m_popup_entry;
   int m_popup_attribute;
   void GetItemMap(MetaGraph *graph, PointMap* &pointmap, ShapeMap* &shapemap);

   // misc
   void SwitchFocusBack();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSideBar)
//	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSelchangingSidebarTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPopupSidebarAttrDelete();
	afx_msg void OnPopupSidebarAttrRename();
	afx_msg void OnPopupSidebarAttrUpdate();
	afx_msg void OnPopupSidebarAttrProperties();
	//}}AFX_MSG
   LRESULT OnFocusGraph(WPARAM wParam, LPARAM lParam);
   
   DECLARE_MESSAGE_MAP()
public:
   afx_msg LRESULT OnNcHitTest(CPoint point);
public:
   afx_msg void OnAttributeListNMClick(NMHDR *pNMHDR, LRESULT *pResult);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIDEBAR_H__8845DCE5_90D7_490C_8129_C0C2EE586CC3__INCLUDED_)
