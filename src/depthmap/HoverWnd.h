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

#if !defined(AFX_HOVERWND_H__9B3425AF_E1C2_4B1F_87EF_0FCF5B1AAAE1__INCLUDED_)
#define AFX_HOVERWND_H__9B3425AF_E1C2_4B1F_87EF_0FCF5B1AAAE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HoverWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHoverWnd window

class CHoverWnd : public CWnd
{
// Construction
public:
	CHoverWnd();

// Attributes
public:
   CBrush m_brush;
   CPoint m_position;
   prefvec<CString> m_strings;
   int m_lineheight;
   
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHoverWnd)
	public:
	virtual BOOL Create(CWnd *parent, const prefvec<CString>& infostrings, const CPoint& position);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHoverWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHoverWnd)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOVERWND_H__9B3425AF_E1C2_4B1F_87EF_0FCF5B1AAAE1__INCLUDED_)
