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

#if !defined(AFX_CEditConnectionsDlg_H__67C1B5C0_67A5_11D4_A8D5_00609787FC1E__INCLUDED_)
#define AFX_CEditConnectionsDlg_H__67C1B5C0_67A5_11D4_A8D5_00609787FC1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditConnectionsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditConnectionsDlg dialog

class CEditConnectionsDlg : public CDialog
{
// Construction
public:
	CEditConnectionsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditConnectionsDlg)
	enum { IDD = IDD_EDIT_CONNECTIONS };
	int		m_join_type;
	BOOL	m_sel_to_pin;
	BOOL	m_pin_to_sel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditConnectionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditConnectionsDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CEditConnectionsDlg_H__67C1B5C0_67A5_11D4_A8D5_00609787FC1E__INCLUDED_)
