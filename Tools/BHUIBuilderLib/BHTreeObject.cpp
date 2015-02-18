

#include "stdafx.h"
#include "BHTreeObject.h"

#include "BHCanvasManager.h"
#include "TexturedShape.h"
#include "ColoredShape.h"
#include "TextShape.h"
#include "CanvasUtilities.h"

namespace mray
{
namespace canvas
{


BHTreeObject::BHTreeObject():m_tree(0)
{
}
BHTreeObject::~BHTreeObject()
{
}

void BHTreeObject::_BuildConnectors()
{
	AddConnector(new BHConnector(BHConnector::EOutput));
}

void BHTreeObject::_BuildShapes()
{
	math::rectf rc;
	canvas::PointList opoints;
	canvas::CanvasUtilities::GenerateChamferedRectangle(math::vector2d(70,70),20,opoints);

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
		video::ImageRegion* r=mngr->GetSymbols()->GetRegion(mT("Tree"));
		if(r)
			rc=r->GetTextureRect();
		shape->SetTexture(mngr->GetSymbols()->GetTexture()->GetTexture(),rc);
		shape->SetShapePoints(opoints);
		AddShape(shape);
	}
	{
		m_label=new TextShape();
		m_label->SetString(core::string(mT("Tree")));
		m_label->SetRect(rc);
		AddShape(m_label);
	}

}
void BHTreeObject::SetTree(AI::IBehaviorTree*c)
{
	m_tree=c;
	OnSetNode();
}

void BHTreeObject::ConnectorConnected(IConnector* conn,IConnector* other)
{
	BHConnector*c=(BHConnector*)conn;
	c->ClearConnections();
	IBHCanvasObject*o=(IBHCanvasObject*)other->GetOwner();
	m_tree->SetRootNode(o->GetNode());
}


}
}

