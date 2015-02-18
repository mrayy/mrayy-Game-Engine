#include "stdafx.h"

#include "SSkyPlaneNode.h"
#include "ISceneManager.h"
#include "TextureResourceManager.h"
#include "IVideoDevice.h"
#include "MeshGenerator.h"
#include "RenderManager.h"
#include "MeshBufferData.h"


namespace mray{
namespace scene{


SSkyPlaneNode::SSkyPlaneNode(const video::ITexturePtr&  texture,int uSegments,int vSegments,
		double texPercentage,float hight,float size)
		/*
USegments(EPT_INT,mT("USegments"),mT("Width Segments")),
VSegments(EPT_INT,mT("VSegments"),mT("Length Segments")),
TexPercentage(EPT_FLOAT,mT("Tilt"),mT("Texture Tilt")),
Hight(EPT_FLOAT,mT("Height"),mT("Sky Height")),
Size(EPT_FLOAT,mT("Size"),mT("Size of the sky")),
Speed(EPT_VECTOR2Df,mT("Speed"),mT("Speed of the sky"))*/
{

	m_uSegments=uSegments;
	m_vSegments=vSegments;
	m_texPercentage=texPercentage;
	m_hight=hight;
	m_size=size;
	m_speed=0;
	
	setup(texture);

	fillProperties();
}


SSkyPlaneNode::~SSkyPlaneNode()
{
}


void SSkyPlaneNode::fillProperties(){
	/*
	addPropertie(&USegments,mT("SkyPlane"));
	addPropertie(&VSegments,mT("SkyPlane"));
	addPropertie(&TexPercentage,mT("SkyPlane"));
	addPropertie(&Hight,mT("SkyPlane"));
	addPropertie(&Size,mT("SkyPlane"));
	addPropertie(&Speed,mT("SkyPlane"));*/
}

void SSkyPlaneNode::setup(const video::ITexturePtr&  tex)
{
	if(m_uSegments==0)m_uSegments=1;
	if(m_vSegments==0)m_vSegments=1;

	int vtxCount=(m_uSegments+1)*(m_vSegments+1);
	int indCount=(m_uSegments)*(m_vSegments)*2*3;


	
	MeshGenerator::getInstance().generateSkyPlane(tex,m_uSegments,m_vSegments,m_texPercentage,m_hight,m_size,getMesh());

}





void SSkyPlaneNode::update(float dt){
	MeshBufferData* bd=getMesh()->getBufferData(0);
// 	if( bd && bd->getMaterial()->getTextureUnit(0))
// 		bd->getMaterial()->getTextureUnit(0)->trans+=math::vector3d(getSpeed().x,0,getSpeed().y)*dt;
		
}

/*
xml::XMLElement* SSkyPlaneNode::loadXMLSettings(xml::XMLElement* e)
{
	xml::XMLElement* elem=getSubElement("SkyPlane");
	if(!elem)return;
	
	video::ITexturePtr  tex=0;

	xml::XMLElement* child=0;
	child=elem->getSubElement(mT("texture"));
	if(child){
		xml::XMLAttribute* attr=0;
		attr=child->getAttribute(mT("fileName"));
		if(attr)
			tex=gTextureResourceManager.loadTexture2D(attr->value);
		if(tex)
			tex->loadXMLSettings(child);
	}

	setup(tex);
}
xml::XMLElement*  SSkyPlaneNode::exportXMLSettings(xml::XMLElement* elem){

	xml::XMLElement* child=0;
	if(getMesh()->getBuffer(0))
		getMesh()->getBufferData(0)->getMaterial()->getTexture(0)->exportXMLSettings(elem);

	return elem;
}
*/

}
}
