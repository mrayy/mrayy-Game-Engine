

/********************************************************************
	created:	2013/09/02
	created:	2:9:2013   23:13
	filename: 	C:\Development\mrayEngine\Projects\TouchShadow\AudioSoundInterface.h
	file path:	C:\Development\mrayEngine\Projects\TouchShadow
	file base:	AudioSoundInterface
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __AudioSoundInterface__
#define __AudioSoundInterface__


#include <DirectSoundOutputStream.h>


namespace mray
{
	class OutputChannel;

	class AudioSoundInterfaceListener;

	struct AudioSoundInterfaceDesc
	{
		bool stereo;
		uint bps;
		uint samplesPerSec;
		bool isFloat;

		static stk::Stk::StkFormat ToStkFormat(uint bps,bool isFloat)
		{
			if(isFloat)
				return stk::Stk::STK_FLOAT32;
			if(bps==8) return stk::Stk::STK_SINT8;
			if(bps==16) return stk::Stk::STK_SINT16;
			if(bps==24) return stk::Stk::STK_SINT24;
			if(bps==32) return stk::Stk::STK_SINT32;
			return stk::Stk::STK_SINT16;
		}
	};
class AudioSoundInterface
{
protected:
	AudioSoundInterfaceListener* m_dsListener;
	sound::OutputStreamDeviceInfo m_streamInfo;
	sound::DirectSoundOutputStream* m_outputStream;


	AudioSoundInterfaceDesc m_desc;

	OutputChannel *m_outChannels[2];//left and right channels
public:
	AudioSoundInterface();
	virtual~AudioSoundInterface();

	bool OpenStreamDevice(const sound::OutputStreamDeviceInfo& ifo,const AudioSoundInterfaceDesc& desc);
	void Start();
	void Stop();

	OutputChannel* GetLeftChannel(){return m_outChannels[0];}
	OutputChannel* GetRightChannel(){return m_outChannels[1];}

	const AudioSoundInterfaceDesc& GetDesc(){return m_desc;}

	void Update(float dt);
};

class AudioInterfaceManager
{
protected:
	std::vector<AudioSoundInterface*> m_interfaces;

	void Clear();
	AudioSoundInterfaceDesc m_desc;

public:
	AudioInterfaceManager();
	virtual~AudioInterfaceManager();

	void ReloadInterfaces(const AudioSoundInterfaceDesc &desc);

	const AudioSoundInterfaceDesc &GetDesc(){return m_desc;}

	AudioSoundInterface* GetInterface(int index);
	int GetInterfaceCount(){return m_interfaces.size();}
};

}


#endif

