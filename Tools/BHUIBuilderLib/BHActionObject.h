


/********************************************************************
	created:	2011/12/04
	created:	4:12:2011   14:44
	filename: 	d:\Development\mrayEngine\Tools\BHUIBuilderLib\BHActionObject.h
	file path:	d:\Development\mrayEngine\Tools\BHUIBuilderLib
	file base:	BHActionObject
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __BHActionObject__
#define __BHActionObject__

#include "IBHCanvasObject.h"
#include "IBHActionNode.h"


namespace mray
{
namespace canvas
{

class BHActionObject:public IBHCanvasObject
{
	DECLARE_RTTI
private:
protected:
	AI::IBHActionNode* m_node;

	virtual void _BuildShapes();
public:
	//DECLARE_PROPERTY_TYPE(ActionName,core::string,);
public:
	BHActionObject();
	virtual~BHActionObject();

	void SetNode(AI::IBHActionNode*node);
	virtual AI::IBHNode* GetNode(){return m_node;}

	void ConnectorConnected(IConnector* conn,IConnector* other);

	bool SetActionName(const core::string& name);
	core::string GetActionName()const;
};

}
}

#endif
