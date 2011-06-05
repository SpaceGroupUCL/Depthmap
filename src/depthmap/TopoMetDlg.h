#pragma once


// CTopoMetDlg dialog

class CTopoMetDlg : public CDialog
{
	DECLARE_DYNAMIC(CTopoMetDlg)

public:
	CTopoMetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTopoMetDlg();

// Dialog Data
	enum { IDD = IDD_TOPOMET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
   int m_topological;
public:
   CString m_radius;
   double m_dradius;
public:
   BOOL m_selected_only;
public:
   afx_msg void OnBnClickedOk();
};
