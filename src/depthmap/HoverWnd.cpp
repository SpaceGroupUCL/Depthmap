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

// HoverWnd.cpp : implementation file
//

#include "stdafx.h"
#include "depthmap.h"
#include "HoverWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHoverWnd

CHoverWnd::CHoverWnd()
{
}

CHoverWnd::~CHoverWnd()
{
}


BEGIN_MESSAGE_MAP(CHoverWnd, CWnd)
	//{{AFX_MSG_MAP(CHoverWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CHoverWnd message handlers

BOOL CHoverWnd::Create(CWnd *parent, const prefvec<CString>& infostrings, const CPoint& position) 
{
   m_strings = infostrings;
   m_position = CPoint(position.x, position.y + 24);
   m_brush.CreateSolidBrush(RGB(255, 255, 224));

   CString wndclass = AfxRegisterWndClass(CS_PARENTDC|CS_SAVEBITS,0,m_brush,0);

   CPoint xform(0,0);
   parent->MapWindowPoints(GetApp()->m_pMainWnd,&xform,1);

   BOOL retvar = CWnd::CreateEx(NULL, wndclass, _T("Info"), WS_CHILD | WS_BORDER | WS_VISIBLE, m_position.x + xform.x, m_position.y + xform.y, 1, 1, GetApp()->m_pMainWnd->GetSafeHwnd(), NULL);

   if (retvar) {
      CDC tempDC;
      tempDC.CreateCompatibleDC(GetDC());
      CFont font;
      font.CreateFont(0, 0, 0, 0, FW_REGULAR, 0, 0, 0, 0, 0, 0, 0, 0, _T("MS Sans Serif"));
      //font.CreateFont(0, 0, 0, 0, FW_REGULAR, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, FF_SWISS, NULL);
      CFont *oldFont = tempDC.SelectObject(&font);
      CSize maxsize = tempDC.GetTextExtent(m_strings[0]);
      for (size_t i = 1; i < m_strings.size(); i++) {
         CSize size = tempDC.GetTextExtent(m_strings[i]);
         if (size.cx > maxsize.cx) {
            maxsize.cx = size.cx;
         }
      }
      tempDC.SelectObject(oldFont);
      CRect rect;
      rect.left = m_position.x + xform.x;
      rect.top = m_position.y + xform.y;
      rect.right = rect.left + maxsize.cx + 4;
      rect.bottom = rect.top + maxsize.cy * (LONG)m_strings.size() + 4;
      m_lineheight = maxsize.cy;
      MoveWindow(&rect,FALSE);
   }

   return retvar;
}

void CHoverWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
   
   CFont font;
   
   font.CreateFont(0, 0, 0, 0, FW_REGULAR, 0, 0, 0, 0, 0, 0, 0, 0, _T("MS Sans Serif"));
   //font.CreateFont(0, 0, 0, 0, FW_REGULAR, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, FF_SWISS, NULL);

   CFont *oldFont = dc.SelectObject(&font);

   CRect rect;
   dc.SetBkColor(RGB(255, 255, 224));
   for (size_t i = 0; i < m_strings.size(); i++) {
      dc.TextOut(1, 1 + int(i) * m_lineheight, m_strings[i]); 
   }

   dc.SelectObject(oldFont);
}

int CHoverWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}
