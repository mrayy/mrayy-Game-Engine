

#ifndef ___TextureResourceManager___
#define ___TextureResourceManager___

#include "IResourceManager.h"
#include "ISingleton.h"


#include "ITexture.h"

namespace mray
{
namespace loaders
{
	class ITexLoader;
	class ITexWriter;

	class TexLoader;
	class TexWriter;
}
namespace video{
	class IVideoDevice;
}
class MRAY_DLL TextureResourceManager:public IResourceManager,public ISingleton<TextureResourceManager>
{
	video::IVideoDevice*m_device;

	typedef std::map<core::string,video::ImageInfoPtr> ImageInfoMap;

	ImageInfoMap m_imageInfoList;

	loaders::TexLoader* m_texLoader;
	loaders::TexWriter* m_texWriter;

	video::ImageInfoPtr m_def1D;
	video::ImageInfoPtr m_def2D;
	video::ImageInfoPtr m_def3D;
	video::ImageInfoPtr m_defCube;

	int m_defaultMipmapsCount;

protected:
	virtual IResourcePtr createInternal(const core::string& name);
	
	virtual IResourcePtr loadResourceFromFile(OS::IStream* file);

	virtual core::string getDefaultGroup();
public:
	TextureResourceManager(video::IVideoDevice*device);
	virtual~TextureResourceManager();

	void SetDefaultMipmapsCount(int c){m_defaultMipmapsCount=c;}
	int GetDefaultMipmapsCount(){return m_defaultMipmapsCount;}

	void setDefaultTexture1D(video::ImageInfoCRef tex);
	void setDefaultTexture2D(video::ImageInfoCRef tex);
	void setDefaultTexture3D(video::ImageInfoCRef tex);
	void setDefaultTextureCube(video::ImageInfoCRef tex);

	video::ImageInfoPtr loadImageInfo(const core::string&name,video::ETextureType target,bool useDefault=false);
	video::ImageInfoPtr loadImageInfo(OS::IStream*stream,video::ETextureType target,bool useDefault=false);

	video::ImageInfoCRef  getDefaultTexture1D();
	video::ImageInfoCRef  getDefaultTexture2D();
	video::ImageInfoCRef  getDefaultTexture3D();
	video::ImageInfoCRef getDefaultTextureCube();

	void setDevice(video::IVideoDevice* dev);

	void addTextureLoader(loaders::ITexLoader*loader);
	void addTextureWriter(loaders::ITexWriter*writer);

	void removeTextureLoader(const core::string& ext);
	void removeTextureWriter(const core::string& ext);

	loaders::TexLoader* GetLoaders(){return m_texLoader;}
	loaders::TexWriter* GetWriters(){return m_texWriter;}

	video::ITexturePtr  loadTexture1D(const core::string&name,bool useDefault=false);
	video::ITexturePtr  loadTexture2D(const core::string&name,bool useDefault=false);
	video::ITexturePtr  loadTexture3D(const core::string&name,bool useDefault=false);

	//files should be 6 in order <+x,-x,+y,-y,+z,-z>
	video::ITexturePtr  loadTextureCube(const core::string&name,const core::string* files,bool useDefault=false);

	video::ITexturePtr  loadTexture1D(const core::string&name,OS::IStream*stream,bool useDefault=false);
	video::ITexturePtr  loadTexture2D(const core::string&name,OS::IStream*stream,bool useDefault=false);
	video::ITexturePtr  loadTexture3D(const core::string&name,OS::IStream*stream,bool useDefault=false);
	//streams count should be 6 in order <+x,-x,+y,-y,+z,-z>
	video::ITexturePtr  loadTextureCube(const core::string&name,OS::IStream**streams,bool useDefault=false);

	video::ITexturePtr  createTexture1D(const core::string&name,int size,video::EPixelFormat format);
	video::ITexturePtr  createTexture2D(const core::string&name,math::vector2di size,video::EPixelFormat format);
	video::ITexturePtr  createTexture3D(const core::string&name,math::vector3di size,video::EPixelFormat format);
	video::ITexturePtr  createTextureCube(const core::string&name,math::vector2di size,video::EPixelFormat format);

	video::ITexturePtr createEmptyTexture1D(const core::string&name,bool internal);
	video::ITexturePtr createEmptyTexture2D(const core::string&name,bool internal);
	video::ITexturePtr createEmptyTexture3D(const core::string&name,bool internal);
	video::ITexturePtr createEmptyTextureCube(const core::string&name,bool internal);

	virtual void writeResourceToDist(const core::string&resName,const core::string&fileName);
	virtual void writeResourceToDist(const IResourcePtr& resource,const core::string&fileName);
	virtual void writeResourceToDist(ResourceHandle resource,const core::string&fileName);

};
#define gTextureResourceManager mray::TextureResourceManager::getInstance()

}



#endif



