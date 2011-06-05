// Depthmap - spatial network analysis platform
// Copyright (C) 2000-2011 University College London, Alasdair Turner 

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

// SplashDlg.cpp : implementation file
//

#include "stdafx.h"
#include "depthmap.h"
#include "SplashDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSplashDlg dialog


CSplashDlg::CSplashDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSplashDlg::IDD, pParent)
   , m_copyright_info(_T(""))
{
	//{{AFX_DATA_INIT(CSplashDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

   m_copyright_info = CString("Copyright (C) 2000-2011 ") + getAuthors();
}


void CSplashDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CSplashDlg)
   // NOTE: the ClassWizard will add DDX and DDV calls here
   //}}AFX_DATA_MAP
   DDX_Text(pDX, IDC_COPYRIGHT_INFO, m_copyright_info);

   m_brush.CreateSolidBrush(RGB(255, 255, 255));
}


BEGIN_MESSAGE_MAP(CSplashDlg, CDialog)
	//{{AFX_MSG_MAP(CSplashDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
//   ON_WM_ERASEBKGND()
//   ON_WM_CTLCOLOR()
ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplashDlg message handlers

// A bit over the top, but it is necessary for this 
// dialog to kill itself in the event that the App is 
// running a modal dialog box concurrently
// (e.g., as the result of double clicking on an unopennable file)

static int g_count = 0;

void CSplashDlg::OnTimer(UINT_PTR nIDEvent) 
{
	CDialog::OnTimer(nIDEvent);

   if (nIDEvent == DMP_TIMER_SPLASH) {
      g_count++;
   }
   if (g_count > 2) {
      KillTimer(DMP_TIMER_SPLASH);
      DestroyWindow();
   }
}

HBRUSH CSplashDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
   HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

   if (pWnd->GetDlgCtrlID() == IDC_COPYRIGHT_INFO)
   {
      // Set the background mode for text to transparent 
      // so background will show thru.
      pDC->SetBkMode(TRANSPARENT);

      // Return handle to our CBrush object
      hbr = m_brush;
   }

   return hbr;
}
