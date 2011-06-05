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

// AttributeSummary.cpp : implementation file
//

#include "stdafx.h"
#include "depthmap.h"
#include "graphdoc.h"
#include "AttributeSummary.h"
#include "ColumnPropertiesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAttributeSummary dialog


CAttributeSummary::CAttributeSummary(CGraphDoc *pDoc, CWnd* pParent /*=NULL*/)
	: CDialog(CAttributeSummary::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAttributeSummary)
	//}}AFX_DATA_INIT
   m_pDoc = pDoc;
}


void CAttributeSummary::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAttributeSummary)
	DDX_Control(pDX, IDC_LIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAttributeSummary, CDialog)
	//{{AFX_MSG_MAP(CAttributeSummary)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAttributeSummary message handlers

BOOL CAttributeSummary::OnInitDialog() 
{
	CDialog::OnInitDialog();

   const AttributeTable& table = m_pDoc->m_meta_graph->getAttributeTable();
	
   m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);
   m_list.InsertColumn(0,_T("Attribute"),LVCFMT_LEFT,220,0);
   m_list.InsertColumn(1,_T("Minimum"),LVCFMT_RIGHT,100,1);
   m_list.InsertColumn(2,_T("Average"),LVCFMT_RIGHT,100,2);
   m_list.InsertColumn(3,_T("Maximum"),LVCFMT_RIGHT,100,3);
   m_list.DeleteAllItems();
   LVITEM lvi;
   for (int i = 0; i < table.getColumnCount(); i++) {
      lvi.iItem = m_list.InsertItem(i,CString(table.getColumnName(i).c_str()));
      lvi.mask = LVIF_TEXT;
      //
      TCHAR text[64];
      // Min
      lvi.iSubItem = 1;
      swprintf(text,_T("%g"),table.getMinValue(i));
      lvi.pszText = text;
      m_list.SetItem(&lvi);
      // Avg
      lvi.iSubItem = 2;
      swprintf(text,_T("%g"),table.getAvgValue(i));
      lvi.pszText = text;
      m_list.SetItem(&lvi);
      // Max
      lvi.iSubItem = 3;
      swprintf(text,_T("%g"),table.getMaxValue(i));
      lvi.pszText = text;
      m_list.SetItem(&lvi);
   }

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAttributeSummary::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

   if (pNMListView->iItem != -1) {
      CColumnPropertiesDlg dlg(&(m_pDoc->m_meta_graph->getAttributeTable()), pNMListView->iItem);
      dlg.DoModal();
   }
   
	*pResult = 0;
}
