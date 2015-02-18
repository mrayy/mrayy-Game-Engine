

/********************************************************************
	created:	2009/10/10
	created:	10:10:2009   22:19
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject\JointNode.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject
	file base:	JointNode
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___JointNode___
#define ___JointNode___

#include <BoneNode.h>

namespace mray{

class JointNode
{
private:
protected:
	GCPtr<scene::BoneNode> m_joint;
public:
	JointNode();
	virtual~JointNode();

};

}


#endif //___JointNode___
