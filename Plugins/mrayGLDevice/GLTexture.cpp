#include "stdafx.h"

#include "GLTexture.h"
#include "GLTextureUtil.h"
#include "PixelUtil.h"
#include "GLTextureBuffer.h"

#include "Engine.h"
#include "GLDev.h"
#include "DeviceCapabilites.h"

namespace mray{
namespace video{


GLTexture::GLTexture(ETextureType type,bool internal)
:ITexture(type,internal)
{
	m_TextureID=0;
	m_BufferID = 0;
	m_TextureChanged=0;
	m_isRenderTarget=0;
	m_dirty=false;


	m_target = GLTextureUtil::getTextureTarget(type);
}
GLTexture::~GLTexture()
{
	unload();
}
void GLTexture::preLoadInternal(bool async)
{
	ITexture::preLoadInternal(async);
	if(!m_TextureID)
		glGenTextures(1,&m_TextureID);
}
void GLTexture::unloadInternal(bool removeTexture)
{
	for (int i=0;i<m_surfaces.size();++i)
	{
		delete m_surfaces[i];
	}
	m_surfaces.clear();

	m_TextureChanged=true;

	m_resourceCreated=false;
	if(removeTexture)
	{
		glDeleteTextures(1,&m_TextureID);
		glDeleteBuffers(1, &m_BufferID);
		m_TextureID=0;
		m_BufferID = 0;
	}

	FIRE_LISTENR_METHOD(OnTextureUnloaded,(this));
}
void GLTexture::unloadInternal()
{
	IResource::unloadInternal();
	unloadInternal(true);
}

uint GLTexture::getTextureID()
{
	_onResourceUsed();
	return m_TextureID;
}

uint GLTexture::calcSizeInternal()
{

	uint sz=sizeof(GLTexture);
	for (int i=0;i<m_surfaces.size();++i)
	{
		sz+=m_surfaces[i]->getSizeInBytes();
	}
	return sz;
}

int GLTexture::getSurfaceCount()
{
	return m_surfaces.size();
}
void GLTexture::createTexture(const math::vector3di&sz,EPixelFormat pf)
{
	if(m_size==sz && m_format==pf)
		return;
	unloadInternal(false);
	m_format=pf;
	m_size=sz;
	_innerCreateTexture();

}
void GLTexture::_innerCreateTexture()
{
	if(m_resourceCreated)
		return;

	m_dirty=true;

	DeviceCapabilites*devCaps=Engine::getInstance().getDevice()->getCapabilities();

	GLenum dataType=GLTextureUtil::getGLDataType(m_format);
	GLenum format=GLTextureUtil::getGLOrginalFormat(m_format);
	GLenum iformat=GLTextureUtil::getClosestFormat(m_format);

	uint width=m_size.x;
	uint height=m_size.y;
	uint depth=m_size.z;

	int surfaces=1;

	if(!m_TextureID)
		glGenTextures(1,&m_TextureID);
	glBindTexture(m_target,m_TextureID);

	glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	int maxMips=GLTextureUtil::getMaxMipmaps(width,height,depth);
	if (m_maxMipmaps>maxMips)
	{
		m_maxMipmaps=maxMips;
	}
	glTexParameteri(m_target,GL_TEXTURE_MAX_LEVEL,m_maxMipmaps);


	if (m_BufferID == 0)
	{
		glGenBuffers(1, &m_BufferID);

		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, m_BufferID);
		uint DATA_SIZE = width*height*depth*PixelUtil::getPixelDescription(m_format).elemSizeB;
		glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, DATA_SIZE, 0, GL_STREAM_DRAW_ARB);
		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
	}

	bool hardwareMipmaps=devCaps->isFeatureSupported(EDF_AutoMipmap);
	bool doSoftwareMipmaps=m_MipmapFilter && m_maxMipmaps>0;// && !hardwareMipmaps;

