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

// GraphDoc.cpp : implementation of the CGraphDoc class
//

#include "stdafx.h"
#include "Sala/salaprogram.h"
#include "Depthmap.h"

#include "PlotView.h"
#include "DepthmapView.h"
#include "GraphDoc.h"

#include "MakeLayerDlg.h"
#include "OptionsDlg.h"
#include "AxialAnalysisOptionsDlg.h"
#include "SegmentAnalysisDlg.h"
#include "GridDialog.h"
#include "MakeOptionsDlg.h"
#include "EditConnectionsDlg.h"
#include "ColourScaleDlg.h"
#include "FewestLineOptionsDlg.h"
#include "WaitDialog.h"
#include "PromptReplace.h"
#include "FilePropertiesDlg.h"
#include "InsertColumnDlg.h"
#include "importedmodules.h"
#include "pushdialog.h"
#include "agentanalysisdlg.h"
#include "NewLayerDlg.h"
#include "AttributeChooserDlg.h"
#include "LayerChooserDlg.h"
#include "RenameObjectDlg.h"
#include "ColumnPropertiesDlg.h"
#include "IsovistPathDlg.h"
#include "ConvertShapesDlg.h"
#include "TopoMetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphDoc

IMPLEMENT_DYNCREATE(CGraphDoc, CDocument)

BEGIN_MESSAGE_MAP(CGraphDoc, CDocument)
	//{{AFX_MSG_MAP(CGraphDoc)
	ON_COMMAND(ID_FILE_IMPORT, OnFileImport)
	ON_COMMAND(ID_FILE_EXPORT, OnFileExport)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORT, OnUpdateFileExport)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_MAKE_GRAPH, OnUpdateToolsMakeGraph)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_RUN, OnUpdateToolsRun)
	ON_UPDATE_COMMAND_UI(ID_FILE_IMPORT, OnUpdateFileImport)
	ON_COMMAND(ID_TOOLS_MAKE_GRAPH, OnToolsMakeGraph)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateEditClear)
	ON_COMMAND(ID_TOOLS_RUN, OnToolsRun)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_PD, OnUpdateToolsPD)
	ON_COMMAND(ID_TOOLS_PD, OnToolsPD)
	ON_COMMAND(ID_TOOLS_AL, OnToolsAxialLines)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_AL, OnUpdateToolsAxialLines)
	ON_COMMAND(ID_ADD_GATE, OnAddGate)
	ON_COMMAND(ID_PUSH_TO_LAYER, OnPushToLayer)
	ON_COMMAND(ID_EDIT_GRID, OnEditGrid)
	ON_UPDATE_COMMAND_UI(ID_EDIT_GRID, OnUpdateEditGrid)
	ON_COMMAND(ID_EDIT_FIXGRID, OnEditFixgrid)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIXGRID, OnUpdateEditFixgrid)
	ON_COMMAND(ID_EDIT_FIXFILL, OnEditFixFill)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIXFILL, OnUpdateEditFixFill)
	ON_COMMAND(ID_TOOLS_MPD, OnToolsMPD)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_MPD, OnUpdateToolsMPD)
	ON_COMMAND(ID_TESTBUTTON, OnTestButton)
	ON_COMMAND(ID_EVOAGENT, OnEvoAgent)
	ON_COMMAND(ID_BINDISPLAY, OnBinDisplay)
	ON_COMMAND(ID_TOOLS_MAKE_FLM, OnToolsMakeFewestLineMap)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_MAKE_FLM, OnUpdateToolsMakeFLM)
	ON_COMMAND(ID_TOOLS_RUN_AXA, OnToolsRunAxa)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_RUN_AXA, OnUpdateToolsRunAxa)
	ON_UPDATE_COMMAND_UI(ID_PUSH_TO_LAYER, OnUpdatePushToLayer)
	ON_UPDATE_COMMAND_UI(ID_ADD_GATE, OnUpdateAddGate)
	ON_COMMAND(ID_TOOLS_RUN_SEG, OnToolsRunSeg)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_RUN_SEG, OnUpdateToolsRunSeg)
	ON_COMMAND(ID_ADD_COLUMN, OnAddColumn)
	ON_UPDATE_COMMAND_UI(ID_ADD_COLUMN, OnUpdateAddColumn)
	ON_COMMAND(ID_REMOVE_COLUMN, OnRemoveColumn)
	ON_UPDATE_COMMAND_UI(ID_REMOVE_COLUMN, OnUpdateRemoveColumn)
	ON_COMMAND(ID_MAGIMIF, OnMagiMif)
	ON_UPDATE_COMMAND_UI(ID_MAGIMIF, OnUpdateMagiMif)
	ON_COMMAND(ID_BINDISTANCES, OnBinDistances)
	ON_COMMAND(ID_SHOW_BINDISTANCES, OnShowBinDistances)
	ON_COMMAND(ID_FILE_PROPERTIES, OnFileProperties)
	ON_COMMAND(ID_TOOLS_APD, OnToolsAPD)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_APD, OnUpdateToolsAPD)
	ON_COMMAND(ID_VIEW_SHOWGRID, OnViewShowGrid)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWGRID, OnUpdateViewShowGrid)
	ON_COMMAND(ID_VIEW_DISPLAYTEXT, OnViewShowText)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DISPLAYTEXT, OnUpdateViewShowText)
	ON_COMMAND(ID_TOOLS_BOUNDARYTOAXIAL, OnToolsBoundaryToAxial)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_BOUNDARYTOAXIAL, OnUpdateToolsBoundaryToAxial)
	ON_COMMAND(ID_VIEW_SUMMARY, OnViewSummary)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SUMMARY, OnUpdateViewSummary)
	ON_COMMAND(ID_SWAP_COLOURS, OnSwapColours)
	ON_UPDATE_COMMAND_UI(ID_SWAP_COLOURS, OnUpdateSwapColours)
	ON_COMMAND(ID_RED_BUTTON, OnRedButton)
	ON_UPDATE_COMMAND_UI(ID_RED_BUTTON, OnUpdateRedButton)
	ON_COMMAND(ID_TOOLS_AXIAL_CLEAR_LINKS, OnToolsAxialClearLinks)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_AXIAL_CLEAR_LINKS, OnUpdateToolsAxialClearLinks)
	ON_COMMAND(ID_UPDATE_COLUMN, OnUpdateColumn)
	ON_UPDATE_COMMAND_UI(ID_UPDATE_COLUMN, OnUpdateUpdateColumn)
	ON_COMMAND(ID_TOOLS_POINT_CONSHAPEMAP, OnToolsPointConvShapeMap)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_POINT_CONSHAPEMAP, OnUpdateToolsPointConvShapeMap)
	ON_COMMAND(ID_TOOLS_AXIAL_CONVSHAPEMAP, OnToolsAxialConvShapeMap)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_AXIAL_CONVSHAPEMAP, OnUpdateToolsAxialConvShapeMap)
	ON_COMMAND(ID_VIEW_SCATTERPLOT, OnViewScatterplot)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SCATTERPLOT, OnUpdateViewScatterplot)
	ON_COMMAND(ID_WINDOW_MAP, OnWindowMap)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_MAP, OnUpdateWindowMap)
	ON_COMMAND(ID_WINDOW_TABLE, OnViewTable)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_TABLE, OnUpdateViewTable)
	ON_COMMAND(ID_WINDOW_3DVIEW, OnWindow3dView)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_3DVIEW, OnUpdateWindow3dView)
	ON_COMMAND(ID_TOOLS_LINE_LOADUNLINKS, OnToolsLineLoadUnlinks)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_LINE_LOADUNLINKS, OnUpdateToolsLineLoadUnlinks)
	ON_COMMAND(ID_TOOLS_AGENT_RUN, OnToolsAgentRun)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_AGENT_RUN, OnUpdateToolsAgentRun)
	ON_COMMAND(ID_REDTEST, OnRedtest)
	ON_COMMAND(ID_GREENTEST, OnGreentest)
	ON_COMMAND(ID_RENAME_COLUMN, OnRenameColumn)
	ON_UPDATE_COMMAND_UI(ID_RENAME_COLUMN, OnUpdateRenameColumn)
	ON_COMMAND(ID_EDIT_QUERY, OnEditQuery)
	ON_UPDATE_COMMAND_UI(ID_EDIT_QUERY, OnUpdateEditQuery)
	ON_COMMAND(ID_COLUMN_PROPERTIES, OnColumnProperties)
	ON_UPDATE_COMMAND_UI(ID_COLUMN_PROPERTIES, OnUpdateColumnProperties)
	ON_COMMAND(ID_PINKTEST, OnPinktest)
	ON_COMMAND(ID_LAYER_NEW, OnLayerNew)
	ON_UPDATE_COMMAND_UI(ID_LAYER_NEW, OnUpdateLayerNew)
	ON_COMMAND(ID_LAYER_DELETE, OnLayerDelete)
	ON_UPDATE_COMMAND_UI(ID_LAYER_DELETE, OnUpdateLayerDelete)
	ON_COMMAND(ID_LAYER_CONVERT, OnLayerConvert)
	ON_UPDATE_COMMAND_UI(ID_LAYER_CONVERT, OnUpdateLayerConvert)
	ON_COMMAND(ID_LAYER_CONVERT_DRAWING, OnLayerConvertDrawing)
	ON_UPDATE_COMMAND_UI(ID_LAYER_CONVERT_DRAWING, OnUpdateLayerConvertDrawing)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_POSITION, OnUpdatePosition)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_DIMENSIONS, OnUpdateDimensions)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_POINTS, OnUpdatePoints)
   ON_COMMAND(ID_EDIT_SELECT_TO_LAYER, &CGraphDoc::OnEditSelectToLayer)
   ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_TO_LAYER, &CGraphDoc::OnUpdateEditSelectToLayer)
   ON_COMMAND(ID_TOOLS_ISOVISTPATH, &CGraphDoc::OnToolsIsovistpath)
   ON_UPDATE_COMMAND_UI(ID_TOOLS_ISOVISTPATH, &CGraphDoc::OnUpdateToolsIsovistpath)
   ON_COMMAND(ID_FILE_SAVE, &CGraphDoc::OnFileSave)
   ON_COMMAND(ID_FILE_SAVE_AS, &CGraphDoc::OnFileSaveAs)
   ON_COMMAND(ID_CONVERT_MAP_SHAPES, &CGraphDoc::OnConvertMapShapes)
   ON_UPDATE_COMMAND_UI(ID_CONVERT_MAP_SHAPES, &CGraphDoc::OnUpdateConvertMapShapes)
   ON_COMMAND(ID_TOOLS_TOPOMET, &CGraphDoc::OnToolsTopomet)
   ON_UPDATE_COMMAND_UI(ID_TOOLS_TOPOMET, &CGraphDoc::OnUpdateToolsTopomet)
   ON_COMMAND(ID_TOOLS_PD_TOPO, &CGraphDoc::OnToolsTPD)
   ON_UPDATE_COMMAND_UI(ID_TOOLS_PD_TOPO, &CGraphDoc::OnUpdateToolsTPD)
   ON_COMMAND(ID_TOOLS_PD_METRIC, &CGraphDoc::OnToolsMPD)
   ON_UPDATE_COMMAND_UI(ID_TOOLS_PD_METRIC, &CGraphDoc::OnUpdateToolsMPD)
   END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphDoc construction/destruction


// MDI 

CGraphDoc::CGraphDoc()
{
   m_flag_lock = false;
   for (int i = 0; i < VIEW_TYPES; i++) {
      m_view[i] = NULL;
      m_redraw_flag[i] = REDRAW_DONE;
      m_remenu_flag[i] = false;
   }
   m_communicator = NULL;

   m_make_algorithm = 0;   // algorithm to make graph
   m_make_maxdist = -1.0;  // maximum distance you can see (set to -1.0 for infinite)

   m_meta_graph = new MetaGraph;

   CString name = GetApp()->GetProfileString(_T("Licence"),_T("Name"));
   CString organization = GetApp()->GetProfileString(_T("Licence"),_T("Organization"));
   CTime time = CTime::GetCurrentTime();
   CString date = time.Format(_T("%d-%b-%y"));
   CString version;
   version.Format(_T("Depthmap v%g.%s"),DEPTHMAP_VERSION,DEPTHMAP_MINOR_VERSION);

   m_meta_graph->setProperties(pstring(name),pstring(organization),pstring(date),pstring(version));
}

CGraphDoc::~CGraphDoc()
{
   delete m_meta_graph;
}

// MDI ONLY

BOOL CGraphDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

   m_base_title = GetTitle();
   // SetTitle(m_base_title + " - Attributes");

   SetRemenuFlag(CGraphDoc::VIEW_ALL,true);

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CGraphDoc serialization

void CGraphDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CGraphDoc diagnostics

#ifdef _DEBUG
void CGraphDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGraphDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGraphDoc commands

// New layer and delete layer

void CGraphDoc::OnLayerNew() 
{
   CNewLayerDlg dlg;
   if (IDOK == dlg.DoModal()) {
      // insert a new layer of the correct type
      // for now, 0 = axial map, and 1 = data map
      ShapeMap *map;
      if (dlg.m_layer_type == 0) {
         int ref = m_meta_graph->addShapeMap(pstring(dlg.m_name));
         map = &(m_meta_graph->getDataMaps().getMap(ref));
      }
      else if (dlg.m_layer_type == 1) {
         int ref = m_meta_graph->addShapeGraph(pstring(dlg.m_name),ShapeMap::CONVEXMAP);
         map = &(m_meta_graph->getShapeGraphs().getMap(ref));
      }
      else if (dlg.m_layer_type == 2) {
         int ref = m_meta_graph->addShapeGraph(pstring(dlg.m_name),ShapeMap::AXIALMAP);
         map = &(m_meta_graph->getShapeGraphs().getMap(ref));
      }
      else if (dlg.m_layer_type == 3) {
         int ref = m_meta_graph->addShapeGraph(pstring(dlg.m_name),ShapeMap::PESHMAP);
         map = &(m_meta_graph->getShapeGraphs().getMap(ref));
      }

      Region r = m_meta_graph->getBoundingBox();
      if (r.isNull()) {
         r = Region(Point2f(-50.0,-50.0),Point2f(50.0,50.0));
      }
      map->init(0,r);
      map->setEditable(true);

      SetUpdateFlag(NEW_TABLE);
      SetRedrawFlag(VIEW_ALL,REDRAW_GRAPH, NEW_DATA);
   }
}

void CGraphDoc::OnLayerDelete() 
{
   // Delete the currently displayed map
   if (IDYES == AfxMessageBox(_T("Are you sure you want to delete this map?\nThis action cannot be undone"),MB_YESNO))
   {
      m_meta_graph->removeDisplayedMap();
      SetUpdateFlag(DELETED_TABLE);
      SetRedrawFlag(VIEW_ALL,REDRAW_GRAPH, NEW_DATA);
   }
}

// Note: import and export are now on the Layer menu, not the file menu

// Import file types: .cat, .dxf, .ntf
// Txt files for points and lines (shapes)

void CGraphDoc::OnFileImport() 
{
   if (m_communicator) {
      return; // Locked
   }

   prefvec<CString> infiles;

   CString template_string;
   template_string += "All formats|*.dxf;*.ntf;*.gml;*.cat;*.rt1;*.mif;*.txt;*.csv|";
   template_string += "Drawing files (*.dxf;*.ntf,*.gml;*.cat,*.rt1)|*.dxf;*.ntf;*.gml;*.cat;*.rt1|MapInfo map (*.mif)|*.mif|Text files (*.txt,*.csv)|*.txt;*.csv|";
   /*
   if (m_meta_graph->getState() & MetaGraph::LINEDATA) {
      template_string += "Points data (*.txt)|*.txt|";   // <- don't think this is supported properly!
   }
   */
   template_string += "All files (*.*)|*.*||";

   CFileDialog openDialog(TRUE,NULL,NULL,
						  OFN_EXPLORER|OFN_HIDEREADONLY|OFN_ALLOWMULTISELECT,
						  template_string,
						  NULL);
   openDialog.m_ofn.lpstrTitle = _T("Import");
   wchar_t buffer[1024];
   buffer[0] = '\0';
   openDialog.m_ofn.lpstrFile = buffer;
   openDialog.m_ofn.nMaxFile = 1024;

   if (openDialog.DoModal() != IDOK) {
      return;
   }  
   CWaitCursor;

   POSITION pos = openDialog.GetStartPosition();
   while (pos != NULL) {
      infiles.push_back(openDialog.GetNextPathName(pos));
      infiles.tail().MakeLower();
   }

   if (!infiles.size()) {
      return;
   }

   CFilePath filepath(infiles[0]);
   CString ext = filepath.m_ext;
   if (ext == _T("CAT") || ext == _T("DXF") || ext == _T("NTF") || ext == _T("RT1") || ext == _T("MIF") || ext == _T("GML") || ext == _T("")) {
      for (size_t i = 1; i < infiles.size(); i++) {
         CFilePath filepath(infiles[i]);
         if (filepath.m_ext != ext || !(filepath.m_ext == _T("RT1") || filepath.m_ext == _T("NTF") || filepath.m_ext == _T("GML") || filepath.m_ext == _T(""))) {
            AfxMessageBox(_T("You have selected more than one file.  Unfortunately, this feature is only currently available with NTF, GML and Tiger line files.\n")
                          _T("Please select a single file to import only."));
            return;
         }
      }
      // OS files don't have GML suffix for some bizarre reason
      if (ext == _T("")) {
         ext = _T("GML");
      }
      int graph_option = MetaGraph::ADD;
      bool ok = true;
      /*
      // No real need to bother with this any more: in the past may have wanted to replace
      // but with graphs having so many layers, now usually want to add
      if (m_meta_graph->getState() & MetaGraph::LINEDATA) {
         CPromptReplace dlg;
         dlg.m_message = _T("A line drawing is already imported, do you want to load this drawing in addition to the original, or do you want replace it?");
         int option = dlg.DoModal();
         if (option == ID_REPLACE) {
            graph_option = MetaGraph::REPLACE;
         }
         else if (option == IDCANCEL) {
            ok = false;
         }
      }
      */
      if (ok) {
         m_communicator = new CMSCommunicator;
         if (ext != _T("RT1") && ext != _T("NTF") && ext != _T("GML")) {  // ntf, gml & rt1 use filesets (all others use standard file at a time)
            m_communicator->SetInfile( infiles[0] );
         }
         if (ext != _T("MIF")) {
            m_communicator->CreateWaitDialog(_T("Importing file..."));
            m_communicator->SetFunction( CMSCommunicator::IMPORT );
            if (ext == _T("CAT")) {
               m_communicator->SetOption( MetaGraph::CAT | graph_option );
            }
            else if (ext == _T("DXF")) {
               m_communicator->SetOption( MetaGraph::DXF | graph_option );
            }
            else if (ext == _T("NTF")) {
               m_communicator->SetOption( MetaGraph::NTF | graph_option );
               m_communicator->SetFileSet( infiles );
            }
            else if (ext == _T("GML")) {
               m_communicator->SetOption( MetaGraph::GML | graph_option );
               m_communicator->SetFileSet( infiles );
            }
            else if (ext == _T("RT1")) {
               m_communicator->SetOption( MetaGraph::RT1 | graph_option );
               m_communicator->SetFileSet( infiles );
            }
         }
         else {
            int thedot = infiles[0].ReverseFind('.');
            CString infile2 = infiles[0].Left(thedot+1) + _T("mid");
            m_communicator->SetInfile2( infile2 );
            m_communicator->CreateWaitDialog(_T("Importing file..."));
            m_communicator->SetFunction( CMSCommunicator::IMPORTMIF );
         }

         m_communicator->SetThread( AfxBeginThread( graphThread, this ) );
      }
   }
   else if (ext == _T("TXT") || ext == _T("CSV")) {
      ifstream file( infiles[0] );
      if (file.fail()) {
         AfxMessageBox( _T("Unable to read text file.\nPlease check that another program is not using it.") );
      }
      else {
         if (m_meta_graph->importTxt( file, pstring(filepath.m_name), (ext == _T("CSV")) ) != -1) {
            // This should have added a new data map:
            SetUpdateFlag(NEW_TABLE);
            SetRedrawFlag(VIEW_ALL,REDRAW_GRAPH, NEW_TABLE);
         }
         else {
            AfxMessageBox( _T("Unable to import text file.\n")
                           _T("Depthmap can import tab-delimited or comma separated files.\n")
                           _T("There must be some spatial data.\n")
                           _T("The spatial data can either be:\n")
                           _T("Points with X and Y values, or\n")
                           _T("points with Easting and Northing values, or\n")
                           _T("lines with X1,Y1 and X2,Y2 values") );
         }
      }
   }
   else {
      AfxMessageBox( _T("Unrecognised file format.  Sorry, unable to import this file.") );
   }
}

