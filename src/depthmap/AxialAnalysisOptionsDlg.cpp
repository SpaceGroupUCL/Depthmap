// AxialAnalysisOptionsDlg.cpp : implementation file
//
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

#include "stdafx.h"
#include "depthmap.h"
#include "AxialAnalysisOptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxialAnalysisOptionsDlg dialog


CAxialAnalysisOptionsDlg::CAxialAnalysisOptionsDlg(MetaGraph *graph, CWnd* pParent /*=NULL*/)
	: CDialog(CAxialAnalysisOptionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAxialAnalysisOptionsDlg)
	m_radius = _T("");
	m_choice = FALSE;
	m_attribute = -1;
	m_weighted = FALSE;
	m_rra = FALSE;
	m_local = FALSE;
	//}}AFX_DATA_INIT

   m_meta_graph = graph;

   m_choice = GetApp()->m_options.choice;
   m_local = GetApp()->m_options.local;
   m_rra = GetApp()->m_options.fulloutput;

   m_radius = "n";

   if (GetApp()->m_options.weighted_measure_col == -1) {
      m_weighted = FALSE;
      m_attribute = -1;
   }
   else {
      m_weighted = TRUE;
      m_attribute = 0;
   }
}

void CAxialAnalysisOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAxialAnalysisOptionsDlg)
	DDX_Control(pDX, IDC_ATTRIBUTE_CHOOSER, m_attribute_chooser);
	DDX_Control(pDX, IDC_RADIUS, m_radius_ctrl);
	DDX_Text(pDX, IDC_RADIUS, m_radius);
	DDX_Check(pDX, IDC_CHOICE, m_choice);
	DDX_CBIndex(pDX, IDC_ATTRIBUTE_CHOOSER, m_attribute);
	DDX_Check(pDX, IDC_WEIGHTED, m_weighted);
	DDX_Check(pDX, IDC_INCLUDE_RA, m_rra);
	DDX_Check(pDX, IDC_LOCAL, m_local);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAxialAnalysisOptionsDlg, CDialog)
	//{{AFX_MSG_MAP(CAxialAnalysisOptionsDlg)
	ON_EN_UPDATE(IDC_RADIUS, OnUpdateRadius)
	ON_BN_CLICKED(IDC_WEIGHTED, OnWeighted)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxialAnalysisOptionsDlg message handlers

BOOL CAxialAnalysisOptionsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

   const ShapeGraph& map = m_meta_graph->getDisplayedShapeGraph();
   const AttributeTable& table = map.getAttributeTable();
   for (int i = 0; i < table.getColumnCount(); i++) {
      m_attribute_chooser.AddString( CString(table.getColumnName(i).c_str()) );
   }
 
   if (m_weighted) {
      GetDlgItem(IDC_ATTRIBUTE_CHOOSER)->EnableWindow(TRUE);
      m_attribute = 0;  
      UpdateData(FALSE);
   }
   else {
      m_attribute = -1;
      GetDlgItem(IDC_ATTRIBUTE_CHOOSER)->EnableWindow(FALSE);
   }
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAxialAnalysisOptionsDlg::OnUpdateRadius() 
{
   CString text;
   GetDlgItem(IDC_RADIUS)->GetWindowText(text);
   if (!text.IsEmpty() && text.FindOneOf(_T("nN123456789")) == -1) {
      AfxMessageBox(_T("The radius must either be numeric or 'n'\n")
                    _T("Alternatively, for multiple radii, type a list of comma separated numeric radii (you can include 'n')"));
      GetDlgItem(IDC_RADIUS)->SetWindowText(_T("n"));
      m_radius_ctrl.SetFocus();
   }
}

void CAxialAnalysisOptionsDlg::OnWeighted() 
{
   if (((CButton *)GetDlgItem(IDC_WEIGHTED))->GetCheck()) {
      UpdateData(TRUE);
      GetDlgItem(IDC_ATTRIBUTE_CHOOSER)->EnableWindow(TRUE);
      m_attribute = 0;
      UpdateData(FALSE);
   }
   else {
      UpdateData(TRUE);
      GetDlgItem(IDC_ATTRIBUTE_CHOOSER)->EnableWindow(FALSE);
      m_attribute = -1;
      UpdateData(FALSE);
   }
}

void CAxialAnalysisOptionsDlg::OnOK() 
{
   UpdateData(TRUE);

   if (m_radius.IsEmpty() || m_radius.FindOneOf(_T("nN123456789")) == -1) {
      AfxMessageBox(_T("The radius must either be numeric or 'n'\n")
                    _T("Alternatively, for multiple radii, type a list of comma separated numeric radii (you can include 'n')"));
      m_radius = _T("n");
      UpdateData(FALSE);
      m_radius_ctrl.SetFocus();
      return;
   }
   // now parse radius list:
   GetApp()->m_options.radius_list.clear();
   CString curr_radius;
   int curr_comma = -1, last_comma = 0;
   bool add_rn = false;
   do {
      curr_comma = m_radius.Find(',',last_comma);
      if (curr_comma != -1) {
         curr_radius = m_radius.Mid(last_comma, curr_comma-last_comma);
         last_comma = curr_comma + 1;
      }
      else {
         curr_radius = m_radius.Mid(last_comma);
      }
      curr_radius.TrimLeft(' ');
      curr_radius.TrimRight(' ');
      if (!curr_radius.IsEmpty()) {
         if (curr_radius == "n" || curr_radius == "N") {
            add_rn = true;
         }
         else {
            int radius = _wtoi(curr_radius);
            if (radius <= 0) {
               AfxMessageBox(_T("Each radius in the list must either be 'n' or a number in the range 1-99"));
               m_radius_ctrl.SetFocus();
               return;
            }
            GetApp()->m_options.radius_list.add((double) radius);
         }
      }
   }
   while (curr_comma != -1);
   if (GetApp()->m_options.radius_list.size() == 0 || add_rn) {
      GetApp()->m_options.radius_list.push_back(-1);
   }

   GetApp()->m_options.choice = m_choice;
   GetApp()->m_options.local = m_local;
   GetApp()->m_options.fulloutput = m_rra;

   // attributes:
   if (!m_weighted) {
      GetApp()->m_options.weighted_measure_col = -1;
   }
   else {
      GetApp()->m_options.weighted_measure_col = m_attribute;
   }


	CDialog::OnOK();
}
