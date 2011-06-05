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

// ViewSelector.cpp : implementation file
//

#include "stdafx.h"
#include "depthmap.h"
#include "ChildFrm.h"
#include "GraphDoc.h"   // <- remove when working
#include "ViewSelector.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewSelector dialog

CViewSelector::CViewSelector()
	: CDialog(IDD_DIALOG_BAR, NULL)
{
	//{{AFX_DATA_INIT(CViewSelector)
	//}}AFX_DATA_INIT
   m_layer_selection = -1;
   m_view_selection = -1;

   m_selview_update = false;
}

void CViewSelector::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewSelector)
	DDX_Control(pDX, IDC_VIEW_SELECTOR, m_view_selector);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CViewSelector, CDialog)
	//{{AFX_MSG_MAP(CViewSelector)
	ON_CBN_SELCHANGE(IDC_LAYER_SELECTOR, OnSelLayer)
	ON_CBN_SELCHANGE(IDC_VIEW_SELECTOR, OnSelView)
	ON_CBN_KILLFOCUS(IDC_VIEW_SELECTOR, OnKillfocusViewSelector)
	ON_CBN_EDITCHANGE(IDC_VIEW_SELECTOR, OnEditChangeViewSelector)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewSelector message handlers

void CViewSelector::OnSelLayer() 
{
   /*
   CChildFrame *cf = (CChildFrame *) this->GetParent()->GetParent();
   int i = m_layer_selector.GetCurSel();
   if (m_layer_selection == i) {
      return;
   }
   else {
      m_layer_selection = i;
   }

   CGraphDoc *gd = (CGraphDoc *) cf->GetActiveView()->GetDocument();

   if (gd && gd->m_meta_graph && m_view_class != MetaGraph::VIEWNONE) {

      int op = m_layer_map_entries.key(i);

      if (m_view_class == MetaGraph::VIEWVGA) {
         gd->m_meta_graph->setDisplayedPointMapRef(op);
      }
      else if (m_view_class == MetaGraph::VIEWAXIAL) {
         gd->m_meta_graph->setDisplayedShapeGraph(op);
      }
      else if (m_view_class == MetaGraph::VIEWDATA) {
         gd->m_meta_graph->setDisplayedDataMap(op);
      }

      RedoViewMenu(*(gd->m_meta_graph));
      gd->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_TABLE );
      cf->GetActiveView()->SetFocus();
   }
   */
}

void CViewSelector::OnSelView() 
{
   CChildFrame *cf = (CChildFrame *) this->GetParent()->GetParent();
   int i = m_view_selector.GetCurSel();
   if (m_view_selection == i) {
      return;
   }
   else {
      m_view_selection = i;
   }

   CGraphDoc *gd = (CGraphDoc *) cf->GetActiveView()->GetDocument();

   if (gd && gd->m_meta_graph) {
      if (m_view_class == MetaGraph::VIEWVGA) {
         // nb. -1 for ref.
         gd->m_meta_graph->getDisplayedPointMap().setDisplayedAttribute(i-1);
      }
      else if (m_view_class == MetaGraph::VIEWAXIAL) {
         // nb. -1 for ref.
         gd->m_meta_graph->getDisplayedShapeGraph().setDisplayedAttribute(i-1);
      }
      else if (m_view_class == MetaGraph::VIEWDATA) {
         // nb. -1 for ref.
         gd->m_meta_graph->getDisplayedDataMap().setDisplayedAttribute(i-1);
      }

      // note: this is only attached to a map view, however, changing the attribute does
      // affects more than this view, so send message to all:
      gd->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_COLUMN );
      cf->GetActiveView()->SetFocus();
   }
}

