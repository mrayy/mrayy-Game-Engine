#include "stdafx.h"

#include "HeightMapRaw8.h"
#include <math.h>
#include "IVideoDevice.h"

namespace mray{
namespace loaders{


HeightMapRaw8::HeightMapRaw8(){
	m_length=0;
}
HeightMapRaw8::~HeightMapRaw8(){
}

const core::string& HeightMapRaw8::getFileName()
{
	return m_fileName;
}
bool HeightMapRaw8::loadHeightMap(OS::IStreamPtr file){
	if(!file)return 0;
	m_fileName=file->getStreamName();
	file->seek(0,OS::ESeek_End);
	m_length=sqrt((float)file->getPos());
	file->seek(0,OS::ESeek_Set);

	std::vector<uchar> bytes;

	bytes.resize(m_length*m_length);
	file->read(&bytes[0],bytes.size());
	double iMaxHeight=1.0f/255.0f;
	m_heightData.resize(m_length*m_length);
	uchar*dataPtr=&bytes[0];
	double*hPtr=&m_heightData[0];
	for (int i=0;i<m_heightData.size();++i)
	{
		hPtr[i]=dataPtr[i]*iMaxHeight;
	}
	return true;
}

double HeightMapRaw8::getHeight(int x,int y){
	if(x>=m_length || y>=m_length )
		return 0;

	return m_heightData[y*m_length+x];
}
int HeightMapRaw8::getWidth(){
	return m_length;
}
int HeightMapRaw8::getHeight(){
	return m_length;
}
double* HeightMapRaw8::getHeightData(){
	return &m_heightData[0];
}
video::ITexturePtr  HeightMapRaw8::GenerateTexture(video::IVideoDevice*device)
{
	video::ITexturePtr  tex=device->createTexture2D(m_length,video::EPixel_LUMINANCE8,true);
	GCPtr<video::IHardwarePixelBuffer> buff=tex->getSurface(0);
	const video::LockedPixelBox&box =buff->lock(math::box3d(0,0,0,m_length,m_length,1),video::IHardwareBuffer::ELO_Discard);

	uchar*ptr=(uchar*)box.data;
	double*dataPtr=&m_heightData[0];
	for (int i=0;i<m_length*m_length;++i)
	{
		ptr[i]=(uchar)dataPtr[i]*255;
	}

	buff->unlock();

	return tex;
}

}
}