// Export file types: .txt (point files)

void CGraphDoc::OnFileExport() 
{
   if (m_communicator) {
     AfxMessageBox(_T("Sorry, cannot export as another process is running"));
     return;  // Locked
   }
   if (m_meta_graph->viewingNone()) {
      AfxMessageBox(_T("Sorry, cannot export as there is no data to export"));
      return;  // No graph to export
   }

   CString suffix;
   int mode = -1;

   bool showlinks = false;
   POSITION pos = GetFirstViewPosition();
   while (pos) {
      CView *view = GetNextView(pos);
      if (view->IsKindOf( RUNTIME_CLASS( CDepthmapView ))) {
         showlinks = ((CDepthmapView *)view)->m_showlinks;
      }
   }


   int view_class = m_meta_graph->getViewClass();
   if (view_class & MetaGraph::VIEWAXIAL) {
      if (showlinks) { 
         mode = 5;
         suffix = _T("unlinks");
      }
      else {
         mode = 0;
         suffix = m_meta_graph->getDisplayedShapeGraph().getName().c_str();
      }
   }
   else if (view_class & MetaGraph::VIEWDATA) {
      if (showlinks) { 
         mode = 6;
         suffix = _T("links");
      }
      else {
         mode = 1;
         suffix = m_meta_graph->getDisplayedDataMap().getName().c_str();
      }
   }
   else if (view_class & MetaGraph::VIEWVGA) {
      if (m_meta_graph->getDisplayedPointMap().isProcessed()) {
         if (showlinks) { 
            mode = 4;
            suffix = _T("merge_lines");
         }
         else {
            mode = 2;
            suffix = _T("vga");
         }
      }
      else {
         mode = 3;
         suffix = _T("points");
      }
   }

   if (mode == -1) {
      AfxMessageBox(_T("Sorry, Depthmap does not support saving the currently displayed layer"));
      return;
   }
   suffix.Replace(' ','_');

   CFilePath path(GetPathName());
   CString defaultname = path.m_path + (path.m_name.IsEmpty() ? GetTitle() : path.m_name) + _T("_") + suffix;

   CString template_string = _T("Tab-delimited text file (*.txt)|*.txt|");
   if (mode < 3) {
      template_string += _T("Comma separated values file (*.csv)|*.csv|");
      template_string += _T("Graph file (*.graph)|*.graph|");
      template_string += _T("MapInfo file (*.mif)|*.mif|");
      template_string += _T("Pajek (*.net)|*.net|");
   }
   template_string += _T("All files (*.*)|*.*||");

   CFileDialog saveDialog(FALSE,_T("txt"),defaultname,
						  OFN_EXPLORER|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, template_string);
   saveDialog.m_ofn.lpstrTitle = _T("Save Output As");

   if (saveDialog.DoModal() == IDOK) {
      CWaitCursor;

      CString outfile = saveDialog.GetPathName();
      CString ext = saveDialog.GetFileExt();
      ext.MakeUpper();

      if (ext != _T("MIF") && ext != _T("GRAPH") && ext != _T("NET")) 
      {
         ofstream stream(outfile);

         char delimiter = '\t';
         if (ext == _T("CSV")) {
            delimiter = ',';
         }

         if (stream.fail() || stream.bad()) {
            AfxMessageBox(_T("Sorry, unable to open file for export"));
            mode = -1;
         }

         switch (mode) {
         case 0:
            m_meta_graph->getDisplayedShapeGraph().output(stream, delimiter);
            break;
         case 1:
            m_meta_graph->getDisplayedDataMap().output(stream, delimiter);
            break;
         case 2:
            m_meta_graph->getDisplayedPointMap().outputSummary( stream, delimiter );
            break;
         case 3:
            m_meta_graph->getDisplayedPointMap().outputPoints( stream, delimiter );
            break;
         case 4:
            m_meta_graph->getDisplayedPointMap().outputMergeLines( stream, delimiter );
            break;
         case 5:
            // note: specific to line graphs
            m_meta_graph->getDisplayedShapeGraph().outputUnlinkPoints( stream, delimiter );
            break;
         default:
            break;
         }

         stream.close();
      }
      else if (ext == _T("GRAPH")) {
         if (mode >= 3) {
            AfxMessageBox(_T("Sorry, Depthmap only supports exporting VGA, axial and shape data to graph files"));
            return;
         }

         if (m_meta_graph->write(pstring(outfile), METAGRAPH_VERSION, true) != MetaGraph::OK) { // <- true writes current layer only
            AfxMessageBox(_T("Sorry, unable to open file for export"));
         }
      }
      else if (ext == _T("NET")) {
         if (mode != 0 && mode != 2) {
            AfxMessageBox(_T("Sorry, Depthmap can only export VGA graphs or shape graphs to Pajek .net files"));
            return;
         }
         ofstream stream(outfile);
         if (!stream) {
            AfxMessageBox(_T("Sorry, unable to open file for export"));
         }
         else {
            if (mode == 0) {
               m_meta_graph->getDisplayedShapeGraph().outputNet(stream);
            }
            else if (mode == 2) {
               m_meta_graph->getDisplayedPointMap().outputNet(stream);
            }
         }
      }
      else {
         if (mode >= 3) {
            AfxMessageBox(_T("Sorry, Depthmap currently only supports exporting VGA, axial and shape data to MapInfo tables"));
            return;
         }

         int thedot = outfile.ReverseFind('.');
         CString outfile2 = outfile.Left(thedot+1) + _T("mid");

         ofstream miffile(outfile);
         if (miffile.fail() || miffile.bad()) {
            AfxMessageBox(_T("Sorry, unable to open file for export"));
            mode = -1;
         }

         ofstream midfile(outfile2);
         if (midfile.fail() || midfile.bad()) {
            AfxMessageBox(_T("Sorry, unable to open associated .mid file for export"));
            mode = -1;
         }

         if (mode == 0) {
            m_meta_graph->getDisplayedShapeGraph().outputMifMap(miffile,midfile);
         }
         else if (mode == 1) {
            m_meta_graph->getDisplayedDataMap().outputMifMap(miffile,midfile);
         }
         else if (mode == 2) {
            m_meta_graph->getDisplayedPointMap().outputMif(miffile,midfile);
         }
      }
   }
}

// When to show a few menu options...

// m_communicator acts as lock

void CGraphDoc::OnUpdateLayerNew(CCmdUI* pCmdUI) 
{
   if (!m_communicator) {
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE);
   }
}

void CGraphDoc::OnUpdateLayerDelete(CCmdUI* pCmdUI) 
{
   if (!m_meta_graph->viewingNone() && !m_communicator) {
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE);
   }
}

void CGraphDoc::OnUpdateFileImport(CCmdUI* pCmdUI) 
{
   if (!m_communicator) {
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE);
   }
}

void CGraphDoc::OnUpdateFileExport(CCmdUI* pCmdUI) 
{
   if (!m_meta_graph->viewingNone() && !m_communicator) {
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE);
   }
}

void CGraphDoc::OnUpdateToolsMakeGraph(CCmdUI* pCmdUI) 
{
   if (m_meta_graph->viewingUnprocessedPoints()) {
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE);
   }
}

void CGraphDoc::OnUpdatePosition(CCmdUI* pCmdUI) 
{
   static char x[16],y[16],z[32];
   if (m_meta_graph->getState() & (MetaGraph::LINEDATA | MetaGraph::SHAPEGRAPHS | MetaGraph::DATAMAPS)) {
      pCmdUI->Enable(TRUE);
      memset(z,'\0',32);
      sprintf(x, "%g", m_position.x );
      sprintf(y, "%g", m_position.y );
      strncat( z, x, 8);
      strcat( z, ", " );
      strncat( z, y, 8);
      pCmdUI->SetText( CString(z) );
   }
   else {
      pCmdUI->Enable(FALSE); 
   }
}

void CGraphDoc::OnUpdateDimensions(CCmdUI* pCmdUI) 
{
   static char x[16],y[16],z[32];
   if (m_meta_graph->getState() & (MetaGraph::LINEDATA | MetaGraph::SHAPEGRAPHS | MetaGraph::DATAMAPS)) {
      pCmdUI->Enable(TRUE); 
      Region r = m_meta_graph->getBoundingBox();
      memset(z,'\0',32);
      sprintf(x, "%g", r.width() );
      sprintf(y, "%g", r.height() );
      strncpy( z, x, 7);
      strcat( z, " x " );
      strncat( z, y, 7);
      pCmdUI->SetText( CString(z) );
   }
   else {
      pCmdUI->Enable(FALSE); 
   }
}

void CGraphDoc::OnUpdatePoints(CCmdUI* pCmdUI) 
{
   if (!m_meta_graph->viewingNone()) {
      pCmdUI->Enable(TRUE); 
      CString text;
      int n = 0;
      int state = m_meta_graph->getState();
      // showing the axial graph
      if ((state & MetaGraph::SHAPEGRAPHS) && m_meta_graph->getViewClass() & MetaGraph::VIEWAXIAL) {
         n = (int) m_meta_graph->getDisplayedShapeGraph().getShapeCount();
      }
      else if ((state & MetaGraph::DATAMAPS) && m_meta_graph->getViewClass() & MetaGraph::VIEWDATA) {
         n = (int) m_meta_graph->getDisplayedDataMap().getShapeCount();
      }
      // either showing or constructing the VGA graph
      else if ((state & MetaGraph::POINTMAPS) && m_meta_graph->getViewClass() & MetaGraph::VIEWVGA) {
         n = (int) m_meta_graph->getDisplayedPointMap().getPointCount();
      }
      if (n > 0) {
         text.Format(_T("%6d"), n );
      }
      pCmdUI->SetText( text );
   }
   else {
      pCmdUI->Enable(FALSE); 
   }
}

/////////////////////////////////////////////////////////////////////////////

/*
void CGraphDoc::OnUpdateViewColourRange(CCmdUI* pCmdUI) 
{
   if (m_meta_graph->viewingProcessed()) {
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE); 
   }	
}
*/
// convert colourscale dialog colorscale to PafColor colorscale
/*
// This is now handled through the mainframe
void CGraphDoc::OnViewColourRange() 
{
   CColourScaleDlg dlg;
   DisplayParams displayparams;
   if (m_meta_graph->getViewClass() & MetaGraph::VIEWVGA) {
      dlg.m_display_min = m_meta_graph->getDisplayedPointMap().getDisplayMinValue();
      dlg.m_display_max = m_meta_graph->getDisplayedPointMap().getDisplayMaxValue();
      displayparams = m_meta_graph->getDisplayedPointMap().getDisplayParams();
      dlg.m_display_blue = displayparams.blue;
      dlg.m_display_red = displayparams.red;
      dlg.m_color = displayparams.colorscale;
      if (dlg.DoModal() == IDOK) {
         displayparams.blue = dlg.m_display_blue;
         displayparams.red = dlg.m_display_red;
         displayparams.colorscale = dlg.m_color;
         m_meta_graph->getDisplayedPointMap().setDisplayParams( displayparams );
      }
   }
   else if (m_meta_graph->getViewClass() & MetaGraph::VIEWAXIAL) {
      ShapeGraph& map = m_meta_graph->getDisplayedShapeGraph();
      dlg.m_display_min = map.getDisplayMinValue();
      dlg.m_display_max = map.getDisplayMaxValue();
      displayparams = map.getDisplayParams();
      dlg.m_display_blue = displayparams.blue;
      dlg.m_display_red = displayparams.red;
      dlg.m_color = displayparams.colorscale;
      if (dlg.DoModal() == IDOK) {
         displayparams.blue = dlg.m_display_blue;
         displayparams.red = dlg.m_display_red;
         displayparams.colorscale = dlg.m_color;
         map.setDisplayParams( displayparams );
      }
   }
   else if (m_meta_graph->getViewClass() & MetaGraph::VIEWDATA) {
      ShapeMap& map = m_meta_graph->getDisplayedDataMap();
      dlg.m_display_min = map.getDisplayMinValue();
      dlg.m_display_max = map.getDisplayMaxValue();
      displayparams = map.getDisplayParams();
      dlg.m_display_blue = displayparams.blue;
      dlg.m_display_red = displayparams.red;
      dlg.m_color = displayparams.colorscale;
      if (dlg.DoModal() == IDOK) {
         displayparams.blue = dlg.m_display_blue;
         displayparams.red = dlg.m_display_red;
         displayparams.colorscale = dlg.m_color;
         map.setDisplayParams( displayparams );
      }
   }
   SetRedrawFlag(VIEW_ALL, CGraphDoc::REDRAW_GRAPH, NEW_DEPTHMAPVIEW_SETUP);
}
*/
void CGraphDoc::OnSwapColours() 
{
   DisplayParams displayparams;
   if (m_meta_graph->getViewClass() & MetaGraph::VIEWVGA ) {
      displayparams = m_meta_graph->getDisplayedPointMap().getDisplayParams();
      float blue = displayparams.blue;
      displayparams.blue = displayparams.red;
      displayparams.red = blue;
      m_meta_graph->getDisplayedPointMap().setDisplayParams( displayparams );
   }
   else if (m_meta_graph->getViewClass() & MetaGraph::VIEWAXIAL) {
      displayparams = m_meta_graph->getDisplayedShapeGraph().getDisplayParams();
      float blue = displayparams.blue;
      displayparams.blue = displayparams.red;
      displayparams.red = blue;
      m_meta_graph->getDisplayedShapeGraph().setDisplayParams( displayparams );
   }
   else if (m_meta_graph->getViewClass() & MetaGraph::VIEWDATA) {
      displayparams = m_meta_graph->getDisplayedDataMap().getDisplayParams();
      float blue = displayparams.blue;
      displayparams.blue = displayparams.red;
      displayparams.red = blue;
      m_meta_graph->getDisplayedDataMap().setDisplayParams( displayparams );
   }

   SetRedrawFlag(VIEW_ALL, CGraphDoc::REDRAW_GRAPH, NEW_DEPTHMAPVIEW_SETUP);
}

void CGraphDoc::OnUpdateSwapColours(CCmdUI* pCmdUI) 
{
   if (!m_communicator && m_meta_graph && m_meta_graph->viewingProcessed()) {
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE);
   }
}


/////////////////////////////////////////////////////////////////////////////

void CGraphDoc::OnEditGrid() 
{
   if (m_communicator) {
      AfxMessageBox(_T("Please wait, another task is running"));
      return;
   }

   bool newmap = false;

   if (!m_meta_graph->PointMaps::size() || m_meta_graph->getDisplayedPointMap().isProcessed()) {
      // this can happen if there are no displayed maps -- so flag new map required:
      newmap = true;
   }
   else if (m_meta_graph->getDisplayedPointMap().getPointCount() != 0) {
      if ( IDYES != AfxMessageBox( _T("This will clear existing points.  Do you want to continue?"), MB_YESNO ) ) {
         return;
      }
   }
   CGridDialog dlg;
   Region r = m_meta_graph->SuperSpacePixel::getRegion();
   dlg.m_maxdimension = __max(r.width(),r.height());
   if (IDOK == dlg.DoModal()) {
      if (newmap) {
         m_meta_graph->PointMaps::addNewMap();
      }
      m_meta_graph->setGrid( dlg.m_spacing, Point2f(0.0f, 0.0f) );
      m_meta_graph->m_showgrid = true;
      SetUpdateFlag(NEW_TABLE);
      SetRedrawFlag(VIEW_ALL,REDRAW_GRAPH, NEW_DATA);
   }
}

void CGraphDoc::OnUpdateEditGrid(CCmdUI* pCmdUI) 
{
   if (m_meta_graph->getState() & MetaGraph::LINEDATA || m_meta_graph->viewingUnprocessedPoints()) {
      pCmdUI->Enable(TRUE);
   }
   else if (!m_communicator) {
      pCmdUI->Enable(FALSE);
   }
}

void CGraphDoc::OnEditFixgrid() 
{
   /*
   // No longer used
   if (m_communicator) {
      AfxMessageBox(_T("Please wait, another task is running"));
      return;
   }
   m_meta_graph->blockLines();

   SetRedrawFlag(VIEW_ALL,REDRAW_GRAPH);
   */
}

void CGraphDoc::OnEditFixFill() 
{
   // only used in Developer mode
   if (m_communicator) {
      AfxMessageBox(_T("Please wait, another task is running"));
      return;
   }
   m_meta_graph->getDisplayedPointMap().fillLines();

   SetRedrawFlag(VIEW_ALL,REDRAW_GRAPH, NEW_DATA);
}

void CGraphDoc::OnUpdateEditFixgrid(CCmdUI* pCmdUI) 
{
   // no longer used
   if (m_meta_graph->viewingUnprocessedPoints()) {
      pCmdUI->Enable(TRUE);
   }
   else if (!m_communicator) {
      pCmdUI->Enable(FALSE);
   }
}

void CGraphDoc::OnUpdateEditFixFill(CCmdUI* pCmdUI) 
{
   // only used in Developer mode
   if (m_meta_graph->viewingUnprocessedPoints()) {
      pCmdUI->Enable(TRUE);
   }
   else if (!m_communicator) {
      pCmdUI->Enable(FALSE);
   }
}

