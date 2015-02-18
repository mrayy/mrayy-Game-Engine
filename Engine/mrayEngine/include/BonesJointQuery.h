
/********************************************************************
	created:	2010/02/11
	created:	11:2:2010   12:18
	file base:	BonesJointQuery
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___BonesJointQuery___
#define ___BonesJointQuery___

#include "IJointQuery.h"

namespace mray{

	namespace scene
	{
		class BoneNode;
	}

namespace animation{

class MRAY_DLL BonesJointQuery:public IJointQuery
{
private:
protected:
	
	scene::IMovable* m_endEffector;
	std::vector<JointDOF> m_bones;
	std::vector<float> m_values;
	typedef std::map<scene::IMovable*, std::vector<int>> JointMap;
	JointMap m_jointsMap;
	int m_dofs;
public:
	BonesJointQuery();
	virtual~BonesJointQuery();

	IJointQuery::JointDOF* addBone(scene::IMovable*bone, EIKJointType type, const math::vector3d& axis);
	void SetEndEffector(scene::IMovable*bone);
	void clearBones();

	virtual void OnIKBegin();
	virtual void OnIKEnd();

	virtual std::vector<JointDOF>&  getJointsDOF();
	virtual std::vector<float>& getJointVector();
	virtual int getJointsDOFCount();
	virtual bool IsLimited(int joint) { return m_bones[joint].hasLimit; }
	virtual bool getLimits(int joint, float &lower, float &upper)
	{ 
		lower = m_bones[joint].lowerLimit;
		upper = m_bones[joint].upperLimit;
		return m_bones[joint].hasLimit; 
	}
	virtual math::vector3d getEndEffectorPos();
	virtual math::quaternion getEndEffectorOri();

	virtual void applyChange(const std::vector<float> &change);
};

}
}


#endif //___BonesJointQuery___
