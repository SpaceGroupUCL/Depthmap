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

#if !defined(AFX_LAYERCHOOSERDLG_H__A7699FD5_6D87_4351_8465_FDF76FBA5044__INCLUDED_)
#define AFX_LAYERCHOOSERDLG_H__A7699FD5_6D87_4351_8465_FDF76FBA5044__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LayerChooserDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLayerChooserDlg dialog

class CLayerChooserDlg : public CDialog
{
// Construction
public:
	CLayerChooserDlg(pvecstring& names = pvecstring(), CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLayerChooserDlg)
	enum { IDD = IDD_LAYER_CHOOSER };
	CComboBox	m_layer_selector;
	CString	m_text;
	int		m_layer;
	//}}AFX_DATA

   pvecstring m_names;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLayerChooserDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLayerChooserDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAYERCHOOSERDLG_H__A7699FD5_6D87_4351_8465_FDF76FBA5044__INCLUDED_)
