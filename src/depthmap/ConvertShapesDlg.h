#pragma once


// CConvertShapesDlg dialog

class CConvertShapesDlg : public CDialog
{
	DECLARE_DYNAMIC(CConvertShapesDlg)

public:
	CConvertShapesDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConvertShapesDlg();

// Dialog Data
	enum { IDD = IDD_CONVERT_SHAPES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
   double m_radius;
public:
   BOOL m_selected_only;
public:
   int m_conversion_type;
};
