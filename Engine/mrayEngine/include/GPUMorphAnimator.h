
/********************************************************************
	created:	2009/06/24
	created:	24:6:2009   10:48
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\GPUMorphAnimator.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	GPUMorphAnimator
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GPUMorphAnimator___
#define ___GPUMorphAnimator___


#include "IMorphAnimator.h"
#include "Point4d.h"

namespace mray{
namespace scene{

	// this has a limitation , can morph only between 2->4 targets
class MRAY_DLL GPUMorphAnimator:public IMorphAnimator
{
private:
protected:
	int m_posStream;
	int m_normalStream;
	bool m_useNormals;

	math::vector4d m_weightVec;
	math::vector4d m_topTargets;
	int m_targetsCnt;

	bool m_changed;

public:
	GPUMorphAnimator(IMeshBuffer*buff,int posStreamStart,int normalStreamStart,bool useNormals);
	virtual~GPUMorphAnimator();

	const math::vector4d& getWeightVec(){return m_weightVec;}

	virtual void update(float dt);

	virtual void setTargetPercent(int index,float v);

	virtual IMeshAnimator* duplicate();
};

}
}

#endif //___GPUMorphAnimator___
