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

// MakeLayerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "depthmap.h"
#include "MakeLayerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMakeLayerDlg dialog


CMakeLayerDlg::CMakeLayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMakeLayerDlg::IDD, pParent)
   , m_keeporiginal(TRUE)
{
	//{{AFX_DATA_INIT(CMakeLayerDlg)
	m_remove_stubs = FALSE;
	m_push_values = FALSE;
	m_percentage = 0;
	m_origin = _T("");
	m_layer_name = _T("");
	//}}AFX_DATA_INIT
   m_mapin = 0;
   m_mapout = 0;
}


void CMakeLayerDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CMakeLayerDlg)
   DDX_Control(pDX, IDC_LAYER_TYPE, m_layer_type);
   DDX_Check(pDX, IDC_REMOVE_STUBS, m_remove_stubs);
   DDX_Check(pDX, IDC_PUSH_VALUES, m_push_values);
   DDX_Text(pDX, IDC_PERCENTAGE_LENGTH, m_percentage);
   DDV_MinMaxInt(pDX, m_percentage, 0, 100);
   DDX_Text(pDX, IDC_ORGIN_LAYERS, m_origin);
   DDX_Text(pDX, IDC_LAYER_NAME, m_layer_name);
   DDV_MaxChars(pDX, m_layer_name, 40);
   //}}AFX_DATA_MAP
   DDX_Check(pDX, IDC_KEEPORIGINAL, m_keeporiginal);
}


