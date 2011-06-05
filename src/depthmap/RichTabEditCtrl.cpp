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

// RichTabEditCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "depthmap.h"
#include "RichTabEditCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRichTabEditCtrl

CRichTabEditCtrl::CRichTabEditCtrl()
{
}

CRichTabEditCtrl::~CRichTabEditCtrl()
{
}

BEGIN_MESSAGE_MAP(CRichTabEditCtrl, CRichEditCtrl)
	//{{AFX_MSG_MAP(CRichTabEditCtrl)
	ON_WM_GETDLGCODE()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRichTabEditCtrl message handlers

UINT CRichTabEditCtrl::OnGetDlgCode() 
{
   UINT result = CRichEditCtrl::OnGetDlgCode();
   result |= DLGC_WANTTAB;
   return result;
}

void CRichTabEditCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
   if (nChar == 9) {
      // tab key... set for Python style code
      for (size_t i = 0; i < nRepCnt; i++) {
         ReplaceSel(_T("    "),TRUE); 
      }
   }
   else {
	   CRichEditCtrl::OnChar(nChar, nRepCnt, nFlags);
   }
}

void CRichTabEditCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
   // ignore default tab key behaviour:
   if (nChar != 9) {	
	   CRichEditCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
   }
}

