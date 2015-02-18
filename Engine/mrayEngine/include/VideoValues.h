
/********************************************************************
	created:	2009/03/01
	created:	1:3:2009   10:19
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\VideoValues.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	VideoValues
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___VideoValues___
#define ___VideoValues___

#include "IValue.h"
#include "RenderMaterial.h"
#include "ITexture.h"

namespace mray{

//////////////////////////////////////////////////////////////////////////

class MRAY_DLL MaterialValue:public IValue
{
public:
	GCPtr<video::RenderMaterial> value;
public:
	MaterialValue(const core::string&name,GCPtr<video::RenderMaterial> v);

	virtual EValueType getType(){return EVT_MATERIAL;}

	virtual const mchar* toString()const;
	virtual void parse(const core::string&v);
	virtual IValue* duplicate();

	virtual void loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};
//////////////////////////////////////////////////////////////////////////

class MRAY_DLL TextureValue:public IValue
{
public:
	GCPtr<video::TextureUnit> value;
public:
	TextureValue(const core::string&name,GCPtr<video::TextureUnit> v);
	virtual~TextureValue();

	virtual EValueType getType(){return EVT_TEXTURE;}

	virtual const mchar* toString()const;
	virtual void parse(const core::string&v);
	virtual IValue* duplicate();

	virtual void loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};

//////////////////////////////////////////////////////////////////////////
class MRAY_DLL ColorValue:public IValue
{
public:
	video::SColor value;
public:
	ColorValue(const core::string&name,const video::SColor& v);

	virtual EValueType getType(){return EVT_COLOR;}

	virtual const mchar* toString()const;
	virtual void parse(const core::string&v);
	virtual IValue* duplicate();

	virtual void loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};

//////////////////////////////////////////////////////////////////////////
class MRAY_DLL ShaderValue:public IValue
{
public:
	GCPtr<video::GPUShader> value;
public:
	ShaderValue(const core::string&name,GCPtr<video::GPUShader> v);

	virtual EValueType getType(){return EVT_SHADER;}

	virtual const mchar* toString()const;
	virtual void parse(const core::string&v);
	virtual IValue* duplicate();

	virtual void loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};
}


#endif //___VideoValues___