void CGraphDoc::OnFillPoints( const Point2f& p, int semifilled ) // semifilled = 0 (intention to use semifilled steps for part filled)
{
   int state = m_meta_graph->getState();
   if (m_communicator) {
      AfxMessageBox(_T("Please wait, another task is running"));
      return;
   }
   if (~state & MetaGraph::LINEDATA) {
      AfxMessageBox(_T("Sorry, line drawing data must be loaded before points may be filled"));
      return;
   }
   if (~state & MetaGraph::POINTMAPS) {
      AfxMessageBox(_T("Please make grid before filling"));
      return;
   }
   if (m_meta_graph->viewingProcessed()) {
      AfxMessageBox(_T("Sorry, once the graph has been constructed, more points may not be added"));
      return;
   }

   m_communicator = new CMSCommunicator();
   m_communicator->SetSeedPoint( p );
   m_communicator->SetOption( semifilled );
   m_communicator->CreateWaitDialog(_T("Generating grid points..."));
   m_communicator->SetFunction( CMSCommunicator::MAKEPOINTS );

   m_communicator->SetThread( AfxBeginThread( graphThread, this ) );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void CGraphDoc::OnToolsBoundaryToAxial() 
{
   int state = m_meta_graph->getState();
   if (m_communicator) {
      AfxMessageBox(_T("Please wait, another task is running"));
      return;
   }
   if (!m_meta_graph->getDisplayedPointMap().isProcessed()) {
      AfxMessageBox(_T("Sorry, a graph must exist to construct map"));
      return;
   }

   // This is easy too... too easy... hmm... crossed-fingers, here goes:
   m_communicator = new CMSCommunicator();
   m_communicator->CreateWaitDialog(_T("Constructing boundary axial map..."));
   m_communicator->SetFunction( CMSCommunicator::MAKEBOUNDARYMAP );
   m_communicator->SetThread( AfxBeginThread( graphThread, this ) );
}

void CGraphDoc::OnUpdateToolsBoundaryToAxial(CCmdUI* pCmdUI) 
{
   // must have a graph to do this
   if (m_meta_graph->viewingProcessedPoints() && m_meta_graph->getDisplayedPointMap().isBoundaryGraph()) {
      pCmdUI->Enable(1);
   }
   else {
      pCmdUI->Enable(0);
   }		
}

// convert any shape layer to any other (certain rules apply)

void CGraphDoc::OnLayerConvert() 
{
   if (m_communicator) {
      AfxMessageBox(_T("Please wait, another task is running"));
      return;
   }

   CMakeLayerDlg dlg;

   dlg.m_mapin = MAKELAYER_DATA;    // mapin is a data map (assume)
   dlg.m_mapout = 0;   // no possible map out

   if (m_meta_graph->viewingProcessedLines()) {
      int type = m_meta_graph->getDisplayedShapeGraph().getMapType();
      // map in is an axial map (of sorts, something with a graph...)
      dlg.m_mapin = MAKELAYER_AXIAL;
      dlg.m_origin = CString("Shape Graphs: ") + CString(m_meta_graph->getDisplayedShapeGraph().getName().c_str());
      if (type == ShapeMap::AXIALMAP) {
         // note, ShapeMap::ALLLINEMAP is deliberately excluded, as making a segment map from it would overproduce segments:
         dlg.m_mapout = MAKELAYER_DRAWING | MAKELAYER_DATA | MAKELAYER_SEGMENT;
      }
      else {
         // convex, segment, pesh and all line maps cannot be converted to segment maps:
         dlg.m_mapout = MAKELAYER_DRAWING | MAKELAYER_DATA;
      }
   }
   else if (m_meta_graph->viewingProcessedShapes()) {
      //
      dlg.m_origin = CString("Data Maps: ") + CString(m_meta_graph->getDisplayedDataMap().getName().c_str());
      // possible to go to anything apart from a data map
      dlg.m_mapout = MAKELAYER_DRAWING | MAKELAYER_AXIAL | MAKELAYER_CONVEX | MAKELAYER_SEGMENT;
   }

   if (dlg.m_mapout != 0 && IDOK == dlg.DoModal()) {
      m_communicator = new CMSCommunicator();
      m_communicator->SetString( dlg.m_layer_name );
      m_communicator->SetOption( dlg.m_keeporiginal ? 1 : 0, 0);  // <- option 0 used for retain original flag
      m_communicator->SetOption( dlg.m_push_values ? 1 : 0, 1);   // <- option 1 used for push values flag
      //
      if (dlg.m_mapout == MAKELAYER_DRAWING) {
         // this one new: data or graph (any sort) -> drawing
         m_communicator->SetOption( dlg.m_mapin == MAKELAYER_DATA ? 0 : 1, 1); // <- option 1 in this case signifies base as graph or data
         // (option 1 overidden, as data cannot be pushed to a drawing layer)
         m_communicator->CreateWaitDialog(_T("Constructing segment map..."));
         m_communicator->SetFunction( CMSCommunicator::MAKEDRAWING );
      }
      if (dlg.m_mapout == MAKELAYER_DATA) {
         // this one new: graph (any sort) -> data
         m_communicator->CreateWaitDialog(_T("Constructing segment map..."));
         m_communicator->SetFunction( CMSCommunicator::MAKEGATESMAP );
      }
      else if (dlg.m_mapout == MAKELAYER_CONVEX) {
         // this one new: data -> convex map
         m_communicator->CreateWaitDialog(_T("Constructing convex map..."));
         m_communicator->SetFunction( CMSCommunicator::MAKECONVEXMAP );
      }
      else if (dlg.m_mapout == MAKELAYER_AXIAL) {
         // this one originally data -> axial map
         m_communicator->CreateWaitDialog(_T("Constructing axial map..."));
         m_communicator->SetFunction( CMSCommunicator::MAKEUSERMAPSHAPE );
      }
      else if (dlg.m_mapout == MAKELAYER_SEGMENT) { 
         if (dlg.m_mapin == MAKELAYER_AXIAL) {
            // this one originally axial -> segment map
            // use option 2 to specify percentage removal
            m_communicator->SetOption( dlg.m_remove_stubs ? dlg.m_percentage : 0, 2);
            m_communicator->CreateWaitDialog(_T("Constructing segment map..."));
            m_communicator->SetFunction( CMSCommunicator::MAKESEGMENTMAP );
         }
         else {
            // this one originally data -> segment map
            m_communicator->CreateWaitDialog(_T("Constructing segment map..."));
            m_communicator->SetFunction( CMSCommunicator::MAKEUSERSEGMAPSHAPE );
         }
      }
      m_communicator->SetThread( AfxBeginThread( graphThread, this ) );
   }
}

void CGraphDoc::OnLayerConvertDrawing() 
{
   if (m_communicator) {
      AfxMessageBox(_T("Please wait, another task is running"));
      return;
   }

   CMakeLayerDlg dlg;

   dlg.m_mapin = MAKELAYER_DRAWING;    // mapin is a drawing map
   dlg.m_mapout = MAKELAYER_DATA | MAKELAYER_AXIAL | MAKELAYER_CONVEX | MAKELAYER_SEGMENT;
   dlg.m_origin = CString(_T("Drawing Layers: All Displayed"));

   if (IDOK == dlg.DoModal()) {
      m_communicator = new CMSCommunicator();
      m_communicator->SetString( dlg.m_layer_name );
      m_communicator->SetOption( dlg.m_keeporiginal ? 1 : 0, 0);  // <- option 0 used for retain original flag
      m_communicator->SetOption( -1, 1);  // this is used to distinguish between 0 or 1 used when converting data layer
      //
      if (dlg.m_mapout == MAKELAYER_DATA) {
         // this one originally drawing -> data map
         m_communicator->CreateWaitDialog(_T("Constructing data map..."));
         m_communicator->SetFunction( CMSCommunicator::MAKEGATESMAP );
      }
      else if (dlg.m_mapout == MAKELAYER_AXIAL) {
         // this one originally drawing -> axial map
         m_communicator->CreateWaitDialog(_T("Constructing axial map..."));
         m_communicator->SetFunction( CMSCommunicator::MAKEUSERMAP );
      }
      else if (dlg.m_mapout == MAKELAYER_CONVEX) {
         // this one new: drawing -> convex map
         m_communicator->CreateWaitDialog(_T("Constructing convex map..."));
         m_communicator->SetFunction( CMSCommunicator::MAKECONVEXMAP );
      }
      else if (dlg.m_mapout == MAKELAYER_SEGMENT) {
         // this one originally drawing -> seg map
         m_communicator->CreateWaitDialog(_T("Constructing segment map..."));
         m_communicator->SetFunction( CMSCommunicator::MAKEUSERSEGMAP );
      }
      m_communicator->SetThread( AfxBeginThread( graphThread, this ) );
   }
}

void CGraphDoc::OnUpdateLayerConvert(CCmdUI* pCmdUI) 
{
   // note, as yet, VGA maps can't be converted into anything!
   if (!m_communicator && (m_meta_graph->viewingProcessedLines() || m_meta_graph->viewingProcessedShapes())) {
      pCmdUI->Enable(1);
   }
   else {
      pCmdUI->Enable(0);
   }
}

void CGraphDoc::OnUpdateLayerConvertDrawing(CCmdUI* pCmdUI) 
{
   if (!m_communicator && (m_meta_graph->getState() & MetaGraph::LINEDATA) == MetaGraph::LINEDATA) {
      pCmdUI->Enable(1);
   }
   else {
      pCmdUI->Enable(0);
   }
}


// arbitrary isovist
void CGraphDoc::OnMakeIsovist(const Point2f& seed, double angle)
{
   int state = m_meta_graph->getState();
   if (m_communicator) {
      AfxMessageBox(_T("Please wait, another task is running"));
      return;
   }
   if (~state & MetaGraph::LINEDATA) {
      AfxMessageBox(_T("Sorry, line drawing data must be loaded before an isovist can be constructed"));
      return;
   }

   CIsovistPathDlg dlg;
   if (dlg.DoModal() == IDOK) {
      double fov = dlg.fov_angle;
      // This is easy too... too easy... hmm... crossed-fingers, here goes:
      m_communicator = new CMSCommunicator();
      m_communicator->SetSeedPoint( seed );
      m_communicator->SetSeedAngle( angle );
      m_communicator->SetSeedFoV( fov );
      m_communicator->CreateWaitDialog(_T("Constructing BSP tree to calculate isovist..."));
      m_communicator->SetFunction( CMSCommunicator::MAKEISOVIST );
      m_communicator->SetThread( AfxBeginThread( graphThread, this ) );
   }
}

void CGraphDoc::OnToolsIsovistpath()
{
   int state = m_meta_graph->getState();
   if (state & MetaGraph::LINEDATA) {
      int view = m_meta_graph->getViewClass();
      if ((view & (MetaGraph::VIEWDATA | MetaGraph::VIEWAXIAL)) != 0 && m_meta_graph->isSelected()) {
         CIsovistPathDlg dlg;
         if (dlg.DoModal() == IDOK) {
            double angle = dlg.fov_angle;
            m_communicator = new CMSCommunicator();
            m_communicator->SetSeedAngle( angle );
            m_communicator->CreateWaitDialog(_T("Constructing BSP tree to calculate isovists..."));
            m_communicator->SetFunction( CMSCommunicator::MAKEISOVISTPATH );
            m_communicator->SetThread( AfxBeginThread( graphThread, this ) );
         }
      }
      else {
         // an explanation of what you need to do as it isn't obvious!
         AfxMessageBox(_T("To get a path you will need some lines for a path.\nTo do so, have an active map which is either a data map or line map of some sort.\nNext, select lines or polylines that you wish to turn into isovist paths.\nThere is no need for you to have a grid set.\n"));
      }
   }
}

void CGraphDoc::OnUpdateToolsIsovistpath(CCmdUI *pCmdUI)
{
   // since it really isn't obvious how this works, I'll let people click on it,
   // and explain it with only the smallest amount of info present:
   int state = m_meta_graph->getState();
   if (state & MetaGraph::LINEDATA) {
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE);
   }
}


// all line map
void CGraphDoc::OnToolsAxialMap(const Point2f& seed) 
{
   int state = m_meta_graph->getState();
   if (m_communicator) {
      AfxMessageBox(_T("Please wait, another task is running"));
      return;
   }
   if (~state & MetaGraph::LINEDATA) {
      AfxMessageBox(_T("Sorry, line drawing data must be loaded before an axial map can be constructed"));
      return;
   }

   if (m_meta_graph->hasAllLineMap()) {
      if (IDYES != AfxMessageBox(_T("This will overwrite your existing all-line map, do you want to continue?"),MB_YESNO)) {
         return;
      }
   }

   // This is easy too... too easy... hmm... crossed-fingers, here goes:
   m_communicator = new CMSCommunicator();
   m_communicator->SetSeedPoint( seed );
   m_communicator->CreateWaitDialog(_T("Constructing all line axial map..."));
   m_communicator->SetFunction( CMSCommunicator::MAKEALLLINEMAP );
   m_communicator->SetThread( AfxBeginThread( graphThread, this ) );
}

// fewest line map
void CGraphDoc::OnToolsMakeFewestLineMap() 
{
   int state = m_meta_graph->getState();
   if (m_communicator) {
      AfxMessageBox(_T("Please wait, another task is running"));
      return;
   }
   if (~state & MetaGraph::SHAPEGRAPHS) {
      AfxMessageBox(_T("Sorry, all line map must exist in order to construct fewest line map"));
      return;
   }
   else if (!m_meta_graph->hasAllLineMap()) {
      AfxMessageBox(_T("Sorry, all line map must exist in order to construct fewest line map"));
      return;
   }

   int replace = 0;

   // check for existing axial maps and warn user if necessary:
   ShapeGraphs& axialmaps = m_meta_graph->getShapeGraphs();
   if (axialmaps.getMapRef("Fewest-Line Map (Subsets)") != paftl::npos ||
       axialmaps.getMapRef("Fewest Line Map (Subsets)") != paftl::npos ||
       axialmaps.getMapRef("Fewest-Line Map (Minimal)") != paftl::npos ||
       axialmaps.getMapRef("Fewest Line Map (Minimal)") != paftl::npos) {
      CPromptReplace dlg;
      dlg.m_message = _T("There is already a fewest line axial map, would you like to add to it or replace it?");
      INT_PTR result = dlg.DoModal();
      if (result == IDCANCEL) {
         return;
      }
      else if (result == ID_REPLACE) {
         replace = 1;
      }
   }

   // This is easy too... too easy... hmm... crossed-fingers, here goes:
   m_communicator = new CMSCommunicator();
   m_communicator->CreateWaitDialog(_T("Constructing fewest line axial map..."));
   m_communicator->SetFunction( CMSCommunicator::MAKEFEWESTLINEMAP );
   m_communicator->SetOption( replace );
   m_communicator->SetThread( AfxBeginThread( graphThread, this ) );
}

void CGraphDoc::OnToolsRunAxa() 
{
   int state = m_meta_graph->getState();
   if (m_communicator) {
      AfxMessageBox(_T("Please wait, another task is running"));
      return;
   }

   CAxialAnalysisOptionsDlg dlg(m_meta_graph);

   if (IDOK == dlg.DoModal()) {
      m_communicator = new CMSCommunicator();
      m_communicator->CreateWaitDialog(_T("Performing axial line analysis..."));
      m_communicator->SetFunction( CMSCommunicator::AXIALANALYSIS );
      m_communicator->SetThread( AfxBeginThread( graphThread, this ) );
   }
}

void CGraphDoc::OnToolsRunSeg() 
{
   int state = m_meta_graph->getState();
   if (m_communicator) {
      AfxMessageBox(_T("Please wait, another task is running"));
      return;
   }

   CSegmentAnalysisDlg dlg(m_meta_graph);

   if (IDOK == dlg.DoModal()) {
      m_communicator = new CMSCommunicator();
      m_communicator->CreateWaitDialog(_T("Performing segment line analysis..."));
      m_communicator->SetFunction( CMSCommunicator::SEGMENTANALYSIS );
      m_communicator->SetThread( AfxBeginThread( graphThread, this ) );
   }
}

void CGraphDoc::OnToolsTopomet()
{
   int state = m_meta_graph->getState();
   if (m_communicator) {
      AfxMessageBox(_T("Please wait, another task is running"));
      return;
   }

   CTopoMetDlg dlg;

   if (IDOK == dlg.DoModal()) {
      m_communicator = new CMSCommunicator();
      GetApp()->m_options.output_type = dlg.m_topological;
      GetApp()->m_options.radius = dlg.m_dradius;
      GetApp()->m_options.sel_only = dlg.m_selected_only;
      if (dlg.m_topological == 0) {
         m_communicator->CreateWaitDialog(_T("Performing topological analysis..."));
      }
      else {
         m_communicator->CreateWaitDialog(_T("Performing metric analysis..."));
      }
      m_communicator->SetFunction( CMSCommunicator::TOPOMETANALYSIS );
      m_communicator->SetThread( AfxBeginThread( graphThread, this ) );
   }
}

////////////////////////////////////////////////////////////////////////

void CGraphDoc::OnToolsAxialClearLinks() 
{
   if (IDYES == AfxMessageBox(_T("Are you sure you want to clear all links and unlinks?"), MB_YESNO)) {
      m_meta_graph->getDisplayedShapeGraph().clearLinks();
      // if currently in join mode, then redraw:
      SetRedrawFlag(VIEW_ALL, REDRAW_GRAPH, NEW_DATA );
   }
}

void CGraphDoc::OnUpdateToolsAxialClearLinks(CCmdUI* pCmdUI) 
{
   if (m_meta_graph && m_meta_graph->getState() & MetaGraph::SHAPEGRAPHS && !m_communicator && !m_meta_graph->getDisplayedShapeGraph().isSegmentMap()) {
      pCmdUI->Enable(1);
   }
   else {
      pCmdUI->Enable(0);
   }
}

////////////////////////////////////////////////////////////////////////

void CGraphDoc::OnUpdateToolsMakeFLM(CCmdUI* pCmdUI) 
{
   // all-line maps only
   int state = m_meta_graph->getState();
   if (state & MetaGraph::SHAPEGRAPHS && !m_communicator &&
       m_meta_graph->getDisplayedShapeGraph().isAllLineMap()) {
      pCmdUI->Enable(1);
   }
   else {
      pCmdUI->Enable(0);
   }
}

void CGraphDoc::OnUpdateToolsRunAxa(CCmdUI* pCmdUI) 
{
   int state = m_meta_graph->getState();
   // non-segment maps only
   if (state & MetaGraph::SHAPEGRAPHS && !m_communicator &&
       !m_meta_graph->getDisplayedShapeGraph().isSegmentMap()) {
      pCmdUI->Enable(1);
   }
   else {
      pCmdUI->Enable(0);
   }	
}

void CGraphDoc::OnUpdateToolsRunSeg(CCmdUI* pCmdUI) 
{
   // segment maps only
   if (m_meta_graph->viewingProcessedLines() && m_meta_graph->getDisplayedShapeGraph().isSegmentMap()) {
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE);
   }	
}

void CGraphDoc::OnUpdateToolsTopomet(CCmdUI *pCmdUI)
{
   // segment maps only
   // segment maps only
   if (m_meta_graph->viewingProcessedLines() && m_meta_graph->getDisplayedShapeGraph().isSegmentMap()) {
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE);
   }	
}

///////////////////////////////////////////////////////////////////////////////////////////

// New agent functionality:

