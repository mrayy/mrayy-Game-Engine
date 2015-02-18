


/********************************************************************
	created:	2011/05/12
	created:	12:5:2011   23:30
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IPhysicContactStreamIterator.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IPhysicContactStreamIterator
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IPhysicContactStreamIterator__
#define __IPhysicContactStreamIterator__

#include <Point3d.h>

namespace mray
{
namespace physics
{
	class IPhysicalShape;

class IPhysicContactStreamIterator
{
public:

	struct PointInfo
	{
		PointInfo():force(0),separation(0)
		{
		}
		math::vector3d point;
		float force;
		float separation;
	};
	typedef std::list<PointInfo> PointsInfoList;
	struct PatchInfo
	{
		PatchInfo()
		{
		}
		math::vector3d normal;
		PointsInfoList points;

	};
	typedef std::list<PatchInfo> PatchInfoList;

	struct PairsInfo
	{
		PairsInfo():s1(0),s2(0)
		{
		}
		IPhysicalShape* s1,*s2;
		PatchInfoList patches;
	};
	typedef std::list<PairsInfo> PairsInfoList;

private:
protected:
	PairsInfoList m_pairs;

public:
	IPhysicContactStreamIterator()
	{
	}
	virtual~IPhysicContactStreamIterator(){}

	const PairsInfoList& GetPairs(){return m_pairs;}

};

}
}

#endif
