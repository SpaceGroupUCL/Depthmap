#if !defined(AFX_AGENTANALYSISDLG_H__F46832BB_E181_4C24_9354_528409B9E86E__INCLUDED_)
#define AFX_AGENTANALYSISDLG_H__F46832BB_E181_4C24_9354_528409B9E86E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AgentAnalysisDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAgentAnalysisDlg dialog

class CAgentAnalysisDlg : public CDialog
{
// Construction
public:
	CAgentAnalysisDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAgentAnalysisDlg)
	enum { IDD = IDD_AGENT_ANALYSIS };
	CComboBox	m_layer_selector;
	int		m_release_location;
	int		m_fov;
	int		m_frames;
	double	m_release_rate;
	int		m_steps;
	int		m_timesteps;
	int		m_occlusion;
	BOOL	m_record_trails;
	int		m_trail_count;
	//}}AFX_DATA

   int m_gatelayer;
   pvecstring m_names;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAgentAnalysisDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAgentAnalysisDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AGENTANALYSISDLG_H__F46832BB_E181_4C24_9354_528409B9E86E__INCLUDED_)
