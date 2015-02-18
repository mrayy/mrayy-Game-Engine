
#include "stdafx.h"
#include "BHActionObject.h"

#include "BHCanvasManager.h"
#include "TexturedShape.h"
#include "ColoredShape.h"
#include "TextShape.h"
#include "CanvasUtilities.h"

namespace mray
{
namespace canvas
{

	IMPLEMENT_RTTI(BHActionObject,ICanvasObject);

BHActionObject::BHActionObject():m_node(0)
{
}
BHActionObject::~BHActionObject()
{
}

void BHActionObject::_BuildShapes()
{
	math::rectf rc;
	canvas::PointList opoints;
	canvas::CanvasUtilities::GenerateEllipse(math::vector2d(50,50),opoints);

	{
		ColoredShape* shape=new ColoredShape();
		video::SColor clrs[]={video::DefaultColors::LightBlue,video::DefaultColors::LightBlue,
			video::DefaultColors::White,video::DefaultColors::White};
		shape->SetColors(clrs);
		shape->SetShapePoints(opoints);

		rc=shape->GetBoundingRect();

		AddShape(shape);
	}
	BHCanvasManager*mngr=(BHCanvasManager*) m_canvasManager;
	if(mngr->GetSymbols())
	{
		TexturedShape* shape=new TexturedShape();
		math::rectf rc(0,0,1,1);
		video::ImageRegion* r=mngr->GetSymbols()->GetRegion(mT("Action"));
		if(r)
			rc=r->GetTextureRect();
		shape->SetTexture(mngr->GetSymbols()->GetTexture()->GetTexture(),rc);
		shape->SetShapePoints(opoints);
		AddShape(shape);
	}
	{
		m_label=new TextShape();
		m_label->SetString(m_node->GetActionName());
		m_label->SetRect(rc);
		AddShape(m_label);
	}

}
void BHActionObject::SetNode(AI::IBHActionNode*node)
{
	m_node=node;
	OnSetNode();
}

void BHActionObject::ConnectorConnected(IConnector* conn,IConnector* other)
{
	BHConnector*c=(BHConnector*)conn;
	c->ClearConnections();
}

bool BHActionObject::SetActionName(const core::string& name)
{
	if(!m_node)
		return false;
	m_node->SetActionName(name);
	return true;
}
core::string BHActionObject::GetActionName()const
{
	if(!m_node)
		return mT("");
	return m_node->GetActionName();
}

//IMPLEMENT_PROPERTY_TYPE_HEADER(ActionName,BHActionObject,core::string,"Action",EPBT_Basic,"","");
//IMPLEMENT_PROPERTY_TYPE_GENERIC(ActionName,BHActionObject,core::string,SetActionName,GetActionName,,,false);

}
}


