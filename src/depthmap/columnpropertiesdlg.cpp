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

// ColumnPropertiesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "depthmap.h"
#include "ColumnPropertiesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColumnPropertiesDlg dialog


CColumnPropertiesDlg::CColumnPropertiesDlg(AttributeTable *table, int col, CWnd* pParent /*=NULL*/)
	: CDialog(CColumnPropertiesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColumnPropertiesDlg)
	m_formula = _T("");
	m_name = _T("");
	m_name_text = _T("");
	m_creator = _T("");
	m_formula_note = _T("");
	//}}AFX_DATA_INIT

   m_table = table;
   m_col = col;

   m_name = m_table->getColumnName(m_col).c_str();
   m_formula = m_table->getColumnFormula(m_col).c_str();

   if (!m_table->isColumnLocked(m_col)) {
      m_name_text = "Name";
   }
   else {
      m_name_text = "Name (column locked and cannot be edited)";
   }

   if (m_formula.IsEmpty()) {
      m_formula_note.Empty();
   }
   else {
      m_formula_note = _T("Note: the formula may have been applied to a subset of objects");
   }
}

void CColumnPropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColumnPropertiesDlg)
	DDX_Control(pDX, IDC_SUMMARY, m_summary);
	DDX_Text(pDX, IDC_FORMULA, m_formula);
	DDX_Text(pDX, IDC_NAME, m_name);
	DDX_Text(pDX, IDC_NAME_TEXT, m_name_text);
	DDX_Text(pDX, IDC_FORMULA_NOTE, m_formula_note);
	//}}AFX_DATA_MAP

   pqvector<CString> rows;
   pvecdouble summary_all;
   pvecdouble summary_sel;

   rows.push_back(_T("Average"));
   rows.push_back(_T("Minimum"));
   rows.push_back(_T("Maximum"));
   rows.push_back(_T("Std Dev"));
   rows.push_back(_T("Count"));

   int i;
   for (i = 0; i < 15; i++) {
      if (i == 1 || i == 2) {
         // minimum and maximum
         summary_all.push_back(-1.0);
         summary_sel.push_back(-1.0);
      }
      else {
         summary_all.push_back(0.0);
         summary_sel.push_back(0.0);
      }
   }

   for (i = 0; i < m_table->getRowCount(); i++) {
      double val = m_table->getValue(i,m_col);
      if (val != -1.0 && m_table->isVisible(i)) {
         summary_all[0] += val;
         summary_all[4] += 1.0;
         if (summary_all[1] == -1.0 || val < summary_all[1]) {
            summary_all[1] = val;
         }
         if (summary_all[2] == -1.0 || val > summary_all[2]) {
            summary_all[2] = val;
         }
         if (m_table->isSelected(i)) {
            summary_sel[0] += val;
            summary_sel[4] += 1.0;
            if (summary_sel[1] == -1.0 || val < summary_sel[1]) {
               summary_sel[1] = val;
            }
            if (summary_sel[2] == -1.0 || val > summary_sel[2]) {
               summary_sel[2] = val;
            }
         }
      }
   }

   bool freqrows = false;
   double unit;
   if (summary_all[1] != -1.0 && summary_all[2] != -1.0 && summary_all[1] != summary_all[2]) {
      freqrows = true;
      unit = (summary_all[2] - summary_all[1]) / 10.0;
      for (int i = 0; i < 10; i++) {
         pstring name;
         if (i == 0) {
            name = pstringify(summary_all[1]+unit,"< %f");
         }
         else if (i == 9) {
            name = pstringify(summary_all[2]-unit,"> %f");
         }
         else {
            name = pstringify(summary_all[1]+unit*i,"%f") + " to " +
                   pstringify(summary_all[1]+unit*(i+1),"%f");
         }
         // Unicode conversion a bit of a mess here AT (01.02.11)
         rows.push_back( CString(name.c_str()) );
      }
   }

   if (summary_all[4] != 0) {
      summary_all[0] /= summary_all[4];
   }
   if (summary_sel[4] != 0) {
      summary_sel[0] /= summary_sel[4];
   }

   // count of things rows: just for visible at the moment

   double var_all = 0.0;
   double var_sel = 0.0;
   for (i = 0; i < m_table->getRowCount(); i++) {
      double val = m_table->getValue(i,m_col);
      if (val != -1.0 && m_table->isVisible(i)) {
         var_all += sqr(val-summary_all[0]);
         if (freqrows) {
            int pos = floor((val - summary_all[1])/unit);
            if (pos == 10) pos = 9; // irritating exactly equal to max
            summary_all[5+pos] += 1;
         }
         if (m_table->isSelected(i)) {
            var_sel += sqr(val-summary_sel[0]);
            if (freqrows) {
               // note: must use summary_all even on selected to make difference
               int pos = floor((val - summary_all[1])/unit);
               if (pos == 10) pos = 9; // irritating exactly equal to max
               summary_sel[5+pos] += 1;
            }
         }
      }
   }

   if (summary_all[4] != 0) {
      summary_all[3] = sqrt(var_all / summary_all[4]);
   }
   if (summary_sel[4] != 0) {
      summary_sel[3] = sqrt(var_sel / summary_sel[4]);
   }

   m_summary.SetExtendedStyle(LVS_EX_FULLROWSELECT);
   m_summary.InsertColumn(0,_T("Value"),LVCFMT_LEFT,100,0);
   m_summary.InsertColumn(1,_T("Attribute"),LVCFMT_RIGHT,100,1);
   m_summary.InsertColumn(2,_T("Selection"),LVCFMT_RIGHT,100,2);
   m_summary.DeleteAllItems();
   LVITEM lvi;
   for (i = 0; i < 15; i++) {
      if (i == 5 && !freqrows) {
         break;
      }
      lvi.iItem = m_summary.InsertItem(i,rows[i]);
      lvi.mask = LVIF_TEXT;
      //
      TCHAR text[64];
      // All
      lvi.iSubItem = 1;
      if (i == 4 || summary_all[4] != 0) {
         swprintf(text,_T("%g"),summary_all[i]);
      }
      else {
         wcscpy(text,_T("No Value"));
      }
      lvi.pszText = text;
      m_summary.SetItem(&lvi);
      // Sel
      lvi.iSubItem = 2;
      if (i == 4 || summary_sel[4] != 0) {
         swprintf(text,_T("%g"),summary_sel[i]);
      }
      else {
         wcscpy(text,_T("No Value"));
      }
      lvi.pszText = text;
      m_summary.SetItem(&lvi);
   }
}

BEGIN_MESSAGE_MAP(CColumnPropertiesDlg, CDialog)
	//{{AFX_MSG_MAP(CColumnPropertiesDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColumnPropertiesDlg message handlers
