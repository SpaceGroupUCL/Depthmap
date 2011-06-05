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

// PushDialog.cpp : implementation file
//

#include "stdafx.h"
#include "depthmap.h"
#include "PushDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPushDialog dialog


CPushDialog::CPushDialog(pqmap<IntPair,pstring>& names, CWnd* pParent /*=NULL*/)
	: CDialog(CPushDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPushDialog)
	m_layer_selection = -1;
	m_origin_attribute = _T("");
	m_origin_layer = _T("");
	m_count_intersections = FALSE;
	m_function = -1;
	//}}AFX_DATA_INIT
   
   m_function = 0;

   for (size_t i = 0; i < names.size(); i++) {
      m_names.push_back(names.value(i));
   }
}

void CPushDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPushDialog)
	DDX_Control(pDX, IDC_LAYER_SELECTOR, m_layer_selector);
	DDX_CBIndex(pDX, IDC_LAYER_SELECTOR, m_layer_selection);
	DDX_Text(pDX, IDC_ORIGIN_ATTRIBUTE, m_origin_attribute);
	DDX_Text(pDX, IDC_ORIGIN_LAYER, m_origin_layer);
	DDX_Check(pDX, IDC_COUNT, m_count_intersections);
	DDX_CBIndex(pDX, IDC_HOWTO, m_function);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPushDialog, CDialog)
	//{{AFX_MSG_MAP(CPushDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPushDialog message handlers

BOOL CPushDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
   for (size_t i = 0; i < m_names.size(); i++) {
      m_layer_selector.AddString(CString(m_names[i].c_str()));
   }
   m_layer_selector.SetCurSel(0);
   
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

