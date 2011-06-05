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

// DepthmapOptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Depthmap.h"
#include "DepthmapOptionsDlg.h"


// CDepthmapOptionsDlg dialog

IMPLEMENT_DYNAMIC(CDepthmapOptionsDlg, CDialog)

CDepthmapOptionsDlg::CDepthmapOptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDepthmapOptionsDlg::IDD, pParent)
   , m_show_research_toolbar(FALSE)
{

}

CDepthmapOptionsDlg::~CDepthmapOptionsDlg()
{
}

void CDepthmapOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   DDX_Check(pDX, IDC_SHOW_RESEARCH_TOOLBAR, m_show_research_toolbar);
}


BEGIN_MESSAGE_MAP(CDepthmapOptionsDlg, CDialog)
END_MESSAGE_MAP()


// CDepthmapOptionsDlg message handlers

BOOL CDepthmapOptionsDlg::OnInitDialog()
{
   CDialog::OnInitDialog();

   GetDlgItem(IDC_SHOW_RESEARCH_TOOLBAR)->EnableWindow(TRUE);

   return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}
