

#include "stdafx.h"
#include "AudioWaveSet.h"
#include "DemoSharedData.h"
#include "AudioSoundInterface.h"
#include "WaveFile.h"
#include "AudioConverter.h"

namespace mray
{

	void CAudioWave::LoadFromXML(xml::XMLElement* e)
	{
		name=e->getValueString("name");
		waveFile=e->getValueString("Path");

		CWaveFile fr;

		core::string path;
		gFileSystem.getCorrectFilePath(waveFile,path);
		if(path!="")
		{
			const AudioSoundInterfaceDesc&desc=DemoSharedData::Instance()->AudioManager->GetDesc();
			if(FAILED(fr.Open((LPTSTR)path.c_str(),WAVEFILE_READ)))//,true,1,AudioSoundInterfaceDesc::ToStkFormat(desc.bps,desc.isFloat),desc.samplesPerSec);
			{
				gLogManager.log("Failed to load:"+path,ELL_WARNING);
				return;
			}
			//stk::StkFrames frames;
			waveFrames.resize(fr.GetSize());
			//fr.read(frames);
			//waveFrames.resize(AudioConverter::GetTargetSamplesSize(frames.size(),desc.bps));
			//AudioConverter::ConvertSamples(&frames[0],frames.size(),&waveFrames[0],desc.bps);
			DWORD r;
			fr.Read((uchar*)&waveFrames[0],waveFrames.size(),&r);
		}
	}


	AudioWaveSet::AudioWaveSet()
	{
	}
	AudioWaveSet::~AudioWaveSet()
	{
	}
	CAudioWave* AudioWaveSet::GetWave(const core::string& name)
	{
		std::map<core::string,CAudioWave>::iterator it= m_audioMap.find(name);
		if(it==m_audioMap.end())
			return 0;
		return &it->second;
	}
	void AudioWaveSet::LoadFromXML(xml::XMLElement* e)
	{
		xml::XMLElement* elem=e->getSubElement("AudioFile");
		while(elem)
		{
			CAudioWave c;
			c.LoadFromXML(elem);
			m_audioMap[c.name]=c;
			elem=elem->nextSiblingElement("AudioFile");
		}

	}
}
