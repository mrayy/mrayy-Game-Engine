

/********************************************************************
	created:	2013/02/10
	created:	10:2:2013   0:01
	filename: 	C:\Development\mrayEngine\Engine\mrayEngine\include\FileTextureSource.h
	file path:	C:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	FileTextureSource
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __FileTextureSource__
#define __FileTextureSource__

#include "ITextureSource.h"


namespace mray
{
namespace video
{

class MRAY_DLL FileTextureSource:public ITextureSource
{
	DECLARE_RTTI;
protected:
	core::string m_fileName;
	GCPtr<ITexture> m_texture;
	ETextureType m_type;

	bool _Load();
	bool _Unload();
public:
	static const core::string FactorySourceType;
public:
	FileTextureSource();
	virtual~FileTextureSource();

	virtual const core::string& GetSourceType(){return FactorySourceType;}

	virtual ITextureCRef GetTexture();
	virtual xml::XMLElement* LoadFromXML(xml::XMLElement* e);
};

DECLARE_TEXTURE_FACTORY(FileTextureSource);

}
}


#endif
