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

// RenameObjectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "depthmap.h"
#include "RenameObjectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRenameObjectDlg dialog


CRenameObjectDlg::CRenameObjectDlg(const CString& object_type, const CString& existing_name, CWnd* pParent /*=NULL*/)
	: CDialog(CRenameObjectDlg::IDD, pParent)
   , m_prompt(_T(""))
{
	//{{AFX_DATA_INIT(CRenameObjectDlg)
	m_object_name = _T("");
	//}}AFX_DATA_INIT
   m_object_name = existing_name;
   
   m_object_type = object_type; // e.g., Column, Layer, etc
}


void CRenameObjectDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CRenameObjectDlg)
   DDX_Text(pDX, IDC_COLUMN_NAME, m_object_name);
   //}}AFX_DATA_MAP
   DDX_Text(pDX, IDC_PROMPT, m_prompt);
}


BEGIN_MESSAGE_MAP(CRenameObjectDlg, CDialog)
	//{{AFX_MSG_MAP(CRenameObjectDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRenameObjectDlg message handlers

BOOL CRenameObjectDlg::OnInitDialog()
{
   CDialog::OnInitDialog();

   CString lower_object_type = m_object_type;
   lower_object_type.MakeLower();

   if (m_object_name.IsEmpty()) {
      CString title = CString("New ") + m_object_type;
      SetWindowText(title);
      m_prompt = CString("New ") + lower_object_type + CString(" name:");
      m_object_name = CString("<New ") + m_object_type + CString(">");
   }
   else {
      CString title = CString("Rename ") + m_object_type;
      SetWindowText(title);
      m_prompt = CString("Rename ") + lower_object_type + CString(" to:");
   }
   UpdateData(FALSE);

   return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}
