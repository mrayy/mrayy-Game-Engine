

/********************************************************************
	created:	2013/09/09
	created:	9:9:2013   2:00
	filename: 	C:\Development\mrayEngine\Projects\TouchShadow\AudioWaveSet.h
	file path:	C:\Development\mrayEngine\Projects\TouchShadow
	file base:	AudioWaveSet
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __AudioWaveSet__
#define __AudioWaveSet__



#include "OutputChannel.h"

namespace mray
{
	class CAudioWave
	{
	public:
		core::string name;
		core::string waveFile;

		FramesList waveFrames;

		void LoadFromXML(xml::XMLElement* e);
	};
class AudioWaveSet
{
protected:
	std::map<core::string,CAudioWave> m_audioMap;
public:
	AudioWaveSet();
	virtual~AudioWaveSet();

	CAudioWave* GetWave(const core::string& name);
	void LoadFromXML(xml::XMLElement* e);
};

}


#endif
