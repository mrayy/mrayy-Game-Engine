
/********************************************************************
	created:	2009/09/30
	created:	30:9:2009   22:45
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject\CustomNode.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject
	file base:	CustomNode
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___CustomNode___
#define ___CustomNode___

#include <ISceneNode.h>

namespace mray{

class CustomNode:public scene::ISceneNode
{
private:
protected:
	core::array<math::vector4d> m_pointsCloud;
	float m_time;
	GCPtr<video::SMaterial> m_mtrl;
public:
	CustomNode(const mchar*name,video::IVideoDevice*dev,float radFact);
	virtual~CustomNode();

	virtual void preRender();
	virtual void update(float dt);

	virtual void render();

};

}


#endif //___CustomNode___
