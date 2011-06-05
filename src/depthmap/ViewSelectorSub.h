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

#if !defined(AFX_VIEWSELECTORSUB_H__0F7C5A30_69A4_4DD6_B0E7_BB205F21E36B__INCLUDED_)
#define AFX_VIEWSELECTORSUB_H__0F7C5A30_69A4_4DD6_B0E7_BB205F21E36B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewSelectorSub.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewSelectorSub dialog

class CViewSelectorSub : public CDialog
{
// Construction
public:
	CViewSelectorSub(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CViewSelectorSub)
	enum { IDD = IDD_DIALOG_BAR1 };
	CStatic	m_label;
	CComboBox	m_view_selector;
	//}}AFX_DATA

   pqmap<int,pstring> m_view_map_entries;
   int m_view_selection;
   int m_axis;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewSelectorSub)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
   
public:
   void RedoMenu( MetaGraph& meta_graph, bool redraw );

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CViewSelectorSub)
	afx_msg void OnSelchangeViewSelector();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWSELECTORSUB_H__0F7C5A30_69A4_4DD6_B0E7_BB205F21E36B__INCLUDED_)
