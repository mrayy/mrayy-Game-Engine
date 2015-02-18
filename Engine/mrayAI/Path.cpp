#include "stdafx.h"

#include "Path.h"

namespace mray{
namespace AI{

Path::Path()
{

}
Path::Path(const PathEdgeList& path)
{
	SetPath(path);
}
Path::Path(const Path& o)
{
	SetPath(o.GetPath());
}
Path::~Path()
{
}

void Path::SetPath(const PathEdgeList& path)
{
	Clear();
	PathEdgeList::const_iterator it=path.begin();
	PathEdgeList::const_iterator end=path.end();
	for (;it!=end;++it)
	{
		m_path.push_back(*it);
	}
}
void Path::Clear()
{
	m_path.clear();
}

PathEdgeList& Path::GetPath()
{
	return m_path;
}

const PathEdgeList& Path::GetPath()const
{
	return m_path;
}

float Path::CalculateLength()
{
	float len=0;
	PathEdgeList::const_iterator it=m_path.begin();
	PathEdgeList::const_iterator end=m_path.end();
	for (;it!=end;++it)
	{
		len+=(*it).GetLength();
	}
	return len;
}

}
}