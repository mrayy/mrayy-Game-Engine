


/********************************************************************
	created:	2011/12/19
	created:	19:12:2011   15:58
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IKSimple.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IKSimple
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IKSimple__
#define __IKSimple__

#include "IInverseKinmatics.h"
#include "IJointQuery.h"

namespace mray
{
namespace animation
{

class MRAY_DLL IKSimple:public IInverseKinmatics
{
private:
protected:
	IJointQuery* m_query;
	math::vector3d m_targetPos;
	int m_maxIteration;

	struct JoinData
	{
		JoinData():rotationAngle(0),length(0)
		{}
		math::vector3d rotationAxis;
		float rotationAngle;
		float length;
		math::quaternion rotation;
	};

	std::vector<float> m_changeRotation;
	std::vector<JoinData> m_jointData;
	const std::vector<IJointQuery::JointDOF> *m_jointsDOF;

public:
	IKSimple();
	virtual~IKSimple();
	
	void setMaxIterations(int n);
	int getMaxIterations();

	void calcOrintation(bool e);
	bool isCalcOrintation();

	void setJointsChain(IJointQuery* q);
	void setTarget(const math::vector3d&p,const math::quaternion&r);

	void update();
};


}
}

#endif