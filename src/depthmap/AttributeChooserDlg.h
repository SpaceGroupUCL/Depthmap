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

#if !defined(AFX_ATTRIBUTECHOOSERDLG_H__AB9D4644_317C_48D7_A60B_1BA64A2FA8C0__INCLUDED_)
#define AFX_ATTRIBUTECHOOSERDLG_H__AB9D4644_317C_48D7_A60B_1BA64A2FA8C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AttributeChooserDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAttributeChooserDlg dialog

class CAttributeChooserDlg : public CDialog
{
// Construction
public:
	CAttributeChooserDlg(AttributeTable& table, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAttributeChooserDlg)
	enum { IDD = IDD_ATTRIBUTE_CHOOSER };
	CComboBox	m_attribute_chooser;
	int		m_attribute;
	CString	m_text;
	//}}AFX_DATA
   AttributeTable *m_table;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAttributeChooserDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAttributeChooserDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATTRIBUTECHOOSERDLG_H__AB9D4644_317C_48D7_A60B_1BA64A2FA8C0__INCLUDED_)
