


/********************************************************************
	created:	2011/12/03
	created:	3:12:2011   20:28
	filename: 	d:\Development\mrayEngine\Engine\mrayAI\IBHDecorator.h
	file path:	d:\Development\mrayEngine\Engine\mrayAI
	file base:	IBHDecorator
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IBHDecorator__
#define __IBHDecorator__

#include "IBHNode.h"

namespace mray
{
namespace AI
{

class IBHDecorator:public IBHNode
{
private:
protected:
	IBHNode* m_node;
public:
	IBHDecorator():IBHNode(EBHNode_Decorator),m_node(0)
	{
	}
	virtual~IBHDecorator()
	{
		delete m_node;
	}

	virtual void SetNode(IBHNode*n){m_node=n;}
	virtual IBHNode* GetNode(){return m_node;}

	virtual bool CheckCondition()
	{
		if(m_node)
			return m_node->CheckCondition();
		return true;
	}

	virtual bool Access(float dt)
	{
		if(m_node)
			return m_node->Access(dt);
		return true;
	}
};

}
}

#endif
