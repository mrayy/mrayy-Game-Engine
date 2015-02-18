
#include "stdafx.h"
#include "SensorCurve.h"

#include "CSpline.h"
#include "LinearCurve.h"


namespace mray
{
namespace VT
{

	class SensorCurveImpl
	{
	protected:
		math::vector2d m_inLimit;
		math::vector2d m_outLimit;

		std::vector<float> m_keys;
		math::CCatmullCurve<float> m_spline;
	public:
		SensorCurveImpl():m_spline(0,0,0),m_inLimit(0,100),m_outLimit(0,100)
		{
		}
		virtual~SensorCurveImpl()
		{
		}
		const std::vector<float>& GetKeys()
		{
			return m_keys;
		}

		const math::vector2d& GetInLimit()
		{
			return m_inLimit;
		}
		const math::vector2d& GetOutLimit()
		{
			return m_outLimit;
		}
		float SampleValue01(float v)
		{
			return m_spline.getPoint(v);
		}
		float ModulateValue(const float& v)
		{

			float t=(math::clamp(v,m_inLimit.x,m_inLimit.y)-m_inLimit.x)/(m_inLimit.y-m_inLimit.x);
			float ret=m_spline.getPoint(t);
			ret=ret*(m_outLimit.y-m_outLimit.x)+m_outLimit.x;
			return ret;
		}

		void LoadFromXML(xml::XMLElement* e)
		{
			if(!e)
				return;
			xml::XMLAttribute* attr;
			m_keys.clear();
			attr=e->getAttribute("InMin");
			if(attr)
				m_inLimit.x=core::StringConverter::toFloat (attr->value);
			attr=e->getAttribute("InMax");
			if(attr)
				m_inLimit.y=core::StringConverter::toFloat (attr->value);

			attr=e->getAttribute("OutMin");
			if(attr)
				m_outLimit.x=core::StringConverter::toFloat (attr->value);
			attr=e->getAttribute("OutMax");
			if(attr)
				m_outLimit.y=core::StringConverter::toFloat (attr->value);

			xml::XMLElement* elem=e->getSubElement("Key");
			while(elem)
			{
				float v=0;
				attr=elem->getAttribute("Value");
				if(attr)
					v=core::StringConverter::toFloat(attr->value);
				m_keys.push_back(v);
				elem=elem->nextSiblingElement("Key");
			}
			m_spline.createCurve(&m_keys[0],m_keys.size(),4);
		}
	};


SensorCurve::SensorCurve()
{
	m_impl=new SensorCurveImpl();
}

SensorCurve::~SensorCurve()
{
	delete m_impl;
}

const math::vector2d& SensorCurve::GetInLimit()
{
	return m_impl->GetInLimit();
}
const math::vector2d& SensorCurve::GetOutLimit()
{
	return m_impl->GetOutLimit();
}
float SensorCurve::SampleValue01(float v)
{
	return m_impl->SampleValue01(v);
}
float SensorCurve::ModulateValue(const float& v)
{
	return m_impl->ModulateValue(v);
}

void SensorCurve::LoadFromXML(xml::XMLElement* e)
{
	m_impl->LoadFromXML(e);
}

const std::vector<float>& SensorCurve::GetKeys()
{
	return m_impl->GetKeys();
}

}
}

