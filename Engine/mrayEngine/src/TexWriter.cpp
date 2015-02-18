#include "stdafx.h"



#include "ITexWriter.h"
#include "TexWriter.h"
#include "IVideoDevice.h"
#include "ITexture.h"
#include "StringUtil.h"


namespace mray{
namespace loaders{

TexWriter::TexWriter()
{	
}
TexWriter::~TexWriter()
{	
	WritersMap::iterator it= m_writers.begin();
	for(;it!=m_writers.end();++it)
		delete it->second;
	m_writers.clear();
}



void TexWriter::addWriter(ITexWriter*writer)
{
	core::string str=core::StringUtil::ToLower(writer->getExt());
	m_writers[str]=writer;
}

void TexWriter::removeWriter(const core::string& ext){

	core::string str=core::StringUtil::ToLower(ext);
	WritersMap::iterator it= m_writers.find(str);
	if(it!=m_writers.end())
	{
		delete it->second;
		m_writers.erase(it);
	}
}
ITexWriter* TexWriter::getWriter(const core::string&ext)
{
	core::string str=core::StringUtil::ToLower(ext);
	WritersMap::iterator it= m_writers.find(str);
	if(it!=m_writers.end())
		return it->second;
	return 0;
}
bool TexWriter::write(video::ITexture* tex,OS::IStream* stream,const core::string&ext)
{
	core::string str=core::StringUtil::ToLower(ext);
	WritersMap::iterator it= m_writers.find(str);
	if(it==m_writers.end())
		return 0;
	video::IHardwarePixelBuffer*buff = tex->getSurface(0);
	if(!buff)
		return 0;

	if(it->second->support(tex->getType()))
	{
		video::ImageInfo info;
		info.autoDel=false;
		info.format=buff->getFormat();
		info.imageDataSize=buff->getSizeInBytes();

		info.Size.set(buff->getWidth(),buff->getHeight(),buff->getDepth());
		math::box3d box(0,info.Size);

		const video::LockedPixelBox& pbox=buff->lock(box,video::IHardwareBuffer::ELO_ReadOnly);
		info.imageData=(uchar*)pbox.data;
		bool ret= it->second->write(&info,stream,tex->getType());


		buff->unlock();
		return ret;
	}
	return 0;
}









}
}
