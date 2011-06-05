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

#if !defined(AFX_TABLEVIEW_H__81F17F65_733C_4F09_B9CA_46C0186A2887__INCLUDED_)
#define AFX_TABLEVIEW_H__81F17F65_733C_4F09_B9CA_46C0186A2887__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TableView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTableView view

class CTableEdit;

class CTableView : public CListView
{
protected:
	CTableView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTableView)

// Attributes
public:
	CGraphDoc* GetDocument();
   enum { CACHECOLWIDTH = 14, CACHEROWCOUNT = 100 };
   wchar_t *m_cache;
   int m_cacherowwidth;
   bool *m_select_cache;
   int m_from;
   int m_to;
   int m_column_count;
   //
   CTableEdit *m_edit;
   IntPair m_editing;
   bool m_protect_edit;

   CImageList m_stateImageList;
   CImageList m_headerImageList;

// Operations
public:
   void RedoTable(CGraphDoc *pDoc);
   void PrepareCache(int from, int to);

// Overrides
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTableView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTableView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// My message map functions
protected:
   LRESULT OnRedraw(WPARAM wParam, LPARAM lParam);
   LRESULT OnGridEditMsg(WPARAM wParam, LPARAM lParam);
	// Generated message map functions
protected:
	//{{AFX_MSG(CTableView)
	afx_msg void OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOdCacheHint(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnItemChanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CTableView.cpp
inline CGraphDoc* CTableView::GetDocument()
   { return (CGraphDoc*)m_pDocument; }
#endif


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABLEVIEW_H__81F17F65_733C_4F09_B9CA_46C0186A2887__INCLUDED_)
