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

// AgentAnalysisDlg.cpp : implementation file
//

#include "stdafx.h"
#include "depthmap.h"
#include "AgentAnalysisDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAgentAnalysisDlg dialog

CAgentAnalysisDlg::CAgentAnalysisDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAgentAnalysisDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAgentAnalysisDlg)
	m_release_location = -1;
	m_fov = 0;
	m_frames = 0;
	m_release_rate = 0.0;
	m_steps = 0;
	m_timesteps = 0;
	m_occlusion = -1;
	m_record_trails = FALSE;
	m_trail_count = 0;
	//}}AFX_DATA_INIT

   m_trail_count = 50;
   m_occlusion = 0;
   m_gatelayer = -1;
   m_release_location = 0;
}

void CAgentAnalysisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAgentAnalysisDlg)
	DDX_Control(pDX, IDC_LAYER_SELECTOR, m_layer_selector);
	DDX_Radio(pDX, IDC_RELEASE_LOCATION, m_release_location);
	DDX_Text(pDX, IDC_FOV, m_fov);
	DDX_Text(pDX, IDC_FRAMES, m_frames);
	DDX_Text(pDX, IDC_RELEASE_RATE, m_release_rate);
	DDX_Text(pDX, IDC_STEPS, m_steps);
	DDX_Text(pDX, IDC_TIMESTEPS, m_timesteps);
	DDX_CBIndex(pDX, IDC_OCCLUSION, m_occlusion);
	DDX_Check(pDX, IDC_RECORD_TRAILS, m_record_trails);
	DDX_Text(pDX, IDC_TRAIL_COUNT, m_trail_count);
	DDV_MinMaxInt(pDX, m_trail_count, 1, 50);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAgentAnalysisDlg, CDialog)
	//{{AFX_MSG_MAP(CAgentAnalysisDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAgentAnalysisDlg message handlers

BOOL CAgentAnalysisDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
   for (size_t i = 0; i < m_names.size(); i++) {
      m_layer_selector.AddString( CString(m_names[i].c_str()) );
   }
   m_layer_selector.SetCurSel(m_gatelayer + 1);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAgentAnalysisDlg::OnOK() 
{
   m_gatelayer = m_layer_selector.GetCurSel() - 1;
   
	CDialog::OnOK();
}
