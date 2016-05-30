

#include "stdafx.h"
#include "GstStreamBin.h"


namespace mray
{
namespace video
{


GstStreamBin::GstStreamBin()
{
}
GstStreamBin::~GstStreamBin()
{
	ClearStreams(true);
}

void GstStreamBin::AddStream(IGStreamerStreamer* Stream, const std::string& name)
{
	m_Streamers[name] = Stream;
}
IGStreamerStreamer* GstStreamBin::GetStream(const std::string& name)
{
	StreamMap::iterator it = m_Streamers.find(name);
	if (it == m_Streamers.end())
		return 0;
	return it->second;
}


IGStreamerStreamer* GstStreamBin::GetStreamerAt(int index){
	if (index >= m_Streamers.size())
		return 0;
	StreamMap::iterator it = m_Streamers.begin();
	std::advance(it, index);
	return it->second;
}
void GstStreamBin::Stream()
{
	StreamMap::iterator it = m_Streamers.begin();
	for (; it != m_Streamers.end(); ++it)
	{
		it->second->Stream();
	}

}

void GstStreamBin::Stop()
{
	StreamMap::iterator it = m_Streamers.begin();
	for (; it != m_Streamers.end(); ++it)
	{
		it->second->Stop();
	}

}
void GstStreamBin::CloseAll()
{
	StreamMap::iterator it = m_Streamers.begin();
	for (; it != m_Streamers.end(); ++it)
	{
		it->second->Close();
	}

}
void GstStreamBin::StartStream(const std::string& name)
{
	StreamMap::iterator it = m_Streamers.find(name);
	if (it == m_Streamers.end())
		return;
	it->second->Stream();
}
void GstStreamBin::StopStream(const std::string& name)
{
	StreamMap::iterator it = m_Streamers.find(name);
	if (it == m_Streamers.end())
		return;
	it->second->Stop();
}
IGStreamerStreamer* GstStreamBin::RemoveStream(const std::string& name, bool close)
{
	StreamMap::iterator it = m_Streamers.find(name);
	if (it == m_Streamers.end())
		return 0;
	IGStreamerStreamer* ret = it->second;
	if (close)
		ret->Close();
	m_Streamers.erase(it);
	return ret;
}
void GstStreamBin::ClearStreams(bool stop)
{
	if (stop)
	{
		StreamMap::iterator it = m_Streamers.begin();
		for (; it != m_Streamers.end(); ++it)
		{
			it->second->Close();
		}
	}
	m_Streamers.clear();
}

}
}


