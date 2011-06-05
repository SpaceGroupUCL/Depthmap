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

#if !defined(AFX_GRIDDIALOG_H__FF527C21_F5C3_11D3_A8D5_00609787FC1E__INCLUDED_)
#define AFX_GRIDDIALOG_H__FF527C21_F5C3_11D3_A8D5_00609787FC1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GridDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGridDialog dialog

class CGridDialog : public CDialog
{
// Construction
public:
	CGridDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGridDialog)
	enum { IDD = IDD_GRID_DIALOG };
	CSpinButtonCtrl	m_spacing_ctrl;
	double	m_spacing;
	//}}AFX_DATA

   double m_maxdimension;
   int m_minexponent;
   int m_maxexponent;
   int m_basemantissa;
   int m_mantissa;
   int m_exponent;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGridDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposSpinSpacing(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDDIALOG_H__FF527C21_F5C3_11D3_A8D5_00609787FC1E__INCLUDED_)
