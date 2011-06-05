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

#if !defined(AFX_MAKELAYERDLG_H__1A8247C5_10BA_485D_AEAD_2323FCF68CE9__INCLUDED_)
#define AFX_MAKELAYERDLG_H__1A8247C5_10BA_485D_AEAD_2323FCF68CE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MakeLayerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMakeLayerDlg dialog

enum {MAKELAYER_DRAWING = 1, 
      MAKELAYER_DATA = 2,
      MAKELAYER_AXIAL = 4,
      MAKELAYER_CONVEX = 8, 
      MAKELAYER_GENERIC = 16,
      MAKELAYER_SEGMENT = 32
};

class CMakeLayerDlg : public CDialog
{
// Construction
public:
	CMakeLayerDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMakeLayerDlg)
	enum { IDD = IDD_MAKE_LAYER };
	CComboBox	m_layer_type;
	BOOL	m_remove_stubs;
	BOOL	m_push_values;
	int		m_percentage;
	CString	m_origin;
	CString	m_layer_name;
	//}}AFX_DATA

   int m_mapin;
   int m_mapout;

   pvecint m_lookup;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMakeLayerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMakeLayerDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeLayerType();
	afx_msg void OnRemoveStubs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
   BOOL m_keeporiginal;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAKELAYERDLG_H__1A8247C5_10BA_485D_AEAD_2323FCF68CE9__INCLUDED_)
