

#include "stdafx.h"
#include "TrackController.h"
#include "CommunicationData.h"
#include "Communicator.h"


namespace mray
{

TrackController::TrackController()
{
	m_calib=false;
}

TrackController::~TrackController()
{
}

void TrackController::Init(int id,const core::string& name)
{
	m_id=id;
	m_name=name;

	reset();
}
void TrackController::reset()
{
	Communicator::Instance()->AddSchemeItem(m_name);
	m_calib=false;
}



void TrackController::OnBodyDataReceived(int id,const math::vector3d &pos,const math::quaternion& rot)
{

	math::vector3d newP=pos;
	if(id!=m_id)
		return;

	if(!math::fEqual(math::quaternion(rot).LengthSQ(),1))
		return;


	math::matrix4x4 m;
	rot.toMatrix(m);
	m.setTranslation(pos);
	math::vector3d angles;
	rot.toEulerAngles(angles);

	if(!m_calib)
	{
	//	m_pos=newP;
		/*m_ori=rot;
		m_initRot=angles;
		math::matrix4x4 m,m2;
		rot.toMatrix(m);
		m.getInverse(m2);
		//m_oriInv=rot.inverse();//calculating quaternion inverse as the new frame
		m_oriInv.fromMatrix(m2);*/
		//m.getInverse(m_homeMat);
	//	m.setTranslation(-pos);
		m_calib=true;

	}
	//m=m*m_homeMat;
	m_lastPos=newP-m_pos;
	/*
	//m_lastOri=rot*m_oriInv; 
	angles=angles-m_initRot;
	m_lastOri.fromEulerAngles(angles.x,angles.y,angles.z);*/
	m_lastOri.fromMatrix(m);
	//m_lastPos= m.getTranslation();
	m_lastPos.x=-m_lastPos.x;
	m_lastPos.z=-m_lastPos.z;
	//math::matrix4x4 mat;
	//da.toMatrix(mat);

}

void TrackController::Send()
{


	float controlVec[7];
	controlVec[0]=m_lastPos.x;
	controlVec[1]=m_lastPos.y;
	controlVec[2]=m_lastPos.z;
	controlVec[3]=m_lastOri.w;
	controlVec[4]=m_lastOri.x;
	controlVec[5]=m_lastOri.y;
	controlVec[6]=m_lastOri.z;

	printf("Sending %s:<%f,%f,%f>\n",m_name.c_str(),m_lastPos.x,m_lastPos.y,m_lastPos.z);
	Communicator::Instance()->AddSendValue(m_name,controlVec,7);
}

void TrackController::Update(float dt)
{
}

}