	switch (m_type)
	{
	case ETT_1DTex:
		glTexImage1D(GL_TEXTURE_1D,0,iformat,width,0,format,dataType,0);
		break;
	case ETT_2DTex:
		glTexImage2D(GL_TEXTURE_2D,0,iformat,width,height,0,format,dataType,0);
		break;
	case ETT_3DTex:
		glTexImage3D(GL_TEXTURE_3D,0,iformat,width,height,depth,0,format,dataType,0);
		break;
	case ETT_CubeTex:
		for (int i=0;i<6;i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0,iformat,width,height,0,format,dataType,0);
		}
		surfaces=6;
		break;
	}

	for (int i=0;i<surfaces;++i)
	{
		IHardwarePixelBuffer* surf=new GLTextureBuffer(m_type,m_TextureID,m_BufferID,i,0,IHardwareBuffer::EUT_Dynamic,doSoftwareMipmaps);
		m_surfaces.push_back(surf);
	}


	glBindTexture(m_target,0);

	m_resourceCreated=true;

	FIRE_LISTENR_METHOD(OnTextureChanged,(this));
}


void GLTexture::onUse()
{
	_onResourceUsed();
	if(m_dirty)
	{

		glBindTexture(m_target,m_TextureID);
		if(m_isRenderTarget){
			if(m_MipmapFilter){
				glGenerateMipmapEXT(m_target);
			}else{
				glTexParameteri(m_target,GL_TEXTURE_MIN_FILTER,(getTrilinearFilter() || getBilinearFilter()) ? GL_LINEAR : GL_NEAREST);
			}
		}else{
			if(m_MipmapFilter){
				glTexParameteri(m_target,GL_TEXTURE_MIN_FILTER,
					getTrilinearFilter()  ? GL_LINEAR_MIPMAP_LINEAR : getBilinearFilter() ? GL_LINEAR_MIPMAP_NEAREST 
					: GL_NEAREST_MIPMAP_NEAREST);
			}else{
				glTexParameteri(m_target,GL_TEXTURE_MIN_FILTER,(getTrilinearFilter() || getBilinearFilter()) ? GL_LINEAR : GL_NEAREST);
			}

		}

		if(m_MipmapFilter){
			glTexParameteri(m_target,GL_TEXTURE_MIN_FILTER,
				getTrilinearFilter()  ? GL_LINEAR_MIPMAP_LINEAR : getBilinearFilter() ? GL_LINEAR_MIPMAP_NEAREST 
				: GL_NEAREST_MIPMAP_NEAREST);
		}
		glBindTexture(m_target,0);

		m_dirty=false;
	}
}

ETextureType GLTexture::getType()
{
	return m_type;
}

const math::vector3di& GLTexture::getSize()
{
	return this->m_size;
}


IHardwarePixelBuffer* GLTexture::getSurface(int face)
{
	_onResourceUsed();
	FATAL_ERROR(face >= m_surfaces.size(), (core::string(mT("face>= m_surfaces.size() : GLTexture::getSurface()")) + this->getResourceName()).c_str());
	return m_surfaces[face];
}


const core::string& GLTexture::getDeviceType()const
{
	return GLDev::m_deviceType;
}


ITexture*GLTexture::getDuplicate()
{
	return NULL;
}


void GLTexture::setRenderTarget(bool rt){
	m_isRenderTarget=rt;

	if(m_isRenderTarget){
		if(m_MipmapFilter){
			glBindTexture(m_target,m_TextureID);
			glGenerateMipmapEXT(m_target);
			glBindTexture(m_target,0);
		}
	}
}
bool GLTexture::isRenderTarget(){
	return m_isRenderTarget;
}

void GLTexture::setMipmapsFilter(bool e)
{
	//_onResourceUsed();
	ITexture::setMipmapsFilter(e);
	m_dirty=true;

}
void GLTexture::setBilinearFilter(bool e)
{
	//_onResourceUsed();

	ITexture::setBilinearFilter(e);
	m_dirty=true;
}

void GLTexture::setTrilinearFilter(bool e)
{
	//_onResourceUsed();
	ITexture::setTrilinearFilter(e);
	m_dirty=true;
}

void GLTexture::setAnisotropicFilter(bool e)
{
	ITexture::setAnisotropicFilter(e);
	
}

}
}

