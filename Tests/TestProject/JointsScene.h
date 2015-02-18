

/********************************************************************
	created:	2009/09/30
	created:	30:9:2009   20:49
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject\JointsScene.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject
	file base:	JointsScene
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___JointsScene___
#define ___JointsScene___


#include "IExampleScene.h"
#include "SegmentDesc.h"
#include "Segment.h"

namespace mray{

namespace scene
{
	class ISceneNode;
}


class Application;

class JointsScene:public IExampleScene
{
private:
protected:
	Application* m_app;
	GCPtr<scene::ISceneNode> m_root;
	GCPtr<Segment> m_rootBone;

	SegmentDesc m_desc;

	void setupNodes();
public:
	JointsScene(Application*app);
	virtual~JointsScene();

	const mchar* getName();
	const mchar* getDescription();

	void createScene();
	void destroyScene();

	void update(float dt);
};

}


#endif //___JointsScene___
