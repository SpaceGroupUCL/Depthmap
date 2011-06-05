// GridDialog.cpp : implementation file
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
#include "GridDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridDialog dialog


CGridDialog::CGridDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CGridDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGridDialog)
	m_spacing = 0.0;
	//}}AFX_DATA_INIT
   m_spacing = 0.01;
   m_maxdimension = 1.0;
}

void CGridDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGridDialog)
	DDX_Control(pDX, IDC_SPIN_SPACING, m_spacing_ctrl);
	DDX_Text(pDX, IDC_GRID_SIZE, m_spacing);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGridDialog, CDialog)
	//{{AFX_MSG_MAP(CGridDialog)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SPACING, OnDeltaposSpinSpacing)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridDialog message handlers

BOOL CGridDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
   m_maxexponent = (int) floor(log10(m_maxdimension)) - 1;
   m_minexponent = m_maxexponent - 2;
   m_basemantissa = (int) floor(m_maxdimension / pow(10.0,double(m_maxexponent+1)));
   
   // current:
   m_mantissa = m_basemantissa;
   m_exponent = m_maxexponent - 1;

   m_spacing = (double) m_mantissa * pow(10.0,double(m_exponent));

   UpdateData(FALSE);
   
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGridDialog::OnDeltaposSpinSpacing(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

   if (pNMUpDown->iDelta < 0) {
      if (m_mantissa < m_basemantissa || m_exponent < m_maxexponent) {
         m_mantissa++;
         if (m_mantissa > 10) {
            m_mantissa = 2;
            m_exponent += 1;
         }
      }
   }
   else if (pNMUpDown->iDelta > 0) {
      if (m_mantissa > m_basemantissa || m_exponent > m_minexponent) {
         m_mantissa--;
         if (m_mantissa < 1) {
            m_mantissa = 9;
            m_exponent -= 1;
         }
      }
   }

   m_spacing = (double) m_mantissa * pow(10.0, double(m_exponent));

   UpdateData(FALSE);

	*pResult = 0;
}


void CGridDialog::OnOK() 
{
   UpdateData(TRUE);

   double truemax = (double) 2 * m_mantissa * pow(10.0, double(m_maxexponent));
   double truemin = (double) m_mantissa * pow(10.0, double(m_minexponent));
   if (m_spacing > truemax || m_spacing < truemin) {
      char formatabsmin[10], formatmin[10], formatmax[10];
      if (m_minexponent < 0) {
         sprintf(formatmin,"%%.%df",abs(m_minexponent));
      }
      else {
         strcpy(formatmin,"%.0f");
      }
      if (m_minexponent-1 < 0) {
         sprintf(formatabsmin,"%%.%df",abs(m_minexponent-1));
      }
      else {
         strcpy(formatabsmin,"%.0f");
      }
      if (m_maxexponent < 0) {
         sprintf(formatmax,"%%.%df",abs(m_maxexponent));
      }
      else {
         strcpy(formatmax,"%.0f");
      }
      char absminstr[24], minstr[24], maxstr[24];
      sprintf(absminstr, formatabsmin, truemin/10);
      sprintf(minstr, formatmin, truemin);
      sprintf(maxstr, formatmax, truemax);
      char msg[256];
      if (m_spacing >= truemin / 10 && m_spacing < truemin) {
         CString msg;
         msg.Format(_T("You are below the suggested minimum grid spacing of %s.  If you use this grid spacing, it may cause processing problems.\n")
                    _T("Are you sure you want to proceed with this grid spacing?"), minstr);
         if (IDNO == AfxMessageBox(msg, MB_YESNO)) {
            return;
         }
      }
      else {
         CString msg;
         msg.Format(_T("Please enter a spacing between %s (at the absolute minimum) and %s"), absminstr, maxstr);
         AfxMessageBox(msg);
         return;
      }
   }

	CDialog::OnOK();
}
