


/********************************************************************
	created:	2009/12/05
	created:	5:12:2009   20:33
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject\ModelScene.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject
	file base:	ModelScene
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ModelScene___
#define ___ModelScene___


#include "IExampleScene.h"
namespace mray{

class Application;

class ModelScene:public IExampleScene
{
private:
protected:
	Application* m_app;
	GCPtr<scene::ISceneNode> m_root;

public:
	ModelScene(Application*app);
	virtual~ModelScene();


	const mchar* getName();
	const mchar* getDescription();

	void createScene();
	void destroyScene();

	void update(float dt);

};

}


#endif //___ModelScene___