void CGraphDoc::OnToolsAgentRun() 
{
   if (m_communicator) {
      AfxMessageBox(_T("Please wait, another task is running"));
      return;
   }

   AgentEngine& eng = m_meta_graph->getAgentEngine();

   // set up eng here...
   if (!eng.size()) {
      eng.push_back(AgentSet());
   }

   CAgentAnalysisDlg dlg;
   dlg.m_timesteps = eng.m_timesteps;
   dlg.m_release_rate = eng.tail().m_release_rate;
   dlg.m_release_location = eng.tail().m_release_locations.size() ? 1 : 0;
   dlg.m_frames = eng.tail().m_lifetime;
   if (eng.tail().m_vbin == -1) {
      dlg.m_fov = 32;
   }
   else {
      dlg.m_fov = eng.tail().m_vbin * 2 + 1;
   }
   dlg.m_steps = eng.tail().m_steps;
   dlg.m_record_trails = eng.m_record_trails;
   dlg.m_trail_count = eng.m_trail_count;
   dlg.m_names.push_back(pstring("<None>"));
   for (size_t i = 0; i < m_meta_graph->getDataMaps().getMapCount(); i++) {
       dlg.m_names.push_back(m_meta_graph->getDataMaps().getMap(i).getName());
   }
   dlg.m_gatelayer = eng.m_gatelayer;

   if (IDOK != dlg.DoModal()) {
      return;
   }

   eng.m_timesteps = dlg.m_timesteps;
   eng.tail().m_release_rate = dlg.m_release_rate;
   eng.tail().m_lifetime = dlg.m_frames;
   if (dlg.m_fov == 32) {
      eng.tail().m_vbin = -1;
   }
   else {
      eng.tail().m_vbin = (dlg.m_fov - 1) / 2;
   }
   eng.tail().m_steps = dlg.m_steps;
   if (dlg.m_occlusion == 0) {
      eng.tail().m_sel_type = AgentProgram::SEL_STANDARD;
   }
   else if (dlg.m_occlusion == 1) {
      eng.tail().m_sel_type = AgentProgram::SEL_LOS;
   }
   else if (dlg.m_occlusion == 2) {
      eng.tail().m_sel_type = AgentProgram::SEL_LOS_OCC;
   }
   else {
      // (dlg.m_occlusion - 2) should be from 1...8
      eng.tail().m_sel_type = AgentProgram::SEL_OCCLUSION + (dlg.m_occlusion - 2);
   }
   if (dlg.m_release_location == 1) {
      eng.tail().m_release_locations = m_meta_graph->getSelSet();
   }
   else {
      eng.tail().m_release_locations.clear();
   }
   eng.m_gatelayer = dlg.m_gatelayer;

   // note, trails currently per run, but output per engine
   if (dlg.m_record_trails) {
      eng.m_record_trails = true;
      eng.m_trail_count = dlg.m_trail_count;
   }

   // then go:

   m_communicator = new CMSCommunicator();
   m_communicator->CreateWaitDialog(_T("Performing agent analysis..."));
   m_communicator->SetFunction( CMSCommunicator::AGENTANALYSIS );

   m_communicator->SetThread( AfxBeginThread( graphThread, this ) );
}

void CGraphDoc::OnUpdateToolsAgentRun(CCmdUI* pCmdUI) 
{
   if (m_meta_graph && m_meta_graph->viewingProcessedPoints() && !m_communicator) {
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE);
   }
}

// some evo agent code... not sure if this works or not!

void CGraphDoc::OnEvoAgent() 
{
   /*
   if (!m_evoagent) {
      m_evoagent = (CEvoAgent *) AfxBeginThread(RUNTIME_CLASS(CEvoAgent));
      if (!m_evoagent) {
         AfxMessageBox(_T("An error occurred trying to start the agent simulation module"));
         return;
      }
      else {
         CEvoAgentSetup dlg;
         if (dlg.DoModal() == IDOK) {
            m_evoagent->Init(this,dlg.m_evolveapply,dlg.m_filename,dlg.m_seed);
            m_evoagent->PostThreadMessage(WM_DMP_RUN,0,0);
         }
         else {
            m_evoagent->PostThreadMessage(WM_DMP_FINISHED_MESSAGE,0,0);
            m_evoagent = NULL;
         }
      }
   }
   else {
      // turn off pause (typically pauses after evaluation run)
      m_evoagent->m_paused = false;
      m_evoagent->PostThreadMessage(WM_DMP_RUN,0,0);
   }
   */
}

/////////////////////////////////////////////////////////////////////////////

void CGraphDoc::OnEditUndo() 
{
   if (!m_meta_graph->canUndo()) {
      AfxMessageBox(_T("Sorry, no undo available for this map"));
      return;
   }

 	m_meta_graph->undo();
   SetModifiedFlag();
   SetRedrawFlag(VIEW_ALL, REDRAW_GRAPH, NEW_DATA );
}

void CGraphDoc::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
   if (m_meta_graph->canUndo()) {
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE);
   }
}

void CGraphDoc::OnEditClear() 
{
   int state = m_meta_graph->getState();

   int editable = m_meta_graph->isEditable();
   if (editable == MetaGraph::NOT_EDITABLE) {
      AfxMessageBox(_T("Cannot delete: the geometry forming this graph cannot be edited."), MB_OK | MB_ICONSTOP);
      return;
   }
   else if (editable == MetaGraph::EDITABLE_OFF) {
      AfxMessageBox(_T("Cannot delete: this graph is currently uneditable."));
      return;
   }

   if (m_meta_graph->viewingUnprocessedPoints()) {
	   m_meta_graph->clearPoints();
   }
   else if (m_meta_graph->viewingProcessedLines()) {
      m_meta_graph->getDisplayedShapeGraph().removeSelected();
   }
   else if (m_meta_graph->viewingProcessedShapes()) {
      m_meta_graph->getDisplayedDataMap().removeSelected();
   }

   SetRedrawFlag(VIEW_ALL, REDRAW_GRAPH, NEW_DATA );
}

void CGraphDoc::OnUpdateEditClear(CCmdUI* pCmdUI) 
{
   int state = m_meta_graph->getState();

   if (m_meta_graph->isEditable()) {
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE);
   }
}

/////////////////////////////////////////////////////////////////////////////

void CGraphDoc::OnToolsMakeGraph() 
{
   int state = m_meta_graph->getState();
   if (m_communicator) {
      AfxMessageBox(_T("Please wait, another task is running"));
      return;
   }
   if (!m_meta_graph->viewingUnprocessedPoints()) {
      AfxMessageBox(_T("Sorry, you need an unprocessed set of points to make the visibility graph"));
      return;
   }
   if (!CheckMemory()) {
      return;
   }

   CMakeOptionsDlg dlg;
   dlg.m_boundarygraph = FALSE;
   if (dlg.DoModal() != IDOK) {
      return;
   }
   if (dlg.m_boundarygraph) {
      m_make_algorithm = 1;
   }
   else {
      m_make_algorithm = 0;
   }
   if (dlg.m_restrict_visibility) {
      m_make_maxdist = dlg.m_maxdist;
   }
   else {
      m_make_maxdist = -1.0;
   }
 
   m_communicator = new CMSCommunicator();
   m_communicator->CreateWaitDialog(_T("Constructing graph..."));
   m_communicator->SetFunction( CMSCommunicator::MAKEGRAPH );

   m_communicator->SetThread( AfxBeginThread( graphThread, this ) );

   /*
   OSVERSIONINFO verinfo;
   verinfo.dwOSVersionInfoSize = sizeof(verinfo);
   GetVersionEx( &verinfo );
   if (verinfo.dwPlatformId == VER_PLATFORM_WIN32_NT) {
      // The above normal thread priority hogs too much processor on NT
      AfxBeginThread( graphThread, this );   
   }
   else {
      // On 95/98, the above normal thread priority makes a small difference
      AfxBeginThread( graphThread, this, THREAD_PRIORITY_ABOVE_NORMAL );   
   }
   */
}

/////////////////////////////////////////////////////////////////////////////

// These are VGA options!

void CGraphDoc::OnUpdateVGAOptions(CCmdUI* pCmdUI) 
{
   if (m_meta_graph->viewingProcessedPoints()) {
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE);
   }	
}

// This is VGA run!

void CGraphDoc::OnUpdateToolsRun(CCmdUI* pCmdUI) 
{
   if (m_meta_graph->viewingProcessedPoints()) {
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE);
   }	
}

void CGraphDoc::OnVGAOptions() 
{
   COptionsDlg dlg;

   dlg.m_layer_names.push_back(pstring("<None>"));
   for (size_t i = 0; i < m_meta_graph->getDataMaps().getMapCount(); i++) {
       dlg.m_layer_names.push_back(m_meta_graph->getDataMaps().getMap(i).getName());
   }

   dlg.DoModal();
}

void CGraphDoc::OnToolsRun() 
{
   if (m_communicator) {
      AfxMessageBox(_T("Please wait, another task is running"));
      return;
   }

   // This is easy!
   COptionsDlg dlg;

   dlg.m_layer_names.push_back(pstring("<None>"));
   for (size_t i = 0; i < m_meta_graph->getDataMaps().getMapCount(); i++) {
       dlg.m_layer_names.push_back(m_meta_graph->getDataMaps().getMap(i).getName());
   }

   if (IDOK != dlg.DoModal()) {
      return;
   }

   // This is easy too... too easy... hmm... crossed-fingers, here goes:
   m_communicator = new CMSCommunicator();
   m_communicator->CreateWaitDialog(_T("Analysing graph..."));
   m_communicator->SetFunction( CMSCommunicator::ANALYSEGRAPH );

   m_communicator->SetThread( AfxBeginThread( graphThread, this ) );

   /*
   OSVERSIONINFO verinfo;
   verinfo.dwOSVersionInfoSize = sizeof(verinfo);
   GetVersionEx( &verinfo );
   if (verinfo.dwPlatformId == VER_PLATFORM_WIN32_NT) {
      // The above normal thread priority hogs too much processor on NT
      // but for now, do it anyway, as this algorithm is going slow...
      AfxBeginThread( graphThread, this, THREAD_PRIORITY_ABOVE_NORMAL ); 
   }
   else {
      // On 95/98, the above normal thread priority makes a small difference
      AfxBeginThread( graphThread, this, THREAD_PRIORITY_ABOVE_NORMAL );   
   }
   */
}

void CGraphDoc::OnToolsRunImportedAnalysis(CImportedModule& module, int analysis)
{
   if (m_communicator) {
      AfxMessageBox(_T("Please wait, another task is running"));
      return;
   }

   FUNC_PREPROCESS preprocess = (FUNC_PREPROCESS)GetProcAddress(module.m_inst,"preprocess");

   int analysis_type = module.m_analysis_type[analysis];

   if (preprocess == NULL || preprocess(analysis)) {
      if ((analysis_type & DLL_NO_ANALYSIS) == 0) {
         /*
         // 10.04.08 DLL_CREATE_LAYER is DEPRECATED -- from now on, the user must use DLL_GRAPH_FILE_ANALYSIS and create their own map using insertShapeMap()
         // only do this if the analysis type is set:
         if (analysis_type & DLL_CREATE_LAYER) {
            ShapeMap *map = NULL;
            if (analysis_type & DLL_AXIAL_ANALYSIS) {
               // new axial map
               int ref = m_meta_graph->addShapeGraph(pstring("Axial Map"),ShapeMap::AXIALMAP);
               map = &(m_meta_graph->getShapeGraphs().getMap(ref));
            }
            else if (analysis_type & DLL_SEGMENT_ANALYSIS) {
               // new segment map
               int ref = m_meta_graph->addShapeGraph(pstring("Segment Map"),ShapeMap::SEGMENTMAP);
               map = &(m_meta_graph->getShapeGraphs().getMap(ref));
            }
            else if (analysis_type & DLL_DATA_ANALYSIS) {
               // new data map
               int ref = m_meta_graph->addShapeMap(pstring("Data Map"));
               map = &(m_meta_graph->getDataMaps().getMap(ref));
            }
            Region r = m_meta_graph->getBoundingBox();
            if (r.isNull()) {
               r = Region(Point2f(-50.0,-50.0),Point2f(50.0,50.0));
            }
            if (map == NULL) {
               AfxMessageBox(_T("Error: Unknown type when module attempted to create a layer"));
               return;
            }
            map->init(0,r);
            map->setEditable(true);
            SetUpdateFlag(NEW_TABLE);
            SetRedrawFlag(VIEW_ALL,REDRAW_GRAPH, NEW_DATA);
         }
         // end 10.04.08 deprecation */
         m_communicator = new CMSCommunicator();
         m_communicator->CreateWaitDialog(_T("Analysing graph..."));
         m_communicator->SetFunction( CMSCommunicator::IMPORTEDANALYSIS );
         m_communicator->SetModule( module );
         m_communicator->SetOption( analysis );
         m_communicator->SetThread( AfxBeginThread( graphThread, this ) );
      }
   }
}

// Point depth either lines, axials or segments: 

void CGraphDoc::OnUpdateToolsPD(CCmdUI* pCmdUI) 
{
   if (m_meta_graph->viewingProcessed() && m_meta_graph->isSelected()) {
      pCmdUI->Enable(TRUE);      
   }
   else {
      pCmdUI->Enable(FALSE);  
   }
}

void CGraphDoc::OnToolsPD() 
{
   if (m_communicator) {
      AfxMessageBox(_T("Please wait, another process is running") );
      return;
   }

   if (m_meta_graph->viewingProcessed()) {
      if (m_meta_graph->isSelected()) {

         // This is easy too... too easy... hmm... crossed-fingers, here goes:
         m_communicator = new CMSCommunicator();
         m_communicator->CreateWaitDialog(_T("Calculating step depth..."));
         m_communicator->SetFunction( CMSCommunicator::POINTDEPTH );

         m_communicator->SetThread( AfxBeginThread( graphThread, this ) );
      }
   }
}

// metric pd only valid for points

void CGraphDoc::OnUpdateToolsMPD(CCmdUI* pCmdUI) 
{
   // VGA and segment maps only
   if ((m_meta_graph->viewingProcessedPoints() || (m_meta_graph->viewingProcessedLines() && m_meta_graph->getDisplayedShapeGraph().isSegmentMap())) && m_meta_graph->isSelected()) {
      pCmdUI->Enable(TRUE);      
   }
   else {
      pCmdUI->Enable(FALSE);  
   }
}

void CGraphDoc::OnToolsMPD() 
{
   if (m_communicator) {
      AfxMessageBox(_T("Please wait, another process is running") );
      return;
   }

   if (m_meta_graph->viewingProcessedPoints() || (m_meta_graph->viewingProcessedLines() && m_meta_graph->getDisplayedShapeGraph().isSegmentMap())) {
      if (m_meta_graph->isSelected()) {

         // This is easy too... too easy... hmm... crossed-fingers, here goes:
         m_communicator = new CMSCommunicator();
         m_communicator->CreateWaitDialog(_T("Calculating metric depth..."));
         m_communicator->SetFunction( CMSCommunicator::METRICPOINTDEPTH );

         m_communicator->SetThread( AfxBeginThread( graphThread, this ) );
      }
   }
}

void CGraphDoc::OnUpdateToolsAPD(CCmdUI* pCmdUI) 
{
   if (m_meta_graph->viewingProcessedPoints() && m_meta_graph->isSelected()) {
      pCmdUI->Enable(TRUE);      
   }
   else {
      pCmdUI->Enable(FALSE);  
   }
}

void CGraphDoc::OnToolsAPD() 
{
   if (m_communicator) {
      AfxMessageBox(_T("Please wait, another process is running") );
      return;
   }

   if (m_meta_graph->viewingProcessedPoints()) {
      if (m_meta_graph->isSelected()) {

         // This is easy too... too easy... hmm... crossed-fingers, here goes:
         m_communicator = new CMSCommunicator();
         m_communicator->CreateWaitDialog(_T("Calculating angular depth..."));
         m_communicator->SetFunction( CMSCommunicator::ANGULARPOINTDEPTH );

         m_communicator->SetThread( AfxBeginThread( graphThread, this ) );
      }
   }
}

void CGraphDoc::OnUpdateToolsTPD(CCmdUI *pCmdUI)
{
   // segment maps only (with selection)
   if (m_meta_graph->viewingProcessedLines() && m_meta_graph->getDisplayedShapeGraph().isSegmentMap() && m_meta_graph->isSelected()) {
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE);
   }	
}

void CGraphDoc::OnToolsTPD()
{
   if (m_communicator) {
      AfxMessageBox(_T("Please wait, another process is running") );
      return;
   }

   if (m_meta_graph->viewingProcessedLines() && m_meta_graph->getDisplayedShapeGraph().isSegmentMap()) {
      if (m_meta_graph->isSelected()) {

         // This is easy too... too easy... hmm... crossed-fingers, here goes:
         m_communicator = new CMSCommunicator();
         m_communicator->CreateWaitDialog(_T("Calculating topological depth..."));
         m_communicator->SetFunction( CMSCommunicator::TOPOLOGICALPOINTDEPTH );

         m_communicator->SetThread( AfxBeginThread( graphThread, this ) );
      }
   }
}

void CGraphDoc::OnBinDisplay() 
{
   if (m_communicator) {
      AfxMessageBox(_T("Please wait, another process is running") );
      return;
   }

   if (m_meta_graph->viewingProcessedPoints()) {
      if (m_meta_graph->isSelected()) {

         // This is easy too... too easy... hmm... crossed-fingers, here goes:
         m_communicator = new CMSCommunicator();
         m_communicator->CreateWaitDialog(_T("Showing bins..."));
         m_communicator->SetFunction( CMSCommunicator::BINDISPLAY );

         m_communicator->SetThread( AfxBeginThread( graphThread, this ) );
      }
   }	
}

/////////////////////////////////////////////////////////////////////////////

void CGraphDoc::OnUpdateToolsAxialLines(CCmdUI* pCmdUI) 
{
   if (m_meta_graph->viewingProcessed()) {
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE);
   }
}

void CGraphDoc::OnToolsAxialLines() 
{
   if (m_communicator) {
      AfxMessageBox(_T("Please wait, another task is running"));
      return;
   }

   // This is easy too... too easy... hmm... crossed-fingers, here goes:
   m_communicator = new CMSCommunicator();
   m_communicator->CreateWaitDialog(_T("Analysing graph..."));
   m_communicator->SetFunction( CMSCommunicator::MAKEAXIALLINES );
   m_communicator->SetThread( AfxBeginThread( graphThread, this ) );

   /*
   OSVERSIONINFO verinfo;
   verinfo.dwOSVersionInfoSize = sizeof(verinfo);
   GetVersionEx( &verinfo );
   if (verinfo.dwPlatformId == VER_PLATFORM_WIN32_NT) {
      // The above normal thread priority hogs too much processor on NT
      AfxBeginThread( graphThread, this );   
   }
   else {
      // On 95/98, the above normal thread priority makes a small difference
      AfxBeginThread( graphThread, this, THREAD_PRIORITY_ABOVE_NORMAL );   
   }
   */
}

/////////////////////////////////////////////////////////////////////////////

