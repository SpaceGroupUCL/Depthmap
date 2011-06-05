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

#if !defined(AFX_COLUMNPROPERTIESDLG_H__E107C33E_670C_47A1_A9C0_58F782B877CF__INCLUDED_)
#define AFX_COLUMNPROPERTIESDLG_H__E107C33E_670C_47A1_A9C0_58F782B877CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColumnPropertiesDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColumnPropertiesDlg dialog

class CColumnPropertiesDlg : public CDialog
{
// Construction
public:
	CColumnPropertiesDlg(AttributeTable *table = NULL, int col = -1, CWnd* pParent = NULL);   // standard constructor

   AttributeTable *m_table;
   int m_col;

// Dialog Data
	//{{AFX_DATA(CColumnPropertiesDlg)
	enum { IDD = IDD_COLUMN_PROPERTIES };
	CListCtrl	m_summary;
	CString	m_formula;
	CString	m_name;
	CString	m_name_text;
	CString	m_creator;
	CString	m_formula_note;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColumnPropertiesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CColumnPropertiesDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLUMNPROPERTIESDLG_H__E107C33E_670C_47A1_A9C0_58F782B877CF__INCLUDED_)
