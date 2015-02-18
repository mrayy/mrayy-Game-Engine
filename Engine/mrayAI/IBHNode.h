


/********************************************************************
	created:	2011/12/03
	created:	3:12:2011   20:31
	filename: 	d:\Development\mrayEngine\Engine\mrayAI\IBHNode.h
	file path:	d:\Development\mrayEngine\Engine\mrayAI
	file base:	IBHNode
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IBHNode__
#define __IBHNode__

#include "compileConfig.h"
#include "mtypes.h"
#include "mstring.h"

namespace mray
{
namespace AI
{

	enum EBHNodeType
	{
		EBHNode_Selector,
		EBHNode_Sequence,
		EBHNode_Decorator,
		EBHNode_Action,
		EBHNode_Condition,

		EBHNode_Other
	};

class IBHNode
{
private:
protected:

	uint m_id;
	EBHNodeType m_type;
	core::string m_description;
public:
	IBHNode(EBHNodeType type):m_type(type)
	{
	}
	virtual~IBHNode(){}

	EBHNodeType GetType(){return m_type;}

	virtual bool CheckCondition()=0;

	virtual bool Access(float dt)=0;

	void SetID(uint id){m_id=id;}
	uint GetID()const{return m_id;}

	void SetDescription(const core::string&str){m_description=str;}
	const core::string& GetDescription()const{return m_description;}

};

typedef std::vector<IBHNode*> BHNodeList;

}
}

#endif