UINT graphThread( LPVOID param )
{
   CGraphDoc *pDoc = (CGraphDoc *) param;
   CMSCommunicator *comm = pDoc->m_communicator;

   if (comm) {

      int ok;

      switch (comm->GetFunction()) {

      case CMSCommunicator::IMPORT:
         ok = pDoc->m_meta_graph->loadLineData( comm, comm->GetOption() );
         if (ok == 1) {
            pDoc->SetModifiedFlag();
         }
         else if (ok == -1) {
            AfxMessageBox(_T("An error was found in the import file"));
         }
         // This might change the line layers available, alert the layer chooser:
         GetApp()->GetMainWnd()->PostMessage( WM_DMP_FOCUS_GRAPH, (WPARAM) pDoc, CGraphDoc::CONTROLS_LOADDRAWING );
         pDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_TOTAL, CGraphDoc::NEW_DATA );
         break;

      case CMSCommunicator::IMPORTMIF:
         ok = pDoc->m_meta_graph->loadMifMap( comm, *comm, comm->GetInfile2() );
         switch (ok) {
         case MINFO_MULTIPLE:
            AfxMessageBox(_T("The imported MIF file contains multiple shapes per object.\n")
                          _T("Please note that Depthmap has broken these up, so each shape has one row of attribute data.\n")
                          _T("Please consult your MapInfo provider for details."));
         case MINFO_OK:
            pDoc->SetUpdateFlag(CGraphDoc::NEW_TABLE);
            break;
         case MINFO_HEADER:
            AfxMessageBox(_T("Depthmap had a problem reading the header information in your MIF file."));
            break;
         case MINFO_TABLE:
            AfxMessageBox(_T("Depthmap had a problem reading the table data in your MID file."));
            break;
         case MINFO_MIFPARSE:
            AfxMessageBox(_T("Depthmap had a problem reading the shape data in your MIF file.\n")
                          _T("Please ensure that your shape data contains only points, lines, polylines or regions."));
            break;
         case MINFO_OBJROWS:
            AfxMessageBox(_T("Depthmap had a problem reading the shape data in your MIF file.\n")
                          _T("It seems as though there are a different number of shapes to rows in the associated table.\n")
                          _T("This may be due to the existance of unsupported shape types in the file."));
            break;
         }
         pDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_TOTAL, CGraphDoc::NEW_DATA );
         break;

      case CMSCommunicator::MAKEPOINTS:

         ok = pDoc->m_meta_graph->makePoints( comm->GetSeedPoint(), comm->GetOption(), comm );
         if (ok) {
            pDoc->SetModifiedFlag();
         }
         GetApp()->GetMainWnd()->PostMessage( WM_DMP_FOCUS_GRAPH, (WPARAM) pDoc, CGraphDoc::CONTROLS_LOADGRAPH );
         pDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_DATA );
         break;

      case CMSCommunicator::MAKEGRAPH:

         ok = pDoc->m_meta_graph->makeGraph( comm, pDoc->m_make_algorithm, pDoc->m_make_maxdist );
         if (ok) {
            pDoc->SetUpdateFlag(CGraphDoc::NEW_DATA);
         }
         pDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_DATA );
         break;

      case CMSCommunicator::ANALYSEGRAPH:

         ok = pDoc->m_meta_graph->analyseGraph( comm, GetApp()->m_options );
         pDoc->SetUpdateFlag(CGraphDoc::NEW_DATA);
         pDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_DATA );
         break;

      case CMSCommunicator::ANALYSEANGULAR:

         ok = pDoc->m_meta_graph->analyseAngular( comm, GetApp()->m_options.process_in_memory );
         if (ok) {
            pDoc->SetUpdateFlag(CGraphDoc::NEW_DATA);
         }
         pDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_DATA );
         break;

      case CMSCommunicator::MAKEISOVIST:

         if (comm->GetSeedAngle() == -1.0) {
            ok = pDoc->m_meta_graph->makeIsovist( comm, comm->GetSeedPoint() );
         }
         else {
            double ang1 = comm->GetSeedAngle() - comm->GetSeedFoV() * 0.5;
            double ang2 = comm->GetSeedAngle() + comm->GetSeedFoV() * 0.5;
            if (ang1 < 0.0) 
               ang1 += 2.0 * M_PI;
            if (ang2 > 2.0 * M_PI)
               ang2 -= 2.0 * M_PI;
            ok = pDoc->m_meta_graph->makeIsovist( comm, comm->GetSeedPoint(), ang1, ang2 );
         }
         if (ok) {
            pDoc->SetUpdateFlag(CGraphDoc::NEW_DATA);
         }
         // Tell the sidebar about the new map:
         GetApp()->GetMainWnd()->PostMessage( WM_DMP_FOCUS_GRAPH, (WPARAM) pDoc, CGraphDoc::CONTROLS_LOADGRAPH );
         pDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_DATA );
         break;

      case CMSCommunicator::MAKEISOVISTPATH:

         // the graph is going to build this path from a selection in a data map:
         // a data map must be topmost with lines or polylines selected
         // linedata must be displayed as per usual
         ok = pDoc->m_meta_graph->makeIsovistPath( comm, comm->GetSeedAngle() );

         // Tell the sidebar about the new map:
         GetApp()->GetMainWnd()->PostMessage( WM_DMP_FOCUS_GRAPH, (WPARAM) pDoc, CGraphDoc::CONTROLS_LOADGRAPH );
         pDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_DATA );
         break;

      case CMSCommunicator::MAKEALLLINEMAP:

         ok = pDoc->m_meta_graph->makeAllLineMap( comm, comm->GetSeedPoint() );
         if (ok) {
            pDoc->SetUpdateFlag(CGraphDoc::NEW_DATA);
         }
         // Tell the sidebar about the new map:
         GetApp()->GetMainWnd()->PostMessage( WM_DMP_FOCUS_GRAPH, (WPARAM) pDoc, CGraphDoc::CONTROLS_LOADGRAPH );
         pDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_DATA );
         break;

      case CMSCommunicator::MAKEFEWESTLINEMAP:

         ok = pDoc->m_meta_graph->makeFewestLineMap( comm, comm->GetOption() );
         if (ok) {
            pDoc->SetUpdateFlag(CGraphDoc::NEW_TABLE);
         }
         pDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_DATA );
         break;

      case CMSCommunicator::MAKEDRAWING:

         // option 1 is: 0 a data map, 1 an axial map
         ok = pDoc->m_meta_graph->convertToDrawing( comm, pstring(comm->GetString()), comm->GetOption(1) );
         if (ok) {
            pDoc->SetModifiedFlag();
         }
         else {
            AfxMessageBox(_T("No objects currently visible in drawing layers"));
         }
         // Tell the sidebar about the new map:
         GetApp()->GetMainWnd()->PostMessage( WM_DMP_FOCUS_GRAPH, (WPARAM) pDoc, CGraphDoc::CONTROLS_LOADDRAWING );
         pDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_TOTAL, CGraphDoc::NEW_DATA );
         break;

      case CMSCommunicator::MAKEUSERMAP:

         ok = pDoc->m_meta_graph->convertDrawingToAxial( comm, pstring(comm->GetString()) );
         if (ok) {
            pDoc->SetUpdateFlag(CGraphDoc::NEW_TABLE);
         }
         else {
            AfxMessageBox(_T("No objects currently visible in drawing layers"));
         }
         pDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_DATA );
         break;

      case CMSCommunicator::MAKEUSERMAPSHAPE:

         ok = pDoc->m_meta_graph->convertDataToAxial( comm, pstring(comm->GetString()), (comm->GetOption(0) == 1), (comm->GetOption(1) == 1) );
         if (ok) {
            if (comm->GetOption(0) == 0) {
               // note: there is both a new table and a deleted table, but deleted table leads to a greater redraw:
               pDoc->SetUpdateFlag(CGraphDoc::DELETED_TABLE);
            }
            else {
               pDoc->SetUpdateFlag(CGraphDoc::NEW_TABLE);
            }
         }
         else {
            AfxMessageBox(_T("No lines available in current layer to convert to axial lines"));
         }
         pDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_DATA );
         break;

      case CMSCommunicator::MAKEUSERSEGMAP:

         ok = pDoc->m_meta_graph->convertDrawingToSegment( comm, pstring(comm->GetString()) );
         if (ok) {
            pDoc->SetUpdateFlag(CGraphDoc::NEW_TABLE);
         }
         else {
            AfxMessageBox(_T("No objects currently visible in drawing layers"));
         }
         pDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_DATA );
         break;

      case CMSCommunicator::MAKEUSERSEGMAPSHAPE:

         ok = pDoc->m_meta_graph->convertDataToSegment( comm, pstring(comm->GetString()), (comm->GetOption(0) == 1), (comm->GetOption(1) == 1) );
         if (ok) {
            if (comm->GetOption(0) == 0) {
               // note: there is both a new table and a deleted table, but deleted table leads to a greater redraw:
               pDoc->SetUpdateFlag(CGraphDoc::DELETED_TABLE);
            }
            else {
               pDoc->SetUpdateFlag(CGraphDoc::NEW_TABLE);
            }
         }
         else {
            AfxMessageBox(_T("No lines available in current layer to convert to segments"));
         }
         pDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_DATA );
         break;
      
      case CMSCommunicator::MAKEGATESMAP:

         // note: relies on the fact that make data map from drawing sets option 1 to -1, whereas make data layer from graph it to either 0 or 1

         ok = pDoc->m_meta_graph->convertToData( comm, pstring(comm->GetString()), (comm->GetOption(0) == 1), comm->GetOption(1) );
         if (ok) {
            if (comm->GetOption(0) == 0) {
               // note: there is both a new table and a deleted table, but deleted table leads to a greater redraw:
               pDoc->SetUpdateFlag(CGraphDoc::DELETED_TABLE);
            }
            else {
               pDoc->SetUpdateFlag(CGraphDoc::NEW_TABLE);
            }
         }
         else {
            AfxMessageBox(_T("No objects currently visible in drawing layers"));
         }
         pDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_DATA );
         break;

      case CMSCommunicator::MAKECONVEXMAP:

         // note: relies on the fact that make convex map from drawing sets option 1 to -1, whereas make convex map from data sets it to either 0 or 1
         ok = pDoc->m_meta_graph->convertToConvex( comm, pstring(comm->GetString()), (comm->GetOption(0) == 1), comm->GetOption(1) );
         if (ok) {
            if (comm->GetOption(0) == 0) {
               // note: there is both a new table and a deleted table, but deleted table leads to a greater redraw:
               pDoc->SetUpdateFlag(CGraphDoc::DELETED_TABLE);
            }
            else {
               pDoc->SetUpdateFlag(CGraphDoc::NEW_TABLE);
            }
         }
         else {
            AfxMessageBox(_T("No polygons currently visible in drawing layers"));
         }
         pDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_DATA );
         break;

      case CMSCommunicator::MAKEBOUNDARYMAP:
/*
         ok = pDoc->m_meta_graph->convertBoundaryGraph( comm );
         if (ok) {
            pDoc->SetUpdateFlag(CGraphDoc::NEW_TABLE);
         }
         pDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_DATA );
         break;
*/
         break;

      case CMSCommunicator::MAKESEGMENTMAP:

         // convert percentage to fraction, and pass to metagraph
         ok = pDoc->m_meta_graph->convertAxialToSegment( comm, pstring(comm->GetString()), (comm->GetOption(0) == 1), (comm->GetOption(1) == 1), double(comm->GetOption(2)) / 100.0);
         if (ok) {
            if (comm->GetOption(0) == 0) {
               // note: there is both a new table and a deleted table, but deleted table leads to a greater redraw:
               pDoc->SetUpdateFlag(CGraphDoc::DELETED_TABLE);
            }
            else {
               pDoc->SetUpdateFlag(CGraphDoc::NEW_TABLE);
            }
         }
         else {
            AfxMessageBox(_T("No lines exist in map to convert to segments"));
         }
         pDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_DATA );
         break;

      case CMSCommunicator::AXIALANALYSIS:

         ok = pDoc->m_meta_graph->analyseAxial( comm, GetApp()->m_options );
         if (ok) {
            pDoc->SetUpdateFlag(CGraphDoc::NEW_DATA);
         }
         pDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_DATA );
         break;

      case CMSCommunicator::SEGMENTANALYSIS:

         ok = pDoc->m_meta_graph->analyseSegments( comm, GetApp()->m_options );
         if (ok) {
            pDoc->SetUpdateFlag(CGraphDoc::NEW_DATA);
         }
         pDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_DATA );
         break;

      case CMSCommunicator::TOPOMETANALYSIS:

         ok = pDoc->m_meta_graph->analyseTopoMet( comm, GetApp()->m_options );
         if (ok) {
            pDoc->SetUpdateFlag(CGraphDoc::NEW_DATA);
         }
         pDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_DATA );
         break;

      case CMSCommunicator::MAKEAXIALLINES:
         ok = pDoc->m_meta_graph->makeAxialLines( comm, GetApp()->m_options.process_in_memory );
         if (ok) {
            pDoc->SetUpdateFlag(CGraphDoc::NEW_DATA);
         }
         pDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_DATA );
         break;

      case CMSCommunicator::POINTDEPTH:
         {
            // Set up for options step depth selection
            Options options;
            options.global = 0;
            options.point_depth_selection = 1;
            
            ok = pDoc->m_meta_graph->analyseGraph( comm, options );
            if (ok) {
               pDoc->SetUpdateFlag(CGraphDoc::NEW_DATA);
            }
            pDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_POINTS, CGraphDoc::NEW_DATA );
         }
         break;     

      case CMSCommunicator::METRICPOINTDEPTH:
         {
            // Set up for options metric point depth selection
            Options options;
            options.global = 0;
            options.point_depth_selection = 2;

            ok = pDoc->m_meta_graph->analyseGraph( comm, options );
            if (ok) {
               pDoc->SetUpdateFlag(CGraphDoc::NEW_DATA);
            }
            pDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_POINTS, CGraphDoc::NEW_DATA );
         }
         break;

      case CMSCommunicator::ANGULARPOINTDEPTH:
         {
            // Set up for options angular point depth selection
            Options options;
            options.global = 0;
            options.point_depth_selection = 3;

            ok = pDoc->m_meta_graph->analyseGraph( comm, options );
            if (ok) {
               pDoc->SetUpdateFlag(CGraphDoc::NEW_DATA);
            }
            pDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_POINTS, CGraphDoc::NEW_DATA );
         }
         break;

      case CMSCommunicator::TOPOLOGICALPOINTDEPTH:
         {
            // Set up for options topological point depth selection (segment maps only)
            Options options;
            options.global = 0;
            options.point_depth_selection = 4;

            ok = pDoc->m_meta_graph->analyseGraph( comm, options );
            if (ok) {
               pDoc->SetUpdateFlag(CGraphDoc::NEW_DATA);
            }
            pDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_POINTS, CGraphDoc::NEW_DATA );
         }
         break;

      case CMSCommunicator::AGENTANALYSIS:
         {
            pDoc->m_meta_graph->runAgentEngine( comm );
            pDoc->SetUpdateFlag(CGraphDoc::NEW_DATA);
            pDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_POINTS, CGraphDoc::NEW_DATA );
         }
         break;

      case CMSCommunicator::BINDISPLAY:
         {
            // Set up for options metric point depth selection
            Options options;
            options.global = 0;
            options.point_depth_selection = 4;

            ok = pDoc->m_meta_graph->analyseGraph( comm, options );
            if (ok) {
               pDoc->SetUpdateFlag(CGraphDoc::NEW_DATA);
            }
            pDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_POINTS, CGraphDoc::NEW_DATA );
         }
         break;     

      case CMSCommunicator::IMPORTEDANALYSIS:
         {
            int analysis_type = comm->GetModule().m_analysis_type[ comm->GetOption() ];

            // temporarily disable VGA from the metagraph to avoid redraw problems:
            int state = pDoc->m_meta_graph->getState();
            if (analysis_type & DLL_GRAPH_FILE_ANALYSIS) {
               state &= ~(MetaGraph::POINTMAPS | MetaGraph::SHAPEGRAPHS | MetaGraph::DATAMAPS);
            }
            else if (analysis_type & DLL_VGA_ANALYSIS) {
               state &= ~MetaGraph::POINTMAPS;
            }
            else if (analysis_type & (DLL_AXIAL_ANALYSIS | DLL_SEGMENT_ANALYSIS)) {
               state &= ~MetaGraph::SHAPEGRAPHS;
            }
            else if (analysis_type & DLL_DATA_ANALYSIS) {
               state &= ~MetaGraph::DATAMAPS;
            }
            pDoc->m_meta_graph->setState(state);
            //
            FUNC_PROCESSVGA processVGA = (FUNC_PROCESSVGA)GetProcAddress(comm->GetModule().m_inst,"processVGA");
            FUNC_PROCESSSHAPE processShape = (FUNC_PROCESSSHAPE)GetProcAddress(comm->GetModule().m_inst,"processShape");
            FUNC_PROCESSATTRIBUTES processAttributes = (FUNC_PROCESSATTRIBUTES)GetProcAddress(comm->GetModule().m_inst,"processAttributes");
            FUNC_PROCESSGRAPHFILE processGraphFile = (FUNC_PROCESSGRAPHFILE)GetProcAddress(comm->GetModule().m_inst,"processGraphFile");
            FUNC_POSTPROCESS postprocess = (FUNC_POSTPROCESS)GetProcAddress(comm->GetModule().m_inst,"postprocess");
            //
            IComm dllcomm;
            dllcomm.setData( (void *) comm );
            IVGAMap dllvga;
            IShapeMap dllshape;
            IGraphFile dllgraph;
            if (analysis_type & DLL_GRAPH_FILE_ANALYSIS) {
               dllgraph.setData( (void *) pDoc->m_meta_graph );
            }
            if (analysis_type & DLL_VGA_ANALYSIS) {
               dllvga.setData( (void *) &(pDoc->m_meta_graph->getDisplayedPointMap()) );
            }
            if (analysis_type & (DLL_AXIAL_ANALYSIS | DLL_SEGMENT_ANALYSIS)) {
               dllshape.setData( (void *) &(pDoc->m_meta_graph->getDisplayedShapeGraph()) );
            }
            else if (analysis_type & DLL_DATA_ANALYSIS) {
               dllshape.setData( (void *) &(pDoc->m_meta_graph->getDisplayedDataMap()) );
            }
            IAttributes dllattr;
            dllattr.setData( (void *) &(pDoc->m_meta_graph->getAttributeTable()), analysis_type );
            //
            ok = false;
            if ((analysis_type & DLL_GRAPH_FILE_ANALYSIS) && processGraphFile != NULL) {
               ok = processGraphFile( comm->GetOption(), &dllcomm, &dllgraph );
            }
            else if ((analysis_type & DLL_VGA_ANALYSIS) && processVGA != NULL) {
               ok = processVGA( comm->GetOption(), &dllcomm, &dllvga, &dllattr );
            }
            else if ((analysis_type & (DLL_AXIAL_ANALYSIS | DLL_SEGMENT_ANALYSIS | DLL_DATA_ANALYSIS)) && processShape != NULL) {
               ok = processShape( comm->GetOption(), &dllcomm, &dllshape, &dllattr );
            }
            else if ((analysis_type & DLL_ATTRIBUTE_ANALYSIS) && processAttributes != NULL) {
               ok = processAttributes( comm->GetOption(), &dllcomm, &dllattr );
            }
            //
            // Now, the new file stuff could have added new layers or deleted them... we'll need to just explicitly check what's in the file and set the state accordingly:
            if (pDoc->m_meta_graph->PointMaps::size() > 0) {
               state |= MetaGraph::POINTMAPS;
            }
            if (pDoc->m_meta_graph->m_data_maps.getMapCount() > 0) {
               state |= MetaGraph::DATAMAPS;
            }
            if (pDoc->m_meta_graph->m_shape_graphs.getMapCount() > 0) {
               state |= MetaGraph::SHAPEGRAPHS;
            }
            // at this stage, before redraw, it is time to set the old state back on:
            pDoc->m_meta_graph->setState(state);
            //
            //
            if (ok) {
               pDoc->SetUpdateFlag(CGraphDoc::NEW_TABLE); // includes set modified flag (note: 10.04.08 NEW_TABLE required because analysis may create a new table)
               if (postprocess != NULL) {
                  postprocess( comm->GetOption() );
               }
            }
            // note: a total redraw is required in case new geometry has been added (new to Dmap 8)
            pDoc->SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_TOTAL, CGraphDoc::NEW_DATA );
         }
         break;
      }

      pDoc->m_communicator->DestroyWaitDialog();
      while (pDoc->m_communicator->IsDialog()) {
         Sleep(25);
      }
      delete pDoc->m_communicator;     // Ensure we delete *the* communicator
      pDoc->m_communicator = NULL;
   }

   return 0;
}

