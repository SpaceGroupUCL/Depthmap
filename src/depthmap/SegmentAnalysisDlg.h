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

#if !defined(AFX_SEGMENTANALYSISDLG_H__1EE566F2_A01B_4274_90FB_23E6BDB76BD5__INCLUDED_)
#define AFX_SEGMENTANALYSISDLG_H__1EE566F2_A01B_4274_90FB_23E6BDB76BD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SegmentAnalysisDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSegmentAnalysisDlg dialog

class CSegmentAnalysisDlg : public CDialog
{
// Construction
public:
	CSegmentAnalysisDlg(MetaGraph *graph = NULL, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSegmentAnalysisDlg)
	enum { IDD = IDD_SEGMENT_ANALYSIS_OPTIONS };
	CComboBox	m_attribute_chooser;
	CEdit	m_radius_ctrl;
	int		m_analysis_type;
	CString	m_radius;
	int		m_tulip_bins;
	int		m_radius_type;
	BOOL	m_choice;
	BOOL	m_weighted;
	int		m_attribute;
	//}}AFX_DATA

   MetaGraph *m_meta_graph;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSegmentAnalysisDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSegmentAnalysisDlg)
	afx_msg void OnAnalysisType();
	afx_msg void OnAnalysisTulip();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnUpdateRadius();
	afx_msg void OnWeighted();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEGMENTANALYSISDLG_H__1EE566F2_A01B_4274_90FB_23E6BDB76BD5__INCLUDED_)