BOOL CViewSelector::OnInitDialog() 
{
   CDialog::OnInitDialog();

   m_view_class = -1;

   m_view_selector.LimitText(128);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CViewSelector::RedoMenu( MetaGraph& meta_graph ) 
{
   // this will be used to distinguish between viewing VGA and axial maps
   int view_class = meta_graph.getViewClass();

   m_view_class = view_class & (MetaGraph::VIEWVGA | MetaGraph::VIEWAXIAL | MetaGraph::VIEWDATA);
   //RedoLayerMenu( meta_graph );
   RedoViewMenu( meta_graph );
}

void CViewSelector::RedoLayerMenu( MetaGraph& meta_graph )
{
   /*
   m_layer_selection = -1;

   int cur_sel = -1;
   int curr_j = -1;
   m_layer_map_entries.clear();

   switch (m_view_class) {
   case MetaGraph::VIEWNONE:
      // nothing available
      break;
   case MetaGraph::VIEWVGA:
      // VGA
      {
         for (int j = 0; j < meta_graph.PointMaps::size(); j++) {
            m_layer_map_entries.add(j, meta_graph.PointMaps::at(j).getName());
         }
         curr_j = meta_graph.getDisplayedPointMapRef();
      }
      break;
   case MetaGraph::VIEWAXIAL:
      // Axial maps
      {
         for (int j = 0; j < meta_graph.ShapeGraphs::size(); j++) {
            m_layer_map_entries.add(j, meta_graph.ShapeGraphs::at(j).getName());
         }
         curr_j = meta_graph.getDisplayedShapeGraphRef();
      }
      break;
   case MetaGraph::VIEWDATA:
      {
         for (int j = 0; j < meta_graph.ShapeMaps::size(); j++) {
            m_layer_map_entries.add(j, meta_graph.ShapeMaps::at(j).getName());
         }
         curr_j = meta_graph.getDisplayedDataMapRef();
      }
   }

   m_layer_selector.ResetContent();

   for (int i = 0; i < m_layer_map_entries.size(); i++) {
      if (curr_j == m_layer_map_entries.key(i)) {
         cur_sel = i;
      }
      m_layer_selector.AddString( m_layer_map_entries.value(i).c_str() );
   }

   if (!m_layer_map_entries.size()) {
      m_layer_selector.EnableWindow(FALSE);
   }
   else {
      m_layer_selector.SetCurSel(cur_sel);
      m_layer_selection = cur_sel;
      m_layer_selector.EnableWindow(TRUE);
   }
   */
}

void CViewSelector::RedoViewMenu( MetaGraph& meta_graph )
{
   m_view_selection = -1;

   int curr_j = 0;

   if (meta_graph.setLock(this)) {

      m_view_map_entries.clear();

      if (m_view_class == MetaGraph::VIEWVGA) {

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
      else if (m_view_class == MetaGraph::VIEWAXIAL) {
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
      else if (m_view_class == MetaGraph::VIEWDATA) {
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
}

void CViewSelector::OnOK() 
{
   if (m_selview_update) {
      m_selview_update = false;
      CheckViewName();
   }
}

void CViewSelector::OnKillfocusViewSelector() 
{
   if (m_selview_update) {
      m_selview_update = false;
      CheckViewName();
   }
}

void CViewSelector::CheckViewName()
{
   if (m_view_selector.GetCount()) {// && m_view_selection != -1) {
      CString text, originaltext = m_view_map_entries.value(m_view_selection).c_str();
      m_view_selector.GetWindowText(text);
      if (text.IsEmpty()) {
         AfxMessageBox(_T("Please ensure a column name is entered"));
         m_view_selector.SetWindowText(originaltext);
      }
      else if (text != originaltext) {
         // user has changed the column name
         if (AfxMessageBox(_T("Do you want to change the name of this column?"), MB_YESNO | MB_ICONQUESTION) == IDYES) {
            if (!ChangeViewName(text)) {
               m_view_selector.SetWindowText(originaltext);
            }
         }
         else {
            m_view_selector.SetWindowText(originaltext);
         }
      }
   }
}

bool CViewSelector::ChangeViewName(const CString& name)
{
   CChildFrame *cf = (CChildFrame *) this->GetParent()->GetParent();
   CGraphDoc *gd = (CGraphDoc *) cf->GetActiveView()->GetDocument();

   bool success = false;

   if (gd && gd->m_meta_graph) {
      if (m_view_selection == 0) {
         AfxMessageBox(_T("Sorry, you cannot rename the reference number column"));
         success = false;
      }
      if (m_view_class & MetaGraph::VIEWVGA) {
         // change attribute column name
         AttributeTable& table = gd->m_meta_graph->getDisplayedPointMap().getAttributeTable();
         // remember old column name:
         int newcol = table.renameColumn(m_view_selection-1,pstring(name));
         if (newcol == -1) {
            AfxMessageBox(_T("Sorry, another column already has this name, please choose a unique column name"));
            success = false;
         }
         else {
           gd->m_meta_graph->getDisplayedPointMap().overrideDisplayedAttribute(newcol);
           // this will completely destroy the view menu order, so redo it now!
           RedoViewMenu( *(gd->m_meta_graph) );
           success = true;
         }
      }
      else if (m_view_class & MetaGraph::VIEWAXIAL) {
         AttributeTable& table = gd->m_meta_graph->getDisplayedShapeGraph().getAttributeTable();
         // remember old column name:
         int newcol = table.renameColumn(m_view_selection-1,pstring(name));
         if (newcol == -1) {
            AfxMessageBox(_T("Sorry, another column already has this name, please choose a unique column name"));
            success = false;
         }
         else {
            gd->m_meta_graph->getDisplayedShapeGraph().overrideDisplayedAttribute(newcol);
            // this will completely destroy the view menu order, so redo it now!
            RedoViewMenu( *(gd->m_meta_graph) );
            success = true;
         }
      }
      else if (m_view_class & MetaGraph::VIEWDATA) {
         AttributeTable& table = gd->m_meta_graph->getDisplayedDataMap().getAttributeTable();
         // remember old column name:
         int newcol = table.renameColumn(m_view_selection-1,pstring(name));
         if (newcol == -1) {
            AfxMessageBox(_T("Sorry, another column already has this name, please choose a unique column name"));
            success = false;
         }
         else {
            gd->m_meta_graph->getDisplayedDataMap().overrideDisplayedAttribute(newcol);
            // this will completely destroy the view menu order, so redo it now!
            RedoViewMenu( *(gd->m_meta_graph) );
            success = true;
         }
      }
   }

   if (success) {
      gd->SetModifiedFlag(TRUE);
      // may have to update other views:
      gd->SetRemenuFlag(CGraphDoc::VIEW_ALL,true);
   }

   return success;
}

void CViewSelector::OnEditChangeViewSelector() 
{
   m_selview_update = true;
}
