

/********************************************************************
	created:	2011/12/04
	created:	4:12:2011   15:48
	filename: 	d:\Development\mrayEngine\Tools\BHUIBuilderLib\BHTreeObject.h
	file path:	d:\Development\mrayEngine\Tools\BHUIBuilderLib
	file base:	BHTreeObject
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __BHTreeObject__
#define __BHTreeObject__

#include "IBHCanvasObject.h"
#include "IBehaviorTree.h"

namespace mray
{
namespace canvas
{

class BHTreeObject:public IBHCanvasObject
{
private:
protected:
	AI::IBehaviorTree* m_tree;

	virtual void _BuildShapes();
	virtual void _BuildConnectors();
public:
	BHTreeObject();
	virtual~BHTreeObject();

	void SetTree(AI::IBehaviorTree*tree);
	virtual AI::IBHNode* GetNode(){return 0;}

	void ConnectorConnected(IConnector* conn,IConnector* other);
};

}
}

#endif

