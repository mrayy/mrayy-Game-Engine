#include "stdafx.h"

#include "GLPBufferTexture.h"
#include "GLDev.h"


namespace mray{
namespace video{


GLPBufferTexture::GLPBufferTexture(GLDev*dev,GLPBuffer*pbuff,int ibuffer)
:GLTexture(ETT_2DTex,true)
{
	m_device=dev;

	setPBuffer(pbuff,ibuffer);

	m_size.x=pbuff->getSize().x;
	m_size.y=pbuff->getSize().y;
	m_size.z=1;
/*
	imageInfo->Size.x=m_pbuff->getSize().x;
	imageInfo->Size.y=m_pbuff->getSize().y;
*/
	create();
}
GLPBufferTexture::~GLPBufferTexture(){

}

void GLPBufferTexture::setPBuffer(GLPBuffer*pbuff,int ibuffer)
{

	m_pbuff=pbuff;
	m_ibuffer=ibuffer;
}

void GLPBufferTexture::create(){
	glGenTextures(1, &m_TextureID);
	glBindTexture(m_target, m_TextureID);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	setMipmapsFilter(false);
}
void GLPBufferTexture::onUse(){
	m_pbuff->bind(m_ibuffer);
	m_device->setCurrentPBufferTexture(this);
	GLTexture::onUse();
}
void GLPBufferTexture::unuse(){
	glBindTexture(m_target, m_TextureID);
	m_pbuff->release(m_ibuffer);
}

}
}
