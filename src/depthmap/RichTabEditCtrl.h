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

#if !defined(AFX_RICHTABEDITCTRL_H__85E1E696_5E7D_48B1_9DB3_5E5A12DC0210__INCLUDED_)
#define AFX_RICHTABEDITCTRL_H__85E1E696_5E7D_48B1_9DB3_5E5A12DC0210__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RichTabEditCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRichTabEditCtrl window

class CRichTabEditCtrl : public CRichEditCtrl
{
// Construction
public:
	CRichTabEditCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRichTabEditCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRichTabEditCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRichTabEditCtrl)
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RICHTABEDITCTRL_H__85E1E696_5E7D_48B1_9DB3_5E5A12DC0210__INCLUDED_)
