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

// EditConnectionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "depthmap.h"
#include "EditConnectionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditConnectionsDlg dialog


CEditConnectionsDlg::CEditConnectionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditConnectionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditConnectionsDlg)
	m_join_type = -1;
	m_sel_to_pin = FALSE;
	m_pin_to_sel = FALSE;
	//}}AFX_DATA_INIT

   // Actually... want a completely different set of values:

   m_pin_to_sel = TRUE;
   m_sel_to_pin = TRUE;
   m_join_type = 0;
}

void CEditConnectionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditConnectionsDlg)
	DDX_Radio(pDX, IDC_TYPE, m_join_type);
	DDX_Check(pDX, IDC_PIN_TO_SEL, m_sel_to_pin);
	DDX_Check(pDX, IDC_SEL_TO_PIN, m_pin_to_sel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditConnectionsDlg, CDialog)
	//{{AFX_MSG_MAP(CEditConnectionsDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditConnectionsDlg message handlers
