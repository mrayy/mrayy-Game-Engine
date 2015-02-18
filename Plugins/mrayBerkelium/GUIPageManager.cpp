
#include "stdafx.h"
#include "GUIPageManager.h"
#include "BrowserWindow.h"
#include "IFileSystem.h"

#include <berkelium/Context.hpp>

namespace mray
{
namespace GUI
{
class GUIPageManagerImpl:public Berkelium::ErrorDelegate
{
protected:
	Berkelium::Context* m_context;
	typedef std::list<GCPtr<GUIWebPageControl>> PageList;
	PageList m_pages;
public:
	GUIPageManagerImpl()
	{
		core::string dir;
		gFileSystem.getTempDirectory(dir);
		core::stringw path=core::string_to_wchar(dir+".\\berk");
		Berkelium::FileString str;
		str=Berkelium::FileString::point_to(path.c_str(),path.length());
		Berkelium::init(str);
		Berkelium::setErrorHandler(this);
		m_context=Berkelium::Context::create();
	}
	~GUIPageManagerImpl()
	{
		ClearPages();
		Berkelium::destroy();
	}

	GCPtr<GUIWebPageControl> CreateWebPage(IGUIManager* creator)
	{
		GCPtr<GUIWebPageControl> ret=new GUIWebPageControl(creator,(void*)m_context);
		m_pages.push_back(ret);
		return ret;
	}


	void RemovePage(GUIWebPageControl*p)
	{
		PageList::iterator it=m_pages.begin();
		for (;it!=m_pages.end();++it)
		{
			if((*it).pointer()==p)
			{
				m_pages.erase(it);
				break;
			}
		}
	}

	void ClearPages()
	{
		m_pages.clear();
	}
};

GUIPageManager::GUIPageManager()
{
	m_impl=new GUIPageManagerImpl();
}

GUIPageManager::~GUIPageManager()
{
	delete m_impl;
}


GCPtr<GUIWebPageControl> GUIPageManager::CreateWebPage(IGUIManager* creator)
{
	return m_impl->CreateWebPage(creator);
}


void GUIPageManager::RemovePage(GUIWebPageControl*p)
{
	m_impl->RemovePage(p);
}

void GUIPageManager::ClearPages()
{
	m_impl->ClearPages();
}

void GUIPageManager::Update()
{
	Berkelium::update();
}

}
}

