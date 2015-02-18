


/********************************************************************
	created:	2011/12/03
	created:	3:12:2011   15:18
	filename: 	d:\Development\mrayEngine\Engine\ShaderBuilderLib\ShaderConnector.h
	file path:	d:\Development\mrayEngine\Engine\ShaderBuilderLib
	file base:	ShaderConnector
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __ShaderConnector__
#define __ShaderConnector__

#include "IConnector.h"

namespace mray
{
namespace canvas
{

	enum EConnectorType
	{
		EInput,
		EOutput
	};

class ShaderConnector:public IConnector
{
private:
protected:
	EConnectorType m_type;
	video::EUniformType m_valueType;
public:
	ShaderConnector(EConnectorType type,video::EUniformType valType):m_type(type),m_valueType(valType)
	{
		if(valType==EInput)
			m_bgColor=video::DefaultColors::LightBlue;
		else
			m_bgColor=video::DefaultColors::LightGreen;
	}
	virtual~ShaderConnector()
	{
	}

	EConnectorType GetType(){return m_type;}

	video::EUniformType GetValueType(){return m_valueType;}

	virtual bool Accepts(IConnector* other)
	{
		ShaderConnector* o=(ShaderConnector*)other;
		return o->GetType()!=m_type && o->GetValueType()==m_valueType; 
	}
};

}
}

#endif
