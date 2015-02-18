


/********************************************************************
	created:	2011/12/04
	created:	4:12:2011   16:26
	filename: 	d:\Development\mrayEngine\Engine\mrayAI\BHRandomNode.h
	file path:	d:\Development\mrayEngine\Engine\mrayAI
	file base:	BHRandomNode
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __BHRandomNode__
#define __BHRandomNode__

#include "IBHNode.h"

namespace mray
{
namespace AI
{

class MRAY_AI_DLL BHRandomNode:public IBHNode
{
private:
protected:
	BHNodeList m_nodes;
public:
	BHRandomNode();
	virtual~BHRandomNode();

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
