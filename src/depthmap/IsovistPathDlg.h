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

#pragma once


// CIsovistPathDlg dialog

class CIsovistPathDlg : public CDialog
{
	DECLARE_DYNAMIC(CIsovistPathDlg)

public:
	CIsovistPathDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIsovistPathDlg();

// Dialog Data
	enum { IDD = IDD_ISOVISTPATH };

   double fov_angle;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
   int fov_selection;
protected:
   virtual void OnOK();
};
