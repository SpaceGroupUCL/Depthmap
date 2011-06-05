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

// DepthmapAtom.cpp: implementation of the CDepthmapAtom class.
//
//////////////////////////////////////////////////////////////////////

// n.b. Unicode conversion (AT 01.02.11) is a pig's breakfast!
// There's pstrings using chars and CStrings using wchar_t...

#include "stdafx.h"
#include "depthmap.h"
#include "DepthmapAtom.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////
// local helper

static time_t ConvertDate(const pstring& date)
{
   tm timestruct;
   timestruct.tm_year = atoi(date.substr(0,4).c_str()) - 1900;
   timestruct.tm_mon = atoi(date.substr(5,2).c_str()) - 1;
   timestruct.tm_mday = atoi(date.substr(8,2).c_str());
   timestruct.tm_hour = atoi(date.substr(11,2).c_str());
   timestruct.tm_min = atoi(date.substr(14,2).c_str());
   timestruct.tm_sec = atoi(date.substr(17,2).c_str());

   // note, locale time correction is lost
   timestruct.tm_isdst = -1;

   return mktime(&timestruct);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDepthmapAtom::CDepthmapAtom()
{

}

CDepthmapAtom::~CDepthmapAtom()
{

}

void CDepthmapAtom::Parse(const CString& path)
{
   ifstream file(path);

   if (file.bad() || file.fail()) {
      AfxMessageBox(_T("Depthmap Alerts have been downloaded, but there is a problem reading them"));
      return;
   }

   bool inentry = false, insummary = false;
   pstring line;
   CDepthmapAtomEntry thisentry;

   while (!file.eof()) {
      file >> line;
      line.ltrim();
      if (!line.empty()) {
         if (!inentry) {
            if (line == "<entry>") {
               inentry = true;
               thisentry.clear();
            }
         }
         else {
            if (line == "</entry>") {
               if (!thisentry.m_id.IsEmpty()) {
                  m_entries.add(thisentry,paftl::ADD_DUPLICATE);
                  thisentry.clear();
               }
               inentry = false;
               insummary = false;
            }
            else if (line == "<summary>") {
               insummary = true;
            }
            else if (insummary) {
               if (line == "</summary>") {
                  insummary = false;
               }
               else {
                  thisentry.m_summary.push_back( CString(line.c_str()) );
               }
            }
            else if (line.substr(0,4) == "<id>") {
               size_t n = line.findindex("</id>",4);
               if (n != paftl::npos) {
                  thisentry.m_id = CString(line.substr(4,n-4).c_str());
               }               
            }
            else if (line.substr(0,7) == "<title>") {
               size_t n = line.findindex("</title>",7);
               if (n != paftl::npos) {
                  thisentry.m_title = CString(line.substr(7,n-7).c_str());
               }               
            }
            else if (line.substr(0,9) == "<updated>") {
               // convert this to a simple number based on yyyy mm dd:
               size_t n = line.findindex("</updated>",9);
               if (n != paftl::npos) {
                  thisentry.m_updated = ConvertDate( line.substr(9,n-9) );
               }
            }
            else if (line.substr(0,11) == "<published>") {
               // convert this to a simple number based on yyyy mm dd:
               size_t n = line.findindex("</published>",11);
               if (n != paftl::npos) {
                  thisentry.m_published = ConvertDate( line.substr(11,n-9) );
               }
            }
         }
      }
   }
}

int CDepthmapAtom::GetFirstUnread(const pqvector<CString>& readlist, const CString& earliestdate)
{
   time_t earliesttime = ConvertDate( pstring(earliestdate) );
   int retvar = -1;
   for (size_t i = 0; i < m_entries.size(); i++) {
      if (m_entries[i].m_updated > earliesttime && readlist.searchindex(m_entries[i].m_id) == paftl::npos) {
         retvar = (int) i;
         break;
      }
   }
   return retvar;
}
