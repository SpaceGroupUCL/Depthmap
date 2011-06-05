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

// DepthmapAlert.cpp : implementation file
//

#include "stdafx.h"
#include "depthmap.h"
#include "HyperLink.h"
#include "DepthmapAtom.h"
#include "DepthmapAlert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDepthmapAlert dialog


CDepthmapAlert::CDepthmapAlert(const CDepthmapAtomEntry& entry, CWnd* pParent /*=NULL*/)
	: CDialog(CDepthmapAlert::IDD, pParent)
{
   m_mode = DMAP_NEWS_ALERT;
   m_entry = entry;

	//{{AFX_DATA_INIT(CDepthmapAlert)
	m_read_it = FALSE;
	//}}AFX_DATA_INIT
}

CDepthmapAlert::CDepthmapAlert(const pqmap<CString,CString>& startup_errors, CWnd* pParent /*=NULL*/)
	: CDialog(CDepthmapAlert::IDD, pParent)
{
   m_mode = DMAP_ERROR_ALERT;
   m_startup_errors = startup_errors;
}


void CDepthmapAlert::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDepthmapAlert)
	DDX_Control(pDX, IDC_MESSAGE, m_message);
	DDX_Control(pDX, IDC_ALERT_LINK, m_alert_link);
	DDX_Check(pDX, IDC_READ_IT, m_read_it);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDepthmapAlert, CDialog)
	//{{AFX_MSG_MAP(CDepthmapAlert)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDepthmapAlert message handlers

BOOL CDepthmapAlert::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
   CHARFORMAT cf;
   cf.cbSize = sizeof(cf);
   cf.dwMask = CFM_BOLD;
   cf.dwEffects = CFE_BOLD;
   m_message.SetWordCharFormat( cf );
   CString message;
   int n;

   if (m_mode == DMAP_NEWS_ALERT) {
      m_message.SetWindowText(CString(m_entry.m_title) + _T("\n"));
      n = m_message.GetTextLength();

      tm *timestruct;
      timestruct = gmtime(&(m_entry.m_updated));
      wchar_t datestr[64];
      wcsftime(datestr,64,_T("%A %d %B %Y"),timestruct);

      m_message.SetSel(m_message.GetTextLength(),m_message.GetTextLength());
      m_message.ReplaceSel(datestr);

      message = CString("\n\n");
      for (size_t i = 0; i < m_entry.m_summary.size(); i++) {
         message += CString(m_entry.m_summary[i]) + _T("\n\n");
      } 
   }
   else {
      // errors cannot be "read" and switched off...
      GetDlgItem(IDC_READ_IT)->ShowWindow(SW_HIDE);

      m_message.SetWindowText(CString("Attention!\nThere were errors starting Depthmap\n"));
      n = m_message.GetTextLength();

      message = CString("\n");
      for (size_t i = 0; i < m_startup_errors.size(); i++) {
         message += "In: " + m_startup_errors.key(i) + "\n";
         message += m_startup_errors.value(i) + "\n\n";
      }
   }
   m_message.SetSel(m_message.GetTextLength(),m_message.GetTextLength());
   m_message.ReplaceSel(message);

   m_message.SetSel(n,m_message.GetTextLength());

   cf.cbSize = sizeof(cf);
   cf.dwMask = CFM_BOLD;
   cf.dwEffects = 0;
   m_message.SetWordCharFormat( cf );
   m_message.SetSel(0,0);
   
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
