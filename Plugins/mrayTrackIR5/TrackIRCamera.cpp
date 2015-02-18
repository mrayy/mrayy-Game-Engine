
#include "Stdafx.h"
#include "TrackIR.h"
#include "TrackIRCamera.h"

namespace mray
{

TrackIRCamera::TrackIRCamera()
{
	m_trackIR=new TrackIR();
}

TrackIRCamera::~TrackIRCamera()
{
	delete m_trackIR;
}


bool TrackIRCamera::Init(ushort programID)
{
	return m_trackIR->Init(programID)==NP_OK;
}

bool TrackIRCamera::Shutdown()
{
	return m_trackIR->Shutdown();
}


bool TrackIRCamera::IsInited()
{
	return m_trackIR->IsInited();
}


bool TrackIRCamera::ReceiveData(math::vector3d& pos,math::vector3d&rot)
{
	LPTRACKIRDATA data;
	if(!m_trackIR->HandleTrackIRData(data)==NP_OK)
		return false;

	pos.x=data.fNPX;
	pos.y=data.fNPY;
	pos.z=data.fNPZ;

	rot.x=data.fNPPitch;
	rot.y=data.fNPYaw;
	rot.z=-data.fNPRoll;

	return true;
}


}
