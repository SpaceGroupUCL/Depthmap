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

// Build information:

#ifndef __BUILDINFO_H__
#define __BUILDINFO_H__

#define DEPTHMAP_VERSION 10.15          // year . month
#define DEPTHMAP_MINOR_VERSION L"00r"   // day of the month, other info (b Beta version, c is MSVC 8 compiled version, r is standard release, s is stable release)

// You don't want Depthmap to warn you about alerts way in the past,
// so this sets the earliest date Depthmap will pick up an alert from
const pstring g_earliest_alert_date = pstring("2010-11-01T00:00:01");

// version 5.0 dll exports have split out the attribute table
// functionality, so a single class operates with the VGA interface
// and the Axial interface, this means previous DLLs will not work
// with version 5.0 and above
// version 5.09 dll has reconfigured the data layers into shape layers:
// this means previous DLLs will not work with version 5.09 and above
// version 7.08 dll has a very slight change so that the analysis type
// is sent to the attribute table -- if it is vga it will work off pixelrefs,
// otherwise it will work off rowids
// version 7.09 dll has a new version number function so that "idepthmap.h"
// can be included multiple times in user projects
// version 8.00 dll has significant upgrades to the naming conventions,
// the ability to perform whole graph analyses and adding shapes
// version 10.04 dll has further upgrades to naming conventions,
// also, extra functions for the sala.dll build: includes graph opening, closing and so on

#define DEPTHMAP_MODULE_VERSION 10.04

// No longer a manual, but a handbook
#define DEFAULT_MANUAL_LOCATION L"http://www.vr.ucl.ac.uk/depthmap/handbook.html"

// Now tutorials are also available:
#define DEFAULT_TUTORIAL_LOCATION L"http://www.vr.ucl.ac.uk/depthmap/tutorials/"

// As well as a separate manual for scripting
#define DEFAULT_SALA_MANUAL_LOCATION L"http://www.vr.ucl.ac.uk/depthmap/scripting/manual.html"

// ..and alerts:
#define DEFAULT_ALERTS_LOCATION L"http://www.vr.ucl.ac.uk/depthmap/alerts/"
#define DEFAULT_ALERTS_ATOM L"http://www.vr.ucl.ac.uk/depthmap/alerts/atom.xml"

#endif
