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

// MakeOptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "depthmap.h"
#include "MakeOptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMakeOptionsDlg dialog


CMakeOptionsDlg::CMakeOptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMakeOptionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMakeOptionsDlg)
	m_boundarygraph = FALSE;
	m_maxdist = 0.0;
	m_restrict_visibility = FALSE;
	//}}AFX_DATA_INIT
}

void CMakeOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMakeOptionsDlg)
	DDX_Check(pDX, IDC_BOUNDARYGRAPH, m_boundarygraph);
	DDX_Text(pDX, IDC_MAXDIST, m_maxdist);
	DDX_Check(pDX, IDC_RESTRICT, m_restrict_visibility);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMakeOptionsDlg, CDialog)
	//{{AFX_MSG_MAP(CMakeOptionsDlg)
	ON_BN_CLICKED(IDC_RESTRICT, OnRestrict)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMakeOptionsDlg message handlers

void CMakeOptionsDlg::OnOK() 
{
   UpdateData(TRUE);

   if (m_restrict_visibility && m_maxdist <= 0.0) {
      AfxMessageBox(_T("Maximum distance must be over 0.0 if visibility is restricted"));
      return;
   }

   CDialog::OnOK();
}

BOOL CMakeOptionsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
   
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMakeOptionsDlg::OnRestrict() 
{
   UpdateData(TRUE);
   
   if (m_restrict_visibility) {
      GetDlgItem(IDC_MAXDIST)->EnableWindow(TRUE);
   }
   else {
      GetDlgItem(IDC_MAXDIST)->EnableWindow(FALSE);
   }
}

