

/********************************************************************
	created:	2011/05/02
	created:	2:5:2011   17:46
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\CustomTexWriter.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	CustomTexWriter
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __CustomTexWriter__
#define __CustomTexWriter__


#include "ITexWriter.h"

namespace mray{
namespace loaders{

class MRAY_DLL CustomTexWriter:public ITexWriter
{
private:
protected:
	static const core::string m_typeExt;
	static const core::string m_typeDesc;
public:
	CustomTexWriter();
	virtual~CustomTexWriter(){}

	virtual bool write(video::ImageInfo*tex,OS::IStream* file,video::ETextureType target);
	virtual bool canWrite(const  core::string&filename);
	virtual const core::string& getExt();

	virtual const core::string& getDescription();
	virtual bool support(video::ETextureType type);

};


}
}

#endif
