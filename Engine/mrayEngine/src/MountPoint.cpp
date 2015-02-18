#include "stdafx.h"

#include "MountPoint.h"
#include "ISceneVisitor.h"
 
namespace mray{
namespace scene{

MountPoint::MountPoint(const core::string&name)
{
	m_name=name;
	m_parent=0;
	m_scale=1;
}
MountPoint::~MountPoint()
{

}

const core::string& MountPoint::getNodeName()const
{
	return m_name;
}
bool MountPoint::setNodeName(const core::string&name)
{
	m_name=name;
	return true;
}

void MountPoint::removeChild(IMovable*elem)
{
	MovableNodeList::iterator p=m_children.begin();
	MovableNodeList::iterator end=m_children.end();
	for(;p!=end;++p)
	{
		if((*p).pointer()==elem){
			m_children.erase(p);
			return;
		}
	}
}
void MountPoint::addChild(IMovableCRef elem)
{
	m_children.push_back(elem);
	if(elem->getParent()){
		elem->getParent()->removeChild(elem);
	}
	elem->setParent(this);
}
void MountPoint::removeFromParent()
{
	if(getParent())
		getParent()->removeChild(this);
}
const MovableNodeList& MountPoint::getChildren()
{
	return m_children;
}

IMovable* MountPoint::getParent()
{
	return m_parent;
}
void MountPoint::setParent(IMovable*p)
{
	m_parent=p;
}
bool MountPoint::NeedChildUpdate(){
	return true;
}

math::vector3d MountPoint::getAbsolutePosition()const
{
	return getAbsoluteTransformation().getTranslation();

}
math::quaternion MountPoint::getAbsoluteOrintation()const
{
	return m_absOrintaion;
	
}
math::vector3d MountPoint::getAbsoluteScale()const
{
	return 1;
}
const math::vector3d& MountPoint::getPosition()const
{
	return m_position;
}
const math::quaternion& MountPoint::getOrintation()const
{
	return m_orintation;

}
const math::vector3d& MountPoint::getScale()const
{
	return m_scale;
}

void MountPoint::updateRelativeTransformation()
{
	m_orintation.toMatrix(m_relTransformation);
	m_relTransformation.setTranslation(m_position);
}
void MountPoint::updateAbsoluteTransformation(){
	updateRelativeTransformation();
	if(m_parent)
	{
		m_absTransformation=m_parent->getAbsoluteTransformation()*getRelativeTransformation();

		m_absOrintaion=m_parent->getAbsoluteOrintation()*m_orintation;
	}
	else{
		m_absTransformation=getRelativeTransformation();
		m_absOrintaion=m_orintation;
	}
}
math::matrix4x4&MountPoint::getAbsoluteTransformation(){
	return m_absTransformation;

}
const math::matrix4x4&MountPoint::getAbsoluteTransformation()const{
	return m_absTransformation;

}
const math::matrix4x4&MountPoint::getRelativeTransformation()const{
	return m_relTransformation;
}

bool MountPoint::setPosition(const math::vector3d& v){
	m_position=v;
	return true;
}
bool MountPoint::setOrintation(const math::quaternion& v){
	m_orintation=v;
	return true;

}
bool MountPoint::setScale(const math::vector3d& v)
{
	return false;
}

const math::box3d& MountPoint::getBoundingBox(){
	return math::box3d::Empty;
}
math::box3d MountPoint::getTransformedBoundingBox(){
	math::box3d b(-1,1);
	m_absTransformation.transformBox(b);
	return b;
}


}
}

