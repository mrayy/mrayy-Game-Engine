

#include "stdafx.h"
#include "AppData.h"




namespace mray
{
namespace kmd
{


	AppData::AppData()
	{
		Debugging = false;
		sessions = 0;

#if USE_LEAP
		leapDevice=0;
#endif
	}
	AppData::~AppData()
	{
	}


	void AppData::SetValue(const core::string&catagory, const core::string&name, const core::string& v)
	{
		s_values.setPropertie(catagory, name, v);
	}

	core::string AppData::GetValue(const core::string&catagory, const core::string&name, const core::string &defaultVal)
	{
		return s_values.getPropertie(catagory, name,defaultVal);
	}
	void AppData::Load(const core::string& path)
	{
		OS::IStreamPtr stream = gFileSystem.openFile(path, OS::BIN_READ);
		if (!stream)
			return;
		s_values.loadSettings(stream);
		stream->close();

	}
	void AppData::Save(const core::string& path)
	{
		OS::IStreamPtr stream = gFileSystem.openFile(path, OS::BIN_WRITE);
		if (!stream)
			return;
		s_values.writeSettings(stream);
		stream->close();
	}

}
}
