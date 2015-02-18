

/********************************************************************
	created:	2010/02/08
	created:	8:2:2010   18:16
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject\SkeletalAnimScene.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject
	file base:	SkeletalAnimScene
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___SkeletalAnimScene___
#define ___SkeletalAnimScene___

#include "IExampleScene.h"
#include <IterativeIK.h>

namespace mray{

	namespace scene
	{
		class ISceneNode;
	}
	class Application;
class SkeletalAnimScene:public IExampleScene
{
private:
protected:
	Application* m_app;
	GCPtr<scene::ISceneNode> m_root;

	GCPtr<animation::IterativeIK> m_ik;
	animation::IAnimationTrack* m_animTrack;

	GCPtr<GUI::GUIStaticText> m_text;
	scene::ISceneNode* m_targetNode;
	scene::ISceneNode* m_Node;

	math::vector3d m_target;
	scene::BoneNode* m_ef;

	scene::BoneNode* m_bone;

	void setupNodes();
	GCPtr<scene::MeshSceneNode> loadChar();
public:
	SkeletalAnimScene(Application*app);
	virtual~SkeletalAnimScene();

	void onEvent(GCPtr<Event> event);

	const mchar* getName();
	const mchar* getDescription();

	void createScene();
	void destroyScene();

	void update(float dt);
};

}


#endif //___SkeletalAnimScene___
