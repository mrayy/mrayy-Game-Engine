

#include "stdafx.h"
#include "UserFrameSchema.h"
#include "AppCommon.h"




namespace mray
{

	bool InteractionAreaSchema::LoadFromXML(xml::XMLElement* e)
	{
		m_name= e->getValueString("Name");
		core::string targetJoint=e->getValueString("Joint");
		if(targetJoint!="")
		{
			for(int i=0;i<15;++i)
			{
				if(targetJoint.equals_ignore_case(JointNames[i]))
				{
					m_abs=false;
					m_relToJoint=(nite::JointType)i;
					break;;
				}
			}
		}

		xml::XMLAttribute*attr=e->getAttribute("Rect");
		if(!attr)
			return false;
		m_percentageArea=core::StringConverter::toRect(attr->value);
		return true;
	}
	void UserFrameSchema::LoadFromXML(xml::XMLElement* e)
	{
		m_areas.clear();
		m_name=e->getValueString("Name");
		xml::XMLElement* elem=e->getSubElement("Area");
		while (elem)
		{
			InteractionAreaSchema area;
			if(area.LoadFromXML(elem))
				m_areas.push_back(area);
			elem=elem->nextSiblingElement("Area");
		}
	}
}