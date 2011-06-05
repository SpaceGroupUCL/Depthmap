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

#if !defined(AFX_PUSHDIALOG_H__C6D4FE1F_3841_400A_BBFD_1EDDB764D25F__INCLUDED_)
#define AFX_PUSHDIALOG_H__C6D4FE1F_3841_400A_BBFD_1EDDB764D25F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PushDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPushDialog dialog

class CPushDialog : public CDialog
{
// Construction
public:
	CPushDialog(pqmap<IntPair,pstring>& names, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPushDialog)
	enum { IDD = IDD_PUSH_DIALOG };
	CComboBox	m_layer_selector;
	int		m_layer_selection;
	CString	m_origin_attribute;
	CString	m_origin_layer;
	BOOL	m_count_intersections;
	int		m_function;
	//}}AFX_DATA

   pvecstring m_names;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPushDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPushDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PUSHDIALOG_H__C6D4FE1F_3841_400A_BBFD_1EDDB764D25F__INCLUDED_)
