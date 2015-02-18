

/********************************************************************
	created:	2012/11/18
	created:	18:11:2012   0:40
	filename: 	D:\Development\mrayEngine\Plugins\mraySound\PortAudioCommon.h
	file path:	D:\Development\mrayEngine\Plugins\mraySound
	file base:	PortAudioCommon
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __PortAudioCommon__
#define __PortAudioCommon__




namespace mray
{
namespace sound
{

class PortAudioCommon
{
protected:

	bool m_Inited;
	int m_refCount;
public:
	PortAudioCommon();
	virtual~PortAudioCommon();


	void Init();
	void Shutdown();
	bool IsInited();

	void AddRef();
	void SubRef();


	static PortAudioCommon Instance;
};

}
}


#endif
