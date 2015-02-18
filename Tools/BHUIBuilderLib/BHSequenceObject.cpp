



#include "stdafx.h"
#include "BHSequenceObject.h"
#include "BHSequenceNode.h"

#include "ColoredShape.h"
#include "TexturedShape.h"
#include "TextShape.h"
#include "CanvasUtilities.h"
#include "BHCanvasManager.h"

namespace mray
{
namespace canvas
{


BHSequenceObject::BHSequenceObject():m_node(0)
{
}
BHSequenceObject::~BHSequenceObject()
{
}

void BHSequenceObject::_BuildShapes()
{
	math::rectf rc;
	canvas::PointList opoints;
	opoints.push_back(math::vector2d(-50,30));
	opoints.push_back(math::vector2d(30,30));
	opoints.push_back(math::vector2d(50,-30));
	opoints.push_back(math::vector2d(-30,-30));
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
		video::ImageRegion* r=mngr->GetSymbols()->GetRegion(mT("Sequence"));
		if(r)
			rc=r->GetTextureRect();
		shape->SetTexture(mngr->GetSymbols()->GetTexture()->GetTexture(),rc);
		shape->SetShapePoints(opoints);
		AddShape(shape);
	}
	{
		m_label=new TextShape();
		m_label->SetString(core::string(mT("Sequence")));
		m_label->SetRect(rc);
		AddShape(m_label);
	}

}

void BHSequenceObject::_BuildConnectors()
{
	IBHCanvasObject::_BuildConnectors();
	AddConnector(new BHConnector(BHConnector::EOutput));

}
void BHSequenceObject::SetNode(AI::BHSequenceNode*node)
{
	m_node=node;
	OnSetNode();
}

void BHSequenceObject::ConnectorConnected(IConnector* conn,IConnector* other)
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