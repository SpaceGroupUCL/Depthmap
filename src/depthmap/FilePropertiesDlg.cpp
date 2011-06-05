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

// FilePropertiesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "depthmap.h"
#include "FilePropertiesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilePropertiesDlg dialog


CFilePropertiesDlg::CFilePropertiesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFilePropertiesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFilePropertiesDlg)
	m_author = _T("");
	m_create_date = _T("");
	m_create_program = _T("");
	m_description = _T("");
	m_location = _T("");
	m_organization = _T("");
	m_title = _T("");
	m_file_version = _T("");
	//}}AFX_DATA_INIT
}


void CFilePropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilePropertiesDlg)
	DDX_Text(pDX, IDC_AUTHOR, m_author);
	DDX_Text(pDX, IDC_CREATE_DATE, m_create_date);
	DDX_Text(pDX, IDC_CREATE_PROGRAM, m_create_program);
	DDX_Text(pDX, IDC_DESCRIPTION, m_description);
	DDV_MaxChars(pDX, m_description, 1024);
	DDX_Text(pDX, IDC_LOCATION, m_location);
	DDV_MaxChars(pDX, m_location, 80);
	DDX_Text(pDX, IDC_ORGANIZATION, m_organization);
	DDX_Text(pDX, IDC_TITLE, m_title);
	DDV_MaxChars(pDX, m_title, 80);
	DDX_Text(pDX, IDC_FILE_VERSION, m_file_version);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFilePropertiesDlg, CDialog)
	//{{AFX_MSG_MAP(CFilePropertiesDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilePropertiesDlg message handlers
