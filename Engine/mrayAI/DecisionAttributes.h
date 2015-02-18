

/********************************************************************
	created:	2010/04/30
	created:	30:4:2010   20:59
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\DecisionAttributes.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	DecisionAttributes
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef DecisionAttributes_h__
#define DecisionAttributes_h__

#include "IDecisionAttribute.h"
#include "StringConverter.h"

namespace mray
{
	namespace xml
	{
		class XMLElement;
	}
namespace AI
{


class BooleanDecisionAttrib:public IDecisionAttribute
{
private:
protected:
	bool m_v;
public:

	BooleanDecisionAttrib(const core::string&name):IDecisionAttribute(name)
	{
	}
	virtual ~BooleanDecisionAttrib()
	{
	}

	virtual ushort GetBucket(const void*value)
	{
		return (*(bool*)value)==false ? 0 : 1;
	}
	virtual ushort GetBucketFromString(const core::string&value)
	{
		bool v=core::StringConverter::toBool(value);
		return GetBucket(&v);
	}
	virtual int GetBucketsCount()
	{
		return 2;
	}
	virtual void* GetValue(ushort bucket)
	{
		m_v=(bucket==0) ? false : true;
		return &m_v;
	}

	virtual core::string GetValueString(ushort bucket)
	{
		m_v=(bucket==0) ? false : true;
		return core::StringConverter::toString(m_v,true);
	}
};


template <class T>
class GenericDecisionAttrib:public IDecisionAttribute
{
private:
protected:
	T m_v;
	int m_bucketsCount;
	T m_step;
	T m_min;
	T m_max;
public:


	GenericDecisionAttrib(const core::string&name,const T&min,const T&max,const T&step):IDecisionAttribute(name),
		m_min(min),m_max(max),m_step(step)
	{
		m_bucketsCount=(m_max-m_min)/m_step;
	}
	virtual ~GenericDecisionAttrib()
	{
	}

	virtual ushort GetBucket(const void*value)
	{
		T v=(*(T*)value);
		if(v>m_max)
			v=m_max;
		return (v-m_min)/m_step;
	}

	virtual ushort GetBucketFromString(const core::string&value)
	{
		T v;
		core::StringConverter::parse(value,v);
		return GetBucket(&v);
	}
	virtual int GetBucketsCount()
	{
		return m_bucketsCount;
	}
	virtual void* GetValue(ushort bucket)
	{
		m_v=(bucket)*m_step+m_min;
		return &m_v;
	}
	virtual core::string GetValueString(ushort bucket)
	{
		m_v=(bucket)*m_step+m_min;
		return core::StringConverter::toString(m_v);
	}
};
class EnumDecisionAttrib:public IDecisionAttribute
{
private:
protected:
	std::vector<core::string> m_enumVals;
public:

	EnumDecisionAttrib(const core::string&name);
	virtual ~EnumDecisionAttrib();

	virtual ushort GetBucketFromString(const core::string&value)
	{
		return GetBucket(value.c_str());
	}
	virtual ushort GetBucket(const void*value);
	virtual int GetBucketsCount();
	virtual void* GetValue(ushort bucket);
	virtual core::string GetValueString(ushort bucket);

	void loadXMLSettings(xml::XMLElement* elem);
};

}
}

#endif // DecisionAttributes_h__
