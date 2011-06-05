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

// ViewSelectorSub.cpp : implementation file
//

#include "stdafx.h"
#include "depthmap.h"
#include "graphdoc.h"
#include "childfrm.h"
#include "PlotView.h"
#include "ViewSelectorSub.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewSelectorSub dialog


CViewSelectorSub::CViewSelectorSub(CWnd* pParent /*=NULL*/)
	: CDialog(CViewSelectorSub::IDD, pParent)
{
	//{{AFX_DATA_INIT(CViewSelectorSub)
	//}}AFX_DATA_INIT

   m_axis = 0;
}


void CViewSelectorSub::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewSelectorSub)
	DDX_Control(pDX, IDC_LABEL, m_label);
	DDX_Control(pDX, IDC_VIEW_SELECTOR, m_view_selector);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewSelectorSub, CDialog)
	//{{AFX_MSG_MAP(CViewSelectorSub)
	ON_CBN_SELCHANGE(IDC_VIEW_SELECTOR, OnSelchangeViewSelector)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewSelectorSub message handlers

void CViewSelectorSub::RedoMenu( MetaGraph& meta_graph, bool redraw ) 
{
   // this will be used to distinguish between viewing VGA and axial maps
   int view_class = meta_graph.getViewClass() & (MetaGraph::VIEWVGA | MetaGraph::VIEWAXIAL | MetaGraph::VIEWDATA);

   m_view_selection = -1;

   int curr_j = 0;

   if (meta_graph.setLock(this)) {

      m_view_map_entries.clear();

      if (view_class == MetaGraph::VIEWVGA) {

         PointMap& map = meta_graph.getDisplayedPointMap();
         int displayed_ref = map.getDisplayedAttribute();

         const AttributeTable& table = map.getAttributeTable();
         m_view_map_entries.add(0, "Ref Number");
         for (int i = 0; i < table.getColumnCount(); i++) {
            m_view_map_entries.add(i+1, table.getColumnName(i));
            if (map.getDisplayedAttribute() == i) {
               curr_j = i + 1;
            }
         }
      }
      else if (view_class == MetaGraph::VIEWAXIAL) {
         // using attribute tables is very, very simple...
         const ShapeGraph& map = meta_graph.getDisplayedShapeGraph();
         const AttributeTable& table = map.getAttributeTable();
         m_view_map_entries.add(0, "Ref Number");
         curr_j = 0;
         for (int i = 0; i < table.getColumnCount(); i++) {
            m_view_map_entries.add(i+1, table.getColumnName(i));
            if (map.getDisplayedAttribute() == i) {
               curr_j = i + 1;
            }
         }
      }
      else if (view_class == MetaGraph::VIEWDATA) {
         // using attribute tables is very, very simple...
         const ShapeMap& map = meta_graph.getDisplayedDataMap();
         const AttributeTable& table = map.getAttributeTable();
         m_view_map_entries.add(0, "Ref Number");
         curr_j = 0;
         for (int i = 0; i < table.getColumnCount(); i++) {
            m_view_map_entries.add(i+1, table.getColumnName(i));
            if (map.getDisplayedAttribute() == i) {
               curr_j = i + 1;
            }
         }
      }

      meta_graph.releaseLock(this);
   }

   int cur_sel = 0;

   m_view_selector.ResetContent();

   for (size_t i = 0; i < m_view_map_entries.size(); i++) {
      if (curr_j == m_view_map_entries.key(i)) {
         cur_sel = (int) i;
      }
      m_view_selector.AddString( CString(m_view_map_entries.value(i).c_str()) );
   }

   if (!m_view_map_entries.size()) {
      m_view_selector.EnableWindow(FALSE);
   }
   else {
      m_view_selector.SetCurSel(cur_sel);
      m_view_selection = cur_sel;
      m_view_selector.EnableWindow(TRUE);
   }

   CChildFrame *cf = (CChildFrame *) this->GetParent()->GetParent();
   ((CPlotView *)cf->GetActiveView())->SetAxis(m_axis,m_view_selection - 1,redraw);
}

void CViewSelectorSub::OnSelchangeViewSelector() 
{
   CChildFrame *cf = (CChildFrame *) this->GetParent()->GetParent();
      int i = m_view_selector.GetCurSel();
   if (m_view_selection == i) {
      return;
   }
   else {
      m_view_selection = i;
   }
   ((CPlotView *)cf->GetActiveView())->SetAxis(m_axis, m_view_selection - 1, true);

   CGraphDoc *gd = (CGraphDoc *) cf->GetActiveView()->GetDocument();

   // note: this is only attached to a scatter view, and changing the attribute only
   // affects the scatter view, so only send draw to the map:
   gd->SetRedrawFlag(CGraphDoc::VIEW_SCATTER, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_COLUMN );
   cf->GetActiveView()->SetFocus();
}

BOOL CViewSelectorSub::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
   if (m_axis == 1) {
      m_label.SetWindowText(_T("y ="));
   }
   
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
