


/********************************************************************
	created:	2011/12/03
	created:	3:12:2011   20:23
	filename: 	d:\Development\mrayEngine\Engine\mrayAI\IBehaviorTree.h
	file path:	d:\Development\mrayEngine\Engine\mrayAI
	file base:	IBehaviorTree
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IBehaviorTree__
#define __IBehaviorTree__

#include "compileConfig.h"

namespace mray
{
namespace AI
{
class IBHNode;

class MRAY_AI_DLL IBehaviorTree
{
private:
protected:
	float m_interval;
	IBHNode* m_rootNode;
	float m_time;

public:
	IBehaviorTree();
	virtual~IBehaviorTree();

	virtual void Update(float dt);

	void SetRootNode(IBHNode*node);
	IBHNode* GetRootNode()const;
};

}
}

#endif
