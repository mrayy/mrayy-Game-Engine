


/********************************************************************
	created:	2011/12/04
	created:	4:12:2011   12:13
	filename: 	d:\Development\mrayEngine\Tools\BHUIBuilderLib\BHSelectorObject.h
	file path:	d:\Development\mrayEngine\Tools\BHUIBuilderLib
	file base:	BHSelectorObject
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __BHSelectorObject__
#define __BHSelectorObject__

#include "IBHCanvasObject.h"
#include "BHSelectorNode.h"

namespace mray
{
namespace AI
{
	class BHSelectorNode;
}
namespace canvas
{
	
class BHSelectorObject:public IBHCanvasObject
{
private:
protected:

	AI::BHSelectorNode* m_node;

	virtual void _BuildShapes();
	virtual void _BuildConnectors();
public:
	BHSelectorObject();
	virtual~BHSelectorObject();

	void SetNode(AI::BHSelectorNode*node);
	virtual AI::IBHNode* GetNode(){return m_node;}

	void ConnectorConnected(IConnector* conn,IConnector* other);
};


}
}

#endif
