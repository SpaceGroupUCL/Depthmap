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

// WaitDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Depthmap.h"
#include "DepthmapView.h"
#include "WaitDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

CMSCommunicator::CMSCommunicator()
{ 
   m_dlg = NULL;
   m_thread = NULL;
   m_function = -1;

   GetApp()->m_process_count += 1;
}

CMSCommunicator::~CMSCommunicator()
{
   if (m_dlg) {
      DestroyWaitDialog();
   }

   GetApp()->m_process_count -= 1;
}

void CMSCommunicator::CreateWaitDialog(const CString& description, CWnd *wnd) 
{
   if (!m_dlg) {
      m_dlg = new CWaitDialog(this);
      m_dlg->m_description = description;
      m_dlg->m_base_description = description;
      m_dlg->Create(IDD_WAIT_DIALOG, wnd);
      m_dlg->CenterWindow();
      m_dlg->ShowWindow(SW_SHOW);
   }
}

void CMSCommunicator::SetBackground()
{
   if (m_dlg) {
      m_dlg->PostMessage( WM_DMP_PROCESS_BACKGROUND );
   }
}

bool CMSCommunicator::GetBackground()
{
   if (m_dlg) {
      return m_dlg->m_background;
   }
   return false;
}

void CMSCommunicator::DestroyWaitDialog() 
{
   m_dlg->SendMessage( WM_DMP_FINISHED_MESSAGE );
   delete m_dlg;
   m_dlg = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CWaitDialog dialog


CWaitDialog::CWaitDialog(CMSCommunicator *comm, CWnd* pParent /*=NULL*/)
	: CDialog(CWaitDialog::IDD, pParent)
{
   m_communicator = comm;
   m_background = false;

   m_num_records = 0;
   m_record = 0;
   m_num_steps = 0;
   m_step = 0;
   m_timer = GetTickCount();

	//{{AFX_DATA_INIT(CWaitDialog)
	m_description = _T("");
	m_information = _T("");
	//}}AFX_DATA_INIT
}


void CWaitDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWaitDialog)
	DDX_Control(pDX, IDC_BACKGROUND, m_background_ctrl);
	DDX_Control(pDX, IDC_PROGRESS_BAR, m_progress_ctrl);
	DDX_Text(pDX, IDC_DESCRIPTION, m_description);
	DDX_Text(pDX, IDC_INFORMATION, m_information);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWaitDialog, CDialog)
	//{{AFX_MSG_MAP(CWaitDialog)
	ON_BN_CLICKED(IDC_BACKGROUND, OnBackground)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
   ON_MESSAGE(WM_DMP_PROGRESS_MESSAGE, OnProgressMessage)
   ON_MESSAGE(WM_DMP_FINISHED_MESSAGE, OnFinishedMessage)
   ON_MESSAGE(WM_DMP_PROCESS_BACKGROUND, OnBackground)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaitDialog message handlers

void CWaitDialog::OnCancel() 
{
   if (m_communicator) {
      if (m_communicator->IsCancelled()) {
         m_description = m_base_description + CString(" attempting to cancel");
         UpdateData(FALSE);
      }
      else {
         m_communicator->Cancel();
         m_description = m_base_description + CString(" attempting to cancel");
         UpdateData(FALSE);
      }
   	// Don't cancel --- cancel should be handled by the thread! 
   }
   else {
      CDialog::OnCancel();
   }
}

LRESULT CWaitDialog::OnBackground(WPARAM, LPARAM)
{
   OnBackground();
   return 0;
}

void CWaitDialog::OnBackground() 
{
   if (m_communicator && m_communicator->GetThread()) {
      if (!m_background) {
         GetApp()->m_pMainWnd->ShowWindow(SW_MINIMIZE);
      }
   }
}

LRESULT CWaitDialog::OnProgressMessage(WPARAM wParam, LPARAM lParam)
{
   switch (wParam) {
      case Communicator::NUM_STEPS:
         m_num_steps = lParam;
         break;
      case Communicator::NUM_RECORDS:
         m_progress_ctrl.SetRange32(0, lParam);
         m_record = 0;
         m_num_records = lParam;
         m_timer = time(NULL);
         break;
      case Communicator::CURRENT_STEP:
         {
            m_step = lParam;
            m_description.Format(_T("%s (step %d of %d)"), m_base_description, m_step, m_num_steps);
         }
         break;
      case Communicator::CURRENT_RECORD:
         m_progress_ctrl.SetPos(lParam);
         m_record = lParam;
         if (m_record > m_num_records) {
            m_record = m_num_records;
         }
         {
            double percent = (100.0 * double(m_record)) / double(m_num_records);
            CString str;
            int timeleft = 1 + int((100.0 / percent - 1.0) * double(time(NULL) - m_timer));
            if (percent > 0.5) {
               if (timeleft >= 3600) {
                  str.Format(_T("%4.1f percent complete%s\nEstimated %d hours %d minutes remaining"), percent, animation[m_record%5], 
                                        timeleft / 3600, (timeleft / 60) % 60 );
               }
               else if (timeleft >= 60) {
                  str.Format(_T("%4.1f percent complete%s\nEstimated %d minutes %d seconds remaining"), percent, animation[m_record%5], 
                                        (timeleft / 60) % 60, timeleft % 60 );
               }
               else {
                  str.Format(_T("%4.1f percent complete%s\nEstimated %d seconds remaining"), percent, animation[m_record%5], 
                                        timeleft % 60);
               }
            }
            else {
               str.Format(_T("%4.1f percent complete%s"), percent, animation[m_record%5] );
            }
            str.TrimLeft();
            m_information = str;
         }
         break;
   }
   UpdateData(FALSE);

   return 0;
}

LRESULT CWaitDialog::OnFinishedMessage(WPARAM wParam, LPARAM lParam)
{
   CDialog::DestroyWindow();

   return 0;
}

void CWaitDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
   if (m_communicator && m_communicator->GetThread() && nStatus != 0)
   {
      if (bShow) {
         m_communicator->GetThread()->SetThreadPriority(THREAD_PRIORITY_NORMAL);
         m_background = false;
      }
      else {
         m_communicator->GetThread()->SetThreadPriority(THREAD_PRIORITY_IDLE);
         m_background = true;
      }
   }

   m_description = m_base_description;
}
