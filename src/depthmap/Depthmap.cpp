// Depthmap - spatial network analysis platform
// Copyright (C) 2000-2011 University College London, Alasdair Turner 

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

// Depthmap.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Depthmap.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "GraphDoc.h"
#include "DepthmapView.h"
#include "TableView.h"
#include "PlotView.h"
#include "3dView.h"

#include "DepthmapAtom.h"
#include "DepthmapAlert.h"

#include "PromptReplace.h"
#include "LicenceDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////

// Please keep this list in order, and add your name to the end!
const wchar_t *g_authors = L"University College London, Alasdair Turner, Eva Friedrich";

const wchar_t *getAuthors()
{
  return g_authors;
}

/////////////////////////////////////////////////////////////////////////////
// CDepthmapApp

BEGIN_MESSAGE_MAP(CDepthmapApp, CWinApp)
	//{{AFX_MSG_MAP(CDepthmapApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_HELP_MANUAL, OnHelpManual)
	ON_COMMAND(ID_WINDOW_FOREGROUND, OnWindowForeground)
	ON_COMMAND(ID_WINDOW_BACKGROUND, OnWindowBackground)
	ON_COMMAND(ID_TOOLS_IMPORT, OnToolsImport)
	ON_COMMAND(ID_HELP_TUTORIALS, OnHelpTutorials)
	ON_COMMAND(ID_HELP_ALERTS, OnHelpAlerts)
	ON_COMMAND(ID_HELP_SALA_MANUAL, OnHelpSalaManual)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CDepthmapApp construction

CDepthmapApp::CDepthmapApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
   m_process_count = 0;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDepthmapApp object

CDepthmapApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDepthmapApp initialization

#include <conio.h>
#include "afxwin.h"

