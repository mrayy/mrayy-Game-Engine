


/********************************************************************
	created:	2011/12/03
	created:	3:12:2011   20:25
	filename: 	d:\Development\mrayEngine\Engine\mrayAI\BHSelectorNode.h
	file path:	d:\Development\mrayEngine\Engine\mrayAI
	file base:	BHSelectorNode
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __BHSelectorNode__
#define __BHSelectorNode__

#include "IBHNode.h"

namespace mray
{
namespace AI
{

class MRAY_AI_DLL BHSelectorNode:public IBHNode
{
private:
protected:
	BHNodeList m_nodes;
public:
	BHSelectorNode();
	virtual~BHSelectorNode();

	virtual bool CheckCondition();
	virtual bool Access(float dt);

	void AddNode(IBHNode* node);
	void RemoveNode(IBHNode* node);
	void ClearNodes();

	const BHNodeList& GetNodes();
};

}
}

#endif
