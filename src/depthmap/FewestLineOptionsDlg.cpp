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

// FewestLineOptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "depthmap.h"
#include "FewestLineOptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFewestLineOptionsDlg dialog


CFewestLineOptionsDlg::CFewestLineOptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFewestLineOptionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFewestLineOptionsDlg)
	m_option = -1;
	//}}AFX_DATA_INIT

   m_option = 0;
}

void CFewestLineOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFewestLineOptionsDlg)
	DDX_Radio(pDX, IDC_FEWESTOPTION, m_option);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFewestLineOptionsDlg, CDialog)
	//{{AFX_MSG_MAP(CFewestLineOptionsDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFewestLineOptionsDlg message handlers
