
/********************************************************************
	created:	2009/09/30
	created:	30:9:2009   22:43
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject\CustomNodeScene.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject
	file base:	CustomNodeScene
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___CustomNodeScene___
#define ___CustomNodeScene___


#include "IExampleScene.h"
namespace mray{

namespace scene
{
	class ISceneNode;
}

class Application;
class CustomNode;


class CustomNodeScene:public IExampleScene
{
private:
protected:
	Application* m_app;
	GCPtr<scene::ISceneNode> m_root;

	typedef std::list<GCPtr<CustomNode>> NodesList;
	NodesList m_nodes;

	void setupNodes();
public:
	CustomNodeScene(Application*app);
	virtual~CustomNodeScene();


	const mchar* getName();
	const mchar* getDescription();

	void createScene();
	void destroyScene();

	void update(float dt);
};

}


#endif //___CustomNodeScene___
