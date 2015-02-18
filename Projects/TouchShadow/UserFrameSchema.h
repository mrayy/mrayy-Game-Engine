
/********************************************************************
	created:	2013/09/08
	created:	8:9:2013   0:50
	filename: 	C:\Development\mrayEngine\Projects\TouchShadow\UserFrameSchema.h
	file path:	C:\Development\mrayEngine\Projects\TouchShadow
	file base:	UserFrameSchema
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __UserFrameSchema__
#define __UserFrameSchema__




namespace mray
{

class InteractionAreaSchema
{
protected:
	core::string m_name;
	math::rectf m_percentageArea;
	bool m_abs;
	nite::JointType m_relToJoint;
public:
	InteractionAreaSchema():m_abs(true)
	{
	}
	bool IsAbs()const{return m_abs;}
	nite::JointType GetTargetJoint()const{return m_relToJoint;}
	const core::string&GetName()const {return m_name;}
	const math::rectf& GetArea()const{return m_percentageArea;}

	bool LoadFromXML(xml::XMLElement* e);
};
class UserFrameSchema
{
protected:
	core::string m_name;
	std::vector<InteractionAreaSchema> m_areas;
public:
	UserFrameSchema()
	{
	}
	const core::string& GetName()const{return m_name;}
	const std::vector<InteractionAreaSchema>& GetAreaSchema()const {return m_areas;}

	void LoadFromXML(xml::XMLElement* e);
};
}


#endif
