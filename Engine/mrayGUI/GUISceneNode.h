

#ifndef ___GUISceneNode___
#define ___GUISceneNode___

#include "GUIElement.h"


namespace mray{
namespace scene{
		
	class ISceneNode;
	class ISceneManager;
}
namespace GUI{

class MRAY_DLL GUISceneNode:public GUIElement
{
	GCPtr<scene::ISceneNode> node;
	scene::ISceneManager* smngr;
public:
	DECLARE_RTTI;

	GUISceneNode(GUIManager* manager,GCPtr<scene::ISceneNode> node,scene::ISceneManager* smngr,const math::vector2d& pos,GUIElement* parent,int id);

	virtual~GUISceneNode();

	virtual void draw(float dt);

	virtual GCPtr<GUIElement> getDuplicate();

};

}
}


#endif




