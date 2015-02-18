
#include "stdafx.h"
#include "ICullable.h"
#include "ISceneVisitor.h"


namespace mray{
namespace scene{
 
		
	//////////////////////////////////////////////////////////////////////////

	ICullable::ICullable():m_cullingType(SCT_NONE),m_viewDist(-1)
	{
		CPropertieDictionary* dic=0;
		if(CreateDictionary(&dic))
		{
			dic->addPropertie(&PropertyTypeViewDist::instance,mT("View"));
		}
	}
	void ICullable::OnVisit(ISceneVisitor*visitor)
	{
		visitor->Visit(this);
	}
	//////////////////////////////////////////////////////////////////////////

	IMPLEMENT_PROPERTY_TYPE_HEADER(ViewDist,ICullable,float,mT("ViewDist"),EPBT_Basic,mT("Node's View Distance"),-1);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(ViewDist,ICullable,float,setViewDistance,getViewDistance,core::StringConverter::toString,core::StringConverter::toFloat,false)

}
}