BOOL CDepthmapApp::InitInstance()
{
	// AfxEnableControlContainer();  // Don't actually need Control Container
                                    // ...some people have had problems, 
                                    // maybe this is one of them

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

   /*
   // Apparently these sets are no longer required:
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
   */
   AfxInitRichEdit( );        // Enable rich edit control

   // Get registry entries
   SetRegistryKey(_T("Stochata"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

   // check agree to licence terms
   if (!AgreeLicence()) {
      return FALSE;
   }

   m_background = COLORREF( GetProfileInt(_T("Settings"),_T("Background"),0x00000000 ) );
   m_foreground = COLORREF( GetProfileInt(_T("Settings"),_T("Foreground"),0x00FFFFFF ) );

   m_formula_cache = GetProfileString(_T("Cache"),_T("Formula"));

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	m_pMapTemplate = new CMultiDocTemplate(
		IDR_GRAPHTYPE,
		RUNTIME_CLASS(CGraphDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CDepthmapView));
	AddDocTemplate(m_pMapTemplate);
	m_pScatterTemplate = new CMultiDocTemplate(
		IDR_GRAPHTYPE2,
		RUNTIME_CLASS(CGraphDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CPlotView));
	AddDocTemplate(m_pScatterTemplate);
	m_pTableTemplate = new CMultiDocTemplate(
		IDR_GRAPHTYPE3,
		RUNTIME_CLASS(CGraphDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CTableView));
	AddDocTemplate(m_pTableTemplate);
	m_p3dViewTemplate = new CMultiDocTemplate(
		IDR_GRAPHTYPE4,
		RUNTIME_CLASS(CGraphDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(C3DView));
	AddDocTemplate(m_p3dViewTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

   m_pMainWnd->CenterWindow();

   // Splash Window --- it's so simple when you know the secret formula... 
   if (!m_pMainWnd->IsIconic() && m_splash.Create(IDD_SPLASH, m_pMainWnd))
	{
      // need to set the size as dialog can be confused by large fonts
      m_splash.SetWindowPos(&CWnd::wndTopMost, 0, 0, 421, 250, SWP_NOMOVE);
      // let windows figure out how to centre it
      m_splash.CenterWindow();
		m_splash.ShowWindow(SW_SHOW);
		m_splash.UpdateWindow();
		m_splash.SetTimer(DMP_TIMER_SPLASH, 1000, NULL);
	}
	m_dwSplashTime = ::GetTickCount();
   // Used by the agent mover
	m_dwAgentPause = 10;   // in milliseconds
	m_dwAgentTime = ::GetTickCount();

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

   m_manual_location = DEFAULT_MANUAL_LOCATION; // In build info (now online handbook)

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
   if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew) {
      cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
   }

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

   // Load imported module DLLs
   LoadImportedModules();

   // wait two seconds (not doing anything!)
   Sleep(2000);

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
   pMainFrame->UpdateWindow();

   // update the viewed toolbars (can only be done after mainframe has loaded)
   int interface_options = GetProfileInt(_T("Settings"),_T("InterfaceOptions"),0);
   if ((interface_options & 0x01) == 0x01) {
      m_show_researchtoolbar = true;
   }
   else {
      m_show_researchtoolbar = false;
   }
   pMainFrame->OnShowResearchtoolbar();

   CheckAlerts();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CHyperLink	m_vrlink;
	CString	m_version_info;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
   CString m_copyright;
public:
   CString m_agreement;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
, m_copyright(_T(""))
, m_agreement(_T(""))
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_version_info = _T("");
	//}}AFX_DATA_INIT

   m_version_info.Format(_T("Version %2.2f.%s"), DEPTHMAP_VERSION, DEPTHMAP_MINOR_VERSION);
   m_copyright = CString("Copyright (C) 2000-2011 ") + getAuthors();
   m_agreement = g_agreement;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CAboutDlg)
   DDX_Control(pDX, IDC_VRLINK, m_vrlink);
   DDX_Text(pDX, IDC_VERSION, m_version_info);
   //}}AFX_DATA_MAP
   DDX_Text(pDX, IDC_COPYRIGHT, m_copyright);
   DDX_Text(pDX, IDC_LICENCE, m_agreement);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CDepthmapApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CDepthmapApp message handlers

BOOL CDepthmapApp::OnIdle(LONG lCount) 
{
	// call base class idle first
	BOOL bResult = CWinApp::OnIdle(lCount);

	// then do our splash screen work
	if (m_splash.m_hWnd != NULL)
	{
		if (::GetTickCount() - m_dwSplashTime > 2500)
		{
			// timeout expired, destroy the splash window
			m_splash.DestroyWindow();
			m_pMainWnd->UpdateWindow();

			// NOTE: don't set bResult to FALSE,
			//  CWinApp::OnIdle may have returned TRUE
		}
		else
		{
			// check again later...
			bResult = TRUE;
		}
	}
   else {
      /*
      // TRYING TO HANDLE WITH A TIMER INSTEAD
      if (m_redraw != NULL) {

         // redraw whichever view claims it needs a redraw...
         m_redraw->PostMessage(WM_DMP_REDRAW_MESSAGE);
         bResult = TRUE; // <- check again

      }
      */
      /*
      // DITTO AGENT MANAGER --- let it run itself!
      if (m_agent_manager != NULL && !m_agent_manager->isPaused() && !m_agent_manager->isLocked()) {
      
         if (GetTickCount() - m_dwAgentTime > m_dwAgentPause) {

      	   m_dwAgentTime = ::GetTickCount();
            // 
            m_agent_manager->PostThreadMessage(WM_DMP_MOVE_AGENT,0,0);

         }

         // While agent manager is running always grab more idle time
         bResult = TRUE;
      }
      */
   }

	return bResult;
}

void CDepthmapApp::OnHelpManual() 
{
   // using the default browser seems to be reactivated...
   HINSTANCE h = ShellExecute(NULL, _T("open"), DEFAULT_MANUAL_LOCATION, NULL, NULL, SW_SHOWNORMAL);
}

void CDepthmapApp::OnHelpTutorials() 
{
   // using the default browser seems to be reactivated...
   HINSTANCE h = ShellExecute(NULL, _T("open"), DEFAULT_TUTORIAL_LOCATION, NULL, NULL, SW_SHOWNORMAL);
}

void CDepthmapApp::OnHelpSalaManual() 
{
   // using the default browser seems to be reactivated...
   HINSTANCE h = ShellExecute(NULL, _T("open"), DEFAULT_SALA_MANUAL_LOCATION, NULL, NULL, SW_SHOWNORMAL);
}

void CDepthmapApp::OnHelpAlerts() 
{
   // using the default browser seems to be reactivated...
   HINSTANCE h = ShellExecute(NULL, _T("open"), DEFAULT_ALERTS_LOCATION, NULL, NULL, SW_SHOWNORMAL);
}

bool CDepthmapApp::AgreeLicence() 
{
   bool ok = false;
   CString signature = GetProfileString(_T("Licence"),_T("Signed"));
   if (signature == _T("Yes")) {
      ok = true;
   }
   else {
      CLicenceDialog dlg;
      if (IDOK == dlg.DoModal()) {
         WriteProfileString(_T("Licence"),_T("Signed"),_T("Yes"));
         ok = true;
      }
   }

   return ok;
}

void CDepthmapApp::CheckAlerts()
{
   if (m_dll_load_errors.size()) {
      // this is more urgent than looking up alerts on the net: warn the user
      CDepthmapAlert alert(m_dll_load_errors);
      alert.DoModal();
   }
   else {
      TCHAR path[256];
      if (GetTempPath(256,path)) {
         wcscat(path,_T("depthmap.xml"));
         if (URLDownloadToFile(NULL,DEFAULT_ALERTS_ATOM,path,0,NULL) == S_OK) {
            CDepthmapAtom atom;
            atom.Parse(path);
            pqvector<CString> readlist;
            ListProfileEntries(_T("Alerts"),readlist);
            CString earliest_alert_date(g_earliest_alert_date.c_str());
            int n = atom.GetFirstUnread(readlist,earliest_alert_date);
            if (n != -1) {
          	   if (m_splash.m_hWnd != NULL)
	            {
                  m_splash.DestroyWindow();
               }
               CDepthmapAtomEntry& entry = atom.GetEntry(n);
               CDepthmapAlert alert(entry);
               alert.DoModal();
               if (alert.m_read_it == TRUE) {
                  WriteProfileInt(_T("Alerts"),entry.m_id,entry.m_updated);
               }
            }
         }
      }
   }
}

void CDepthmapApp::OnWindowBackground() 
{
   CColorDialog dlg( m_background );

   if (dlg.DoModal() == IDOK) {

      m_background = dlg.GetColor();
      WriteProfileInt(_T("Settings"),_T("Background"),m_background);
      TriggerGraphRedraw();
   }
}

void CDepthmapApp::OnWindowForeground() 
{
   CColorDialog dlg( m_foreground );

   if (dlg.DoModal() == IDOK) {

      m_foreground = dlg.GetColor();
      WriteProfileInt(_T("Settings"),_T("Foreground"),m_foreground);
      TriggerGraphRedraw();

   }
}

////////////////////////////////////////////////////////////////////////

void CDepthmapApp::TriggerGraphRedraw()
{
   POSITION pos1 = GetFirstDocTemplatePosition( );
   while (pos1) {
      CDocTemplate *dt = GetNextDocTemplate(pos1);
      POSITION pos2 = dt->GetFirstDocPosition();
      while (pos2) {
         CDocument *doc = dt->GetNextDoc(pos2);
         if (doc->IsKindOf(RUNTIME_CLASS(CGraphDoc))) {
            ((CGraphDoc *)doc)->SetRedrawFlag(NULL, CGraphDoc::REDRAW_TOTAL, CGraphDoc::NEW_DEPTHMAPVIEW_SETUP);
         }
      }
   }
}

//////////////////////////////////////////////////////////////////////////

// Load agent tool bar (very similar to imported modules...)

void CDepthmapApp::LoadAgentMenu()
{
   // DEPRECATED: This is just on the main menu now
   /*
   // now add to the menu (note -- your menu should exist!)
   CMenu *menu = m_pMainWnd->GetMenu();
   ASSERT(menu != NULL);
   CMenu *toolsmenu;
   if (menu->GetMenuItemCount() == 8) {
      toolsmenu = menu->GetSubMenu(4);
   }
   else {
      // sometimes icon loaded at beginning of menu shifts this one
      toolsmenu = menu->GetSubMenu(5);
   }

   if (toolsmenu->GetMenuItemCount() < 5) {
      toolsmenu->InsertMenu(4,MF_BYPOSITION|MF_SEPARATOR);
   }
   m_agent_menu.CreateMenu();
   toolsmenu->InsertMenu(5,MF_BYPOSITION|MF_POPUP, (UINT) m_agent_menu.m_hMenu, "Agent Tools");

   m_agent_menu.AppendMenu(MF_STRING, ID_TOOLS_AGENT_LOAD_PROGRAM, "Load Agent Program");
   m_agent_menu.AppendMenu(MF_STRING, ID_TOOLS_AGENT_RUN, "Run Agent Analysis");
   */
}

// Import analysis tools (which will then be added to the graph type menu)

void CDepthmapApp::OnToolsImport() 
{
   CString infile;

   CFileDialog openDialog(TRUE,NULL,infile,
						  OFN_EXPLORER|OFN_HIDEREADONLY,
						  _T("Module librarys (*.dll)|*.dll|All files (*.*)|*.*||"),
						  NULL);
   openDialog.m_ofn.lpstrTitle = _T("Import Analysis Module");

   int ref = -1;
   if (openDialog.DoModal() == IDOK) {
      ref = ImportModule(openDialog.GetPathName());
      if (m_dll_load_errors.size()) {
         size_t n = m_dll_load_errors.searchindex(openDialog.GetPathName());
         if (n != paftl::npos) {
            AfxMessageBox(CString("Error loading module: ") + 
                          m_dll_load_errors.key(n) + _T("\n") + 
                          m_dll_load_errors.value(n));
         }
      }
   }  
}

int CDepthmapApp::ImportModule(const CString& path)
{
   CImportedModule module;

   module.m_path = path;
   module.m_inst = AfxLoadLibrary(module.m_path);

   if (module.m_inst == NULL) {
      m_dll_load_errors.add(path,_T("Failed to load DLL module"));
      return -1;
   }

   FUNC_GETMODULENAME getModuleName = (FUNC_GETMODULENAME)GetProcAddress(module.m_inst,"getModuleName");

   if (getModuleName == NULL) {
      // this may well be true when other DLLs are in the same folder
      // so, don't complain
      // it does mean that if someone forgets module name, they'll
      // never see a bug, so you should reinstate for SDK testing version
      // m_dll_load_errors.add(path,_T("Could not retrieve module name.\nPlease check that this DLL is a Depthmap plug-in module"));
      return -1;
   }

   module.m_name = getModuleName();

   FUNC_GETMODULEVERSION getModuleVersion = (FUNC_GETMODULEVERSION)GetProcAddress(module.m_inst,"getModuleVersion");

   if (getModuleVersion == NULL) {
      m_dll_load_errors.add(path,_T("Could not retrieve version information from module.\nPlease check that this DLL is a Depthmap plug-in module."));
      return -1;
   }
   else {
      float x= getModuleVersion();
      float y= float(DEPTHMAP_MODULE_VERSION);
      float z= float(DEPTHMAP_VERSION);
      if (x > z) {
         CString version;
         version.Format(_T("The %s module requires at least Depthmap version %g in order to run.\nPlease update your copy of Depthmap."), module.m_name, getModuleVersion());
         m_dll_load_errors.add(path,version);
         return -1;
      }
      else if (x < y) {
         CString version;
         version.Format(_T("The %s module is incompatible with Depthmap version %g and above.\nPlease ask your module provider to supply a more recent version."), module.m_name, y);
         m_dll_load_errors.add(path,version);
         return -1;
      }
   }

   FUNC_GETANALYSISNAME getAnalysisName = (FUNC_GETANALYSISNAME)GetProcAddress(module.m_inst,"getAnalysisName");
   FUNC_GETANALYSISTYPE getAnalysisType = (FUNC_GETANALYSISTYPE)GetProcAddress(module.m_inst,"getAnalysisType");

   int count = 0;
   if (getAnalysisName != NULL) {
      for (int i = 0; i < 8; i++) {
         if (getAnalysisName(i) != NULL) {
            module.m_analysis_name[i] = getAnalysisName(i);
            if (getAnalysisType != NULL) {
               module.m_analysis_type[i] = getAnalysisType(i);            }
            else {
               module.m_analysis_type[i] = DLL_NO_ANALYSIS;
            }
            count++;
         }
         else {
            break;
         }
      }
   }

   if (getAnalysisName == NULL || count == 0) {
      m_dll_load_errors.add(path,_T("Could not retrieve analysis name(s).\nPlease check that this DLL is a Depthmap plug-in module"));
      return -1;
   }

   bool reload = false;
   int ref = -1;

   for (size_t i = 0; i < m_imported_modules.size(); i++) {
      if (module.m_name == m_imported_modules[i].m_name) {
         if (module.m_path == m_imported_modules[i].m_path) {
            ref = (int) i;
            reload = true;
            break;
         }
         else {
            m_dll_load_errors.add(path,_T("A module with this name is already imported."));
            return -1;
         }
      }
   }

   if (ref == -1) {
      ref = (int) m_imported_modules.add(m_next_module_id,module);
      m_next_module_id++;
   }
   else {
      if (m_imported_modules[ref].m_inst != NULL) {
         AfxFreeLibrary(m_imported_modules[ref].m_inst);
      }
      m_imported_modules[ref] = module;
   }
   int module_index = m_imported_modules.key(ref);

   // now add to the menu (note -- your menu should exist!)
   CMenu *menu = m_pMainWnd->GetMenu();
   ASSERT(menu != NULL);
   CMenu *toolsmenu;
   if (menu->GetMenuItemCount() == 8) {
      toolsmenu = menu->GetSubMenu(4);
   }
   else {
      // sometimes icon loaded at beginning of menu shifts this one
      toolsmenu = menu->GetSubMenu(5);
   }

   CMenu *modulemenu;
   if (!reload) {
      if (toolsmenu->GetMenuItemCount() < 7) {
         toolsmenu->InsertMenu(5,MF_BYPOSITION|MF_SEPARATOR);
      }
      m_modulemenus.push_back( new CMenu() );
      modulemenu = m_modulemenus.tail();
      modulemenu->CreateMenu();
      // place above "Options..." and the separator above options
      toolsmenu->InsertMenu(toolsmenu->GetMenuItemCount()-2,MF_BYPOSITION|MF_POPUP, (UINT) modulemenu->m_hMenu, module.m_name);
      m_imported_modules[ref].m_menupos = toolsmenu->GetMenuItemCount() - 1;
   }
   else {
     modulemenu = toolsmenu->GetSubMenu(m_imported_modules[ref].m_menupos);
     while (modulemenu->GetMenuItemCount()) {
         modulemenu->RemoveMenu(0, MF_BYPOSITION);
     }
   }

   for (int j = 0; j < count; j++) {
      modulemenu->AppendMenu(MF_STRING, m_next_imported_menu_id, module.m_analysis_name[j]);
      m_imported_menu_items.add(m_next_imported_menu_id, CImportedModuleMenuItem(module_index,j) );
      m_next_imported_menu_id++;
   }

   // Testing -- reinstate for SDK testing version
   // m_dll_load_errors.add(path,_T("Module loaded successfully"));

   
   //if (!silent) {
   //   if (!reload) {
   //      CString message = CString("Loaded module \"") + module.m_name.c_str() + CString("\" successfully");
   //      AfxMessageBox(message,MB_OK|MB_ICONINFORMATION);
   //   }
   //   else {
   //      CString message = CString("Reloaded module \"") + module.m_name.c_str() + CString("\" successfully");
   //      AfxMessageBox(message,MB_OK);
   //   }
   //}
   
   return ref;
}

void CDepthmapApp::UnloadModule(int ref)
{
   size_t index = m_imported_modules.searchindex(ref);
   if (index != paftl::npos) {
      // remove from menu
      CMenu *menu = m_pMainWnd->GetMenu();
      ASSERT(menu != NULL);
      CMenu *toolsmenu;
      if (menu->GetMenuItemCount() == 6) {
         toolsmenu = menu->GetSubMenu(2);
      }
      else {
         // sometimes icon loaded at beginning of menu shifts this one
         toolsmenu = menu->GetSubMenu(3);
      }
      toolsmenu->RemoveMenu(m_imported_modules[index].m_menupos, MF_BYPOSITION);
      // remove from memory
      if (m_imported_modules[index].m_inst != NULL) {
         AfxFreeLibrary(m_imported_modules[index].m_inst);
      }
      // remove from module list
      m_imported_modules.remove_at(index);
   }
}

void CDepthmapApp::LoadImportedModules()
{
   m_next_module_id = 0;
   m_next_imported_menu_id = 16000;
 
   // this is a bit tricky -> I'm not sure how to get the program directory explicitly
   // apparently _pgmptr can be a bit flakey
   // (looks like it doesn't work on Vista)
   //CFilePath programpath(_pgmptr);

   // I'm going to try this version instead for now:
   TCHAR dmappath[_MAX_PATH];
   GetModuleFileName(NULL,dmappath,_MAX_PATH);
   CFilePath programpath(dmappath);

   prefvec<CString> paths;
   // note, we read first, load later as if AfxLoadLibrary fails, 
   // the finddata structure may be overwritten
   WIN32_FIND_DATA finddata;
   HANDLE h = FindFirstFile(programpath.m_path+_T("*.dll"), &finddata);
   while (GetLastError() == ERROR_SUCCESS) {
      CString path = finddata.cFileName; //GetProfileString("Modules",number);
      if (!path.IsEmpty()) {
         path.MakeLower(); 
         // AT 27-FEB-11 this next line may seem odd:
         // I have integrated topomet.dll into Depthmap proper, so in future the topomet DLL is not required:
         if (path != "topomet.dll") {
            paths.push_back(path);
         }
      }
      FindNextFile(h, &finddata);
   }
   FindClose(h);
   for (size_t i = 0; i < paths.size(); i++) {
      ImportModule(programpath.m_path+paths[i]);
   }
}

int CDepthmapApp::ExitInstance() 
{
   int interface_options = 0;
   if (m_show_researchtoolbar) {
      interface_options |= 0x01;
   }
   WriteProfileInt(_T("Settings"),_T("InterfaceOptions"),interface_options);

   WriteProfileString(_T("Cache"),_T("Formula"),m_formula_cache);
   //
   int count = 1;
   for (size_t i = 0; i < m_imported_modules.size(); i++) {
      if (m_imported_modules[i].m_inst) {
         AfxFreeLibrary(m_imported_modules[i].m_inst);
         // record it for reload next time:
         CString number;
         number.Format(_T("Module%d"), count++);
         WriteProfileString(_T("Modules"),number,m_imported_modules[i].m_path);
      }
   }
   // remove any extra registry entries user may have entered
   bool removing = false;
   do {
      removing = false;
      CString number;
      number.Format(_T("Module%d"), count++);
      if (!GetProfileString(_T("Modules"),number).IsEmpty()) {
         RemoveProfileValue(_T("Modules"),number);
         removing = true;
      }
   } while (removing);
   m_imported_modules.clear();
   
   for (size_t i = 0; i < m_modulemenus.size(); i++) {
      delete m_modulemenus[i];
   }

	return CWinApp::ExitInstance();
}

BOOL CDepthmapApp::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
   if (pHandlerInfo == NULL) {
      bool handled = false;
      if (nID >= 16000 && nID < m_next_imported_menu_id) {
         size_t index = m_imported_menu_items.searchindex(nID);
         if (index != paftl::npos && m_imported_menu_items.at(index).m_analysis_ref == -1) {
            if (nCode == CN_COMMAND) {
               UnloadModule(m_imported_menu_items.at(index).m_module_ref);
            }
            else if (nCode == CN_UPDATE_COMMAND_UI) {
               ((CCmdUI*)pExtra)->Enable(TRUE);
               handled = true;
            }
         }
      }
      if (handled) {
         return TRUE;
      }
   }
	
	return CWinApp::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

// this follows MS code for writing and getting profile strings / ints
void CDepthmapApp::RemoveProfileValue(LPCTSTR lpszSection, LPCTSTR lpszEntry)
{
	if (m_pszRegistryKey != NULL)
	{
		HKEY hSecKey = GetSectionKey(lpszSection);
		if (hSecKey == NULL)
			return;
		LONG lResult = RegDeleteValue(hSecKey, (LPTSTR)lpszEntry);
      ASSERT (lResult == ERROR_SUCCESS);
		RegCloseKey(hSecKey);
	}
}

void CDepthmapApp::ListProfileEntries(LPCTSTR lpszSection, pqvector<CString>& subkeys)
{
	if (m_pszRegistryKey != NULL)
	{
		HKEY hSecKey = GetSectionKey(lpszSection);
		if (hSecKey == NULL)
			return;
      int nextvalue = 0;
      TCHAR buffer[256];
      unsigned long bufsize = 256;
      while (ERROR_SUCCESS == RegEnumValue(hSecKey, nextvalue, (LPTSTR)buffer, &bufsize, NULL, NULL, NULL, NULL)) { 
         // note: add these (so they can be searched easily)
         subkeys.add(buffer);
         nextvalue++;
         bufsize = 256;
      }
		RegCloseKey(hSecKey);
	}
}
