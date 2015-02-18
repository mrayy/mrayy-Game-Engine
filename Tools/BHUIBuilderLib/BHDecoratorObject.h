

/********************************************************************
	created:	2011/12/04
	created:	4:12:2011   16:16
	filename: 	d:\Development\mrayEngine\Tools\BHUIBuilderLib\BHDecoratorObject.h
	file path:	d:\Development\mrayEngine\Tools\BHUIBuilderLib
	file base:	BHDecoratorObject
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __BHDecoratorObject__
#define __BHDecoratorObject__


#include "IBHCanvasObject.h"
#include "IBHDecorator.h"

namespace mray
{
namespace canvas
{

class BHDecoratorObject:public IBHCanvasObject
{
private:
protected:
	AI::IBHDecorator* m_node;

	virtual void _BuildShapes();
	void _BuildConnectors();
public:
	BHDecoratorObject();
	virtual~BHDecoratorObject();

	void SetNode(AI::IBHDecorator*node);
	virtual AI::IBHNode* GetNode(){return m_node;}

	void ConnectorConnected(IConnector* conn,IConnector* other);
};

}
}

#endif


