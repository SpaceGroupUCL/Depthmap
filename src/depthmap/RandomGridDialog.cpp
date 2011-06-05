// RandomGridDialog.cpp : implementation file
//

#include "stdafx.h"
#include "depthmap.h"
#include "RandomGridDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRandomGridDialog dialog


CRandomGridDialog::CRandomGridDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRandomGridDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRandomGridDialog)
	m_block_size = 0.0;
	m_environment_size = 0.0;
	m_number_blocks = 0;
	m_overlapallowed = FALSE;
	//}}AFX_DATA_INIT
}


void CRandomGridDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRandomGridDialog)
	DDX_Text(pDX, IDC_BLOCK_SIZE, m_block_size);
	DDV_MinMaxDouble(pDX, m_block_size, 1.e-003, 1000.);
	DDX_Text(pDX, IDC_ENVIRONMENT_SIZE, m_environment_size);
	DDV_MinMaxDouble(pDX, m_environment_size, 1., 1000.);
	DDX_Text(pDX, IDC_NUMBER_BLOCKS, m_number_blocks);
	DDV_MinMaxInt(pDX, m_number_blocks, 1, 100000);
	DDX_Check(pDX, IDC_OVERLAPALLOWED, m_overlapallowed);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRandomGridDialog, CDialog)
	//{{AFX_MSG_MAP(CRandomGridDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRandomGridDialog message handlers