BEGIN_MESSAGE_MAP(CMakeLayerDlg, CDialog)
	//{{AFX_MSG_MAP(CMakeLayerDlg)
	ON_CBN_SELCHANGE(IDC_LAYER_TYPE, OnSelchangeLayerType)
	ON_BN_CLICKED(IDC_REMOVE_STUBS, OnRemoveStubs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMakeLayerDlg message handlers

BOOL CMakeLayerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

   if (m_mapout & MAKELAYER_DRAWING) {
      m_layer_type.AddString(_T("Drawing Map"));
      m_lookup.push_back(MAKELAYER_DRAWING);
   }
   if (m_mapout & MAKELAYER_DATA) {
      m_layer_type.AddString(_T("Data Map"));
      m_lookup.push_back(MAKELAYER_DATA);
   }
   if (m_mapout & MAKELAYER_AXIAL) {
      m_layer_type.AddString(_T("Axial Map"));
      m_lookup.push_back(MAKELAYER_AXIAL);
   }
   if (m_mapout & MAKELAYER_CONVEX) {
      m_layer_type.AddString(_T("Convex Map"));
      m_lookup.push_back(MAKELAYER_CONVEX);
   }
   if (m_mapout & MAKELAYER_SEGMENT) {
      m_layer_type.AddString(_T("Segment Map"));
      m_lookup.push_back(MAKELAYER_SEGMENT);
   }

   m_layer_type.SetCurSel(0);
   OnSelchangeLayerType();

   if (m_mapin == MAKELAYER_DRAWING) {
      // hide push values:
      GetDlgItem(IDC_PUSH_VALUES)->ShowWindow(FALSE);
      // hide retain map:
      GetDlgItem(IDC_KEEPORIGINAL)->ShowWindow(FALSE);
      // make the dialog a bit smaller...
      CRect winrect;
      GetWindowRect(winrect);
      winrect.bottom -= 75;
      MoveWindow(winrect);
      CWnd *wnd = GetDlgItem(IDOK);
      wnd->GetWindowRect(winrect);
      ScreenToClient(winrect);
      winrect.top -= 75;
      winrect.bottom -=75;
      wnd->MoveWindow(winrect);
      wnd = GetDlgItem(IDCANCEL);
      wnd->GetWindowRect(winrect);
      ScreenToClient(winrect);
      winrect.top -= 75;
      winrect.bottom -= 75;
      wnd->MoveWindow(winrect);
   }
   if (m_mapin != MAKELAYER_AXIAL) {
      // hide remove stubs:
      GetDlgItem(IDC_REMOVE_STUBS)->ShowWindow(FALSE);
      GetDlgItem(IDC_PERCENTAGE_LENGTH)->ShowWindow(FALSE);
      GetDlgItem(IDC_REMOVE_STUBS_MORE)->ShowWindow(FALSE);
   }

   m_layer_type.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMakeLayerDlg::OnOK() 
{
   m_mapout = m_lookup[m_layer_type.GetCurSel()];

	CDialog::OnOK();
}

void CMakeLayerDlg::OnSelchangeLayerType() 
{
   int which = m_lookup[m_layer_type.GetCurSel()];

   switch (which) {
   case MAKELAYER_DRAWING:
      GetDlgItem(IDC_LAYER_NAME)->SetWindowText(_T("Drawing Map"));
      break;
   case MAKELAYER_DATA:
      GetDlgItem(IDC_LAYER_NAME)->SetWindowText(_T("Gate Map"));
      break;
   case MAKELAYER_AXIAL:
      GetDlgItem(IDC_LAYER_NAME)->SetWindowText(_T("Axial Map"));
      break;
   case MAKELAYER_CONVEX:
      GetDlgItem(IDC_LAYER_NAME)->SetWindowText(_T("Convex Map"));
      break;
   case MAKELAYER_SEGMENT:
      GetDlgItem(IDC_LAYER_NAME)->SetWindowText(_T("Segment Map"));
      break;
   }

   if (which == MAKELAYER_SEGMENT) {
      GetDlgItem(IDC_KEEPORIGINAL)->EnableWindow(TRUE);
      GetDlgItem(IDC_PUSH_VALUES)->EnableWindow(TRUE);
      GetDlgItem(IDC_REMOVE_STUBS)->EnableWindow(TRUE);
      GetDlgItem(IDC_PERCENTAGE_LENGTH)->EnableWindow(TRUE);
      GetDlgItem(IDC_REMOVE_STUBS_MORE)->EnableWindow(TRUE);
   }
   else if (which == MAKELAYER_DRAWING) {
      GetDlgItem(IDC_KEEPORIGINAL)->EnableWindow(FALSE);
      ((CButton *)GetDlgItem(IDC_KEEPORIGINAL))->SetCheck(TRUE);
      GetDlgItem(IDC_PUSH_VALUES)->EnableWindow(FALSE);
      ((CButton *)GetDlgItem(IDC_PUSH_VALUES))->SetCheck(FALSE);
      GetDlgItem(IDC_REMOVE_STUBS)->EnableWindow(FALSE);
      ((CButton *)GetDlgItem(IDC_REMOVE_STUBS))->SetCheck(FALSE);
      GetDlgItem(IDC_PERCENTAGE_LENGTH)->EnableWindow(FALSE);
      GetDlgItem(IDC_PERCENTAGE_LENGTH)->SetWindowText(_T("0"));
      GetDlgItem(IDC_REMOVE_STUBS_MORE)->EnableWindow(FALSE);
   }
   else {
      GetDlgItem(IDC_KEEPORIGINAL)->EnableWindow(TRUE);
      GetDlgItem(IDC_PUSH_VALUES)->EnableWindow(TRUE);
      GetDlgItem(IDC_REMOVE_STUBS)->EnableWindow(FALSE);
      GetDlgItem(IDC_PERCENTAGE_LENGTH)->EnableWindow(FALSE);
      GetDlgItem(IDC_REMOVE_STUBS_MORE)->EnableWindow(FALSE);
   }
}

void CMakeLayerDlg::OnRemoveStubs() 
{
   if (((CButton *)GetDlgItem(IDC_REMOVE_STUBS))->GetCheck()) {
      GetDlgItem(IDC_PERCENTAGE_LENGTH)->SetWindowText(_T("25"));
      GetDlgItem(IDC_PERCENTAGE_LENGTH)->EnableWindow(TRUE);
   }
   else {
      GetDlgItem(IDC_PERCENTAGE_LENGTH)->SetWindowText(_T("0"));
      GetDlgItem(IDC_PERCENTAGE_LENGTH)->EnableWindow(FALSE);
   }
}
