
#include "stdafx.h"
#include "CustomNode.h"

namespace mray{

CustomNode::CustomNode(const mchar*name,video::IVideoDevice*dev,float radFact)
:scene::ISceneNode(name,dev)
{
	m_pointsCloud.resize(40);
	for(int i=0;i<m_pointsCloud.size();++i){
		m_pointsCloud[i].x=math::Randomizer::rand01()*360;//alpha
		m_pointsCloud[i].y=math::Randomizer::rand01()*360;//theta
		m_pointsCloud[i].z=radFact*(math::Randomizer::rand01()*10+10);//radius
	}
	m_time=0;

	m_mtrl=gMaterialResourceManager.getMaterial(mT("CustomNodeMtrl"));
	if(!m_mtrl){
		m_mtrl=new video::SMaterial();
		m_mtrl->thickness=2;
		m_mtrl->setRenderState(video::RS_Lighting,video::ES_DontUse);
		gMaterialResourceManager.addResource(m_mtrl,mT("CustomNodeMtrl"));
	}
}
CustomNode::~CustomNode()
{
}

void CustomNode::preRender()
{
	scene::ISceneNode::preRender();
	if(getSceneManager()){
		scene::RenderOperation op;
		op.node=this;
		op.mesh=0;
		op.customRender=true;
		getSceneManager()->getRenderManager()->addOperation(scene::RGH_Solid,op);
	}
}
void CustomNode::update(float dt)
{
	scene::ISceneNode::update(dt);

	for(int i=0;i<m_pointsCloud.size();++i){
		m_pointsCloud[i].x+=10*dt;
		m_pointsCloud[i].y+=10*dt;
//		m_pointsCloud[i].w=m_pointsCloud[i].z+5*math::sind(m_time);
	}
	m_time+=5*dt;
}

void CustomNode::render()
{
	scene::ISceneNode::render();

	if(!device)return;
	device->setTransformationState(video::TS_WORLD,this->getAbsoluteTransformation());

	device->useMaterial(m_mtrl);

	math::vector3d pt1=0;
	math::vector3d pt2=0;
	for(int i=0;i<m_pointsCloud.size();++i){
		const math::vector4d&p=m_pointsCloud[i];
/*		pt2.x=p.w * math::sind(p.x)*math::cosd(p.y);
		pt2.y=p.w * math::sind(p.y);
		pt2.z=p.w * math::cosd(p.x)*math::cosd(p.y);

*/		device->draw3DLine(pt1,pt2,video::DefaultColors::White);
	}
}


}


