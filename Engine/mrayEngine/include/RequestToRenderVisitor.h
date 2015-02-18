


/********************************************************************
	created:	2010/10/28
	created:	28:10:2010   15:56
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\RequestToRenderVisitor.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	RequestToRenderVisitor
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef RequestToRenderVisitor_h__
#define RequestToRenderVisitor_h__

#include "ISceneVisitor.h"

namespace mray
{
namespace scene
{
class IRenderPass;

class RequestToRenderVisitor:public ISceneVisitor
{
private:
protected:
	ISceneManager* m_smngr;
	IRenderPass*m_pass;

	void _AddRenderableNode(IRenderable*r);

public:
	RequestToRenderVisitor(IRenderPass*pass);
	virtual ~RequestToRenderVisitor();

	virtual void Visit(ISceneManager*mngr);
	virtual void Visit(ISceneNode*node);
	virtual void Visit(CameraNode*c);
	virtual void Visit(LightNode*l);

	virtual void Visit(IRenderable*r);
	virtual void Visit(IMovable*m);

};

}
}


#endif // RequestToRenderVisitor_h__
