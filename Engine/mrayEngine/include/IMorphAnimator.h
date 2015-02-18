
/********************************************************************
	created:	2009/06/24
	created:	24:6:2009   10:48
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IMorphAnimator.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IMorphAnimator
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IMorphAnimator___
#define ___IMorphAnimator___


#include "IMeshAnimator.h"
#include "MorphTarget.h"


namespace mray{
namespace scene{
	class IMeshBuffer;

class MRAY_DLL IMorphAnimator:public IMeshAnimator
{
private:
protected:
	IMeshBuffer*m_buffer;
	struct MorphTargetInfo
	{
		MorphTargetInfo(const MorphTargetPtr& target){
			this->target=target;
			oldVal=0;
		}
		MorphTargetInfo(){
			target=0;
			oldVal=0;
		}
		MorphTargetPtr target;
		float oldVal;
	};

	std::vector<MorphTargetInfo> m_targets;
	int m_vertexCount;
public:
	IMorphAnimator(IMeshBuffer*buff);
	virtual~IMorphAnimator();

	IMeshBuffer*getMeshBuffer();
	virtual void setMeshBuffer(IMeshBuffer*b);

	void setTarget(int t,scene::IMeshBuffer*buff,float p);
	virtual const MorphTargetPtr&  addTarget(scene::IMeshBuffer*buff,float p);
	int getTargetCount();
	const MorphTargetPtr& getTarget(int index);
	void clearTargets();

	virtual void setTargetPercent(int index,float v);
	virtual float getTargetPercent(int index,float v);
	

};

MakeSharedPtrType(IMorphAnimator);

}
}


#endif //___IMorphAnimator___
