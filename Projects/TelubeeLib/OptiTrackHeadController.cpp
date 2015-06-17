

#include "stdafx.h"
#include "OptiTrackHeadController.h"
#include "AppData.h"
#include "OptiTrackDataSource.h"

namespace mray
{
namespace TBee
{

OptiTrackHeadController::OptiTrackHeadController(int headId)
{
	m_headID = headId;
}
OptiTrackHeadController::~OptiTrackHeadController()
{
}


bool OptiTrackHeadController::GetHeadOrientation(math::quaternion& v, bool abs)
{
	math::quaternion* q= AppData::Instance()->optiDataSource->GetOrientationByID(m_headID);
	if (!q)
		return false;
	math::vector3d a;
	q->toEulerAngles(a);

	if (m_oriAvg.size() < 10)
	{
		m_oriAvg.push_back(a);
	}
	else
	{
		m_oriAvg.remove(m_oriAvg.front());
		m_oriAvg.push_back(a);
	}
	a = 0;
	std::list<math::vector3d>::iterator it = m_oriAvg.begin();
	for (; it != m_oriAvg.end(); ++it)
	{
		a += *it;
	}
	a /= (float)m_oriAvg.size();
	v = math::quaternion(-a.x, a.y, -a.z);

	/////////////////////////////////////
	v = math::quaternion(a.y, a.x, a.z);	//in the lab
	
	return true;
}
bool OptiTrackHeadController::GetHeadPosition(math::vector3d& v, bool abs)
{
	math::vector3d* p=AppData::Instance()->optiDataSource->GetPositionByID(m_headID);
	if (!p)
		return false;

	if (m_posAvg.size() < 10)
	{
		m_posAvg.push_back(*p);
	}
	else
	{
		m_posAvg.remove(m_posAvg.front());
		m_posAvg.push_back(*p);
	}
	v = 0;
	std::list<math::vector3d>::iterator it= m_posAvg.begin();
	for (; it != m_posAvg.end();++it)
	{
		v += *it;
	}
	v /= (float)m_posAvg.size();


	/////////////////////////////////////

	v.set(v.x, v.z, v.y);
	return true;
}

}
}
