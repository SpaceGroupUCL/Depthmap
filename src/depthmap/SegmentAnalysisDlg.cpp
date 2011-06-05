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

// SegmentAnalysisDlg.cpp : implementation file
//

#include "stdafx.h"
#include "depthmap.h"
#include "SegmentAnalysisDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSegmentAnalysisDlg dialog


CSegmentAnalysisDlg::CSegmentAnalysisDlg(MetaGraph *graph, CWnd* pParent /*=NULL*/)
	: CDialog(CSegmentAnalysisDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSegmentAnalysisDlg)
	m_analysis_type = -1;
	m_radius = _T("");
	m_tulip_bins = 0;
	m_radius_type = -1;
	m_choice = FALSE;
	m_weighted = FALSE;
	m_attribute = -1;
	//}}AFX_DATA_INIT

   m_meta_graph = graph;

   if (GetApp()->m_options.tulip_bins == 0) {
      m_analysis_type = 1;
      m_tulip_bins = 1024;
   }
   else {
      m_analysis_type = 0;
      m_tulip_bins = GetApp()->m_options.tulip_bins;
   }
   m_choice = GetApp()->m_options.choice;
   m_radius_type = GetApp()->m_options.radius_type;
   if ((int) GetApp()->m_options.radius == -1) {
      m_radius = CString("n");
   }
   else {
	   char number[32];
	   sprintf( number, "%g", GetApp()->m_options.radius );
      m_radius = CString(number);
   }
   if (GetApp()->m_options.weighted_measure_col == -1) {
      m_weighted = FALSE;
      m_attribute = -1;
   }
   else {
      m_weighted = TRUE;
      m_attribute = 0;
   }
}

void CSegmentAnalysisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSegmentAnalysisDlg)
	DDX_Control(pDX, IDC_ATTRIBUTE_CHOOSER, m_attribute_chooser);
	DDX_Control(pDX, IDC_RADIUS, m_radius_ctrl);
	DDX_Radio(pDX, IDC_ANALYSIS_TYPE, m_analysis_type);
	DDX_Text(pDX, IDC_RADIUS, m_radius);
	DDX_Text(pDX, IDC_TULIP_BINS, m_tulip_bins);
	DDV_MinMaxInt(pDX, m_tulip_bins, 4, 1024);
	DDX_Radio(pDX, IDC_RADIUS_TYPE, m_radius_type);
	DDX_Check(pDX, IDC_CHOICE, m_choice);
	DDX_Check(pDX, IDC_WEIGHTED, m_weighted);
	DDX_CBIndex(pDX, IDC_ATTRIBUTE_CHOOSER, m_attribute);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSegmentAnalysisDlg, CDialog)
	//{{AFX_MSG_MAP(CSegmentAnalysisDlg)
	ON_BN_CLICKED(IDC_ANALYSIS_TYPE, OnAnalysisType)
	ON_BN_CLICKED(IDC_RADIO2, OnAnalysisTulip)
	ON_EN_UPDATE(IDC_RADIUS, OnUpdateRadius)
	ON_BN_CLICKED(IDC_WEIGHTED, OnWeighted)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSegmentAnalysisDlg message handlers

void CSegmentAnalysisDlg::OnAnalysisType() 
{
   UpdateData(TRUE);
   GetDlgItem(IDC_TULIP_BINS)->EnableWindow(TRUE);
   GetDlgItem(IDC_CHOICE)->EnableWindow(TRUE);
   GetDlgItem(IDC_RADIUS_TYPE)->EnableWindow(TRUE);
   GetDlgItem(IDC_TYPE_METRIC)->EnableWindow(TRUE);
   GetDlgItem(IDC_WEIGHTED)->EnableWindow(TRUE);
   GetDlgItem(IDC_ATTRIBUTE_CHOOSER)->EnableWindow(FALSE);
}

void CSegmentAnalysisDlg::OnAnalysisTulip() 
{
   // actually, not tulip -- they're switched, this is on analyse angular!!
   UpdateData(TRUE);
   m_choice = FALSE;
   m_radius_type = 2;
   m_analysis_type = 1;
   m_weighted = FALSE;
   m_attribute = -1;
   UpdateData(FALSE);
   GetDlgItem(IDC_TULIP_BINS)->EnableWindow(FALSE);
   GetDlgItem(IDC_CHOICE)->EnableWindow(FALSE);
   GetDlgItem(IDC_RADIUS_TYPE)->EnableWindow(FALSE);
   GetDlgItem(IDC_TYPE_METRIC)->EnableWindow(FALSE);
   GetDlgItem(IDC_WEIGHTED)->EnableWindow(FALSE);
   GetDlgItem(IDC_ATTRIBUTE_CHOOSER)->EnableWindow(FALSE);
}

