#if !defined(AFX_RANDOMGRIDDIALOG_H__D3222DE0_3C80_11D4_A8D5_00609787FC1E__INCLUDED_)
#define AFX_RANDOMGRIDDIALOG_H__D3222DE0_3C80_11D4_A8D5_00609787FC1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RandomGridDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRandomGridDialog dialog

class CRandomGridDialog : public CDialog
{
// Construction
public:
	CRandomGridDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRandomGridDialog)
	enum { IDD = IDD_RANDOM_GRID };
	double	m_block_size;
	double	m_environment_size;
	int		m_number_blocks;
	BOOL	m_overlapallowed;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRandomGridDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRandomGridDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RANDOMGRIDDIALOG_H__D3222DE0_3C80_11D4_A8D5_00609787FC1E__INCLUDED_)
