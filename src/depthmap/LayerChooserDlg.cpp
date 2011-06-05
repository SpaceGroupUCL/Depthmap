// LayerChooserDlg.cpp : implementation file
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
#include "LayerChooserDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLayerChooserDlg dialog


CLayerChooserDlg::CLayerChooserDlg(pvecstring& names, CWnd* pParent /*=NULL*/)
	: CDialog(CLayerChooserDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLayerChooserDlg)
	m_text = _T("");
	m_layer = -1;
	//}}AFX_DATA_INIT

   m_layer = 0;

   m_names = names;
}


void CLayerChooserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLayerChooserDlg)
	DDX_Control(pDX, IDC_LAYER, m_layer_selector);
	DDX_Text(pDX, IDC_TEXT, m_text);
	DDX_CBIndex(pDX, IDC_LAYER, m_layer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLayerChooserDlg, CDialog)
	//{{AFX_MSG_MAP(CLayerChooserDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLayerChooserDlg message handlers

BOOL CLayerChooserDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
   for (size_t i = 0; i < m_names.size(); i++) {
      m_layer_selector.AddString(CString(m_names[i].c_str()));
   }
   m_layer_selector.SetCurSel(0);
   
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
