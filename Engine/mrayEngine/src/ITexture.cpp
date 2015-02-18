#include "stdafx.h"


#include "ITexture.h"
#include "PixelUtil.h"
#include "TextureResourceManager.h"
#include "AsyncLoadManager.h"
#include "IAsyncLoadRequest.h"


namespace mray{
namespace video{

	class TextureAsyncLoader:public IAsyncLoadRequest
	{
	protected:
		ITexture* m_tex;
	public:
		TextureAsyncLoader(ITexture* tex):m_tex(tex)
		{
		}
		bool StartLoading()
		{
			m_tex->loadInternal();
			m_loaded=true;
			return true;
		}
		void OnFailedLoading()
		{
		}
	};

ITexture::ITexture(ETextureType type,bool internal):m_type(type),m_BilinearFilter(0),m_resourceCreated(false),
m_TrilinearFilter(1), m_MipmapFilter(1), m_AnisotropicFilter(0), m_isRenderTarget(false), m_internal(internal), m_format(EPixel_R8G8B8A8)
{
	m_pixelFormat=new UnkownPixelFormat();
	m_size=1;
	m_loadRequest=0;

	m_maxMipmaps=gTextureResourceManager.GetDefaultMipmapsCount();
}
ITexture::~ITexture()
{
	if(m_loadRequest)
		delete m_loadRequest;
}

void ITexture::AddLoadStream(OS::IStream* stream)
{
	m_loadStreams.push_back(stream);
}
void ITexture::ClearLoadStreams()
{
	m_loadStreams.clear();
}

void ITexture::preLoadInternal(bool async)
{
	if(m_isRenderTarget || m_internal)
	{
		_onLoaded();
		return;
	}
	if(async)
	{
		m_loadRequest=new TextureAsyncLoader(this);
		AsyncLoadManager::getInstance().AddRequest(m_loadRequest,EALP_Medium);
	}
}
void ITexture::loadInternal()
{
	if(m_isRenderTarget || m_internal)
		return;
	for(int i=0;i<m_loadStreams.size();++i)
	{
		if(m_loadStreams[i]!=0)
		{
			m_imageIfo.push_back(gTextureResourceManager.loadImageInfo(m_loadStreams[i],getType(),true));
			m_loadStreams[i]->close();
		}else
		{
			m_imageIfo.push_back(0);
		}
	}
}
void ITexture::postLoadInternal()
{
	if(m_imageIfo.size()>0)
	{
		std::vector<video::ImageInfo*> lst;
		for(int i=0;i<m_imageIfo.size();++i)
			lst.push_back(m_imageIfo[i].pointer());
		loadSurfaces((const video::ImageInfo**)&lst[0],lst.size());

		FIRE_LISTENR_METHOD(OnTextureLoaded,(this));
	}
}

int ITexture::getColorDepth()
{
	return PixelUtil::getPixelDescription(getImageFormat()).componentsCount*8;
}

ETextureType ITexture::getType()
{
	return m_type;
}


const math::vector3di& ITexture::getSize()
{
	return m_size;
}

EPixel_TYPE ITexture::getDataType()const 
{
	return PixelUtil::getPixelDescription(m_format).type;
}
EPixelFormat ITexture::getImageFormat()const
{
	return m_format;
}

bool ITexture::loadSurfaces(const ImageInfo** surfaces,int count){
	if(!surfaces || !count)
		return 0;
	if(!surfaces[0])
		return false;

	m_size=surfaces[0]->Size;
	m_format=surfaces[0]->format;

	_innerCreateTexture();

	if(count>getTexNumberOfFaces())
		count=getTexNumberOfFaces();

	LockedPixelBox box(m_size.x,m_size.y,m_size.z,m_format,0);
	for(int i=0;i<count;++i)
	{
		IHardwarePixelBuffer* buf=getSurface(i);
		box.data=surfaces[i]->imageData;
		buf->blitFromMemory(box);
	}
	return true;
}

int ITexture::getTexNumberOfFaces(){
	if(getType()==ETT_CubeTex)
		return 6;
	return 1;
}

}
}




