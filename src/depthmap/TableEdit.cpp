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

// TableEdit.cpp : implementation file
//

#include "stdafx.h"
#include "depthmap.h"
#include "TableEdit.h"
#include "TableView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTableEdit

CTableEdit::CTableEdit()
{
}

CTableEdit::~CTableEdit()
{
}


BEGIN_MESSAGE_MAP(CTableEdit, CEdit)
	//{{AFX_MSG_MAP(CTableEdit)
	ON_WM_CHAR()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTableEdit message handlers

void CTableEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
   if (nChar == 0x0d || nChar == 0x09) {
      GetParent()->PostMessage(WM_DMP_GRID_EDIT_MSG,1,0);	
   }
   else {
   	CEdit::OnChar(nChar, nRepCnt, nFlags);
   }
}

void CTableEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	
   if (!((CTableView *)GetParent())->m_protect_edit) {
      GetParent()->PostMessage(WM_DMP_GRID_EDIT_MSG,0,0);	
   }
}


BOOL CTableEdit::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= ES_RIGHT;
   
	return CEdit::PreCreateWindow(cs);
}
