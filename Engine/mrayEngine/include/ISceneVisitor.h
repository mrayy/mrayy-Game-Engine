


/********************************************************************
	created:	2010/10/28
	created:	28:10:2010   14:48
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ISceneVisitor.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ISceneVisitor
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ISceneVisitor_h__
#define ISceneVisitor_h__


namespace mray
{
namespace scene
{

	class ISceneManager;
	class ISceneNode;
	class CameraNode;
	class LightNode;
	class IRenderable;
	class IMovable;
	class ICullable;

class ISceneVisitor
{
private:
protected:
public:
	ISceneVisitor(){}
	virtual ~ISceneVisitor(){}
	
	virtual void Visit(ISceneManager*mngr){}
	virtual void Visit(ISceneNode*node){}
	virtual void Visit(CameraNode*c){}
	virtual void Visit(LightNode*l){}

	virtual void Visit(IRenderable*r){}
	virtual void Visit(IMovable*m){}
	virtual void Visit(ICullable*m){}

};

}
}
#endif // ISceneVisitor_h__
