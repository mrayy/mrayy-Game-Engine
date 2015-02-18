

#include "stdafx.h"
#include "InternetCacheManager.h"
#include "IFileSystem.h"
#include "XMLTree.h"
#include "StreamWriter.h"
#include "XMLWriter.h"
#include "INetwork.h"
#include "IWebRequest.h"

namespace mray
{
namespace network
{


InternetCacheManager::InternetCacheManager()
{
	SetCachePath("Cache\\");
}

InternetCacheManager::~InternetCacheManager()
{
	SaveCache();
}
core::string InternetCacheManager::_getPath(const core::string& url)
{
	GUID itemGUID(url);
	core::string p= m_cachePath + core::StringConverter::toString(itemGUID.ID());
	int i=url.findlast('.');
	if (i != -1)
	{
		p+= url.substr(i, url.length() - i);
	}
	return p;
}

void InternetCacheManager::SetCachePath(const core::string& path)
{
	m_cachePath = path;
	gFileSystem.createDirs(m_cachePath);

	xml::XMLTree t;
	if (!t.load(path + "cache.xml"))
		return;

	xml::XMLElement* root= t.getSubElement("Cache");
	if (!root)
		return;
	xml::XMLElement* e = root->getSubElement("Item");
	while (e)
	{
		CachedItem item;
		item.path = e->getValueString("Path");
		item.url = e->getValueString("URL");
		item.size = e->getValueInt("Size");
		GUID itemGUID(item.url);
		m_cachedItems[itemGUID.ID()] = item;
		e = e->nextSiblingElement("Item");
	}

}


OS::IStreamPtr InternetCacheManager::AddCachedItem(OS::IStream* stream)
{
	CachedItem item;
	item.url= stream->getStreamName();
	GUID itemGUID(item.url);
	item.path = _getPath(item.url);
	item.size = stream->length();
	if (item.size == 0)
		return OS::IStreamPtr::Null;
	m_cachedItems[itemGUID.ID()] = item;

	core::string pa= gFileSystem.getAppPath();

	OS::IStreamPtr outS = gFileSystem.createBinaryFileWriter(pa+item.path);
	stream->seek(0, OS::ESeek_Set);
	byte buffer[1024];
	while (!stream->eof())
	{
		int bytes=stream->read(buffer, 1024);
		if (bytes>0)
			outS->write(buffer, bytes);
	}
	outS->seek(0,OS::ESeek_Set);
	stream->seek(0, OS::ESeek_Set);
	outS->close();
	return outS;

}

OS::IStreamPtr InternetCacheManager::LoadCachedItem(const core::string& url)
{
	GUID itemGUID(url);
	CacheMap::iterator it = m_cachedItems.find(itemGUID.ID());
	if (it == m_cachedItems.end())
		return OS::IStreamPtr::Null;
	return gFileSystem.openFile(it->second.path);
}
OS::IStreamPtr InternetCacheManager::GetOrCreateItem(const core::string& url)
{
	OS::IStreamPtr stream= LoadCachedItem(url);
	if (stream == OS::IStreamPtr::Null)
	{
		network::IWebRequestPtr req= network::INetwork::getInstance().CreateWebRequest();
		if (!req->Connect(url))
		{
			return OS::IStreamPtr::Null;
		}
		stream= AddCachedItem(req->GetStream());
		if (stream)
			stream->reopen(OS::BIN_READ);
	}
	return stream;
}


void InternetCacheManager::ClearCache()
{
	CacheMap::iterator it = m_cachedItems.begin();
	for (; it != m_cachedItems.end();++it)
	{
		gFileSystem.deleteFile(it->second.path);
	}
	m_cachedItems.clear();
}

void InternetCacheManager::SaveCache()
{
	xml::XMLWriter w;
	xml::XMLElement* root = new xml::XMLElement("Cache");

	CacheMap::iterator it = m_cachedItems.begin();
	for (; it != m_cachedItems.end(); ++it)
	{
		xml::XMLElement* e = new xml::XMLElement("Item");
		root->addSubElement(e);

		e->addAttribute("Path", it->second.path);
		e->addAttribute("URL", it->second.url);
		e->addAttribute("Size", core::StringConverter::toString(it->second.size));
	}
	w.addElement(root);
	core::string xmlStr= w.flush();
	OS::IStreamPtr stream= gFileSystem.createTextFileWriter(m_cachePath + "cache.xml");
	OS::StreamWriter wrtr(stream);
	wrtr.writeString(xmlStr);
	stream->close();


	delete root;
}


}
}
