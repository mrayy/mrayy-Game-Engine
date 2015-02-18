
/********************************************************************
	created:	2009/06/28
	created:	28:6:2009   0:47
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ISkeletonAnimator.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ISkeletonAnimator
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ISkeletonAnimator___
#define ___ISkeletonAnimator___

#include "IMeshAnimator.h"
#include "common.h"

namespace mray{
namespace scene{

	class Skeleton;
	class ISkinCluster;

class ISkeletonAnimator:public IMeshAnimator
{
private:
public:

	enum EAttributesAffect
	{
		EAA_Position = BIT(0),
		EAA_Normal	= BIT(1),
		EAA_Binormal= BIT(2),
		EAA_Tangent = BIT(3),

		EAA_AttrsCount=4
	};

protected:
public:

	virtual Skeleton*getSkeleton()=0;
	virtual ISkinCluster* getSkinCluster()=0;
};

}
}


#endif //___ISkeletonAnimator___