/////////////////////////////////////////////////////////////////////////////

BOOL CGraphDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
   if (m_communicator) {
      AfxMessageBox(_T("Please wait, another process is running") );
      return FALSE;
   }

   CWaitCursor wc;

   DeleteContents();
   SetModifiedFlag(TRUE);

   int ok = m_meta_graph->read( lpszPathName );

   SetModifiedFlag(FALSE);
   SetUpdateFlag(CGraphDoc::NEW_FILE,false);
   BOOL ret = FALSE;

   switch (ok) {
   case MetaGraph::OK:
      ret = TRUE;
      break;
   case MetaGraph::WARN_BUGGY_VERSION:
      AfxMessageBox(_T("Warning: this graph was made with a version of Depthmap that contained slight errors") );
      ret = TRUE;
      break;
   case MetaGraph::WARN_CONVERTED:
      AfxMessageBox(_T("Warning: this graph was made with an older version of Depthmap.\n")
                    _T("Some aspects of the graph may not have been translated to the new Depthmap properly.") );
      ret = TRUE;
      break;
   case MetaGraph::NOT_A_GRAPH:
      AfxMessageBox(_T("Unable to open graph: not recognised as a graph file.") );
      break;
   case MetaGraph::DAMAGED_FILE:
      AfxMessageBox(_T("Unable to open graph: the graph file is damaged.") );
      break;
   case MetaGraph::DISK_ERROR:
      AfxMessageBox(_T("Unable to open graph: an error occurred while trying to read from the disk.") );
      break;
   case MetaGraph::NEWER_VERSION:
      AfxMessageBox(_T("Unable to open graph: this graph has been written by a newer version of Depthmap.") );
      break;
   case MetaGraph::DEPRECATED_VERSION:
      AfxMessageBox(_T("Unable to open graph: this is a graph file format not supported by this version of Depthmap.") );
      break;
   default:
      {
         pstring err = pstringify(ok);
         AfxMessageBox( CString("Unable to open graph: error number ") + err.c_str() + CString(" occurred.") );
      }
      break;
   }

	return ret;
}

void CGraphDoc::OnFileSave()
{
   CDocument::OnFileSave();
}

void CGraphDoc::OnFileSaveAs()
{
   // This is based on Microsoft's "DoSave" function, but
   // it allows two options for saving: one as the current 
   // graph format, and one as the original

	CString newName = m_strPathName;
 	if (newName.IsEmpty()) {
      newName = m_strTitle + _T(".graph");
	}

   CFileDialog dlg(FALSE,_T(".graph"),newName,OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT,_T("Graph file|*.graph|Depthmap 8 graph|*.graph|All files (*.*)|*.*||"));
   
   if (IDOK != dlg.DoModal()) {
      return;
   }

   // get version from filter selected by user:
   int version = METAGRAPH_VERSION;
   // note appears to 1-based array (nFilterIndex 1 is standard, 2 is v8, and 3 is all files
   if (dlg.m_ofn.nFilterIndex == 2) {
      version = VERSION_81500;
      if (IDNO == AfxMessageBox(_T("This will save in a previous graph format, which may not support the latest Depthmap features.\nDo you want to continue?"),MB_YESNO)) {
         return;
      }
   }

   newName = dlg.GetPathName();

	CWaitCursor wait;

	if (!OnSaveDocument(newName,version))
	{
		CFile::Remove(newName);
      return;
	}

	// reset the title and change the document name
	SetPathName(newName);
}

BOOL CGraphDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
   // default: save in current version format
   int version = m_meta_graph->getVersion();
   // version == -1 is unsaved, which is fine to save in current version
   if (version != -1 && version != METAGRAPH_VERSION) {
      int response = AfxMessageBox(_T("This will overwrite the file with the latest graph format, which may not be readable by previous versions of Depthmap.\nDo you want to overwrite it?"),MB_YESNOCANCEL);
      if (IDYES == response) {
         return OnSaveDocument(lpszPathName, METAGRAPH_VERSION);
      }
      else if (IDNO == response) {
         return TRUE;
      }
      else {
         return FALSE;
      }
   }
   return OnSaveDocument(lpszPathName, METAGRAPH_VERSION);
}

BOOL CGraphDoc::OnSaveDocument(LPCTSTR lpszPathName, int version)
{
   if (m_communicator) {
      AfxMessageBox(_T("Please wait, another process is running") );
      return FALSE;
   }

   if (m_meta_graph->getState() & MetaGraph::BUGGY) {
      if (IDNO == AfxMessageBox(_T("This graph file was created by a version of Depthmap with slight errors.  Are you sure you want to save it?"), MB_YESNO | MB_ICONSTOP)) {
         return FALSE;
      }
   }

   CWaitCursor wc;

   SetModifiedFlag(TRUE);

   int ok = m_meta_graph->write( lpszPathName, version );

   if (ok == MetaGraph::OK) {
      SetModifiedFlag(FALSE);
      return TRUE;
   }
   else if (ok == MetaGraph::DISK_ERROR) {
      AfxMessageBox(_T("Unable to save graph: is there enough disk space?") );
   }

   return FALSE;
}

void CGraphDoc::OnCloseDocument() 
{
   if (m_communicator) {
      AfxMessageBox(_T("Depthmap is processing, please kill the process to continue"));
      return;
   }

   GetApp()->GetMainWnd()->PostMessage(WM_DMP_FOCUS_GRAPH, (WPARAM) this, CONTROLS_DESTROYALL);

	CDocument::OnCloseDocument();
}

// For view menu options -- deprecated, now in CViewSelector

BOOL CGraphDoc::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
   if (pHandlerInfo == NULL) {
      bool handled = false;
      if (nID >= 16000 && nID < GetApp()->m_next_imported_menu_id) {
         size_t index = GetApp()->m_imported_menu_items.searchindex(nID);
         if (index != paftl::npos && GetApp()->m_imported_menu_items.at(index).m_analysis_ref != -1) {
            CImportedModuleMenuItem& item = GetApp()->m_imported_menu_items.at(index);
            CImportedModule& module = GetApp()->m_imported_modules.search(item.m_module_ref);
            if (nCode == CN_COMMAND) {
               OnToolsRunImportedAnalysis(module, item.m_analysis_ref);
               handled = true;
            }
            else if (nCode == CN_UPDATE_COMMAND_UI) {
               bool enable = FALSE;
               switch (module.m_analysis_type[item.m_analysis_ref] & DLL_ANALYSIS_TYPE)
               {
               case DLL_NO_ANALYSIS:
                  enable = true;
                  break;
               case DLL_VGA_ANALYSIS:
                  if (m_meta_graph &&
                      m_meta_graph->getState() & MetaGraph::POINTMAPS && 
                      m_meta_graph->getViewClass() & MetaGraph::VIEWVGA && 
                      m_meta_graph->getDisplayedPointMap().isProcessed()) {
                     enable = true;
                  }
                  break;
               case DLL_AXIAL_ANALYSIS:
                  if (m_meta_graph &&
                      m_meta_graph->getState() & MetaGraph::SHAPEGRAPHS &&
                      m_meta_graph->getViewClass() & MetaGraph::VIEWAXIAL && 
                      m_meta_graph->getDisplayedShapeGraph().isAxialMap()) {
                     enable = true;
                  }
                  break;
               case DLL_SEGMENT_ANALYSIS:
                  if (m_meta_graph &&
                      m_meta_graph->getState() & MetaGraph::SHAPEGRAPHS &&
                      m_meta_graph->getViewClass() & MetaGraph::VIEWAXIAL && 
                      m_meta_graph->getDisplayedShapeGraph().isSegmentMap()) {
                     enable = true;
                  }
                  break;
               case DLL_DATA_ANALYSIS:
                  if (m_meta_graph &&
                      (m_meta_graph->getState() & MetaGraph::DATAMAPS) &&
                      (m_meta_graph->getViewClass() & MetaGraph::VIEWDATA)) {
                     enable = true;
                  }
                  break;
               case DLL_ATTRIBUTE_ANALYSIS:
                  if (m_meta_graph && m_meta_graph->viewingProcessed()) {
                     enable = true;
                  }
                  break;
               }
               /*
               // 10.04.08 DLL_CREATE_LAYER is DEPRECATED -- the user should use DLL_GRAPH_FILE_ANALYSIS and insertShapeMap() instead
               if (module.m_analysis_type[item.m_analysis_ref] & DLL_CREATE_LAYER) {
                  // if instructed that you can create a layer, always true provided there's a graph:
                  enable = true;
               }
               else // end 10.04.08 deprecation */
               if (module.m_analysis_type[item.m_analysis_ref] == DLL_GRAPH_FILE_ANALYSIS) {
                  // note "== DLL_GRAPH_FILE_ANALYSIS" -- the analysis type or'd above catches specific
                  // subcategories: e.g., DLL_GRAPH_FILE_ANALYSIS | DLL_SEGMENT_ANALYSIS means hand
                  // over the graph, but only allow if a segment map is topmost
                  enable = true;
               }
               ((CCmdUI*)pExtra)->Enable(enable);
               handled = true;
            }
         }
      }
      if (handled) {
         return TRUE;
      }
   }

   return CDocument::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

// Deprecated, now in CViewSelector

bool CGraphDoc::ViewHandler(int nCode, void *pExtra, int viewing, int layer)
{
/*
   if (nCode == CN_COMMAND) {
      if (layer != -1) {
         if (!m_meta_graph->setCurrentLayerRef(layer)) {
            // Layer doesn't exist for some reason...
            return false;
         }
         SetTitle(m_base_title + " - " + m_meta_graph->getCurrentLayer().getLayerName().c_str());
      }
      else {
         SetTitle(m_base_title + " - Attributes");
      }
      if (m_meta_graph->getViewing() != viewing) {
         m_meta_graph->setViewing(viewing, *m_meta_graph);
      }
      SetRedrawFlag(VIEW_ALL, true );
      SetRedrawFlag(VIEW_ALL, CGraphDoc::REDRAW_GRAPH );
   }
   else if (nCode == CN_UPDATE_COMMAND_UI) {
      ((CCmdUI*)pExtra)->Enable(TRUE);
      if (m_meta_graph->getViewing() == viewing && 
          (layer == -1 || m_meta_graph->getCurrentLayerRef() == layer)) {
         ((CCmdUI*)pExtra)->SetCheck(1);
      }
      else {
         ((CCmdUI*)pExtra)->SetCheck(0);
      }
   }
*/
   return true;
}

bool CGraphDoc::CheckMemory(const CString& filename) 
{
   // Check there's enough memory to hold the graph (based on benchmarks)
   MEMORYSTATUS memstat;
   memstat.dwLength = sizeof(MEMORYSTATUS);
   GlobalMemoryStatus(&memstat);
   double avail = double(memstat.dwAvailPhys) / double(1024 * 1024);

   double min_required = double(m_meta_graph->getDisplayedPointMap().getPointCount()) * 0.0050;
   double absolute_min_req = double(m_meta_graph->getDisplayedPointMap().getPointCount()) * 0.0025;

   if (!filename.IsEmpty()) {
      ifstream test(filename, ios::binary);
      if (test.fail()) {
         AfxMessageBox(CString("Couldn't open ") + filename);
         if (test.is_open()) {
            test.close();
         }
         return false;
      }
      char header[3];
      test.read( (char *) &header, 3 );
      if (header[0] != 'g' || header[1] != 'r' || header[2] != 'f') {
         AfxMessageBox(_T("Sorry: this is not a Depthmap graph file"));
         test.close();
         return false;
      }
      int version;
      test.read( (char *) &version, sizeof(version));
      test.seekg(0,ios::end);
      if (version < VERSION_NGRAPH_INTROD) {
         // Should be able to fit old graphs into about the same memory:
         min_required = 1.1 * double(test.tellg()) / double(1024 * 1024);
      }
      else {
         // Don't know why, but the current version appears double the file size when in memory:
         min_required = 1.1 * double(test.tellg()) / double(1024 * 1024); // .... did use to be 2.2
      }
      test.close();
      if (version < VERSION_NGRAPH_INTROD) {
         if (IDYES != AfxMessageBox(_T("Warning, this graph will require conversion, which may take a few minutes.\nDo you want to continue?"), MB_YESNO)) {
            return false;
         }
      }
   }

   if (avail < min_required) {
      CString str;
      str.Format(_T("%.1f MB available, %.1f MB advised"), avail, min_required );
      if ( IDYES != AfxMessageBox( CString("According to benchmark stats, there may not be enough memory to process this graph.\n") +
                                   _T("(") + str + _T(")\n") +
                                   _T("The analysis may take much longer than usual.  Do you still want to continue?"), MB_YESNO ) ) {
         return false;
      }
   }

   return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CGraphDoc::OnAddGate() 
{
   if (m_communicator) {
      AfxMessageBox(_T("Please wait, another process is running") );
      return;
   }
   if (m_meta_graph->getViewClass() & MetaGraph::VIEWVGA) {
      /*
      if (!m_meta_graph->getDisplayedPointMap().convertSelToDataObject( *m_meta_graph )) {
         AfxMessageBox(_T("Sorry, there was an error adding object to layer."));
      }
      */
      // try the new thingy:
      if (m_meta_graph->convertPointsToShape()) {
         // redraw all as change of view is affected here:
         // (also possibly even the sidebar menu:
         GetApp()->GetMainWnd()->PostMessage( WM_DMP_FOCUS_GRAPH, (WPARAM) this, CGraphDoc::CONTROLS_LOADGRAPH );
         SetRedrawFlag(VIEW_ALL, REDRAW_GRAPH, NEW_DEPTHMAPVIEW_SETUP );
      }
      else {
         AfxMessageBox(_T("There was an error trying to convert points to a shape.\nPlease check that you have selected a single polygon without holes."));
      }
   }
}

void CGraphDoc::OnUpdateAddGate(CCmdUI* pCmdUI) 
{
   if (m_meta_graph && 
       (m_meta_graph->getViewClass() & MetaGraph::VIEWVGA) && 
       m_meta_graph->isSelected()) {
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE);
   }
}

void CGraphDoc::OnPushToLayer() 
{
   if (m_meta_graph->viewingProcessed()) {
      int toplayerclass = (m_meta_graph->getViewClass() & MetaGraph::VIEWFRONT);
      pstring origin_layer;
      pstring origin_attribute;
      pqmap<IntPair,pstring> names;
      // I'm just going to allow push from any layer to any other layer
      // (apart from VGA graphs, which cannot map onto themselves
      if (toplayerclass == MetaGraph::VIEWVGA) {
         // bit clunky just to get two names out...
         PointMap& map = m_meta_graph->getDisplayedPointMap();
         origin_layer = pstring("Visibility Graphs: ") + map.getName(); 
         origin_attribute = map.getAttributeTable().getColumnName(map.getDisplayedAttribute());
      }
      else if (toplayerclass == MetaGraph::VIEWAXIAL) {
         // bit clunky just to get two names out...
         ShapeGraph& map = m_meta_graph->getDisplayedShapeGraph();
         origin_layer = pstring("Shape Graphs: ") + map.getName(); 
         origin_attribute = map.getAttributeTable().getColumnName(map.getDisplayedAttribute());
      }
      else if (toplayerclass == MetaGraph::VIEWDATA) {
         // bit clunky just to get two names out...
         ShapeMap& map = m_meta_graph->getDisplayedDataMap();
         origin_layer = pstring("Data Maps: ") + map.getName(); 
         origin_attribute = map.getAttributeTable().getColumnName(map.getDisplayedAttribute());
      }
      else {
         // eek!
         AfxMessageBox(_T("No valid map displayed from which to push data"));
         return;
      }
      // layers to push to:
      size_t i;
      ShapeMaps<ShapeMap>& datamaps = m_meta_graph->getDataMaps();
      for (i = 0; i < datamaps.getMapCount(); i++) {
         if (toplayerclass != MetaGraph::VIEWDATA || i != datamaps.getDisplayedMapRef()) {
            names.add(IntPair(MetaGraph::VIEWDATA,int(i)),pstring("Data Maps: ") + datamaps.getMap(i).getName());
         }
      }
      ShapeGraphs& shapegraphs = m_meta_graph->getShapeGraphs();
      for (i = 0; i < shapegraphs.getMapCount(); i++) {
         if (toplayerclass != MetaGraph::VIEWAXIAL || i != shapegraphs.getDisplayedMapRef()) {
            names.add(IntPair(MetaGraph::VIEWAXIAL,int(i)),pstring("Shape Graphs: ") + shapegraphs.getMap(i).getName());
         }
      }
      for (i = 0; i < m_meta_graph->PointMaps::size(); i++) {
         // note 1: no VGA graph can push to another VGA graph (point onto point transforms)
         // note 2: I simply haven't written "axial" -> vga yet, not that it can't be possible (e.g., "axial" could actually be a convex map)
         if (toplayerclass != MetaGraph::VIEWVGA && toplayerclass != MetaGraph::VIEWAXIAL) {
            names.add(IntPair(MetaGraph::VIEWVGA,int(i)),pstring("Visibility Graphs: ") + m_meta_graph->PointMaps::at(i).getName());
         }
      }
      CPushDialog dlg(names);
      dlg.m_origin_layer = origin_layer.c_str();
      dlg.m_origin_attribute = origin_attribute.c_str();
      if (IDOK == dlg.DoModal()) {
         m_communicator = new CMSCommunicator;   // dummy value to prevent draw while this operation is in progress
         // now have to separate vga and axial layers again:
         int sel = dlg.m_layer_selection;
         IntPair dest = names.key(sel);
         CWaitCursor c;
         m_meta_graph->pushValuesToLayer(dest.a, dest.b, dlg.m_function, dlg.m_count_intersections);
         delete m_communicator;
         m_communicator = NULL;
         SetUpdateFlag(NEW_TABLE);
      }
   }
}

