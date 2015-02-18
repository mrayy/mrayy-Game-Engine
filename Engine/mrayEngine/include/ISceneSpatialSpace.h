


/********************************************************************
	created:	2010/10/15
	created:	15:10:2010   18:49
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ISceneSpatialSpace.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ISceneSpatialSpace
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ISceneSpatialSpace_h__
#define ISceneSpatialSpace_h__


namespace mray
{
namespace scene
{

class ISceneSpatialSpace
{
private:
protected:
public:
	ISceneSpatialSpace(){}
	virtual ~ISceneSpatialSpace(){}
	
	virtual void SetSceneGraph(ISceneManager*mngr)=0;

	//traverse over the scene graph to determine the visibility of the nodes to render
	virtual void DetermineVisibility(IViewNode*node)=0;
};

}
}
#endif // ISceneSpatialSpace_h__
