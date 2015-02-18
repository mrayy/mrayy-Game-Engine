

/********************************************************************
	created:	2011/12/04
	created:	4:12:2011   15:40
	filename: 	d:\Development\mrayEngine\Tools\BHUIBuilderLib\BHSequenceObject.h
	file path:	d:\Development\mrayEngine\Tools\BHUIBuilderLib
	file base:	BHSequenceObject
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __BHSequenceObject__
#define __BHSequenceObject__


#include "IBHCanvasObject.h"
#include "BHSequenceNode.h"

namespace mray
{
namespace canvas
{

class BHSequenceObject:public IBHCanvasObject
{
private:
protected:

	AI::BHSequenceNode* m_node;

	virtual void _BuildShapes();
	virtual void _BuildConnectors();
public:
	BHSequenceObject();
	virtual~BHSequenceObject();

	void SetNode(AI::BHSequenceNode*node);
	virtual AI::IBHNode* GetNode(){return m_node;}

	void ConnectorConnected(IConnector* conn,IConnector* other);
};


}

}

#endif

