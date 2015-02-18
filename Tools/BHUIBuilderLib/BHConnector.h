


/********************************************************************
	created:	2011/12/04
	created:	4:12:2011   12:07
	filename: 	d:\Development\mrayEngine\Tools\BHUIBuilderLib\BHConnector.h
	file path:	d:\Development\mrayEngine\Tools\BHUIBuilderLib
	file base:	BHConnector
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __BHConnector__
#define __BHConnector__

#include "IConnector.h"

namespace mray
{
namespace canvas
{

class BHConnector:public IConnector
{
public:
	enum EConnectorType
	{
		EInput,
		EOutput
	};

private:
protected:
	EConnectorType m_type;
public:
	BHConnector(EConnectorType type):m_type(type)
	{}
	virtual~BHConnector()
	{}
	EConnectorType GetType(){return m_type;}

	virtual EAcceptanceResult Accepts(IConnector* other)
	{
		BHConnector* o=(BHConnector*)other;
		if(GetOwner()==other->GetOwner())
			return EAR_No;	//doesn't accept
		if(o->GetType()==m_type)
			return EAR_No;	//doesn't accept
		if(m_type==EInput)return EAR_Source;	//this is the source
		return EAR_Destination;//this is the destination
	}

};

typedef std::vector<BHConnector*> ConnectorList;

}
}

#endif