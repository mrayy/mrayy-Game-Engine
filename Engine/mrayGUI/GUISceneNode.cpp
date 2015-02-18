


#include "GUISceneNode.h"
#include "ISceneNode.h"
#include "ISceneManager.h"
#include "SceneHelper.h"


namespace mray{
namespace GUI{


GUISceneNode::GUISceneNode(GUIManager* manager,GCPtr<scene::ISceneNode> node,scene::ISceneManager* smngr,const math::vector2d& pos,GUIElement* parent,int id):
GUIElement(manager,parent,pos,1,0,id)
{
	this->node=node;
	this->smngr=smngr;
}

GUISceneNode::~GUISceneNode(){
}

void GUISceneNode::draw(float dt){
	if(!Visible)return;
	if(!node || !smngr)return;

	smngr->getActiveCamera()->updateView();
	math::vector3d vpos=scene::SceneHelper::getInstance().getVectorFromScreen(getDevice()->getViewportRect(),getPos(),10,smngr->getActiveCamera());
	node->position=vpos;
//	node->updateAbsoluteTransformation();

	device->clearBuffer(video::EDB_DEPTH);
	node->render(0);
	node->update(dt);

	device->set2DMode();

	GUIElement::draw(dt);

}

GCPtr<GUIElement> GUISceneNode::getDuplicate(){
	//GUGCPtr<scene::ISceneNode> node=new GUscene::ISceneNode();

	return 0;
}


}
}
