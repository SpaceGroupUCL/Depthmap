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

#if !defined(AFX_WAITDIALOG_H__94C3DE81_F4EC_11D3_A8D5_00609787FC1E__INCLUDED_)
#define AFX_WAITDIALOG_H__94C3DE81_F4EC_11D3_A8D5_00609787FC1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WaitDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWaitDialog dialog

#include "importedmodules.h"

class CWaitDialog;

const CString animation[] = {".","..","...","....","....."};

class CMSCommunicator : public Communicator
{
public:
   enum { IMPORT, IMPORTMIF, MAKEPOINTS, MAKEGRAPH, ANALYSEGRAPH, 
          POINTDEPTH, METRICPOINTDEPTH, ANGULARPOINTDEPTH, TOPOLOGICALPOINTDEPTH, ANALYSEANGULAR, 
          MAKEISOVIST, MAKEISOVISTPATH,
          MAKEAXIALLINES, MAKEALLLINEMAP, MAKEFEWESTLINEMAP, MAKEDRAWING,
          MAKEUSERMAP, MAKEUSERMAPSHAPE, MAKEUSERSEGMAP, MAKEUSERSEGMAPSHAPE, MAKEGATESMAP, MAKEBOUNDARYMAP, MAKESEGMENTMAP, 
          MAKECONVEXMAP, 
          AXIALANALYSIS, SEGMENTANALYSIS, TOPOMETANALYSIS, AGENTANALYSIS, BINDISPLAY, IMPORTEDANALYSIS };
protected:
   pvecint m_options;
   int m_function;
   CWaitDialog *m_dlg;
   CWinThread *m_thread;
   Point2f m_seed_point;
   double m_seed_angle;
   double m_seed_fov;
   CImportedModule m_module;
   CString m_string;   // for a generic string
   //
public:
   CMSCommunicator();
   virtual ~CMSCommunicator();
   virtual void CommPostMessage(int m, int x, int y) const;  // Inline below CWaitDialog   
   //
   void CreateWaitDialog(const CString& description, CWnd *wnd = NULL);
   void DestroyWaitDialog();
   bool IsDialog()
   { return (m_dlg != NULL); }
   //
   void SetThread(CWinThread *thread)
   { m_thread = thread; }
   CWinThread *GetThread()
   { return m_thread; }
   void SetBackground();
   bool GetBackground();
   //
   void SetFunction(int function)
   { m_function = function; }
   int GetFunction() const
   { return m_function; }
   //
   void SetOption(int option, size_t which = 0)
   { while (which >= m_options.size()) m_options.push_back(-1); m_options[which] = option; }
   int GetOption(size_t which = 0) const
   { return (which >= m_options.size()) ? -1 : m_options[which]; }
   void ClearOptions()
   { m_options.clear(); }
   //
   void SetSeedPoint(const Point2f& p)
   { m_seed_point = p; }
   const Point2f& GetSeedPoint() const 
   { return m_seed_point; }
   void SetSeedAngle(const double angle)
   { m_seed_angle = angle; }
   double GetSeedAngle() const 
   { return m_seed_angle; }
   void SetSeedFoV(const double fov)
   { m_seed_fov = fov; }
   double GetSeedFoV() const 
   { return m_seed_fov; }
   //
   void SetModule(const CImportedModule& module)
   { m_module = module; }
   const CImportedModule& GetModule()
   { return m_module; }
   //
   void SetString(const CString& str)
   { m_string= str; }
   const CString& GetString() const
   { return m_string; }

   //
   void SetFileSet(const prefvec<CString>& strings)
   { m_fileset.clear(); for (size_t i = 0; i < strings.size(); i++) m_fileset.push_back(wstring(strings[i])); }
};

class CWaitDialog : public CDialog
{
protected:
   time_t m_timer;
   int m_num_steps;
   int m_num_records;
   int m_step;
   int m_record;
   
 // Construction
public:
	CWaitDialog(CMSCommunicator *comm, CWnd* pParent = NULL);   // standard constructor

   CMSCommunicator *m_communicator;
   bool m_background;

   void SetCommunicator( CMSCommunicator *communicator)
      { m_communicator = communicator; }

// Dialog Data
	//{{AFX_DATA(CWaitDialog)
	enum { IDD = IDD_WAIT_DIALOG };
	CButton	m_background_ctrl;
	CProgressCtrl	m_progress_ctrl;
	CString	m_description;
	CString	m_information;
	//}}AFX_DATA
   CString  m_base_description;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaitDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

   // Hand added message map functions
   afx_msg LRESULT OnProgressMessage(WPARAM wParam, LPARAM lParam);
   afx_msg LRESULT OnFinishedMessage(WPARAM wParam, LPARAM lParam);
   afx_msg LRESULT OnBackground(WPARAM, LPARAM);

	// Generated message map functions
	//{{AFX_MSG(CWaitDialog)
	virtual void OnCancel();
	afx_msg void OnBackground();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

inline void CMSCommunicator::CommPostMessage(int m, int x, int y) const
{
   if (m_dlg) {
      m_dlg->PostMessage(WM_DMP_PROGRESS_MESSAGE, m, x);
   }
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAITDIALOG_H__94C3DE81_F4EC_11D3_A8D5_00609787FC1E__INCLUDED_)
