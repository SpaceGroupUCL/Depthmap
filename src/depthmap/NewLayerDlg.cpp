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

// NewLayerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "depthmap.h"
#include "NewLayerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewLayerDlg dialog


CNewLayerDlg::CNewLayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewLayerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewLayerDlg)
	m_layer_type = -1;
	m_name = _T("");
	//}}AFX_DATA_INIT
   m_layer_type = 0;
   m_name = _T("Gate Map");
}


void CNewLayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewLayerDlg)
	DDX_Control(pDX, IDC_LAYER_TYPE, m_layer_selector);
	DDX_CBIndex(pDX, IDC_LAYER_TYPE, m_layer_type);
	DDX_Text(pDX, IDC_NAME, m_name);
	DDV_MaxChars(pDX, m_name, 32);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewLayerDlg, CDialog)
	//{{AFX_MSG_MAP(CNewLayerDlg)
	ON_CBN_SELCHANGE(IDC_LAYER_TYPE, OnSelchangeLayerType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewLayerDlg message handlers

void CNewLayerDlg::OnOK() 
{
   UpdateData(TRUE);

   if (m_name.IsEmpty()) {
      AfxMessageBox(_T("Please enter a name for the new map"));
   }
   
	CDialog::OnOK();
}

void CNewLayerDlg::OnSelchangeLayerType() 
{
   int which = m_layer_selector.GetCurSel();

   switch (which) {
   case 0:
      GetDlgItem(IDC_NAME)->SetWindowText(_T("Gate Map"));
      break;
   case 1:
      GetDlgItem(IDC_NAME)->SetWindowText(_T("Convex Map"));
      break;
   case 2:
      GetDlgItem(IDC_NAME)->SetWindowText(_T("Axial Map"));
      break;
   case 3:
      GetDlgItem(IDC_NAME)->SetWindowText(_T("Pesh Map"));
      break;
   }
}
