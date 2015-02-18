

/********************************************************************
	created:	2010/02/11
	created:	11:2:2010   11:27
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IJointQuery.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IJointQuery
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	this is used to make DOF queries on joints,
				this is primary used for InverseKinmatics
*********************************************************************/

#ifndef ___IJointQuery___
#define ___IJointQuery___

#include "compileConfig.h"
#include "IMovable.h"

namespace mray{
namespace scene{
	class IMovable;
}
namespace animation{

	enum class EIKJointAxis
	{
		X,
		Y,
		Z
	};
	enum class EIKJointType
	{
		Linear,
		Prismatic
	};

class MRAY_DLL IJointQuery
{
private:
protected:
public:
	struct JointDOF
	{
		JointDOF() :node(0), type(EIKJointType::Prismatic), jointAxis(math::vector3d::YAxis), hasLimit(false)
		{}
		EIKJointType type;
		math::vector3d jointAxis;
		scene::IMovable* node;
		bool hasLimit;

		float lowerLimit, upperLimit;

		void SetLimits(float low, float upper){ lowerLimit = low; upperLimit = upper; hasLimit = true; }

		math::vector3d GetAxis()const
		{
			return node->getAbsoluteOrintation()*jointAxis;
		}
		float limitValue(float v)
		{
			if (hasLimit)
				return math::clamp(v, lowerLimit, upperLimit);
			return v;
		}
	};

	IJointQuery(){}
	virtual~IJointQuery(){}

	virtual void OnIKBegin()=0;
	virtual void OnIKEnd() = 0;

	virtual std::vector<JointDOF>&  getJointsDOF(/*const math::vector3d&targetPos*/) = 0;
	virtual std::vector<float>& getJointVector()=0;
	virtual bool IsLimited(int joint) = 0;
	virtual bool getLimits(int joint, float &lower, float &upper) = 0;
	virtual int getJointsDOFCount()=0;
	virtual math::vector3d getEndEffectorPos()=0;
	virtual math::quaternion getEndEffectorOri()=0;

	virtual void applyChange(const std::vector<float> &change) = 0;
};

}
}


#endif //___IJointQuery___
