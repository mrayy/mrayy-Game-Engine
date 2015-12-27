

#ifndef GstStreamBin_h__
#define GstStreamBin_h__


#include "IGStreamerStreamer.h"
#include "GCPtr.h"
#include <string>
#include <map>

namespace mray
{
namespace video
{

class GstStreamBin
{
protected:
	typedef std::map<std::string, GCPtr<IGStreamerStreamer>> StreamMap;
	StreamMap m_Streamers;
public:
	GstStreamBin();
	virtual ~GstStreamBin();

	void AddStream(IGStreamerStreamer* Streamer, const std::string& name);
	IGStreamerStreamer* GetStream(const std::string& name);

	void Stream();
	void Stop();
	void CloseAll();

	void StartStream(const std::string& name);
	void StopStream(const std::string& name);
	IGStreamerStreamer* RemoveStream(const std::string& name, bool close);
	void ClearStreams(bool stop);
};

}
}


#endif // GstStreamBin_h__
