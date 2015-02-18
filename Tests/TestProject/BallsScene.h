
/********************************************************************
	created:	2009/09/30
	created:	30:9:2009   19:58
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject\BallsScene.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject
	file base:	BallsScene
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___BallsScene___
#define ___BallsScene___

#include "IExampleScene.h"

namespace mray{

	namespace scene
	{
		class ISceneNode;
	}

	class Application;

class BallsScene:public IExampleScene
{
private:
protected:
	Application* m_app;
	GCPtr<scene::ISceneNode> m_root;

	void setupNodes();
public:
	BallsScene(Application*app);
	virtual~BallsScene();

	const mchar* getName();
	const mchar* getDescription();

	void createScene();
	void destroyScene();

	void update(float dt);
};

}


#endif //___BallsScene___
