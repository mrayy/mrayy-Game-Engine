
/********************************************************************
	created:	2009/03/15
	created:	15:3:2009   23:50
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\MorphAnimator.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	MorphAnimator
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___MorphAnimator___
#define ___MorphAnimator___

#include "IMorphAnimator.h"



namespace mray{
namespace scene{

class MRAY_DLL MorphAnimator:public IMorphAnimator
{
private:
protected:
	std::vector<math::vector3d> m_tempVector;
	std::vector<math::vector3d> m_baseVector;
	bool m_needUpdate;
public:
	MorphAnimator(IMeshBuffer*buff);
	virtual~MorphAnimator();

	virtual void setMeshBuffer(IMeshBuffer*b);

	virtual void update(float dt);

	virtual const MorphTargetPtr& addTarget(scene::IMeshBuffer*buff,float p);
	virtual void setTargetPercent(int index,float v);
	virtual IMeshAnimator* duplicate();
};

}
}


#endif //___MorphAnimator___

