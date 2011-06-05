// ConvertShapesDlg.cpp : implementation file
//
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

#include "stdafx.h"
#include "Depthmap.h"
#include "ConvertShapesDlg.h"


// CConvertShapesDlg dialog

IMPLEMENT_DYNAMIC(CConvertShapesDlg, CDialog)

CConvertShapesDlg::CConvertShapesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConvertShapesDlg::IDD, pParent)
   , m_radius(10)
   , m_selected_only(false)
   , m_conversion_type(0)
{

}

CConvertShapesDlg::~CConvertShapesDlg()
{
}

void CConvertShapesDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   DDX_Text(pDX, IDC_RADIUS, m_radius);
   DDV_MinMaxDouble(pDX, m_radius, 0.00001, 10000);
   DDX_Check(pDX, IDC_SELECTED_ONLY, m_selected_only);
   DDX_CBIndex(pDX, IDC_CONVERSION_TYPE, m_conversion_type);
}


BEGIN_MESSAGE_MAP(CConvertShapesDlg, CDialog)
END_MESSAGE_MAP()


// CConvertShapesDlg message handlers
