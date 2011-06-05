// DepthmapAtom.h: interface for the CDepthmapAtom class.
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

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEPTHMAPATOM_H__41857492_D1BD_4D26_8A6A_03571A37A549__INCLUDED_)
#define AFX_DEPTHMAPATOM_H__41857492_D1BD_4D26_8A6A_03571A37A549__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct CDepthmapAtomEntry
{
   time_t m_updated;
   time_t m_published;
   CString m_id;
   CString m_title;
   pqvector<CString> m_summary;
   //
   CDepthmapAtomEntry()
   { m_updated = 0; m_published = 0; }
   void clear()
   { m_updated = 0; m_published = 0; m_id.Empty(); m_title.Empty(); m_summary.clear(); }
   //
   friend bool operator < (const CDepthmapAtomEntry& a, const CDepthmapAtomEntry& b);
   friend bool operator > (const CDepthmapAtomEntry& a, const CDepthmapAtomEntry& b);
   friend bool operator == (const CDepthmapAtomEntry& a, const CDepthmapAtomEntry& b);
};
inline bool operator < (const CDepthmapAtomEntry& a, const CDepthmapAtomEntry& b)
{ return a.m_updated < b.m_updated; }
inline bool operator > (const CDepthmapAtomEntry& a, const CDepthmapAtomEntry& b)
{ return a.m_updated > b.m_updated; }
inline bool operator == (const CDepthmapAtomEntry& a, const CDepthmapAtomEntry& b)
{ return a.m_updated == b.m_updated; }

class CDepthmapAtom  
{
protected:
   pqvector<CDepthmapAtomEntry> m_entries;
public:
	CDepthmapAtom();
	virtual ~CDepthmapAtom();
   //
   void Parse(const CString& path);
   int GetFirstUnread(const pqvector<CString>& readlist, const CString& earliestdate);
   //
   const CDepthmapAtomEntry& GetEntry(int n) const
   { return m_entries[n]; }
   CDepthmapAtomEntry& GetEntry(int n)
   { return m_entries[n]; }
};

#endif // !defined(AFX_DEPTHMAPATOM_H__41857492_D1BD_4D26_8A6A_03571A37A549__INCLUDED_)
