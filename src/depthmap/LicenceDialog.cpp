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

// LicenceDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Depthmap.h"
#include "LicenceDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLicenceDialog dialog


CLicenceDialog::CLicenceDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CLicenceDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLicenceDialog)
	m_message = _T("");
	m_agreement = _T("");
	//}}AFX_DATA_INIT
}

void CLicenceDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLicenceDialog)
	//DDX_Control(pDX, IDC_VRLINK2, m_vrlink);
	DDX_Text(pDX, IDC_LICENCE_BOX, m_agreement);
	DDX_Text(pDX, IDC_MESSAGE, m_message);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLicenceDialog, CDialog)
	//{{AFX_MSG_MAP(CLicenceDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLicenceDialog message handlers

// AT 25-APR-11 New open source agreement:

const wchar_t *g_agreement =
   L"This program is free software: you can redistribute it and/or modify "
   L"it under the terms of the GNU General Public License as published by "
   L"the Free Software Foundation, either version 3 of the License, or "
   L"(at your option) any later version.\x0D\x0D\x0A\x0D\x0D\x0A"
   L"This program is distributed in the hope that it will be useful, "
   L"but WITHOUT ANY WARRANTY; without even the implied warranty of "
   L"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
   L"GNU General Public License for more details.\x0D\x0D\x0A\x0D\x0D\x0A"
   L"You should have received a copy of the GNU General Public License "
   L"along with this program.  If not, see <http://www.gnu.org/licenses/>.";

BOOL CLicenceDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
   SetWindowText( m_title );

   m_message =
      _T("By clicking on the 'Accept' button below, you agree to be bound by ")
      _T("the following terms and conditions:");
   m_agreement = 
      CString("Copyright (C) 2000-2011 ") + getAuthors() +
      CString("\x0D\x0D\x0A\x0D\x0D\x0A") + g_agreement;

   UpdateData(FALSE);

   return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
