
#include "stdafx.h"
#include "BHDecoratorObject.h"

#include "BHCanvasManager.h"
#include "TexturedShape.h"
#include "ColoredShape.h"
#include "TextShape.h"
#include "CanvasUtilities.h"


namespace mray
{
namespace canvas
{


BHDecoratorObject::BHDecoratorObject():m_node(0)
{
}
BHDecoratorObject::~BHDecoratorObject()
{
}

void BHDecoratorObject::_BuildShapes()
{
	math::rectf rc;
	canvas::PointList opoints;
	opoints.push_back(math::vector2d(0,30));
	opoints.push_back(math::vector2d(30,0));
	opoints.push_back(math::vector2d(0,-30));
	opoints.push_back(math::vector2d(-30,0));

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
		video::ImageRegion* r=mngr->GetSymbols()->GetRegion(mT("Decorator"));
		if(r)
			rc=r->GetTextureRect();
		shape->SetTexture(mngr->GetSymbols()->GetTexture()->GetTexture(),rc);
		shape->SetShapePoints(opoints);
		AddShape(shape);
	}
	{
		m_label=new TextShape();
		m_label->SetString(core::string(mT("Decorator")));
		m_label->SetRect(rc);
		AddShape(m_label);
	}
}

void BHDecoratorObject::_BuildConnectors()
{
	IBHCanvasObject::_BuildConnectors();
	AddConnector(new BHConnector(BHConnector::EOutput));
}
void BHDecoratorObject::SetNode(AI::IBHDecorator*node)
{
	m_node=node;
	OnSetNode();
}

void BHDecoratorObject::ConnectorConnected(IConnector* conn,IConnector* other)
{
	BHConnector*c=(BHConnector*)conn;
	c->ClearConnections();
	if(c->GetType()==BHConnector::EOutput)
	{

		IBHCanvasObject*o=(IBHCanvasObject*)other->GetOwner();
		m_node->SetNode(o->GetNode());
	}
}

}
}


