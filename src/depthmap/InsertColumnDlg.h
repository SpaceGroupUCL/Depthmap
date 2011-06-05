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

#if !defined(AFX_INSERTCOLUMNDLG_H__B3DC6F67_D4D8_43BB_8880_971EDF7D169E__INCLUDED_)
#define AFX_INSERTCOLUMNDLG_H__B3DC6F67_D4D8_43BB_8880_971EDF7D169E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InsertColumnDlg.h : header file
//

#include "RichTabEditCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CInsertColumnDlg dialog

class CInsertColumnDlg : public CDialog
{
// Construction
public:
	CInsertColumnDlg(AttributeTable *table = NULL, int col = -1, CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CInsertColumnDlg)
	enum { IDD = IDD_INSERT_COLUMN };
	CRichTabEditCtrl	m_formula;
	CButton	m_selection_desc;
	CStatic	m_formula_desc;
	CButton	m_use_column;
	CListBox	m_column_names;
	BOOL	m_selection_only;
	//}}AFX_DATA

   int m_col;
   pvecstring m_col_names;
   pstring m_formula_text;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInsertColumnDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInsertColumnDlg)
	afx_msg void OnUseAttribute();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelChangeColumnNames();
	afx_msg void OnSelCancelColumnNames();
	virtual void OnOK();
	afx_msg void OnDblclkColumnNames();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INSERTCOLUMNDLG_H__B3DC6F67_D4D8_43BB_8880_971EDF7D169E__INCLUDED_)
