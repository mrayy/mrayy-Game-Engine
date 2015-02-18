#include "stdafx.h"

#include "GLTextureBuffer.h"
#include "GLTextureUtil.h"
#include "PixelUtil.h"


namespace mray{
namespace video{

GLTextureBuffer::GLTextureBuffer(ETextureType texType,uint texId,int face,int level,EUsageType usage,bool softwareMipmaps)
:GLHardwarePixelBuffer(0,0,0,texType,EPixel_Unkown,usage,texId),m_face(face),m_level(level),
	m_texID(texId),m_softwareMipmaps(softwareMipmaps)
{

	GLint val;

	m_target=GLTextureUtil::getTextureTarget(texType);

	glBindTexture(m_target,m_texID);

	m_targetFace=m_target;
	if(m_target==GL_TEXTURE_CUBE_MAP)
		m_targetFace=GL_TEXTURE_CUBE_MAP_POSITIVE_X + m_face;

	glGetTexLevelParameteriv(m_targetFace,m_level,GL_TEXTURE_WIDTH,&val);
	m_width=val;

	if(m_target==GL_TEXTURE_1D)
		val=1;
	else
		glGetTexLevelParameteriv(m_targetFace,m_level,GL_TEXTURE_HEIGHT,&val);
	m_height=val;

	if(m_target!=GL_TEXTURE_3D)
		val=1;
	else
		glGetTexLevelParameteriv(m_targetFace,m_level,GL_TEXTURE_DEPTH,&val);
	m_depth=val;

	glGetTexLevelParameteriv(m_targetFace,m_level,GL_TEXTURE_INTERNAL_FORMAT,&val);
	m_glFormat=val;
	m_format=GLTextureUtil::getClosestPixelFormat(m_glFormat);

	m_rowPitch=m_width;
	m_slicePitch=m_width*m_height;
	m_sizeInBytes=m_width*m_height*m_depth*PixelUtil::getPixelDescription(m_format).elemSizeB;

	m_buffer=LockedPixelBox(m_width,m_height,m_depth,m_format,0);
	/*
	if(!m_width || !m_height || !m_depth)
		return;*/
}
GLTextureBuffer::~GLTextureBuffer()
{
}

void GLTextureBuffer::download(const LockedPixelBox&b)
{
	if(b.box.getWidth() != getWidth() ||
		b.box.getHeight() != getHeight() ||
		b.box.getDepth() != getDepth())
		FATAL_ERROR(true, mT("only download of entire buffer is supported by GL"));

	glBindTexture( m_target, m_texID );

	if(b.box.getWidth() != b.rowPitch)
		glPixelStorei(GL_PACK_ROW_LENGTH, b.rowPitch);
	else
		glPixelStorei(GL_PACK_ROW_LENGTH, 0);
	if(b.box.getHeight()*b.box.getWidth() != b.slicePitch)
		glPixelStorei(GL_PACK_IMAGE_HEIGHT, (b.slicePitch/b.box.getWidth()));
	else 
		glPixelStorei(GL_PACK_IMAGE_HEIGHT, 0);
	if(((uint)b.box.getWidth()*PixelUtil::getPixelDescription(b.format).elemSizeB) & 3) {
		// Standard alignment of 4 is not right
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
	}else
		glPixelStorei(GL_PACK_ALIGNMENT, 4);
	// We can only get the entire texture
	glGetTexImage(m_targetFace, 0, 
		GLTextureUtil::getGLOrginalFormat(b.format),
		GLTextureUtil::getGLDataType(b.format),b.data);
	// Restore defaults
	glPixelStorei(GL_PACK_ROW_LENGTH, 0);
	glPixelStorei(GL_PACK_IMAGE_HEIGHT, 0);
	glPixelStorei(GL_PACK_ALIGNMENT, 4);
}
void GLTextureBuffer::upload(const LockedPixelBox&b)
{
	glBindTexture(m_target,m_texID);

	if(b.getRowPitchSkip()!=0)
		glPixelStorei(GL_UNPACK_ROW_LENGTH,b.rowPitch);
	if(b.box.getWidth()*b.box.getHeight()!=b.slicePitch)
		glPixelStorei(GL_UNPACK_IMAGE_HEIGHT,b.slicePitch/b.box.getWidth());
	
	if(m_softwareMipmaps){
		GLint components=PixelUtil::getPixelDescription(m_format).componentsCount;
		switch (m_target)
		{
		case GL_TEXTURE_1D:
			gluBuild1DMipmaps(GL_TEXTURE_1D,components,b.box.getWidth(),
				GLTextureUtil::getGLOrginalFormat(b.format),
				GLTextureUtil::getGLDataType(b.format),b.data);
			break;
		case GL_TEXTURE_2D:
		case GL_TEXTURE_CUBE_MAP:
			gluBuild2DMipmaps(m_targetFace,components,b.box.getWidth(),b.box.getHeight(),
				GLTextureUtil::getGLOrginalFormat(b.format),
				GLTextureUtil::getGLDataType(b.format),b.data);
			break;
		case GL_TEXTURE_3D:
			glTexImage3D(GL_TEXTURE_3D,0,components,b.box.getWidth(),b.box.getHeight(),b.box.getDepth(),0,
				GLTextureUtil::getGLOrginalFormat(b.format),
				GLTextureUtil::getGLDataType(b.format),b.data);
			break;
		}
	}else{
		if(((uint)b.box.getWidth()*PixelUtil::getPixelDescription(b.format).elemSizeB) & 3) {
			// Standard alignment of 4 is not right
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		}
		switch (m_target)
		{
		case GL_TEXTURE_1D:
			glTexSubImage1D(GL_TEXTURE_1D,0,b.box.MinP.x,b.box.getWidth(),
				GLTextureUtil::getGLOrginalFormat(b.format),
				GLTextureUtil::getGLDataType(b.format),b.data);
			break;
		case GL_TEXTURE_2D:
		case GL_TEXTURE_CUBE_MAP:
			glTexSubImage2D(m_targetFace,0,b.box.MinP.x,b.box.MinP.y,b.box.getWidth(),b.box.getHeight(),
				GLTextureUtil::getGLOrginalFormat(b.format),
				GLTextureUtil::getGLDataType(b.format),b.data);
			break;
		case GL_TEXTURE_3D:
			glTexSubImage3D(GL_TEXTURE_3D,0,b.box.MinP.x,b.box.MinP.y,b.box.MinP.z,
				b.box.getWidth(),b.box.getHeight(),b.box.getDepth(),
				GLTextureUtil::getGLOrginalFormat(b.format),
				GLTextureUtil::getGLDataType(b.format),b.data);
			break;
		}
	}
	glPixelStorei(GL_UNPACK_ROW_LENGTH,0);
	glPixelStorei(GL_UNPACK_IMAGE_HEIGHT,0);
	glPixelStorei(GL_UNPACK_ALIGNMENT,4);

}

void GLTextureBuffer::bindToFramebuffer(GLenum attachment, uint zoffset)
{
	FATAL_ERROR(zoffset >= m_depth,mT("ZOffset exceded the depth: GLTextureBuffer::bindToFramebuffer()"));
	switch(m_target)
	{
	case GL_TEXTURE_1D:
		glFramebufferTexture1DEXT(GL_FRAMEBUFFER_EXT, attachment,
			m_targetFace, m_texID, 0);
		break;
	case GL_TEXTURE_2D:
	case GL_TEXTURE_CUBE_MAP:
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachment,
			m_targetFace, m_texID, 0);
		break;
	case GL_TEXTURE_3D:
		glFramebufferTexture3DEXT(GL_FRAMEBUFFER_EXT, attachment,
			m_targetFace, m_texID, 0, zoffset);
		break;
	}
}


void GLTextureBuffer::copyFromFramebuffer(uint zoffset){
	glBindTexture(m_target, m_texID);
	switch(m_target)
	{
	case GL_TEXTURE_1D:
		glCopyTexSubImage1D(m_targetFace, 0, 0, 0, 0, m_width);
		break;
	case GL_TEXTURE_2D:
	case GL_TEXTURE_CUBE_MAP:
		glCopyTexSubImage2D(m_targetFace, 0, 0, 0, 0, 0, m_width, m_height);
		break;
	case GL_TEXTURE_3D:
		glCopyTexSubImage3D(m_targetFace, 0, 0, 0, zoffset, 0, 0, m_width, m_height);
		break;
	}
}

}
}