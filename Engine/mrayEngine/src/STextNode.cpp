
#include "STextNode.h"
#include "IVideoDevice.h"



#include "SceneHelper.h"


//#include <gl\gl.h>
 

namespace mray{
namespace scene{


STextNode::STextNode(GUI::IFont*font)
{
	this->font=font;
	m_autoResize=1;

	m_fontAttrs.fontSize=400;
}

STextNode::~STextNode()
{
}

void STextNode::setText(const  core::string&text)
{
	this->text=text;
}
void STextNode::setFont(GUI::IFont*font)
{
	this->font=font;
}


bool STextNode::preRender()
{
	return false;
}
bool STextNode::render()
{
	if(!font )return true;
	return true;
/*
	math::vector3d pos=SceneHelper::getInstance().getScreenCoordFromPos(getAbsolutePosition(),sceneMngr->getActiveCamera());;

	//=sceneMngr->getActiveCamera()->getAbsolutePosition().getDistSQ(getAbsolutePosition())
	float dist=pos.z;
	if(dist>1)
		dist=1.0f/dist;
	else dist=1;

	bool oldZ=getDevice()->getRenderState(video::RS_ZTest);
	bool oldA=getDevice()->getRenderState(video::RS_AlphaTest);

	getDevice()->set2DMode();

	getDevice()->setRenderState(video::RS_ZTest,0);
	getDevice()->setRenderState(video::RS_AlphaTest,0);

	float mainSize=m_fontAttrs.fontSize;
	if(m_autoResize)
		m_fontAttrs.fontSize*=dist;
	font->print(math::rectf(pos.x,pos.y-7,0,0),&m_fontAttrs,0,text.c_str());
	m_fontAttrs.fontSize=mainSize;

	getDevice()->set3DMode();

	getDevice()->setRenderState(video::RS_ZTest,oldZ);
	getDevice()->setRenderState(video::RS_AlphaTest,oldA);


	getDevice()->set2DMode();
	//	getDevice()->resetMatrix();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_ALPHA_TEST);
		float sz=500*dist;
		float height=font->getDimension(L'A',sz).y*0.5;
		font->print(math::rectf(pos.x-200,pos.y-height,pos.x+200,pos.y+height),sz,0,fontColor,0,1,0,text.c_str());
	getDevice()->set3DMode();*/
}
void STextNode::update(float dt)
{
}







}
}
