// HyperLink.cpp : implementation file
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
#include "HyperLink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHyperLink

CHyperLink::CHyperLink()
{
   m_color = RGB(0,0,255);       // blue
   m_cursor = NULL;
}

CHyperLink::~CHyperLink()
{
}


BEGIN_MESSAGE_MAP(CHyperLink, CStatic)
	//{{AFX_MSG_MAP(CHyperLink)
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHyperLink message handlers

HBRUSH CHyperLink::CtlColor(CDC* pDC, UINT nCtlColor) 
{
   ASSERT(nCtlColor == CTLCOLOR_STATIC);
   DWORD dwStyle = GetStyle();
   if (!(dwStyle & SS_NOTIFY)) {
      // Turn on notify flag to get mouse messages and STN_CLICKED.
      // Otherwise, I'll never get any mouse clicks!
      ::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle | SS_NOTIFY);
   }
    
   HBRUSH hbr = NULL;
   if ((dwStyle & 0xFF) <= SS_RIGHT) {

      // this is a text control: set up font and colors
      if (!(HFONT)m_font) {
         // first time init: create font
         LOGFONT lf;
         GetFont()->GetObject(sizeof(lf), &lf);
         lf.lfUnderline = TRUE;
         m_font.CreateFontIndirect(&lf);
      }
      if (!m_cursor) {
         // also load the cursor
         m_cursor = AfxGetApp()->LoadCursor(IDC_LINKCURSOR);
         ::SetCursor(m_cursor);
      }

      // use underline font and visited/unvisited colors
      pDC->SelectObject(&m_font);
      pDC->SetTextColor(m_color);
      pDC->SetBkMode(TRANSPARENT);

      // return hollow brush to preserve parent background color
      hbr = (HBRUSH)::GetStockObject(HOLLOW_BRUSH);
   }
   return hbr;
}

/////////////////
// Handle mouse click: open URL/file.
//
void CHyperLink::OnClicked()
{
   if (m_link.IsEmpty())         // if URL/filename not set..
      GetWindowText(m_link);    // ..get it from window text

   // Attempt to use Default browser to open:
   HINSTANCE h = ShellExecute(NULL, TEXT("open"), m_link, NULL, NULL, SW_SHOWNORMAL);
}   

BOOL CHyperLink::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
   BOOL bResult = CStatic::OnSetCursor(pWnd, nHitTest, message);
 
   if (m_cursor) {
      ::SetCursor(m_cursor);
   }

	return bResult;
}
