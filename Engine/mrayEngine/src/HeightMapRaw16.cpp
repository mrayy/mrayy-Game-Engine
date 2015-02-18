#include "stdafx.h"

#include "HeightMapRaw16.h"
#include <math.h>
#include "IVideoDevice.h"
#include "ColorConverter.h"

namespace mray{
namespace loaders{


	HeightMapRaw16::HeightMapRaw16(){
		m_length=0;
}
HeightMapRaw16::~HeightMapRaw16(){
}

const core::string& HeightMapRaw16::getFileName()
{
	return m_fileName;
}
bool HeightMapRaw16::loadHeightMap(OS::IStreamPtr file){
	if(!file)return 0;
	m_fileName=file->getStreamName();
	file->seek(0,OS::ESeek_Set);
	m_length=sqrt((float)file->length()/2);
	int length=m_length*m_length;


	std::vector<ushort> bytes;

	bytes.resize(length);
	int r=file->read(&bytes[0],bytes.size()*sizeof(ushort));
	double iMaxHeight=1.0f/65535.0f;
	m_heightData.resize(m_length*m_length);
	ushort*dataPtr=&bytes[0];
	double*hPtr=&m_heightData[0];
	for (int i=0;i<m_heightData.size();++i)
	{
		hPtr[i]=dataPtr[i]*iMaxHeight;
	}
	return true;
}

double HeightMapRaw16::getHeight(int x,int y){
	if(x>=m_length || y>=m_length )
		return 0;

	return m_heightData[y*m_length+x];
}
int HeightMapRaw16::getWidth(){
	return m_length;
}
int HeightMapRaw16::getHeight(){
	return m_length;
}
double* HeightMapRaw16::getHeightData(){
	return &m_heightData[0];
}
video::ITexturePtr  HeightMapRaw16::GenerateTexture(video::IVideoDevice*device)
{
	video::ITexturePtr  tex=device->createEmptyTexture2D(true);//m_length,video::EPixel_LUMINANCE8);

	const video::ImageInfo* infoArr[1];
	video::ImageInfo info;
	infoArr[0]=&info;

	info.createData(math::vector3d(m_length,m_length,1),video::EPixel_LUMINANCE8);

	uchar*ptr=info.imageData;
	double*dataPtr=&m_heightData[0];
	for (int i=0;i<m_length*m_length;++i)
	{
		ptr[i]=(uchar)dataPtr[i]*255;
	}
	//video::ColorConverter::createNormalMap(&info,1);
	tex->loadSurfaces(infoArr,1);
	/*
	GCPtr<video::IHardwarePixelBuffer> buff=tex->getSurface(0);
	const video::LockedPixelBox&box =buff->lock(math::box3d(0,0,0,m_length,m_length,1),video::IHardwareBuffer::ELO_Discard);


	buff->unlock();*/

	return tex;
}

}
}