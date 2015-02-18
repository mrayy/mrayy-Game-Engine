
#include "stdafx.h"
#include "WinMonitorFileInformation.h"

namespace mray
{
namespace OS
{

WinMonitorFileInformation::WinMonitorFileInformation(WIN32_FIND_DATA& fd, const core::string &dir)
{
	m_dir = dir;
	memcpy(&m_fd, &fd, sizeof(fd));
}
WinMonitorFileInformation::~WinMonitorFileInformation()
{
}

core::string WinMonitorFileInformation::GetDirectory()const
{
	return m_dir;
}
core::string WinMonitorFileInformation::GetPath()const
{
	return ConcPath(m_dir, m_fd.cFileName);
}
core::string WinMonitorFileInformation::GetFileName()const
{
	return m_fd.cFileName;
}
int WinMonitorFileInformation::GetFileSize()const
{
	return m_fd.nFileSizeLow;
}
bool WinMonitorFileInformation::IsFileExist()const
{
	core::string path = GetPath();
	if (path == "")
		return false;
	FILE* f = fopen(path.c_str(), "r");
	if (!f)
		return false;
	fclose(f);
	return true;
}
bool WinMonitorFileInformation::IsSystem()const
{
	return m_fd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM;
}
bool WinMonitorFileInformation::IsReadOnly()const
{
	return m_fd.dwFileAttributes & FILE_ATTRIBUTE_READONLY;

}
bool WinMonitorFileInformation::IsHidden()const
{
	return m_fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN;
}
bool WinMonitorFileInformation::IsNormal()const
{
	return m_fd.dwFileAttributes & FILE_ATTRIBUTE_NORMAL;
}
bool WinMonitorFileInformation::IsArchive()const
{
	return m_fd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE;
}
bool WinMonitorFileInformation::IsDirectory()const
{
	return m_fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
}
bool WinMonitorFileInformation::IsCurrentRoot()const
{
	return GetFileName() == ".";
}

bool WinMonitorFileInformation::IsRoot()const
{
	return IsCurrentRoot() || IsParentDirectory();
}


bool WinMonitorFileInformation::IsParentDirectory()const
{
	return GetFileName() == "..";

}
bool WinMonitorFileInformation::IsTemporary()const
{
	return m_fd.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY;
}
bool WinMonitorFileInformation::IsActualFile()const
{
	return !IsDirectory()
		&& !IsCurrentRoot()
		&& !IsParentDirectory()
		&& !IsHidden()
		&& !IsSystem();
}
bool WinMonitorFileInformation::IsOk()const
{
	return GetFileName() != "";
}

FILETIME WinMonitorFileInformation::GetLastWriteTime()const
{
	return m_fd.ftLastWriteTime;
}


bool WinMonitorFileInformation::equlas(const WinMonitorFileInformation* o)const
{
	return GetPath() == o->GetPath()
		&& GetFileSize() == o->GetFileSize()
		&& m_fd.dwFileAttributes == o->m_fd.dwFileAttributes
		&& GetLastWriteTime().dwHighDateTime == o->GetLastWriteTime().dwHighDateTime
		&& GetLastWriteTime().dwLowDateTime == o->GetLastWriteTime().dwLowDateTime;
}

core::string WinMonitorFileInformation::ConcPath(const core::string& root, const core::string& sub)
{
	if (!root.length() || root[root.length() - 1] == '\\')
		return root + sub;
	
	return root + "\\" + sub;
}

int WinMonitorFileInformation::EnumFiles(const core::string& root, MonitorFileList &lst)
{
	WIN32_FIND_DATA ffd;
	core::string path = ConcPath(root, mT("*.*"));
	HANDLE          sh = FindFirstFile(path.c_str(), &ffd);

	if (INVALID_HANDLE_VALUE == sh)
		return 0;

	do
	{
		WinMonitorFileInformation* pFDI = new WinMonitorFileInformation(ffd, root);

		if (pFDI->IsRoot())
		{
			delete pFDI;
		}
		else if (pFDI->IsDirectory())
		{
			lst.push_back(pFDI);
			EnumFiles(ConcPath(root, pFDI->GetPath()), lst);
		}
		else if (pFDI->IsActualFile())
		{
			lst.push_back(pFDI);
		}
		else
		{
			delete pFDI;
		}
	} while (FindNextFile(sh, &ffd));

	FindClose(sh);

	return lst.size();
}

void WinMonitorFileInformation::ClearFiles(MonitorFileList &lst)
{
	MonitorFileList::iterator it = lst.begin();
	for (; it != lst.end();++it)
	{
		delete *it;
	}
	lst.clear();
}

EMonitorAction WinMonitorFileInformation::Compare(const MonitorFileList &oldLst, const MonitorFileList &newLst, IMonitorFileInformation*& f)
{
	int nOld = oldLst.size();
	int nNew = newLst.size();

	MonitorFileList::const_iterator it1;
	MonitorFileList::const_iterator it2;
	EMonitorAction ret = EMonitorAction::None;;
	if (nOld == nNew)
	{
		it1 = oldLst.begin();
		it2 = newLst.begin();
		while (it1!=oldLst.end() && it2!=newLst.end())
		{
			WinMonitorFileInformation* a = dynamic_cast<WinMonitorFileInformation*>(*it1);
			WinMonitorFileInformation* b = dynamic_cast<WinMonitorFileInformation*>(*it2);
			if (!a->equlas(b))
			{
				f = b;
				ret = EMonitorAction::Change;
				break;
			}
			it1++;
			it2++;
		}
	}
	else if (nOld > nNew)
	{
		it1 = oldLst.begin();
		while (it1 != oldLst.end())
		{
			WinMonitorFileInformation* a = dynamic_cast<WinMonitorFileInformation*>(*it1);
			it2 = newLst.begin();
			bool found = true;

			while (it2 != newLst.end())
			{
				WinMonitorFileInformation* b = dynamic_cast<WinMonitorFileInformation*>(*it2);
				if (a->equlas(b))
				{
					found = false;
					break;;
				}
				++it2;
			}
			if (found)
			{
				f = a;
				ret = EMonitorAction::Delete;
				break;;
			}
			++it1;
		}
	}
	else 
	{
		it2 = newLst.begin();
		while (it2 != newLst.end())
		{
			WinMonitorFileInformation* b = dynamic_cast<WinMonitorFileInformation*>(*it2);
			it1 = oldLst.begin();
			bool found = true;

			while (it1 != oldLst.end())
			{
				WinMonitorFileInformation* a = dynamic_cast<WinMonitorFileInformation*>(*it1);
				if (a->equlas(b))
				{
					found = false;
					break;;
				}
				++it1;
			}
			if (found)
			{
				f = b;
				ret = EMonitorAction::Create;
				break;;
			}
			++it2;
		}
	}
	return ret;
}

}
}

