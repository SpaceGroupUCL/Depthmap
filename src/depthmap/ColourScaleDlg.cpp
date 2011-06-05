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

// ColourScaleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "depthmap.h"
#include "MainFrm.h"
#include "ColourScaleDlg.h"
#include "GraphDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColourScaleDlg dialog


CColourScaleDlg::CColourScaleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CColourScaleDlg::IDD, pParent)
   , m_show_lines(FALSE)
   , m_show_fill(FALSE)
   , m_show_centroids(FALSE)
{
	//{{AFX_DATA_INIT(CColourScaleDlg)
	m_blue = _T("");
	m_red = _T("");
	m_color = -1;
	//}}AFX_DATA_INIT

   m_docked = false;

   m_viewDoc = NULL;

   m_red_brush.CreateSolidBrush(RGB(255,128,128));
   m_blue_brush.CreateSolidBrush(RGB(128,128,255));
}

void CColourScaleDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CColourScaleDlg)
   DDX_Control(pDX, IDC_RED_VALUE, m_red_value_window);
   DDX_Control(pDX, IDC_BLUE_VALUE, m_blue_value_window);
   DDX_Control(pDX, IDC_RED_SLIDER, m_red_slider_ctrl);
   DDX_Control(pDX, IDC_BLUE_SLIDER, m_blue_slider_ctrl);
   DDX_Text(pDX, IDC_BLUE, m_blue);
   DDX_Text(pDX, IDC_RED, m_red);
   //}}AFX_DATA_MAP
   DDX_Check(pDX, IDC_SHOW_LINES, m_show_lines);
   DDX_Check(pDX, IDC_SHOW_FILL, m_show_fill);
   DDX_Check(pDX, IDC_SHOW_CENTROIDS, m_show_centroids);
   DDX_Control(pDX, IDC_COLOR, m_color_type);
}


BEGIN_MESSAGE_MAP(CColourScaleDlg, CDialog)
	//{{AFX_MSG_MAP(CColourScaleDlg)
	ON_EN_CHANGE(IDC_BLUE_VALUE, OnChangeBlueValue)
	ON_EN_CHANGE(IDC_RED_VALUE, OnChangeRedValue)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_RED_SLIDER, OnReleasedRedSlider)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_BLUE_SLIDER, OnReleasedBlueSlider)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COLOR, OnSelchangeColor)
	ON_WM_CTLCOLOR()
	ON_WM_MOVE()
	//}}AFX_MSG_MAP
   ON_MESSAGE(WM_DMP_FOCUS_GRAPH, OnFocusGraph)
   ON_BN_CLICKED(IDC_SHOW_LINES, &CColourScaleDlg::OnBnClickedShowLines)
   ON_BN_CLICKED(IDC_SHOW_FILL, &CColourScaleDlg::OnBnClickedShowFill)
   ON_BN_CLICKED(IDC_SHOW_CENTROIDS, &CColourScaleDlg::OnBnClickedShowCentroids)
   ON_BN_CLICKED(IDC_APPLYTOALL, &CColourScaleDlg::OnBnClickedApplytoall)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColourScaleDlg message handlers

double CColourScaleDlg::GetActualValue(double sliderpos)
{
   return sliderpos * (m_display_max - m_display_min) + m_display_min;
}

float CColourScaleDlg::GetNormValue(double actualval)
{
   return ((actualval - m_display_min) / (m_display_max - m_display_min));
}

LRESULT CColourScaleDlg::OnFocusGraph(WPARAM wParam, LPARAM lParam)
{
   CGraphDoc *pDoc = (CGraphDoc *) wParam;

   if (lParam == CGraphDoc::CONTROLS_DESTROYALL && pDoc == m_viewDoc) {      // Lost graph
      m_viewDoc = NULL;
      MyUpdateData(FALSE,FALSE);
   }
   else if (lParam == CGraphDoc::CONTROLS_LOADALL && pDoc != m_viewDoc) {    // [Possible] change of window (sent on focus)
      m_viewDoc = pDoc;
      MyUpdateData(FALSE,FALSE);
   }
   else if (lParam != CGraphDoc::CONTROLS_LOADALL && pDoc == m_viewDoc) {    // Force update if match current window
      MyUpdateData(FALSE,FALSE);
   }

   return 0;
}

void CColourScaleDlg::Clear()
{
   m_color = -1;
   GetDlgItem(IDC_COLOR)->EnableWindow(FALSE);
   m_blue = "Min";
   m_red = "Max";
   m_blue_value_window.EnableWindow(FALSE);
   m_blue_value_window.SetWindowText(_T(""));
   m_red_value_window.EnableWindow(FALSE);
   m_red_value_window.SetWindowText(_T(""));
   m_blue_slider_ctrl.EnableWindow(FALSE);
   m_blue_slider_ctrl.SetPos(0);
   m_red_slider_ctrl.EnableWindow(FALSE);
   m_red_slider_ctrl.SetPos(100);
   GetDlgItem(IDOK)->EnableWindow(FALSE);
   GetDlgItem(IDC_APPLYTOALL)->EnableWindow(FALSE);
}

