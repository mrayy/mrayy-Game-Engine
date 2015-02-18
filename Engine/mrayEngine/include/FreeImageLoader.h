
/********************************************************************
	created:	2009/02/13
	created:	13:2:2009   18:12
	filename: 	i:\Programing\GameEngine\mrayEngine\FreeImage\FreeImageLoader.h
	file path:	i:\Programing\GameEngine\mrayEngine\FreeImage
	file base:	FreeImageLoader
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___FreeImageLoader___
#define ___FreeImageLoader___

#include <ITexLoader.h>
#include <FreeImage.h>

namespace mray{
namespace loaders{

class FreeImageLoader:public ITexLoader
{
	void initialize();
	void shutdown();

	FREE_IMAGE_FORMAT m_FTType;
	core::string m_ext;
	core::string m_extDesc;
	bool m_isDummy;
public:
	FreeImageLoader();
	FreeImageLoader(const core::string& ext,FREE_IMAGE_FORMAT ftType);
	virtual~FreeImageLoader();
	virtual bool load(OS::IStream* file,video::ImageInfo* tex,video::ETextureType target);
	virtual bool load(const  core::string&name,video::ImageInfo* tex,video::ETextureType target);
	virtual bool canLoad(const core::string& fileName);
	virtual bool canLoad(OS::IStream* file);

	virtual bool support(video::ETextureType type);

	virtual const core::string& getExt(){return m_ext;}
	virtual const core::string& getDescription();
};

}
}


#endif //___FreeImageLoader___
