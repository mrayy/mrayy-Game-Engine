

/********************************************************************
	created:	2013/02/09
	created:	9:2:2013   10:31
	filename: 	C:\Development\mrayEngine\Engine\mrayEngine\include\ITextureSource.h
	file path:	C:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	ITextureSource
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __ITextureSource__
#define __ITextureSource__

#include "IObject.h"
#include "ITexture.h"
#include "ObjectFactory.h"

namespace mray
{
namespace video
{
	class ITextureSource;

	class ITextureSourceFactory:public ObjectFactory<ITextureSource>
	{
	public:
		ITextureSourceFactory(){}
	};
#define DECLARE_TEXTURE_FACTORY(type)\
	class TextureSourceFactory_##type:public ITextureSourceFactory\
	{\
	public:\
	TextureSourceFactory_##type(){}\
	const core::string& GetType(){return type::FactorySourceType;}\
	ITextureSource* CreateObject(){return new type();}\
	};
#define IMPLEMENT_TEXTURE_FACTORY(type)



class MRAY_DLL ITextureSource:public IObject
{
	DECLARE_RTTI;
protected:

	bool m_loaded;
	virtual bool _Load()=0;
	virtual bool _Unload()=0;
public:
	ITextureSource(){m_loaded=false;}
	virtual~ITextureSource(){}

	virtual const core::string& GetSourceType()=0;

	bool IsLoaded(){return m_loaded;}
	bool Load()
	{
		if(m_loaded)
			return false;
		m_loaded=true;
		return _Load();
	}
	bool Unload()
	{
		if(!m_loaded)
			return false;
		m_loaded=false;
		return _Unload();
	}

	virtual void Update(float dt){}

	virtual ITextureCRef GetTexture()=0;
	virtual xml::XMLElement* LoadFromXML(xml::XMLElement* e)=0;
};

MakeSharedPtrType(ITextureSource);

}
}


#endif
