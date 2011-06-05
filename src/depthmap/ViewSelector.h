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

#if !defined(AFX_VIEWSELECTOR_H__53A0A474_EFA8_11D4_AFE2_E2A4D9000000__INCLUDED_)
#define AFX_VIEWSELECTOR_H__53A0A474_EFA8_11D4_AFE2_E2A4D9000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewSelector.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewSelector dialog

class CViewSelector : public CDialog
{
// Construction
public:
	CViewSelector();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CViewSelector)
	enum { IDD = IDD_DIALOG_BAR };
	CComboBox	m_view_selector;
	//}}AFX_DATA

   int m_view_class;

   pqmap<int,pstring> m_layer_map_entries;
   pqmap<int,pstring> m_view_map_entries;

   int m_view_selection;
   int m_layer_selection;

   // indicator that the user has edited a column name
   bool m_selview_update;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewSelector)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
   void RedoMenu( MetaGraph& meta_graph );
   void RedoLayerMenu( MetaGraph& meta_graph );
   void RedoViewMenu( MetaGraph& meta_graph );
   
// Implementation
protected:

   void OnOK();
   void CheckViewName();
   bool ChangeViewName(const CString& name);

	// Generated message map functions
	//{{AFX_MSG(CViewSelector)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelLayer();
	afx_msg void OnSelView();
	afx_msg void OnKillfocusViewSelector();
	afx_msg void OnEditChangeViewSelector();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWSELECTOR_H__53A0A474_EFA8_11D4_AFE2_E2A4D9000000__INCLUDED_)
