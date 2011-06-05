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

// SetAttrValueDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Depthmap.h"
#include "SetAttrValueDlg.h"


// CSetAttrValueDlg dialog

IMPLEMENT_DYNAMIC(CSetAttrValueDlg, CDialog)

CSetAttrValueDlg::CSetAttrValueDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetAttrValueDlg::IDD, pParent)
   , m_value(_T(""))
{

}

CSetAttrValueDlg::~CSetAttrValueDlg()
{
}

void CSetAttrValueDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   DDX_Text(pDX, IDC_VALUE, m_value);
   DDX_Control(pDX, IDOK, m_tickbox);
   DDX_Control(pDX, IDCANCEL, m_cancelbox);
}


BEGIN_MESSAGE_MAP(CSetAttrValueDlg, CDialog)
END_MESSAGE_MAP()


// CSetAttrValueDlg message handlers

BOOL CSetAttrValueDlg::OnInitDialog()
{
   CDialog::OnInitDialog();

   CRect r;
   GetWindowRect(r);
   MoveWindow(m_pos.x-r.Width(),m_pos.y,r.Width(),r.Height());

   m_tickbox.AutoLoad(IDOK,this);
   m_cancelbox.AutoLoad(IDCANCEL,this);

   if (!m_locked_column) {
      GotoDlgCtrl(GetDlgItem(IDC_VALUE));
   }
   else {
      GetDlgItem(IDC_VALUE)->EnableWindow(FALSE);
   }

   return FALSE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}
