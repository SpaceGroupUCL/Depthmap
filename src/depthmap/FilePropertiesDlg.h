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

#if !defined(AFX_FILEPROPERTIESDLG_H__1B87A2BC_3FD3_4344_BB40_93F7081EE7B6__INCLUDED_)
#define AFX_FILEPROPERTIESDLG_H__1B87A2BC_3FD3_4344_BB40_93F7081EE7B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilePropertiesDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFilePropertiesDlg dialog

class CFilePropertiesDlg : public CDialog
{
// Construction
public:
	CFilePropertiesDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFilePropertiesDlg)
	enum { IDD = IDD_FILEPROPERTIES };
	CString	m_author;
	CString	m_create_date;
	CString	m_create_program;
	CString	m_description;
	CString	m_location;
	CString	m_organization;
	CString	m_title;
	CString	m_file_version;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilePropertiesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFilePropertiesDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEPROPERTIESDLG_H__1B87A2BC_3FD3_4344_BB40_93F7081EE7B6__INCLUDED_)