void CGraphDoc::OnUpdatePushToLayer(CCmdUI* pCmdUI) 
{
   if (m_meta_graph && 
       (m_meta_graph->viewingProcessedPoints() && 
        m_meta_graph->getDataMaps().getMapCount()) ||
       (m_meta_graph->viewingProcessedLines() && 
        m_meta_graph->getDataMaps().getMapCount() || m_meta_graph->getShapeGraphs().getMapCount() > 1) ||
        m_meta_graph->viewingProcessedShapes() &&
        (m_meta_graph->getShapeGraphs().getMapCount() || m_meta_graph->getDataMaps().getMapCount() > 1 || m_meta_graph->PointMaps::size())) {
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE);
   }
}


///////////////////////////////////////////////////////////////////////////

void CGraphDoc::OnRedtest() 
{
   // OzlemSpecial2
   // chop rays if they intersect other buildings,
   // exclude them altogether if there is just a short gap between buildings

   // OzlemSpecial3
   // chop rays according to encountered feature codes

   // expect this to be the rays:
   int a = (int) m_meta_graph->getDataMaps().getMapRef("Axis Rays");

   // expect this to be the buildings:
   int b = 2;  // hmm -- just guess a number! (0 for OzlemSpecial2 and 2 for OzlemSpecial3 as designed to be run)

   m_meta_graph->getDataMaps().getMap(a).ozlemSpecial3( m_meta_graph->getDataMaps().getMap(b) );

   SetUpdateFlag(CGraphDoc::NEW_DATA);
   SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_POINTS, CGraphDoc::NEW_DATA );
}

void CGraphDoc::OnGreentest() 
{
   // OzlemSpecial5
   // mark up buildings according to exposure types

   // expect this to be the rays:
   int a = (int) m_meta_graph->getDataMaps().getMapRef("Axis Rays");

   // expect this to be the buildings:
   int b = 0;  // hmm -- just guess a number! (0 for OzlemSpecial5)

   m_meta_graph->getDataMaps().getMap(a).ozlemSpecial5( m_meta_graph->getDataMaps().getMap(b) );
  
//   m_meta_graph->getDisplayedDataMap().ozlemSpecial6();
}

void CGraphDoc::OnPinktest() 
{
   // note: expects 0 to be points, 1 to be lines
   m_meta_graph->getDataMaps().getMap(0).ozlemSpecial7( m_meta_graph->getDataMaps().getMap(1) );
}

void CGraphDoc::OnTestButton() 
{
   AfxMessageBox(_T("test!"));
   AttributeTable *tester = new AttributeTable();
   for (int i = 0; i < 1000000; i++) {
      tester->insertRow(i);
   }
   delete tester;
   AfxMessageBox(_T("tested!"));
   /*
   // remember which shape map is current:
   int a = m_meta_graph->getDataMaps().getDisplayedMapRef();
   // now add a new one:
   int b = m_meta_graph->getDataMaps().addMap("Axis Rays",ShapeMap::DATAMAP);
   
   m_meta_graph->getDataMaps().at(a).ozlemSpecial( m_meta_graph->getDataMaps().at(b) );

   SetUpdateFlag(CGraphDoc::NEW_TABLE);
   SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_POINTS, CGraphDoc::NEW_DATA );
   */
   /*
   CFileDialog saveDialog(FALSE,"txt",NULL,
						  OFN_EXPLORER|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "Text files (*.txt)|*.txt|All files (*.*)|*.*||");
   saveDialog.m_ofn.lpstrTitle = "Save Output As";

   if (saveDialog.DoModal() == IDOK) {
      CWaitCursor;
      CString outfile = saveDialog.GetPathName();
      ofstream stream(outfile);
      stream << "BuildIDInt\tBearing32_1\tBearing32_2\tBearing32_3\tBearing32_4\tBearing72_1\tBearing72_2\tBearing72_3\tBearing72_4\tDifficult\tImpossible" << endl;

      SetUpdateFlag(CGraphDoc::NEW_DATA);
      SetRedrawFlag(CGraphDoc::VIEW_ALL, CGraphDoc::REDRAW_POINTS, CGraphDoc::NEW_DATA );
   }
*/

   /*
   if (saveDialog.DoModal() == IDOK) {
      CWaitCursor;
      CString outfile = saveDialog.GetPathName();
      ofstream stream(outfile);
      stream << "Building\tRoad1\tRoad2\tangle" << endl;
      m_meta_graph->getDataMaps().at(1).ozlemSpecial( stream, m_meta_graph->getDataMaps().at(0));
   }
*/
   /*
   ifstream file("F:\\alasdair\\apps\\saladtester\\paths.txt");
                                                                   // <<
   m_evolved_paths.clear();
   for (int i = 0; i < 19; i++) {
      m_evolved_paths.push_back(pvecpoint());
      for (int j = 0; j < 1200; j++) {
         Point2f p;
         file >> p.x >> p.y;
         m_evolved_paths.tail().push_back(p);
      }
   }
*/

   /*
   int counter = 0;

   int seed = 1;
   pafsrand(seed);
   int nextrand;

   int buckets[100];
   for (int i = 0; i < 100; i++)
   {
      buckets[i] = 0;
   }

   do {
      nextrand = pafrand();
      buckets[nextrand % 100] += 1;
   } while (++counter < PAF_RAND_MAX * 10);

   ofstream data("data.txt");

   for (int j = 0; j < 100; j++) {
      data << buckets[j] << endl;
   }

   if (counter == PAF_RAND_MAX) {
      AfxMessageBox(_T("blah!"));
   }
   else {
      AfxMessageBox(_T("wah!"));
   }
*/
   //m_meta_graph->highlightBorder();
   //SetRedrawFlag(VIEW_ALL,REDRAW_GRAPH);
/*   if (m_agent_manager && m_agent_manager->isPaused()) {
      Evoecomorph& evo = m_agent_manager->getEvoecomorph();
      double val = evo.getLatestFitness();
      char out[64];
      printf(out,"%f",val);
      AfxMessageBox(out);
      m_agent_manager->unPause();
   }*/
/*
   m_meta_graph->loadGraphAgent();
   m_meta_graph->getLineLayer(0,0).setEditable(true);
   */
/*
   for (int run = 0; run < 3; run++) {
      int seed = run + 2;
      int return_time = 600;
      //
      if (m_evoecomorph) {
         delete m_evoecomorph;
         m_evoecomorph = NULL;
      }
      pafsrand(seed);
      m_evoecomorph = new Evoecomorph;
      delete m_meta_graph;
      m_meta_graph = new MetaGraph;
      m_evoecomorph->init( *m_meta_graph, return_time );
      for (int genset = 0; genset < 10; genset++) {
         if (genset != 0) {
            delete m_meta_graph;
            m_meta_graph = new MetaGraph;
            m_evoecomorph->next( *m_meta_graph );
            //m_evoecomorph->blank( *m_meta_graph ); // this was set to blank, I don't know why
         }
         for (int i = 0; i < 500; i++) {
            char blah2[128];
            for (int j = 0; j < 300 + (return_time * 3) / 2; j++) { // moved down from 600 to 300 in line with changes to evoecomorph loading
               m_evoecomorph->step(*m_meta_graph);
            }
            m_evoecomorph->evaluate(*m_meta_graph);
            sprintf(blah2, "seed %d gen %d fit %f", seed, i + genset * 500, m_evoecomorph->m_fitness);
            SetTitle(blah2);
            delete m_meta_graph;
            if (i < 499) {
               m_meta_graph = new MetaGraph;
               m_evoecomorph->next( *m_meta_graph );
            }
         }
         m_meta_graph = new MetaGraph;
         m_evoecomorph->best( *m_meta_graph );
         for (int k = 0; k < 300 + (return_time * 3) / 2; k++) {  // moved down from 600 to 300
            m_evoecomorph->step(*m_meta_graph);
         }
         m_evoecomorph->evaluate(*m_meta_graph);
         char blah3[128];
         char blah4[128];
         sprintf(blah3, "dcc%d-seed%d-gen%d-noexit.graph", return_time, seed, i + genset * 500);
         sprintf(blah4, "dcc%d-seed%d-gen%d-noexit.txt", return_time, seed, i + genset * 500);
         ofstream summary(blah4);
         char blah[256];
         sprintf(blah, "Bad rooms %d, bad agents %d, avg diff %f, fitness %f",
            m_evoecomorph->m_bad_rooms, m_evoecomorph->m_bad_agents, m_evoecomorph->m_avg_diff, m_evoecomorph->m_fitness);
         summary << blah << endl;
         m_meta_graph->write(blah3, METAGRAPH_VERSION);
      }
   }

   SetUpdateFlag(CGraphDoc::NEW_DATA);
   SetRedrawFlag(VIEW_ALL, REDRAW_GRAPH, NEW_DATA );

   char blah[256];
   sprintf(blah, "Bad rooms %d, bad agents %d, avg diff %f, fitness %f",
      m_evoecomorph->m_bad_rooms, m_evoecomorph->m_bad_agents, m_evoecomorph->m_avg_diff, m_evoecomorph->m_fitness);

   AfxMessageBox(blah);
*/
   /*
   static int x = 0;
   static bool widdywoo = false;

   if (m_meta_graph && m_meta_graph->getState() & MetaGraph::GRAPH) {
      // cross fingers and go for it!
      if (!widdywoo) {
         x = m_meta_graph->addLineDynamic(Line(Point2f(0.0,0.0),Point2f(10.0,10.0)));
         widdywoo = true;
      }
      else {
         m_meta_graph->removeLineDynamic(x);
         widdywoo = false;
      }

      m_meta_graph->dynamicSparkGraph2();
   }
   */
}

void CGraphDoc::SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU) 
{
   // Set path name calls window retitle... here we override the title to add some more of our own...	
	CDocument::SetPathName(lpszPathName, bAddToMRU);

   m_base_title = GetTitle();
   //SetTitle(m_base_title + " - Attributes");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void CGraphDoc::OnAddColumn() 
{
   CRenameObjectDlg dlg("Column");  // using the constructor without a column name sets rename column dialog to insert column name mode
   bool success = false;
   while (dlg.DoModal() == IDOK && !success) {
      if (dlg.m_object_name.IsEmpty()) {
         AfxMessageBox(_T("Column name cannot be empty"));
      }
      else {
         AttributeTable& tab = m_meta_graph->getAttributeTable();
         bool found = false;
         for (int i = 0; i < tab.getColumnCount(); i++) {
            if (tab.getColumnName(i) == pstring(dlg.m_object_name)) {
               AfxMessageBox(_T("Sorry, another column already has this name, please choose a unique column name"));
               found = true;
               break;
            }
         }
         if (!found) {
            success = true;
            break;
         }
      }
   }
   if (success) {
      int col = m_meta_graph->addAttribute(pstring(dlg.m_object_name));
      m_meta_graph->setDisplayedAttribute(col);
      SetUpdateFlag(CGraphDoc::NEW_DATA);
      // Tell the views to update their menus
      SetRedrawFlag(VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_COLUMN );
   }
}

void CGraphDoc::OnRenameColumn() 
{
   AttributeTable *tab = &(m_meta_graph->getAttributeTable());
   int col = m_meta_graph->getDisplayedAttribute();
   // -1 is reference number, -2 is displaying nothing (-2 shouldn't happen but is)
   if (col == -1 || col == -2 || m_meta_graph->isAttributeLocked(col)) {
      AfxMessageBox(_T("Cannot rename locked column"));
      return;
   }

   int newcol = RenameColumn(tab,col);
   if (newcol != -1) {
      m_meta_graph->setDisplayedAttribute(newcol);
      SetUpdateFlag(CGraphDoc::NEW_DATA);
      SetRedrawFlag(VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_COLUMN );
   }
}

int CGraphDoc::RenameColumn(AttributeTable *tab, int col)
{
   CString colname = tab->getColumnName(col).c_str();
   CRenameObjectDlg dlg("Column",colname);  // using the column name sets the dialog to replace column name mode
   bool success = false;
   while (dlg.DoModal() == IDOK && !success && dlg.m_object_name != colname) {
      int newcol = tab->renameColumn(col,pstring(dlg.m_object_name));
      if (newcol == -1) {
         AfxMessageBox(_T("Sorry, another column already has this name, please choose a unique column name"));
      }
      else {
         return newcol;
      }
   }
   return -1;
}

void CGraphDoc::OnColumnProperties() 
{
   AttributeTable *tab = &(m_meta_graph->getAttributeTable());
   int col = m_meta_graph->getDisplayedAttribute();

   CColumnPropertiesDlg dlg(tab,col);
   
   dlg.DoModal();
}

///////////////////////////////////////////////////////////////////////////////////////

void CGraphDoc::OnUpdateColumn() 
{
   int col = m_meta_graph->getDisplayedAttribute();
   // -1 is reference number, -2 is displaying nothing (-2 shouldn't happen but is)
   if (col == -1 || col == -2 || m_meta_graph->isAttributeLocked(col)) {
      AfxMessageBox(_T("Cannot edit locked column"));
      return;
   }

   PointMap *pointmap = NULL;
   ShapeMap *shapemap = NULL;
   int vc = m_meta_graph->getViewClass();
   if (vc & MetaGraph::VIEWVGA) {
      pointmap = &(m_meta_graph->getDisplayedPointMap());
   }
   else if (vc & MetaGraph::VIEWAXIAL) {
      shapemap = &(m_meta_graph->getDisplayedShapeGraph());
   }
   else if (vc & MetaGraph::VIEWDATA) {
      shapemap = &(m_meta_graph->getDisplayedDataMap());
   }

   if (ReplaceColumnContents(pointmap,shapemap,col)) {
      m_meta_graph->setDisplayedAttribute(col);
      SetUpdateFlag(CGraphDoc::NEW_DATA);
      SetRedrawFlag(VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_DATA );
   }
}

// Either shapemap or pointmap should be NULL:
bool CGraphDoc::ReplaceColumnContents(PointMap *pointmap, ShapeMap *shapemap, int col)
{
   SalaObj program_context;
   SalaGrf graph;
   if (pointmap != NULL) {
      graph.map.point = pointmap;
      program_context = SalaObj(SalaObj::S_POINTMAPOBJ, graph);
   }
   else if (shapemap != NULL) {
      SalaGrf graph;
      graph.map.shape = shapemap;
      program_context = SalaObj(SalaObj::S_SHAPEMAPOBJ, graph);
   }
   else {
      AfxMessageBox(_T("Replace column contents requires either a shape map or visibility graph"));
      return false;
   }

   AttributeTable *table = program_context.getTable();

   // insert dialog is a misnomer now!
   CInsertColumnDlg dlg(table,col);  // Using a column number sets it to use the replace text rather than select text
   
   bool error = true;
   while (error && IDOK == dlg.DoModal()) {
      error = false;
      size_t n = dlg.m_formula_text.length();
      char *text = new char[n+1];
      if (n == 0) {
         AfxMessageBox(_T("Please enter a formula"));
         error = true;
      }
      else {
         strcpy(text,dlg.m_formula_text.c_str());
         istrstream stream(text);
         SalaProgram proggy(program_context);
         if (!proggy.parse(stream)) {
            CString msg = CString("There was an error parsing your formula:\n\n") + 
               proggy.getLastErrorMessage().c_str();
            AfxMessageBox(msg);
            error = true;
         }
         else {
            // just check you really are viewing the layers:
            bool retvar;
            if (dlg.m_selection_only) {
               retvar = proggy.runupdate(col,pointmap ? pointmap->getSelSet() : shapemap->getSelSet());
            }
            else {
               retvar = proggy.runupdate(col);
            }
            if (!retvar) {
               CString msg = CString("There was an error running your formula:\n\n") + 
                  proggy.getLastErrorMessage().c_str();
               AfxMessageBox(msg);
               error = true;
            }
         }
      }
      if (!error) {
         table->setColumnFormula(col,text);
      }
      delete [] text;
   }
   return !error;
}

void CGraphDoc::OnEditQuery() 
{
   PointMap *pointmap = NULL;
   ShapeMap *shapemap = NULL;
   int vc = m_meta_graph->getViewClass();
   if (vc & MetaGraph::VIEWVGA) {
      pointmap = &(m_meta_graph->getDisplayedPointMap());
   }
   else if (vc & MetaGraph::VIEWAXIAL) {
      shapemap = &(m_meta_graph->getDisplayedShapeGraph());
   }
   else if (vc & MetaGraph::VIEWDATA) {
      shapemap = &(m_meta_graph->getDisplayedDataMap());
   }

   if (SelectByQuery(pointmap,shapemap)) {
      SetRedrawFlag(VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_DATA );
   }
}

// Either shapemap or pointmap should be NULL:
bool CGraphDoc::SelectByQuery(PointMap *pointmap, ShapeMap *shapemap)
{
   SalaObj program_context;
   SalaGrf graph;
   if (pointmap != NULL) {
      graph.map.point = pointmap;
      program_context = SalaObj(SalaObj::S_POINTMAPOBJ, graph);
   }
   else if (shapemap != NULL) {
      SalaGrf graph;
      graph.map.shape = shapemap;
      program_context = SalaObj(SalaObj::S_SHAPEMAPOBJ, graph);
   }
   else {
      AfxMessageBox(_T("Replace column contents requires either a shape map or visibility graph"));
      return false;
   }

   AttributeTable *table = program_context.getTable();
   // insert dialog is a misnomer now!
   CInsertColumnDlg dlg(table,-1);  // -1 sets it to use the select text rather than replace text
   bool error = true;
   while (error && IDOK == dlg.DoModal()) {
      error = false;
      // unicode conversion (AT 31.01.11) -- seems easiest at the mo to simply feed through pstring converter
      pstring multibytetext(GetApp()->m_formula_cache);
      char *text = new char[multibytetext.length()+1];
      strcpy(text,multibytetext.c_str());
      istrstream stream(text);
      SalaProgram proggy(program_context);
      if (!proggy.parse(stream)) {
         CString msg = CString("There was an error parsing your formula:\n") + 
            proggy.getLastErrorMessage().c_str();
         AfxMessageBox(msg);
         error = true;
      }
      else {
         // just check you really are viewing the layers:
         bool retvar;
         pvecint selset;
         if (dlg.m_selection_only) {
            retvar = proggy.runselect(selset,pointmap ? pointmap->getSelSet() : shapemap->getSelSet());
         }
         else {
            retvar = proggy.runselect(selset);
         }
         if (!retvar) {
            CString msg = CString("There was an error running your formula:\n") + 
               proggy.getLastErrorMessage().c_str();
            AfxMessageBox(msg);
            error = true;
         }
         else {
            // make the selection using the selset:
            if (pointmap) {
               pointmap->setCurSel(selset);
            }
            else {
               // note, shape maps have been working with rowids directly:
               shapemap->setCurSelDirect(selset);
            }
         }
      }
      delete [] text;
      if (!error) {
         return true;
      }
   }
   return false;
}

void CGraphDoc::OnEditSelectToLayer()
{
   if ((m_meta_graph->getViewClass() & (MetaGraph::VIEWAXIAL|MetaGraph::VIEWDATA))
      && m_meta_graph->isSelected()) {

      CRenameObjectDlg dlg("Layer"); // note, without specifying existing layer name, this defaults to "New layer" behaviour
      if (IDOK == dlg.DoModal()) {

         pstring layer_name = dlg.m_object_name;
         if (layer_name.empty()) {
            layer_name = "Untitled";
         }

         bool retvar = false;
         if (m_meta_graph->getViewClass() & (MetaGraph::VIEWAXIAL)) {
            retvar = m_meta_graph->getDisplayedShapeGraph().selectionToLayer(layer_name);
         }
         else {
            retvar = m_meta_graph->getDisplayedDataMap().selectionToLayer(layer_name);
         }
         if (retvar) {
            SetRedrawFlag(VIEW_ALL,REDRAW_GRAPH, NEW_DATA);
            SetUpdateFlag(CGraphDoc::NEW_TABLE);
         }
         else {
            AfxMessageBox(_T("Couldn't create new layer.\nPlease note there is a limit of 64 layers per map."));
         }
      }
   }
}

void CGraphDoc::OnUpdateEditSelectToLayer(CCmdUI *pCmdUI)
{
   if ((m_meta_graph->getViewClass() & (MetaGraph::VIEWAXIAL|MetaGraph::VIEWDATA))
       && m_meta_graph->isSelected()) {
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE);
   }
}

