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

// PromptReplace.cpp : implementation file
//

#include "stdafx.h"
#include "depthmap.h"
#include "PromptReplace.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPromptReplace dialog


CPromptReplace::CPromptReplace(CWnd* pParent /*=NULL*/)
	: CDialog(CPromptReplace::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPromptReplace)
	m_message = _T("");
	//}}AFX_DATA_INIT
}


void CPromptReplace::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPromptReplace)
	DDX_Text(pDX, IDC_MESSAGE, m_message);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPromptReplace, CDialog)
	//{{AFX_MSG_MAP(CPromptReplace)
	ON_BN_CLICKED(ID_ADD, OnAdd)
	ON_BN_CLICKED(ID_REPLACE, OnReplace)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPromptReplace message handlers

void CPromptReplace::OnAdd() 
{
   EndDialog(ID_ADD);	
}

void CPromptReplace::OnReplace() 
{
   EndDialog(ID_REPLACE);	
}
