

/********************************************************************
	created:	2010/04/30
	created:	30:4:2010   20:53
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\IDecisionAttribute.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	IDecisionAttribute
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	Decision Attribute holds a scheme for an attribute
*********************************************************************/

#ifndef IDecisionAttribute_h__
#define IDecisionAttribute_h__

#include "mTypes.h"
#include "mstring.h"

namespace mray
{
namespace AI
{

class IDecisionAttribute
{
private:
protected:
	core::string m_name;
public:
	IDecisionAttribute(const core::string&name):m_name(name)
	{}
	virtual ~IDecisionAttribute(){}

	void SetName(const core::string&name){m_name=name;}
	const core::string& GetName()const{return m_name;}

	virtual ushort GetBucket(const void*value)=0;
	virtual ushort GetBucketFromString(const core::string& value)=0;
	virtual int GetBucketsCount()=0;
	virtual void* GetValue(ushort bucket)=0;
	virtual core::string GetValueString(ushort bucket)=0;

	template<class T>
	ushort GetBucketTyped(const T&v)
	{
		return GetBucket(&v);
	}

	template<class T>
	T GetTypedValue(ushort bucket)
	{
		T v;
		return *(T*)GetValue(bucket);
	}
	
};

}
}

#endif // IDecisionAttribute_h__
