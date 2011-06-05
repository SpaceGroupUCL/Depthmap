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

#include "afxwin.h"
#if !defined(AFX_COLOURSCALEDLG_H__FFDD61F2_F279_42A7_9D2D_F306E1A44394__INCLUDED_)
#define AFX_COLOURSCALEDLG_H__FFDD61F2_F279_42A7_9D2D_F306E1A44394__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColourScaleDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColourScaleDlg dialog

class CColourScaleDlg : public CDialog
{
// Construction
public:
	CColourScaleDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   CGraphDoc *m_viewDoc;

	//{{AFX_DATA(CColourScaleDlg)
	enum { IDD = IDD_COLOUR_SCALE };
	CEdit	m_red_value_window;
	CEdit	m_blue_value_window;
	CSliderCtrl	m_red_slider_ctrl;
	CSliderCtrl	m_blue_slider_ctrl;
	CString	m_blue;
	CString	m_red;
	int		m_color;
	//}}AFX_DATA

   bool m_docked;

   double m_display_min;
   double m_display_max;
   double GetActualValue(double sliderpos);
   float GetNormValue(double actualval);

   CBrush m_red_brush;
   CBrush m_blue_brush;
   DisplayParams m_displayparams;

   void MyUpdateData(BOOL dir, BOOL apply_to_all);
   void Clear();
   void Fill();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColourScaleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CColourScaleDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeBlueValue();
	afx_msg void OnChangeRedValue();
	virtual void OnOK();
	afx_msg void OnReleasedRedSlider(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedBlueSlider(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnSelchangeColor();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnMove(int x, int y);
	//}}AFX_MSG
   LRESULT OnFocusGraph(WPARAM wParam, LPARAM lParam);

   DECLARE_MESSAGE_MAP()
public:
   BOOL m_show_lines;
public:
   BOOL m_show_fill;
public:
   afx_msg void OnBnClickedShowLines();
public:
   afx_msg void OnBnClickedShowFill();
public:
   afx_msg void OnBnClickedShowCentroids();
public:
   BOOL m_show_centroids;
public:
   CComboBox m_color_type;
   pvecint m_color_type_map;
public:
   afx_msg void OnBnClickedApplytoall();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLOURSCALEDLG_H__FFDD61F2_F279_42A7_9D2D_F306E1A44394__INCLUDED_)
