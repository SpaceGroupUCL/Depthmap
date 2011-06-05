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

// TopoMetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Depthmap.h"
#include "TopoMetDlg.h"


// CTopoMetDlg dialog

IMPLEMENT_DYNAMIC(CTopoMetDlg, CDialog)

CTopoMetDlg::CTopoMetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTopoMetDlg::IDD, pParent)
   , m_topological(0)
   , m_radius(_T(""))
   , m_selected_only(FALSE)
{
   m_radius = _T("n");
}

CTopoMetDlg::~CTopoMetDlg()
{
}

void CTopoMetDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   DDX_Radio(pDX, IDC_TOPOLOGICAL, m_topological);
   DDX_Text(pDX, IDC_RADIUS, m_radius);
   DDX_Check(pDX, IDC_SELECTED_ONLY, m_selected_only);
}


BEGIN_MESSAGE_MAP(CTopoMetDlg, CDialog)
   ON_BN_CLICKED(IDOK, &CTopoMetDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CTopoMetDlg message handlers

void CTopoMetDlg::OnBnClickedOk()
{
   UpdateData(TRUE);

   m_radius.TrimLeft(' ');
   m_radius.TrimRight(' ');

   // my own validate on the radius (note: on fail to convert, atoi returns 0)
   if (m_radius.IsEmpty() || m_radius.FindOneOf(_T("nN123456789")) == -1) {
      AfxMessageBox(_T("The radius must either be numeric or 'n'"));
      m_radius = _T("n");
      UpdateData(FALSE);
      return;
   }
   
   if (m_radius == "n" || m_radius == "N") {
      m_dradius = -1.0;
   }
   else {
      m_dradius = _wtof(m_radius);
      if (m_dradius <= 0.0) {
         AfxMessageBox(_T("The radius must either be 'n' or a number in the range 0.0 to infinity"));
         return;
      }
   }

   OnOK();
}
