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

// InsertColumnDlg.cpp : implementation file
//

#include "stdafx.h"
#include "depthmap.h"
#include "InsertColumnDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInsertColumnDlg dialog


CInsertColumnDlg::CInsertColumnDlg(AttributeTable *table, int col, CWnd* pParent /*=NULL*/)
	: CDialog(CInsertColumnDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInsertColumnDlg)
	m_selection_only = FALSE;
	//}}AFX_DATA_INIT

   // if true, then this dialog is used for replacing column contents, if false, for querying
   m_col = col;

   m_col_names.push_back("Ref Number");
   for (int i = 0; i < table->getColumnCount(); i++) {
      m_col_names.push_back(table->getColumnName(i));
   }
   if (m_col == -1) {
      m_formula_text = pstring(GetApp()->m_formula_cache);
   }
   else {
      m_formula_text = table->getColumnFormula(m_col);
   }
}

void CInsertColumnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInsertColumnDlg)
	DDX_Control(pDX, IDC_FORMULA, m_formula);
	DDX_Control(pDX, IDC_SELECTION_ONLY, m_selection_desc);
	DDX_Control(pDX, IDC_FORMULA_DESC, m_formula_desc);
	DDX_Control(pDX, IDC_USE_ATTRIBUTE, m_use_column);
	DDX_Control(pDX, IDC_COLUMN_NAMES, m_column_names);
	DDX_Check(pDX, IDC_SELECTION_ONLY, m_selection_only);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInsertColumnDlg, CDialog)
	//{{AFX_MSG_MAP(CInsertColumnDlg)
	ON_BN_CLICKED(IDC_USE_ATTRIBUTE, OnUseAttribute)
	ON_LBN_SELCHANGE(IDC_COLUMN_NAMES, OnSelChangeColumnNames)
	ON_LBN_SELCANCEL(IDC_COLUMN_NAMES, OnSelCancelColumnNames)
	ON_LBN_DBLCLK(IDC_COLUMN_NAMES, OnDblclkColumnNames)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInsertColumnDlg message handlers

BOOL CInsertColumnDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

   // set the formula to have a fixed width font:
   CHARFORMAT cf;
   cf.dwMask = CFM_CHARSET | CFM_FACE;
   cf.szFaceName[0] = '\0';
   cf.bCharSet = ANSI_CHARSET;
   cf.bPitchAndFamily = FF_MODERN;
   cf.cbSize = sizeof(cf);

   m_formula.SetDefaultCharFormat( cf );

   // take the table and list the current boxes:
   for (size_t i = 0; i < m_col_names.size(); i++) {
      m_column_names.AddString(CString(m_col_names[i].c_str()));
   }
   m_formula.SetWindowText(CString(m_formula_text.c_str()));
   m_column_names.SetCurSel(-1);
   m_use_column.EnableWindow(FALSE);

   if (m_col == -1) {
      // use for selection query
      // override title and names:
      SetWindowText(_T("Make selection"));

      m_formula_desc.SetWindowText(_T("Query"));
      m_selection_desc.SetWindowText(_T("Apply query to selected objects only"));
   }
   else {
      // it's important for the user to know the column name:
      // (note our column names lookup has "Ref Number" in the zero position, so add one:
      SetWindowText(CString("Replace values for ") + CString(m_col_names[m_col+1].c_str()));

      m_formula_desc.SetWindowText(_T("Formula"));
      m_selection_desc.SetWindowText(_T("Apply formula to selected objects only"));
   }

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInsertColumnDlg::OnSelChangeColumnNames() 
{
   m_use_column.EnableWindow(TRUE);
}

void CInsertColumnDlg::OnSelCancelColumnNames() 
{
   m_use_column.EnableWindow(FALSE);
}

void CInsertColumnDlg::OnUseAttribute() 
{
   // take the currently selected list item and use in formula	
   //int start,end;
   //m_formula.GetSel(start,end);
   CString string;
   m_column_names.GetText(m_column_names.GetCurSel(),string);
   string = "value(\""+ string + "\")";
   m_formula.ReplaceSel(string,TRUE);
   m_formula.SetModify(TRUE);
   m_formula.SetFocus();
}

void CInsertColumnDlg::OnDblclkColumnNames() 
{
   //int start,end;
   //m_formula.GetSel(start,end);
   CString string;
   m_column_names.GetText(m_column_names.GetCurSel(),string);
   string = "value(\""+ string + "\")";
   m_formula.ReplaceSel(string,TRUE);
   m_formula.SetModify(TRUE);
   m_formula.SetFocus();
}

void CInsertColumnDlg::OnOK() 
{
   UpdateData(TRUE);

   CString text;
   m_formula.GetWindowText(text);

   GetApp()->m_formula_cache = text;

   m_formula_text = pstring(text);

   // note formula text for column will have to be updated by calling function
   
	CDialog::OnOK();
}
