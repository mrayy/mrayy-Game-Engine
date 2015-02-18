

/********************************************************************
	created:	2012/11/18
	created:	18:11:2012   0:47
	filename: 	D:\Development\mrayEngine\Plugins\mraySound\DirectSoundCommon.h
	file path:	D:\Development\mrayEngine\Plugins\mraySound
	file base:	DirectSoundCommon
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __DirectSoundCommon__
#define __DirectSoundCommon__




namespace mray
{
namespace sound
{

class DirectSoundCommon
{
protected:
public:
	bool m_Inited;
	int m_refCount;
public:
	DirectSoundCommon();
	virtual~DirectSoundCommon();


	void Init();
	void Shutdown();
	bool IsInited();

	void AddRef();
	void SubRef();


	static DirectSoundCommon Instance;
};

}
}

#endif
