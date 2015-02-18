
#include "stdafx.h"
#include "BHSelectorObject.h"
#include "BHSelectorNode.h"

#include "ColoredShape.h"
#include "TextShape.h"
#include "TexturedShape.h"
#include "CanvasUtilities.h"
#include "BHCanvasManager.h"

namespace mray
{
namespace canvas
{


BHSelectorObject::BHSelectorObject():m_node(0)
{
}
BHSelectorObject::~BHSelectorObject()
{
}

void BHSelectorObject::_BuildShapes()
{
	math::rectf rc;
	canvas::PointList opoints;
	opoints.push_back(math::vector2d(0,-30));
	opoints.push_back(math::vector2d(40,30));
	opoints.push_back(math::vector2d(-40,30));
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
		video::ImageRegion* r=mngr->GetSymbols()->GetRegion(mT("Selector"));
		if(r)
			rc=r->GetTextureRect();
		shape->SetTexture(mngr->GetSymbols()->GetTexture()->GetTexture(),rc);
		shape->SetShapePoints(opoints);
		AddShape(shape);
	}
	{
		m_label=new TextShape();
		m_label->SetString(core::string(mT("Selector")));
		m_label->SetRect(rc);
		AddShape(m_label);
	}

}

void BHSelectorObject::_BuildConnectors()
{
	IBHCanvasObject::_BuildConnectors();
	AddConnector(new BHConnector(BHConnector::EOutput));
}
void BHSelectorObject::SetNode(AI::BHSelectorNode*node)
{
	m_node=node;
	OnSetNode();
}

void BHSelectorObject::ConnectorConnected(IConnector* conn,IConnector* other)
{
	BHConnector*c=(BHConnector*)conn;
	bool createConn=true;
	for(int i=0;i<m_output.size();++i)
	{
		BHConnector*cc=(BHConnector*)m_output[i];
		if(cc!=c && cc->GetConnections().size()==0)
		{
			createConn=false;
			break;
		}
	}
	c->ClearConnections();
	if(c->GetType()==BHConnector::EOutput)
	{
		if(createConn)
			AddConnector(new BHConnector(BHConnector::EOutput));
		IBHCanvasObject*o=(IBHCanvasObject*)other->GetOwner();
		m_node->AddNode( o->GetNode());
	}
}


}
}