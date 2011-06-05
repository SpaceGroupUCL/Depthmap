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

#ifndef __IMPORTEDMODULES_H__
#define __IMPORTEDMODULES_H__

struct CImportedModule
{
   HINSTANCE m_inst;
   CString m_name;
   CString m_path;
   int m_analysis_type[8]; // DLL_NO_ANALYSIS, DLL_VGA_ANALYSIS, DLL_AXIAL_ANALYSIS, DLL_SEGMENT_ANALYSIS, DLL_DATA_ANALYSIS or DLL_ATTRIBUTE_ANALYSIS: one for each menu option
   CString m_analysis_name[8];
   int m_menupos;
   CImportedModule()
   { m_inst = NULL; m_menupos = -1; }
};

struct CImportedModuleMenuItem {
   int m_module_ref;
   int m_analysis_ref;  // n.b., analysis_ref -1 is used to *unload* the module
   CImportedModuleMenuItem(int module_ref = -1, int analysis_ref = -1)
   { m_module_ref = module_ref; m_analysis_ref = analysis_ref; }
};

#endif