void CSegmentAnalysisDlg::OnWeighted() 
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

BOOL CSegmentAnalysisDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

   const ShapeGraph& map = m_meta_graph->getDisplayedShapeGraph();
   const AttributeTable& table = map.getAttributeTable();
   for (int i = 0; i < table.getColumnCount(); i++) {
      m_attribute_chooser.AddString(CString(table.getColumnName(i).c_str()));
   }
   
   if (m_analysis_type == 1) {
      m_choice = FALSE;
      m_radius_type = 2;
      m_weighted = FALSE;
      UpdateData(FALSE);
      GetDlgItem(IDC_TULIP_BINS)->EnableWindow(FALSE);
      GetDlgItem(IDC_CHOICE)->EnableWindow(FALSE);
      GetDlgItem(IDC_RADIUS_TYPE)->EnableWindow(FALSE);
      GetDlgItem(IDC_TYPE_METRIC)->EnableWindow(FALSE);
      GetDlgItem(IDC_WEIGHTED)->EnableWindow(FALSE);
      GetDlgItem(IDC_ATTRIBUTE_CHOOSER)->EnableWindow(FALSE);
   }
   else {
      GetDlgItem(IDC_TULIP_BINS)->EnableWindow(TRUE);
      GetDlgItem(IDC_CHOICE)->EnableWindow(TRUE);
      GetDlgItem(IDC_RADIUS_TYPE)->EnableWindow(TRUE);
      GetDlgItem(IDC_TYPE_METRIC)->EnableWindow(TRUE);
      GetDlgItem(IDC_WEIGHTED)->EnableWindow(TRUE);
      if (m_weighted) {
         GetDlgItem(IDC_ATTRIBUTE_CHOOSER)->EnableWindow(TRUE);
         m_attribute = 0;  
         UpdateData(FALSE);
      }
      else {
         m_attribute = -1;
         GetDlgItem(IDC_ATTRIBUTE_CHOOSER)->EnableWindow(FALSE);
      }
   }
   
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSegmentAnalysisDlg::OnUpdateRadius() 
{
   CString text;
   GetDlgItem(IDC_RADIUS)->GetWindowText(text);
   if (!text.IsEmpty() && text.FindOneOf(_T("nN0123456789.")) == -1) {
      AfxMessageBox(_T("The radius must either be numeric or 'n'\nAlternatively, for multiple radii, type a list of comma separated numeric radii (you can include 'n')"));
      GetDlgItem(IDC_RADIUS)->SetWindowText(_T("n"));
      m_radius_ctrl.SetFocus();
   }
}

void CSegmentAnalysisDlg::OnOK() 
{
   UpdateData(TRUE);

   // my own validate on the radius (note: on fail to convert, atoi returns 0)
   if (m_radius.IsEmpty() || m_radius.FindOneOf(_T("nN123456789")) == -1) {
      AfxMessageBox(_T("The radius must either be numeric or 'n'\nAlternatively, for multiple radii, type a list of comma separated numeric radii (you can include 'n')"));
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
            double radius;
            if (m_radius_type == 0) {
               radius = _wtoi(curr_radius);
               if (radius < 1 || radius > 99) {
                  AfxMessageBox(_T("Each radius in the list must either be 'n' or a number in the range 1-99"));
                  m_radius_ctrl.SetFocus();
                  return;
               }
            }
            else {
               radius = _wtof(curr_radius);
               if (radius <= 0.0) {
                  AfxMessageBox(_T("Each radius in the list must either be 'n' or a number in the range 0.0 to infinity"));
                  m_radius_ctrl.SetFocus();
                  return;
               }
            }
            GetApp()->m_options.radius_list.add((double) radius);
         }
      }
   }
   while (curr_comma != -1);
   if (GetApp()->m_options.radius_list.size() == 0 || add_rn) {
      GetApp()->m_options.radius_list.push_back(-1);
   }

   if (m_tulip_bins % 2 != 0) { 
      AfxMessageBox(_T("The number of tulip bins must be an even number"));
      return;
   }

   GetApp()->m_options.choice = m_choice;
   GetApp()->m_options.radius_type = m_radius_type;

   if (m_analysis_type == 1) {
      GetApp()->m_options.tulip_bins = 0;
   }
   else {
      GetApp()->m_options.tulip_bins = m_tulip_bins;
   }

   // attributes:
   if (!m_weighted) {
      GetApp()->m_options.weighted_measure_col = -1;
   }
   else {
      GetApp()->m_options.weighted_measure_col = m_attribute;
   }

	CDialog::OnOK();
}
