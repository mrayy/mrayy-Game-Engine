

#include "stdafx.h"
#include "IBHCanvasObject.h"



namespace mray
{
namespace canvas
{


IBHCanvasObject::IBHCanvasObject()
{
	m_label=0;
}
IBHCanvasObject::~IBHCanvasObject()
{
	AddConnector(new BHConnector(BHConnector::EOutput));
}
void IBHCanvasObject::_BuildConnectors()
{
	m_input=new BHConnector(BHConnector::EInput);
	AddConnector(m_input);
}

void IBHCanvasObject::OnSetNode()
{
	ClearShapes();
	ClearConnectors();

	_BuildShapes();
	_BuildConnectors();

}

void IBHCanvasObject::AddConnector(IConnector* s)
{
	BHConnector*c=(BHConnector*)s;
	if(c->GetType()==BHConnector::EInput)
	{
		m_input=c;
		c->SetPlacement(ECP_Top);
		c->SetRelativePos(0.5);
	}
	else{
		m_output.push_back(c);
		c->SetPlacement(ECP_Bot);
		float step=1.0f/(m_output.size()+1);
		for(int i=0;i<m_output.size();++i)
		{
			m_output[i]->SetRelativePos(step*(i+1.0f));
		}
	}
	ICanvasObject::AddConnector(c);

}
void IBHCanvasObject::ClearConnectors()
{
	m_input=0;
	m_output.clear();
	ICanvasObject::ClearConnectors();
}

}
}
