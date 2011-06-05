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

#if !defined(AFX_LICENCEDIALOG_H__FF527C28_F5C3_11D3_A8D5_00609787FC1E__INCLUDED_)
#define AFX_LICENCEDIALOG_H__FF527C28_F5C3_11D3_A8D5_00609787FC1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LicenceDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLicenceDialog dialog

extern const wchar_t *g_agreement;

class CLicenceDialog : public CDialog
{
// Construction
public:
	CLicenceDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLicenceDialog)
	enum { IDD = IDD_LICENCE_DIALOG };
	CHyperLink	m_vrlink;
	CEdit	m_agreement_control;
	CString	m_message;
	CString	m_agreement;
	//}}AFX_DATA

   CString  m_title;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLicenceDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLicenceDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LICENCEDIALOG_H__FF527C28_F5C3_11D3_A8D5_00609787FC1E__INCLUDED_)