void CColourScaleDlg::Fill()
{
   if (m_color == 0 || m_color == 3 || m_color == 5) {
      m_red = "Red";
      m_blue = "Blue";
   }
   else if (m_color == 4) {
      m_red = "Orange";
      m_blue = "Purple";
   }
   else if (m_color == 1) {
      m_red = "White";
      m_blue = "Black";
   }
   else {
      m_red = "Thick";
      m_blue = "Thin";
   }

   TCHAR text[32];
   swprintf(text, _T("%.2f"), GetActualValue(m_displayparams.blue));
   m_blue_value_window.SetWindowText(text);
   swprintf(text, _T("%.2f"), GetActualValue(m_displayparams.red));
   m_red_value_window.SetWindowText(text);

   m_blue_slider_ctrl.SetPos(int(m_displayparams.blue * 100));
   m_red_slider_ctrl.SetPos(int(m_displayparams.red * 100));

   m_color_type.EnableWindow(TRUE);
   m_blue_value_window.EnableWindow(TRUE);
   m_red_value_window.EnableWindow(TRUE);
   m_blue_slider_ctrl.EnableWindow(TRUE);
   m_red_slider_ctrl.EnableWindow(TRUE);
   GetDlgItem(IDOK)->EnableWindow(FALSE);
   GetDlgItem(IDC_APPLYTOALL)->EnableWindow(FALSE);
}

BOOL CColourScaleDlg::OnInitDialog() 
{
   CDialog::OnInitDialog();
	
   m_blue_slider_ctrl.SetRange(0,100);
   m_red_slider_ctrl.SetRange(0,100);
   m_blue_slider_ctrl.SetTicFreq(10);
   m_red_slider_ctrl.SetTicFreq(10);

   // these are out of order...
   m_color_type.AddString(_T("Equal Ranges (3-Colour)"));    // 0
   m_color_type.AddString(_T("Equal Ranges (Blue-Red)"));    // 5
   m_color_type.AddString(_T("Equal Ranges (Purple-Orange)"));    // 4
   m_color_type.AddString(_T("Depthmap Classic"));           // 3
   m_color_type.AddString(_T("Equal Ranges (Greyscale)"));   // 1
   m_color_type.AddString(_T("Equal Ranges (Monochrome)"));  // 2

   m_color_type_map.push_back(0);
   m_color_type_map.push_back(5);
   m_color_type_map.push_back(4);
   m_color_type_map.push_back(3);
   m_color_type_map.push_back(1);
   m_color_type_map.push_back(2);

   Clear();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CColourScaleDlg::OnReleasedBlueSlider(NMHDR* pNMHDR, LRESULT* pResult) 
{
   double value = double(m_blue_slider_ctrl.GetPos()) / 100.0;
   TCHAR text[32];
   swprintf(text, _T("%.2f"), GetActualValue(value));
   m_blue_value_window.SetWindowText(text);

	*pResult = 0;
}

void CColourScaleDlg::OnReleasedRedSlider(NMHDR* pNMHDR, LRESULT* pResult) 
{
   double value = double(m_red_slider_ctrl.GetPos()) / 100.0;
   TCHAR text[32];
   swprintf(text, _T("%.2f"), GetActualValue(value));
   m_red_value_window.SetWindowText(text);

	*pResult = 0;
}

void CColourScaleDlg::OnChangeBlueValue() 
{
   CString str;
   m_blue_value_window.GetWindowText(str);
   m_displayparams.blue = GetNormValue(_wtof(str));
   m_blue_slider_ctrl.SetPos(m_displayparams.blue * 100.0);
   GetDlgItem(IDOK)->EnableWindow(TRUE);
   GetDlgItem(IDC_APPLYTOALL)->EnableWindow(TRUE);
}

void CColourScaleDlg::OnChangeRedValue() 
{
   CString str;
   m_red_value_window.GetWindowText(str);
   m_displayparams.red = GetNormValue(_wtof(str));
   m_red_slider_ctrl.SetPos(m_displayparams.red * 100);
   GetDlgItem(IDOK)->EnableWindow(TRUE);
   GetDlgItem(IDC_APPLYTOALL)->EnableWindow(TRUE);
}

void CColourScaleDlg::OnSelchangeColor() 
{
   UpdateData(TRUE);

   m_color = m_color_type_map[m_color_type.GetCurSel()];

   if (m_color == 0 || m_color == 3 || m_color == 5) {
      m_red = "Red";
      m_blue = "Blue";
   }
   else if (m_color == 4) {
      m_red = "Orange";
      m_blue = "Purple";
   }
   else if (m_color == 1) {
      m_red = "White";
      m_blue = "Black";
   }
   else {
      m_red = "Thick";
      m_blue = "Thin";
   }

   UpdateData(FALSE);

   GetDlgItem(IDOK)->EnableWindow(TRUE);
   GetDlgItem(IDC_APPLYTOALL)->EnableWindow(TRUE);
}

void CColourScaleDlg::MyUpdateData(BOOL dir, BOOL apply_to_all)
{
   if (dir == FALSE) {
      // push data to controls:
      if (m_viewDoc == NULL) {
         Clear();
      }
      else {
         MetaGraph *graph = m_viewDoc->m_meta_graph;
         if (graph->viewingProcessed()) {
            if (graph->getViewClass() & MetaGraph::VIEWVGA) {
               PointMap& map = graph->getDisplayedPointMap();
               m_display_min = map.getDisplayMinValue();
               m_display_max = map.getDisplayMaxValue();
               m_displayparams = map.getDisplayParams();
               m_color = m_displayparams.colorscale;
            }
            else if (graph->getViewClass() & MetaGraph::VIEWAXIAL) {
               ShapeGraph& map = graph->getDisplayedShapeGraph();
               m_display_min = map.getDisplayMinValue();
               m_display_max = map.getDisplayMaxValue();
               m_displayparams = map.getDisplayParams();
               m_color = m_displayparams.colorscale;
               bool show_lines = m_show_lines, show_fill = m_show_fill, show_centroids = m_show_centroids;
               map.getPolygonDisplay(show_lines,show_fill,show_centroids);
               m_show_lines = show_lines; m_show_fill = show_fill; m_show_centroids = show_centroids;
            }
            else if (graph->getViewClass() & MetaGraph::VIEWDATA) {
               ShapeMap& map = graph->getDisplayedDataMap();
               m_display_min = map.getDisplayMinValue();
               m_display_max = map.getDisplayMaxValue();
               m_displayparams = map.getDisplayParams();
               m_color = m_displayparams.colorscale;
               bool show_lines = m_show_lines, show_fill = m_show_fill, show_centroids = m_show_centroids;
               map.getPolygonDisplay(show_lines,show_fill,show_centroids);
               m_show_lines = show_lines; m_show_fill = show_fill; m_show_centroids = show_centroids;
            }
            for (int i = 0; i < m_color_type_map.size(); i++) {
               if (m_color == m_color_type_map[i]) {
                  m_color_type.SetCurSel(i);
               }
            }
            Fill();
         }
         else {
            Clear();
         }
      }
      UpdateData(FALSE);
   }
   else {
      // get data from controls:
      UpdateData(TRUE);

      if (m_viewDoc != NULL) {
         MetaGraph *graph = m_viewDoc->m_meta_graph;
         m_color = m_color_type_map[m_color_type.GetCurSel()];
         m_displayparams.colorscale = m_color;
         if (graph->getViewClass() & MetaGraph::VIEWVGA) {
            graph->getDisplayedPointMap().setDisplayParams( m_displayparams, apply_to_all );
         }
         else if (graph->getViewClass() & MetaGraph::VIEWAXIAL) {
            graph->getDisplayedShapeGraph().setDisplayParams( m_displayparams, apply_to_all );
            graph->getDisplayedShapeGraph().setPolygonDisplay(m_show_lines,m_show_fill,m_show_centroids);
         }
         else if (graph->getViewClass() & MetaGraph::VIEWDATA) {
            graph->getDisplayedDataMap().setDisplayParams( m_displayparams, apply_to_all );
            graph->getDisplayedDataMap().setPolygonDisplay(m_show_lines,m_show_fill,m_show_centroids);
         }
      }
      m_viewDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_DEPTHMAPVIEW_SETUP);
   }
}

