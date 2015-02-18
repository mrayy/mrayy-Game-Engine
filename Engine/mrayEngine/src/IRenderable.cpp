
#include "stdafx.h"
#include "IRenderable.h"
#include "ISceneVisitor.h"
#include "SMesh.h"
#include "ISceneNode.h"


namespace mray{
namespace scene{




IRenderable::IRenderable():m_layer(0),m_shadowCaster(0),m_shadowReciver(0),
m_owner(0),m_targetRenderGroup(0),m_customRenderGroup(false),m_useIdentityMatrix(false)
{
	CPropertieDictionary* dic=0;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeShadowCaster::instance,mT("Render"));
		dic->addPropertie(&PropertyTypeShadowReciver::instance,mT("Render"));
	}
}

void IRenderable::SetUseIdentityMatrix(bool i)
{
	m_useIdentityMatrix=i;
}
bool IRenderable::IsUseIdentityMatrix()
{
	return m_useIdentityMatrix;
}

int IRenderable::GetTargetRenderGroup()
{
	return m_targetRenderGroup;
}

void IRenderable::SetTargetRenderGroup(int g)
{
	m_targetRenderGroup=g;
}


bool IRenderable::HasCustomRenderGroup()
{
	return m_customRenderGroup;
}

void IRenderable::SetHasCustomRenderGroup(bool e)
{
	m_customRenderGroup=e;
}
SMesh* IRenderable::getMesh()
{
	return 0;
}
void IRenderable::setOwner(ISceneNode*o)
{
	if(m_owner)
		_OnDeattachedFromOwner();
	m_owner=o;
	if(m_owner)
		_OnAttachedToOwner();
}
math::box3d IRenderable::getBoundingBox()
{
	if(getMesh())
		getMesh()->getBoundingBox();
	return math::box3d(0,0);
}
void IRenderable::NotifyChangeBoundingBox()
{
	if(m_owner)
	{
		if(getMesh())
			getMesh()->calcCurrBoundingBox();
		m_owner->_UpdateBoundingBox();
	}
}

void IRenderable::OnVisit(ISceneVisitor*visitor)
{
	visitor->Visit(this);
}

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(ShadowCaster,IRenderable,bool,mT("ShadowCaster"),EPBT_Struct,mT("Is Shadow Caster"),false);
IMPLEMENT_PROPERTY_TYPE_GENERIC(ShadowCaster,IRenderable,bool,setShadowCaster,isShadowCaster,core::StringConverter::toString,core::StringConverter::toBool,false)

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(ShadowReciver,IRenderable,bool,mT("ShadowReciver"),EPBT_Struct,mT("Is Shadow Reciver"),false);
IMPLEMENT_PROPERTY_TYPE_GENERIC(ShadowReciver,IRenderable,bool,setShadowReciver,isShadowReciver,core::StringConverter::toString,core::StringConverter::toBool,false)



}
}