///////////////////////////////////////////////////////////////////////

void CGraphDoc::OnRemoveColumn() 
{
   // just check you really are viewing the layers:
   int col = m_meta_graph->getDisplayedAttribute();
   // -1 is reference number, -2 is displaying nothing (-2 shouldn't happen but is)
   if (col == -1 || col == -2 || m_meta_graph->isAttributeLocked(col)) {
      AfxMessageBox(_T("Cannot remove locked column"));
   }
   else if (IDYES == AfxMessageBox(_T("Are you sure you want to delete the currently displayed column?"), MB_YESNO)) {
      // note this -1 simply means shift back one
      m_meta_graph->setDisplayedAttribute(col-1);
      m_meta_graph->removeAttribute(col);
      SetUpdateFlag(CGraphDoc::NEW_DATA);
      SetRedrawFlag(VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_COLUMN );
   }
}

void CGraphDoc::OnUpdateAddColumn(CCmdUI* pCmdUI) 
{
   if (m_meta_graph && !m_communicator && m_meta_graph->viewingProcessed()) {
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE);
   }
}


void CGraphDoc::OnUpdateRenameColumn(CCmdUI* pCmdUI) 
{
   if (m_meta_graph && !m_communicator && m_meta_graph->viewingProcessed()) {
      int col = m_meta_graph->getDisplayedAttribute();
      // note: -1 is reference number column (cannot be renamed), -2 is displaying nothing (shouldn't be happening but is)
      if (col == -1 || col == -2 || m_meta_graph->isAttributeLocked(col)) {
         pCmdUI->Enable(FALSE);
      }
      else {
         pCmdUI->Enable(TRUE);
      }
   }
   else {
      pCmdUI->Enable(FALSE);
   }
}

void CGraphDoc::OnUpdateUpdateColumn(CCmdUI* pCmdUI) 
{
   if (m_meta_graph && !m_communicator && m_meta_graph->viewingProcessed()) {
      int col = m_meta_graph->getDisplayedAttribute();
      // note: -1 is reference number column (cannot be replaced), -2 is not displaying anything (shouldn't be happening but is)
      if (col == -1 || col == -2 || m_meta_graph->isAttributeLocked(col)) {
         pCmdUI->Enable(FALSE);
      }
      else {
         pCmdUI->Enable(TRUE);
      }
   }
   else {
      pCmdUI->Enable(FALSE);
   }
}

void CGraphDoc::OnUpdateColumnProperties(CCmdUI* pCmdUI)
{
   if (m_meta_graph && !m_communicator && m_meta_graph->viewingProcessed()) {
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE);
   }
}

// not strictly column related, but code is almost identical to the UpdateColumn
void CGraphDoc::OnUpdateEditQuery(CCmdUI* pCmdUI) 
{
   if (m_meta_graph && !m_communicator && m_meta_graph->viewingProcessed()) {
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE);
   }
}

void CGraphDoc::OnUpdateRemoveColumn(CCmdUI* pCmdUI) 
{
   if (m_meta_graph && !m_communicator && m_meta_graph->viewingProcessed()) {
      int col = m_meta_graph->getDisplayedAttribute();
      // note: -1 is reference number column (cannot be removed), -2 is nothing displayed (shouldn't be happening but is)
      if (col == -1 || col == -2 || m_meta_graph->isAttributeLocked(col)) {
         pCmdUI->Enable(FALSE);
      }
      else {
         pCmdUI->Enable(TRUE);
      }
   }
   else {
      pCmdUI->Enable(FALSE);
   }
}

////////////////////////////////////////////////////////////////////////////////

void CGraphDoc::OnFileProperties() 
{
   if (!m_meta_graph) {
      AfxMessageBox(_T("No graph"));
   }
   else {
      CFilePropertiesDlg dlg;
      // editables
	   dlg.m_title = m_meta_graph->getTitle().c_str();
	   dlg.m_location = m_meta_graph->getLocation().c_str();
	   dlg.m_description = m_meta_graph->getDescription().c_str();
      // non-editables
	   dlg.m_author = m_meta_graph->getPerson().c_str();
	   dlg.m_create_date = m_meta_graph->getDate().c_str();
	   dlg.m_create_program = m_meta_graph->getProgram().c_str();
	   dlg.m_organization = m_meta_graph->getOrganization().c_str();
      if (m_meta_graph && m_meta_graph->getVersion() != -1) {
         CString info;
         info.Format(_T("%d"), m_meta_graph->getVersion());
         dlg.m_file_version = info;
      }
      else {
         dlg.m_file_version = _T("<Unsaved>");
      }
      if (IDOK == dlg.DoModal()) {
         m_meta_graph->setTitle(pstring(dlg.m_title));
         m_meta_graph->setLocation(pstring(dlg.m_location));
         m_meta_graph->setDescription(pstring(dlg.m_description));
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

// a bit of testing

void CGraphDoc::OnMagiMif() 
{
   if (m_meta_graph && !m_communicator) {
      if (m_meta_graph->getState() & MetaGraph::SHAPEGRAPHS) {

         CFilePath path(GetPathName());
         CString defaultname = path.m_path + (path.m_name.IsEmpty() ? GetTitle() : path.m_name) + _T("_polys");

         CFileDialog saveDialog(FALSE,_T("mif"),defaultname,
            OFN_EXPLORER|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("MapInfo file (*.mif)|*.mif||"));
         saveDialog.m_ofn.lpstrTitle = _T("Save Output As");
         
         if (saveDialog.DoModal() == IDOK) {
            CWaitCursor;
            
            CString outfile = saveDialog.GetPathName();
            CString ext = saveDialog.GetFileExt();
            ext.MakeUpper();
            
            if (ext == CString("MIF")) 
            {
               int thedot = outfile.ReverseFind('.');
               CString outfile2 = outfile.Left(thedot+1) + _T("mid");
               
               ofstream miffile(outfile);
               if (miffile.fail() || miffile.bad()) {
                  AfxMessageBox(_T("Sorry, unable to open file for export"));
               }
               
               ofstream midfile(outfile2);
               if (midfile.fail() || midfile.bad()) {
                  AfxMessageBox(_T("Sorry, unable to open associated .mid file for export"));
               }
               
               m_meta_graph->getDisplayedShapeGraph().outputMifPolygons(miffile,midfile);
            }
         }
      }
   }
}

void CGraphDoc::OnUpdateMagiMif(CCmdUI* pCmdUI) 
{
   if (m_meta_graph && !m_communicator && (m_meta_graph->getState() & MetaGraph::SHAPEGRAPHS) && (m_meta_graph->getViewClass() & MetaGraph::VIEWAXIAL)) {
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE);
   }
}

void CGraphDoc::OnBinDistances() 
{
   m_meta_graph->getDisplayedPointMap().binMap(NULL);
}

void CGraphDoc::OnShowBinDistances() 
{
   pvecint a = m_meta_graph->getDisplayedPointMap().getSelSet();
   Point& p = m_meta_graph->getDisplayedPointMap().getPoint(a.head());
   CString all;
   for (int i = 0; i < 32; i++) {
      CString blah;
      blah.Format(_T("%2d: %f\n"),i,p.getBinDistance(i));
      all += blah;
   }
   AfxMessageBox(all);
}

void CGraphDoc::OnViewShowGrid() 
{
   if (m_meta_graph->m_showgrid) {
      m_meta_graph->m_showgrid = false;
   }
   else {
      m_meta_graph->m_showgrid = true;
   }
   SetRedrawFlag(VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_DEPTHMAPVIEW_SETUP);
}

void CGraphDoc::OnUpdateViewShowGrid(CCmdUI* pCmdUI) 
{
   if (m_meta_graph->m_showgrid) {
      pCmdUI->SetCheck(1);
   }
   else {
      pCmdUI->SetCheck(0);
   }
}

void CGraphDoc::OnViewShowText() 
{
   if (m_meta_graph->m_showtext) {
      m_meta_graph->m_showtext = false;
   }
   else {
      m_meta_graph->m_showtext = true;
   }
   SetRedrawFlag(VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_DEPTHMAPVIEW_SETUP);
}

void CGraphDoc::OnUpdateViewShowText(CCmdUI* pCmdUI) 
{
   if (m_meta_graph && m_meta_graph->m_showtext) {
      pCmdUI->SetCheck(1);
   }
   else {
      pCmdUI->SetCheck(0);
   }
}

#include "AttributeSummary.h"

void CGraphDoc::OnViewSummary() 
{
   CAttributeSummary dlg(this);
   dlg.DoModal();
}

void CGraphDoc::OnUpdateViewSummary(CCmdUI* pCmdUI) 
{
   if (!m_communicator && m_meta_graph && !m_meta_graph->viewingNone()) {
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE);
   }
}

////////////////////////////////////////////////////////////////////////////////////////////

// Red button tests the rapid agent program:

void CGraphDoc::OnRedButton() 
{
   AfxMessageBox(_T("Disabled: use main through vision calculator (with vga analysis) instead"));
   // m_meta_graph->analyseThruVision();
   //
   SetUpdateFlag(CGraphDoc::NEW_DATA);
   SetRedrawFlag(VIEW_ALL, CGraphDoc::REDRAW_GRAPH, CGraphDoc::NEW_DATA );
}

void CGraphDoc::OnUpdateRedButton(CCmdUI* pCmdUI) 
{
   if (m_meta_graph->viewingProcessedPoints()) {
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE); 
   }	
}

void CGraphDoc::OnToolsPointConvShapeMap() 
{
   CWaitCursor wait;
   m_meta_graph->getDisplayedPointMap().mergeFromShapeMap(m_meta_graph->getDisplayedDataMap());
   m_meta_graph->setViewClass(MetaGraph::SHOWVGATOP);
   SetUpdateFlag(CGraphDoc::NEW_TABLE);
   SetRedrawFlag(VIEW_ALL,REDRAW_GRAPH, NEW_DATA);
}

void CGraphDoc::OnUpdateToolsPointConvShapeMap(CCmdUI* pCmdUI) 
{
   if ( !m_communicator && m_meta_graph &&
         m_meta_graph->viewingProcessedShapes() && 
        (m_meta_graph->getState() & MetaGraph::POINTMAPS) && 
         m_meta_graph->getDisplayedPointMap().isProcessed()) {
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE); 
   }	
}

// this is unlink from a set of points!
void CGraphDoc::OnToolsAxialConvShapeMap() 
{
   if (m_meta_graph->getDataMaps().getMapCount() == 0) {
      AfxMessageBox(_T("No data source layers for unlink points"));
      return;
   }

   pvecstring names;
   for (size_t i = 0; i < m_meta_graph->getDataMaps().getMapCount(); i++) {
      names.push_back(pstring("Data Maps: ") + m_meta_graph->getDataMaps().getMap(i).getName());
   }

   // choose shape map...
   CLayerChooserDlg dlg(names);
   dlg.m_text = "Please select source layer for unlink points";

   if (dlg.DoModal()) {
      CWaitCursor wait;
      m_meta_graph->getDisplayedShapeGraph().unlinkFromShapeMap(m_meta_graph->getDataMaps().getMap(dlg.m_layer));
      m_meta_graph->setViewClass(MetaGraph::SHOWAXIALTOP);
      SetUpdateFlag(CGraphDoc::NEW_TABLE);
      SetRedrawFlag(VIEW_ALL,REDRAW_GRAPH, NEW_DATA);
   }
}

void CGraphDoc::OnUpdateToolsAxialConvShapeMap(CCmdUI* pCmdUI) 
{
   if ( !m_communicator && m_meta_graph &&
         m_meta_graph->viewingProcessedLines() &&
         m_meta_graph->getDisplayedShapeGraph().getMapType() == ShapeMap::AXIALMAP) {
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE); 
   }	
}

void CGraphDoc::OnToolsLineLoadUnlinks() 
{
   // Get file from user 
   CFileDialog openDialog(TRUE,NULL,NULL,
						  OFN_EXPLORER|OFN_HIDEREADONLY,
						  _T("Text files (*.txt)|*.txt|All files (*.*)|*.*||"),
						  NULL);
   openDialog.m_ofn.lpstrTitle = _T("Import Unlink IDs");
   if (openDialog.DoModal() != IDOK) {
      return;
   }  
   pstring filename = openDialog.GetPathName();
   if (filename.empty()) {
      return;
   }
   ifstream stream(filename.c_str());
   if (stream.fail()) {
      AfxMessageBox(_T("There was an error opening the file.\n")
                    _T("Please check the file is not already open"));
      return;
   }
   // Check to see if they want to use another key column for the unlink:
   CAttributeChooserDlg dlg(m_meta_graph->getDisplayedShapeGraph().getAttributeTable());
   dlg.m_text = _T("Please select the attribute the file uses as the key for the unlinks");
   if (dlg.DoModal() == IDOK) {
      // Run the process
      CWaitCursor wait;
      if (!m_meta_graph->getDisplayedShapeGraph().unlinkShapeSet(stream,dlg.m_attribute)) {
         AfxMessageBox(_T("There was an error reading the unlink file.\n")
                       _T("Depthmap is expecting a tab delimited set of unlink IDs"));
         return;
      }
      m_meta_graph->setViewClass(MetaGraph::SHOWAXIALTOP);
      SetUpdateFlag(CGraphDoc::NEW_DATA);
      SetRedrawFlag(VIEW_ALL,REDRAW_GRAPH,NEW_DATA);
   }
}

void CGraphDoc::OnUpdateToolsLineLoadUnlinks(CCmdUI* pCmdUI) 
{
   if ( !m_communicator && m_meta_graph &&
         m_meta_graph->viewingProcessedLines() && 
         !m_meta_graph->getDisplayedShapeGraph().isSegmentMap()) {
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE); 
   }	
}

void CGraphDoc::OnViewScatterplot() 
{
   if (!m_view[CGraphDoc::VIEW_SCATTER]) {
      CFrameWnd* wnd = GetApp()->m_pScatterTemplate->CreateNewFrame(this,NULL);
      GetApp()->m_pScatterTemplate->InitialUpdateFrame(wnd,this,TRUE);
   }
   else {
      m_view[CGraphDoc::VIEW_SCATTER]->GetParent()->PostMessage(WM_CLOSE);
   }
}

void CGraphDoc::OnUpdateViewScatterplot(CCmdUI* pCmdUI) 
{
   if (m_meta_graph && m_meta_graph->viewingProcessed()) {
      pCmdUI->Enable(TRUE);
      if (m_view[CGraphDoc::VIEW_SCATTER]) {
         pCmdUI->SetCheck(1);
      }
      else {
         pCmdUI->SetCheck(0);
      }
   }
   else {
      pCmdUI->Enable(FALSE);
   }
}

void CGraphDoc::OnViewTable() 
{
   if (!m_view[CGraphDoc::VIEW_TABLE]) {
      CFrameWnd* wnd = GetApp()->m_pTableTemplate->CreateNewFrame(this,NULL);
      GetApp()->m_pTableTemplate->InitialUpdateFrame(wnd,this,TRUE);
   }
   else {
      m_view[CGraphDoc::VIEW_TABLE]->GetParent()->PostMessage(WM_CLOSE);
   }
}

void CGraphDoc::OnUpdateViewTable(CCmdUI* pCmdUI) 
{
   if (m_meta_graph && m_meta_graph->viewingProcessed()) {
      pCmdUI->Enable(TRUE);
      if (m_view[CGraphDoc::VIEW_TABLE]) {
         pCmdUI->SetCheck(1);
      }
      else {
         pCmdUI->SetCheck(0);
      }
   }
   else {
      pCmdUI->Enable(FALSE);
   }
}


void CGraphDoc::OnWindowMap() 
{
   if (!m_view[CGraphDoc::VIEW_MAP]) {
      CFrameWnd* wnd = GetApp()->m_pMapTemplate->CreateNewFrame(this,NULL);
      GetApp()->m_pMapTemplate->InitialUpdateFrame(wnd,this,TRUE);
   }
   else {
      m_view[CGraphDoc::VIEW_MAP]->GetParent()->PostMessage(WM_CLOSE);
   }
}

void CGraphDoc::OnUpdateWindowMap(CCmdUI* pCmdUI) 
{
   if (m_view[CGraphDoc::VIEW_MAP]) {
      pCmdUI->SetCheck(1);
   }
   else {
      pCmdUI->SetCheck(0);
   }
}

void CGraphDoc::OnWindow3dView() 
{
   if (!m_view[CGraphDoc::VIEW_3D]) {
      CFrameWnd* wnd = GetApp()->m_p3dViewTemplate->CreateNewFrame(this,NULL);
      GetApp()->m_pMapTemplate->InitialUpdateFrame(wnd,this,TRUE);
   }
   else {
      m_view[CGraphDoc::VIEW_3D]->GetParent()->PostMessage(WM_CLOSE);
   }
}

void CGraphDoc::OnUpdateWindow3dView(CCmdUI* pCmdUI) 
{
   if (m_view[CGraphDoc::VIEW_3D]) {
      pCmdUI->SetCheck(1);
   }
   else {
      pCmdUI->SetCheck(0);
   }
}

void CGraphDoc::OnConvertMapShapes()
{
   if (m_meta_graph && m_meta_graph->viewingShapes()) {
      CConvertShapesDlg dlg;
      if (IDOK == dlg.DoModal()) {
         int viewclass = m_meta_graph->getViewClass();
         if (viewclass & MetaGraph::VIEWDATA) {
            m_meta_graph->getDisplayedDataMap().convertPointsToPolys(dlg.m_radius,dlg.m_selected_only);
         }
         else if (viewclass & MetaGraph::VIEWAXIAL) {
            m_meta_graph->getDisplayedDataMap().convertPointsToPolys(dlg.m_radius,dlg.m_selected_only);
         }
         else {
            AfxMessageBox(_T("Cannot perform conversion on this map type"));
         }
      }
   }
}

void CGraphDoc::OnUpdateConvertMapShapes(CCmdUI *pCmdUI)
{
   if (m_meta_graph && m_meta_graph->viewingShapes()) {
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE);
   }
}

