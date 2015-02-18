


/********************************************************************
	created:	2011/12/03
	created:	3:12:2011   20:40
	filename: 	d:\Development\mrayEngine\Engine\mrayAI\IBHActionNode.h
	file path:	d:\Development\mrayEngine\Engine\mrayAI
	file base:	IBHActionNode
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IBHActionNode__
#define __IBHActionNode__

#include "IBHNode.h"

namespace mray
{
namespace AI
{

class IBHActionNode:public IBHNode
{
private:
protected:
	core::string m_actionName;
public:
	IBHActionNode():IBHNode(EBHNode_Action)
	{}
	virtual~IBHActionNode(){}

	virtual bool CheckCondition(){return true;}
	virtual bool Access(float dt){return true;}
	void SetActionName(const core::string& n){m_actionName=n;}
	const core::string& GetActionName()const{return m_actionName;}
};

}
}

#endif
