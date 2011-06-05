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

// FindLocDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Depthmap.h"
#include "FindLocDlg.h"


// CFindLocDlg dialog

IMPLEMENT_DYNAMIC(CFindLocDlg, CDialog)

CFindLocDlg::CFindLocDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFindLocDlg::IDD, pParent)
   , m_x(0)
   , m_y(0)
{

}

CFindLocDlg::~CFindLocDlg()
{
}

void CFindLocDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   DDX_Text(pDX, IDC_EDITX, m_x);
   DDX_Text(pDX, IDC_EDITY, m_y);
}


BEGIN_MESSAGE_MAP(CFindLocDlg, CDialog)
END_MESSAGE_MAP()


// CFindLocDlg message handlers

void CFindLocDlg::OnOK()
{
   UpdateData(TRUE);

   Point2f p(m_x,m_y);
   if (!m_bounds.contains_touch(p)) {
      AfxMessageBox(_T("This point is outside the bounds of your map"));
      return;
   }

   CDialog::OnOK();
}
