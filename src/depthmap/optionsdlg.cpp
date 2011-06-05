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

// OptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "depthmap.h"
#include "OptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg dialog

COptionsDlg::COptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptionsDlg)
	m_global = FALSE;
	m_local = FALSE;
	m_radius = _T("");
	m_gates_only = FALSE;
	m_output_type = -1;
	m_radius2 = _T("");
	//}}AFX_DATA_INIT

   m_output_type = GetApp()->m_options.output_type;

   m_local   = GetApp()->m_options.local;
   m_global  = GetApp()->m_options.global;
   m_gates_only = GetApp()->m_options.gates_only;

   if ((int) GetApp()->m_options.radius == -1) {
      m_radius = CString("n");
      m_radius2 = CString("n");
   }
   else if (m_output_type == Options::OUTPUT_VISUAL) {
	   char number[2];
	   sprintf( number, "%d", (int) GetApp()->m_options.radius );
      m_radius = CString(number);
      m_radius2 = "n";
   }
   else {
	   char number[32];
	   sprintf( number, "%g", GetApp()->m_options.radius );
      m_radius = "n";
      m_radius2 = CString(number);
   }
}


void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(COptionsDlg)
   DDX_Check(pDX, IDC_GLOBAL, m_global);
   DDX_Check(pDX, IDC_LOCAL, m_local);
   DDX_Text(pDX, IDC_RADIUS, m_radius);
   DDV_MaxChars(pDX, m_radius, 2);
   DDX_Radio(pDX, IDC_OUTPUT_TYPE, m_output_type);
   DDX_Text(pDX, IDC_RADIUS2, m_radius2);
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_LAYER_SELECTOR, m_layer_selector);
}


BEGIN_MESSAGE_MAP(COptionsDlg, CDialog)
	//{{AFX_MSG_MAP(COptionsDlg)
	ON_BN_CLICKED(IDC_OUTPUT_TYPE, OnOutputType)
	ON_BN_CLICKED(IDC_RADIO1, OnOutputType)
	ON_BN_CLICKED(IDC_RADIO2, OnOutputType)
	ON_BN_CLICKED(IDC_RADIO3, OnOutputType)
	ON_BN_CLICKED(IDC_RADIO4, OnOutputType)
	ON_EN_UPDATE(IDC_RADIUS, OnUpdateRadius)
	ON_EN_UPDATE(IDC_RADIUS2, OnUpdateRadius2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg message handlers

BOOL COptionsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
   for (size_t i = 0; i < m_layer_names.size(); i++) {
      m_layer_selector.AddString(CString(m_layer_names[i].c_str()));
   }
   m_layer_selector.SetCurSel(GetApp()->m_options.gatelayer + 1);

	OnOutputType();
   
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void COptionsDlg::OnUpdateRadius() 
{
   CString text;
   GetDlgItem(IDC_RADIUS)->GetWindowText(text);
   if (text.GetLength()) {
      if (!_wtoi(text) && text != _T("n")) {
         AfxMessageBox(_T("The radius must either be n or number in range 1-99"));
         GetDlgItem(IDC_RADIUS)->SetWindowText(_T("n"));
      }
   }		
}

void COptionsDlg::OnUpdateRadius2() 
{
   CString text;
   GetDlgItem(IDC_RADIUS2)->GetWindowText(text);
   if (text.GetLength()) {
      if (_wtof(text) == 0.0 && text != _T("n")) {
         AfxMessageBox(_T("The radius must either be n or a positive number"));
         GetDlgItem(IDC_RADIUS2)->SetWindowText(_T("n"));
      }
   }		
}

void COptionsDlg::OnOutputType() 
{
   UpdateData(TRUE);

   if (m_output_type == Options::OUTPUT_VISUAL) {
      GetDlgItem(IDC_LOCAL)->EnableWindow(TRUE);
      GetDlgItem(IDC_GLOBAL)->EnableWindow(TRUE);
      GetDlgItem(IDC_RADIUS)->EnableWindow(TRUE);
   }
   else {
      GetDlgItem(IDC_LOCAL)->EnableWindow(FALSE);
      GetDlgItem(IDC_GLOBAL)->EnableWindow(FALSE);
      GetDlgItem(IDC_RADIUS)->EnableWindow(FALSE);
      GetDlgItem(IDC_RADIUS)->SetWindowText(_T("n")); // <- essentially, undo changes
   }

   if (m_output_type == Options::OUTPUT_METRIC) {
      GetDlgItem(IDC_RADIUS2)->EnableWindow(TRUE);
   }
   else {
      GetDlgItem(IDC_RADIUS2)->SetWindowText(_T("n")); // <- essentially, undo changes
      GetDlgItem(IDC_RADIUS2)->EnableWindow(FALSE);
   }

}

void COptionsDlg::OnOK() 
{
	UpdateData(TRUE);

   GetApp()->m_options.local  = m_local;
   GetApp()->m_options.global = m_global;
   GetApp()->m_options.output_type = m_output_type;
   GetApp()->m_options.gates_only = m_gates_only;
   GetApp()->m_options.gatelayer = m_layer_selector.GetCurSel() - 1;

   if (m_output_type == Options::OUTPUT_VISUAL) {
      if (m_radius.Compare(_T("n")) == 0) { // 0 means identical
         GetApp()->m_options.radius = -1.0;
      }
      else {
         GetApp()->m_options.radius = (double) _wtoi(m_radius);
         if (GetApp()->m_options.radius <= 0.0) {
            AfxMessageBox(_T("The radius must either be n or a number in the range 1-99"));
            return;
         }
      }
   }
   else {
      if (m_radius2.Compare(_T("n")) == 0) { // 0 means identical
         GetApp()->m_options.radius = -1.0;
      }
      else {
         GetApp()->m_options.radius = _wtof(m_radius2);
         if (GetApp()->m_options.radius <= 0.0) {
            AfxMessageBox(_T("The radius must either be n or a positive number"));
            return;
         }
      }
   }
	
	CDialog::OnOK();
}
