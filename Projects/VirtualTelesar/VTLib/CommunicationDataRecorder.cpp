

#include "stdafx.h"
#include "CommunicationDataRecorder.h"


namespace mray
{
namespace VT
{


CommunicationDataRecorder::CommunicationDataRecorder()
{
	m_currentFrame=0;
	m_speed=1;
	m_mode=None;
}
CommunicationDataRecorder::~CommunicationDataRecorder()
{
	if(m_saveStream)
		m_saveStream->close();
}

void CommunicationDataRecorder::OnDataArrived(ICommunicationLayer* layer,const CommunicationData*d)
{
	if(m_mode!=Recording)
		return;
	//const CommunicationData*data=dynamic_cast<const CommunicationDataF*>(d);

	m_commData.push_back(*d);
	const std::list<CommunicationData::DataPair>& lst = d->GetData();
	std::list<CommunicationData::DataPair>::const_iterator it = lst.begin();
	for (; it != lst.end(); ++it)
	{
		const CommunicationData::DataPair& p = *it;

	}
}

void CommunicationDataRecorder::SetRecording(const core::string& path)
{
	m_mode=Recording;
	m_saveStream=gFileSystem.openFile(path,OS::BIN_WRITE);
}
void CommunicationDataRecorder::SetPlayback(const core::string& path)
{
	if(m_saveStream)
		m_saveStream->close();
	m_mode=Playback;
}

void CommunicationDataRecorder::Update(float dt)
{
	if(m_mode!=Playback)
		return;

	if(m_currentFrame<m_commData.size())
	{
		FIRE_LISTENR_METHOD(OnDataArrived,(this,&m_commData[(int)m_currentFrame]));
	}
	m_currentFrame+=m_speed*dt;
	if(m_currentFrame>m_commData.size())
		m_currentFrame=0;
}


}
}
