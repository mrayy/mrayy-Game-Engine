
/********************************************************************
	created:	2010/03/08
	created:	8:3:2010   13:08
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\IWorldVisualizer.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	IWorldVisualizer
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IWorldVisualizer___
#define ___IWorldVisualizer___


#include "INavWorldNode.h"
#include "IGraphLink.h"

namespace mray{
namespace AI{
	class IRenderDevice;

class IWorldVisualizer
{
private:
protected:
public:

	virtual void DrawWorld(IRenderDevice*dev)=0;
	virtual void DrawNodes(IRenderDevice*dev,const NavWorldNodeListPtr&nodes,bool drawConnections)=0;
	virtual void DrawNode(IRenderDevice*dev,AI::INavWorldNode*p)=0;
	virtual void DrawLinks(IRenderDevice*dev,GraphLinksList&links)=0;
};

}
}


#endif //___IWorldVisualizer___
