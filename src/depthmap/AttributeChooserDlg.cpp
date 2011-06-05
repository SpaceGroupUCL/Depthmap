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

// AttributeChooserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "depthmap.h"
#include "AttributeChooserDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAttributeChooserDlg dialog


CAttributeChooserDlg::CAttributeChooserDlg(AttributeTable& table, CWnd* pParent /*=NULL*/)
	: CDialog(CAttributeChooserDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAttributeChooserDlg)
	m_attribute = -1;
	m_text = _T("");
	//}}AFX_DATA_INIT
   m_table = &table;
}


void CAttributeChooserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAttributeChooserDlg)
	DDX_Control(pDX, IDC_ATTRIBUTE, m_attribute_chooser);
	DDX_CBIndex(pDX, IDC_ATTRIBUTE, m_attribute);
	DDX_Text(pDX, IDC_TEXT, m_text);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAttributeChooserDlg, CDialog)
	//{{AFX_MSG_MAP(CAttributeChooserDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAttributeChooserDlg message handlers

BOOL CAttributeChooserDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
   m_attribute_chooser.AddString(_T("Ref Number"));
   for (int i = 0; i < m_table->getColumnCount(); i++) {
      m_attribute_chooser.AddString( CString(m_table->getColumnName(i).c_str()) );
   }
   m_attribute_chooser.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAttributeChooserDlg::OnOK() 
{
   UpdateData(TRUE);

   // start attribute from -1 (reference column)
   m_attribute--;
	
	CDialog::OnOK();
}
