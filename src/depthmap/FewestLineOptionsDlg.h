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

#if !defined(AFX_FEWESTLINEOPTIONSDLG_H__4287272C_4843_41A8_BA01_38CFFF605058__INCLUDED_)
#define AFX_FEWESTLINEOPTIONSDLG_H__4287272C_4843_41A8_BA01_38CFFF605058__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FewestLineOptionsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFewestLineOptionsDlg dialog

class CFewestLineOptionsDlg : public CDialog
{
// Construction
public:
	CFewestLineOptionsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFewestLineOptionsDlg)
	enum { IDD = IDD_FEWESTOPTIONS };
	int		m_option;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFewestLineOptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFewestLineOptionsDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FEWESTLINEOPTIONSDLG_H__4287272C_4843_41A8_BA01_38CFFF605058__INCLUDED_)
