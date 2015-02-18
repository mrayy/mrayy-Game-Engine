
#include "stdafx.h"
#include "ISceneObjectsDesc.h"

#include "StringConverter.h"


namespace mray
{
namespace scene
{


void ISceneNodeDesc::loadFromXML(xml::XMLElement* e)
{
	xml::XMLAttribute* attr;
	
	if((attr=e->getAttribute(mT("Name")))!=0)
		name=attr->value;
	if((attr=e->getAttribute(mT("Position")))!=0)
		position=core::StringConverter::toVector3d(attr->value);
	if((attr=e->getAttribute(mT("Orintation")))!=0)
		orintation=core::StringConverter::toQuaternion(attr->value);
	if((attr=e->getAttribute(mT("Scale")))!=0)
		scale=core::StringConverter::toVector3d(attr->value);
	if((attr=e->getAttribute(mT("Visible")))!=0)
		visible=core::StringConverter::toBool(attr->value);

	if((attr=e->getAttribute(mT("CullType")))!=0)
		cullType=(ENodeCullingType)core::StringConverter::toInt(attr->value);
	if((attr=e->getAttribute(mT("ViewDist")))!=0)
		viewDist=core::StringConverter::toFloat(attr->value);
}
void ISceneNodeDesc::saveToXML(xml::XMLElement* e)
{
	e->addAttribute(mT("Name"),name);
	e->addAttribute(mT("Position"),core::StringConverter::toString(position));
	e->addAttribute(mT("Orintation"),core::StringConverter::toString(orintation));
	e->addAttribute(mT("Scale"),core::StringConverter::toString(scale));
	e->addAttribute(mT("Visible"),core::StringConverter::toString(visible));
	e->addAttribute(mT("CullType"),core::StringConverter::toString(cullType));
	e->addAttribute(mT("ViewDist"),core::StringConverter::toString(viewDist));
}
//////////////////////////////////////////////////////////////////////////

void CameraNodeDesc::loadFromXML(xml::XMLElement* e)
{
	ISceneNodeDesc::loadFromXML(e);
	xml::XMLAttribute* attr;

	if((attr=e->getAttribute(mT("FovY")))!=0)fovY=core::StringConverter::toFloat(attr->value);
	if((attr=e->getAttribute(mT("ZNear")))!=0)zNear=core::StringConverter::toFloat(attr->value);
	if((attr=e->getAttribute(mT("ZFar")))!=0)zFar=core::StringConverter::toFloat(attr->value);
	if((attr=e->getAttribute(mT("Aspect")))!=0)aspect=core::StringConverter::toFloat(attr->value);
	if((attr=e->getAttribute(mT("AutoUpdateAspect")))!=0)autoUpdateAspect=core::StringConverter::toBool(attr->value);
	if((attr=e->getAttribute(mT("IsPerspective")))!=0)isPerspective=core::StringConverter::toBool(attr->value);
	if((attr=e->getAttribute(mT("Target")))!=0)target=core::StringConverter::toVector3d(attr->value);
}
void CameraNodeDesc::saveToXML(xml::XMLElement* e)
{
	ISceneNodeDesc::saveToXML(e);
	e->addAttribute(mT("FovY"),core::StringConverter::toString(fovY));
	e->addAttribute(mT("ZNear"),core::StringConverter::toString(zNear));
	e->addAttribute(mT("ZFar"),core::StringConverter::toString(zFar));
	e->addAttribute(mT("Aspect"),core::StringConverter::toString(aspect));
	e->addAttribute(mT("AutoUpdateAspect"),core::StringConverter::toString(autoUpdateAspect));
	e->addAttribute(mT("IsPerspective"),core::StringConverter::toString(isPerspective));
	e->addAttribute(mT("Target"),core::StringConverter::toString(target));
}
//////////////////////////////////////////////////////////////////////////

void LightNodeDesc::loadFromXML(xml::XMLElement* e)
{
	ISceneNodeDesc::loadFromXML(e);
	xml::XMLAttribute* attr;

	if((attr=e->getAttribute(mT("Radius")))!=0)radius=core::StringConverter::toFloat(attr->value);
	if((attr=e->getAttribute(mT("Ambient")))!=0)ambient=core::StringConverter::toColor(attr->value);
	if((attr=e->getAttribute(mT("Diffuse")))!=0)diffuse=core::StringConverter::toColor(attr->value);
	if((attr=e->getAttribute(mT("Specular")))!=0)specular=core::StringConverter::toColor(attr->value);
	if((attr=e->getAttribute(mT("CastShadows")))!=0)castShadows=core::StringConverter::toBool(attr->value);
	if((attr=e->getAttribute(mT("Enabled")))!=0)enabled=core::StringConverter::toBool(attr->value);
	if((attr=e->getAttribute(mT("Target")))!=0)target=core::StringConverter::toVector3d(attr->value);
}
void LightNodeDesc::saveToXML(xml::XMLElement* e)
{
	ISceneNodeDesc::saveToXML(e);
	e->addAttribute(mT("Radius"),core::StringConverter::toString(radius));
	e->addAttribute(mT("Ambient"),core::StringConverter::toString(ambient));
	e->addAttribute(mT("Diffuse"),core::StringConverter::toString(diffuse));
	e->addAttribute(mT("Specular"),core::StringConverter::toString(specular));
	e->addAttribute(mT("CastShadows"),core::StringConverter::toString(castShadows));
	e->addAttribute(mT("Enabled"),core::StringConverter::toString(enabled));
	e->addAttribute(mT("Target"),core::StringConverter::toString(target));
}
//////////////////////////////////////////////////////////////////////////
void IRenderableNodeDesc::loadFromXML(xml::XMLElement* e)
{
	xml::XMLAttribute* attr;

	if((attr=e->getAttribute(mT("ShadowCaster")))!=0)
		shadowCaster=core::StringConverter::toBool(attr->value);
	if((attr=e->getAttribute(mT("ShadowReceiver")))!=0)
		shadowReceiver=core::StringConverter::toBool(attr->value);
	if((attr=e->getAttribute(mT("CustomRenderGroup")))!=0)
		customRenderGroup=core::StringConverter::toBool(attr->value);
	if((attr=e->getAttribute(mT("UseIdentityMatrix")))!=0)
		useIdentityMatrix=core::StringConverter::toBool(attr->value);
	if((attr=e->getAttribute(mT("TargetRenderGroup")))!=0)
		targetRenderGroup=core::StringConverter::toInt(attr->value);
}
void IRenderableNodeDesc::saveToXML(xml::XMLElement* e)
{
	e->addAttribute(mT("ShadowCaster"),core::StringConverter::toString(shadowCaster));
	e->addAttribute(mT("ShadowReceiver"),core::StringConverter::toString(shadowReceiver));
	e->addAttribute(mT("CustomRenderGroup"),core::StringConverter::toString(customRenderGroup));
	e->addAttribute(mT("UseIdentityMatrix"),core::StringConverter::toString(useIdentityMatrix));
	e->addAttribute(mT("TargetRenderGroup"),core::StringConverter::toString(targetRenderGroup));
}

}
}

