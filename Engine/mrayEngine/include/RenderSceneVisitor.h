

/********************************************************************
	created:	2010/10/29
	created:	29:10:2010   15:19
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\RenderSceneVisitor.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	RenderSceneVisitor
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef RenderSceneVisitor_h__
#define RenderSceneVisitor_h__

#include "ISceneVisitor.h"

namespace mray
{
namespace scene
{

	class IRenderPass ;

class RenderSceneVisitor:public ISceneVisitor
{
private:
protected:
	ISceneManager* m_smngr;
	IRenderPass *m_pass;
public:
	RenderSceneVisitor(IRenderPass *pass);
	virtual ~RenderSceneVisitor();

	virtual void Visit(ISceneManager*mngr);
	virtual void Visit(ISceneNode*node);
	virtual void Visit(CameraNode*c);
	virtual void Visit(LightNode*l);

	virtual void Visit(IRenderable*r);
	virtual void Visit(IMovable*m);
};

}
}


#endif // RenderSceneVisitor_h__

