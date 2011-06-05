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

#if !defined(AFX_MAKEOPTIONSDLG_H__25D4E120_21C1_11D4_A8D5_00609787FC1E__INCLUDED_)
#define AFX_MAKEOPTIONSDLG_H__25D4E120_21C1_11D4_A8D5_00609787FC1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MakeOptionsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMakeOptionsDlg dialog

class CMakeOptionsDlg : public CDialog
{
// Construction
public:
	CMakeOptionsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMakeOptionsDlg)
	enum { IDD = IDD_MAKE_OPTIONS };
	BOOL	m_boundarygraph;
	double	m_maxdist;
	BOOL	m_restrict_visibility;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMakeOptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMakeOptionsDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnRestrict();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAKEOPTIONSDLG_H__25D4E120_21C1_11D4_A8D5_00609787FC1E__INCLUDED_)
