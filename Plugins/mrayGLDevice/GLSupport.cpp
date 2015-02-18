

#include "stdafx.h"
#include "GLSupport.h"

#include <gl/ExtGL.h>
#include <GL/gl.h>
#include <VideoLoggerSystem.h>
#include <sstream>

namespace mray
{
namespace video
{

	GLSupport* GLSupport::s_instance=0;
GLSupport::GLSupport()
{
	s_instance=this;
}
GLSupport::~GLSupport()
{
}
void GLSupport::getConfigParams(OptionContainer& params)
{
}

bool GLSupport::CheckExtension(const core::string& ext)
{
	if(m_extensionList.find(ext)==m_extensionList.end())
		return false;
	return true;
}
void GLSupport::InitExtensions()
{
	// Set version string
	const GLubyte* pcVer = glGetString(GL_VERSION);


	assert(pcVer && "Problems getting GL version string using glGetString");

	core::string tmpStr ;
	core::char_to_string( (const char*)pcVer,tmpStr);
	gVideoLoggerSystem.log(mT("GL_VERSION = ") + tmpStr,ELL_INFO);

	m_version = tmpStr.substr(0, tmpStr.find(mT(" ")));

	// Get vendor
	const GLubyte* pcVendor = glGetString(GL_VENDOR);
	core::char_to_string( (const char*)pcVendor,tmpStr);
	gVideoLoggerSystem.log(mT("GL_VENDOR = ") + tmpStr,ELL_INFO);
	m_vendor = tmpStr.substr(0, tmpStr.find(mT(" ")));

	// Get renderer
	const GLubyte* pcRenderer = glGetString(GL_RENDERER);
	core::char_to_string( (const char*)pcRenderer,tmpStr);
	gVideoLoggerSystem.log(mT("GL_RENDERER = ") + tmpStr,ELL_INFO);

	// Set extension list
	std::stringstream ext;
	std::string str;

	const GLubyte* pcExt = glGetString(GL_EXTENSIONS);

	assert(pcExt && "Problems getting GL extension string using glGetString");

	core::char_to_string( (const char*)pcExt,tmpStr);
	gVideoLoggerSystem.log(mT("GL_EXTENSIONS = ") + tmpStr,ELL_INFO);

	ext << pcExt;

	while(ext >> str)
	{
		core::char_to_string( str.c_str(),tmpStr);
		m_extensionList.insert(tmpStr);
	}
}


}
}