void CColourScaleDlg::OnOK() 
{
   MyUpdateData(TRUE,FALSE);

   // don't destroy
   GetDlgItem(IDOK)->EnableWindow(FALSE);
}

void CColourScaleDlg::OnBnClickedApplytoall()
{
   MyUpdateData(TRUE,TRUE);

   // don't destroy
   GetDlgItem(IDOK)->EnableWindow(FALSE);
   GetDlgItem(IDC_APPLYTOALL)->EnableWindow(FALSE);
}


void CColourScaleDlg::OnCancel() 
{
   // don't destroy, simply hide:
   ShowWindow(SW_HIDE);
}

void CColourScaleDlg::OnClose() 
{
   // don't destroy, simply hide:
   ShowWindow(SW_HIDE);
}


HBRUSH CColourScaleDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
   if (pWnd == &m_red_slider_ctrl) {
      hbr = m_red_brush;
   }
   else if (pWnd == &m_blue_slider_ctrl) {
      hbr = m_blue_brush;
   }

	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CColourScaleDlg::OnMove(int x, int y) 
{
   if (IsWindowVisible()) {
      CRect recta,rectb;
      GetParent()->GetWindowRect(recta);
      GetWindowRect(rectb);
      if (abs(x - (recta.right - 7 - rectb.Width())) < 32) {
         m_docked = true;
      }
      else {
         m_docked = false;
      }
   }

	CDialog::OnMove(x, y);
}

void CColourScaleDlg::OnBnClickedShowLines()
{
   GetDlgItem(IDOK)->EnableWindow(TRUE);
}

void CColourScaleDlg::OnBnClickedShowFill()
{
   GetDlgItem(IDOK)->EnableWindow(TRUE);
}

void CColourScaleDlg::OnBnClickedShowCentroids()
{
   GetDlgItem(IDOK)->EnableWindow(TRUE);
}

