

/********************************************************************
	created:	2012/07/25
	created:	25:7:2012   15:43
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\CommunicationDataRecorder.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	CommunicationDataRecorder
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___CommunicationDataRecorder___
#define ___CommunicationDataRecorder___


#include "ICommunicationLayer.h"
#include "CommunicationData.h"

namespace mray
{
namespace VT
{

class CommunicationDataRecorder:public ICommunicationLayerListener,public ICommunicationLayer
{
public:
	enum EMode
	{
		None,
		Recording,
		Playback
	};
protected:

	std::vector<CommunicationData> m_commData;
	EMode m_mode;
	float m_speed;

	float m_currentFrame;
	GCPtr<OS::IStream> m_saveStream;

	std::vector<core::string> m_scheme;
public:
	CommunicationDataRecorder();
	virtual~CommunicationDataRecorder();

	virtual void OnDataArrived(ICommunicationLayer* layer,const CommunicationData*d);
	
	void SetRecording(const core::string& path);
	void SetPlayback(const core::string& path);

	virtual void Start(){}
	virtual void Close(){}

	void Update(float dt);

	bool IsRecording(){return m_mode==Recording;}
	virtual const std::vector<core::string>& GetScheme(){return m_scheme;}

};

}
}

#endif
