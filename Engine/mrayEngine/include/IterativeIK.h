

/********************************************************************
	created:	2010/02/11
	created:	11:2:2010   11:45
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IterativeIK.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IterativeIK
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IterativeIK___
#define ___IterativeIK___

#include "IInverseKinmatics.h"
#include "IJacobian.h"

namespace mray{
namespace animation{

class MRAY_DLL IterativeIK:public IInverseKinmatics
{
private:
protected:
	class JoinData
	{
	public:
		math::vector3d axis;
		math::vector3d pivot;
	};

	math::IJacobian* m_jacobian;
	math::IJacobian m_inverseJacobian;
	
	IJointQuery* m_query;
	math::vector3d m_targetPos;
	math::quaternion m_targetOri;
	math::vector3d m_targetOriV;
	std::vector<float> m_changeVector;

	float m_deltaTarget[6];

	const std::vector<IJointQuery::JointDOF> *m_jointsDOF;
	std::vector<JoinData> m_jointsData;

	bool m_calcOrintation;

	int m_maxIteration;
	float m_allowedError;
	float m_beta;
	int m_jCols;

	float computeJacobian();
	float computeChange();
	void applyConstraints();
public:
	IterativeIK();
	virtual~IterativeIK();

	void setMaxIterations(int n);
	int getMaxIterations();

	virtual void calcOrintation(bool e);
	virtual bool isCalcOrintation();

	void setBeta(float b);
	float getBeta();

	void setAllowedError(float e);
	float getAllowedError();

	void setJacobian(math::IJacobian*j);

	virtual void setJointsChain(IJointQuery* q);
	virtual void setTarget(const math::vector3d&p,const math::quaternion&r);

	virtual void update();
};

}
}


#endif //___IterativeIK___
