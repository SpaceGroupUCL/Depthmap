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

#include "afxwin.h"
#if !defined(AFX_RESERACHOPTIONSDLG_H__8A747020_0B21_11D4_A8D5_00609787FC1E__INCLUDED_)
#define AFX_RESERACHOPTIONSDLG_H__8A747020_0B21_11D4_A8D5_00609787FC1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReserachOptionsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg dialog

class COptionsDlg : public CDialog
{
// Construction
public:
	COptionsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptionsDlg)
	enum { IDD = IDD_ANALYSIS_OPTIONS };
	BOOL	m_global;
	BOOL	m_local;
	CString	m_radius;
	BOOL	m_gates_only;
	int		m_output_type;
	CString	m_radius2;
	//}}AFX_DATA

   pvecstring m_layer_names;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptionsDlg)
	virtual void OnOK();
	afx_msg void OnOutputType();
	virtual BOOL OnInitDialog();
	afx_msg void OnUpdateRadius();
	afx_msg void OnUpdateRadius2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
   CComboBox m_layer_selector;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESERACHOPTIONSDLG_H__8A747020_0B21_11D4_A8D5_00609787FC1E__INCLUDED_)
