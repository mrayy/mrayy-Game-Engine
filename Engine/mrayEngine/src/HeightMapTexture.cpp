#include "stdafx.h"


#include "HeightMapTexture.h"
#include "ILogManager.h"
#include "IVideoDevice.h"


namespace mray{
namespace loaders{

HeightMapTexture::HeightMapTexture(){
	m_size=0;
}
HeightMapTexture::~HeightMapTexture(){
}

const core::string& HeightMapTexture::getFileName()
{
	return m_fileName;
}

bool HeightMapTexture::setTexture(video::ITexture* tex){
	if(!tex || !tex->getType()==video::ETT_2DTex)
		return false;

	video::PixelFormat*format= tex->getPixelFormat();
	if(!format)return false;


	m_fileName=tex->getResourceName();

	video::IHardwarePixelBuffer*buff= tex->getSurface(0);

	m_size.x=buff->getWidth();
	m_size.y=buff->getHeight();

	int count=m_size.x*m_size.y;
	m_heightData.resize(count);


	math::box3d box(0,0,0,m_size.x,m_size.y,1);

	video::LockedPixelBox pBox=buff->lock(box,video::IHardwareBuffer::ELO_ReadOnly);

	uchar*data=(uchar*)pBox.data;
	double iscope=format->getColorsScope();
	if(iscope){
		iscope=1.0/iscope;
	}
	else{
		gLogManager.log(mT("Terrain heightmap Format Unkwon: ")+tex->getResourceName(),ELL_WARNING);
		return 0;
	}
	int offset=0;
	for(int i=0;i<count;++i){
		ulong c=0;
		offset+=format->convertToColor(&data[offset],c);
		m_heightData[i]=c*iscope;
	}
	buff->unlock();
	return true;
}

double HeightMapTexture::getHeight(int x,int y){
	if(x>=m_size.x || y>=m_size.y)
		return 0;
	return m_heightData[y*m_size.x+x];
}
int HeightMapTexture::getWidth(){
	return m_size.x;
}
int HeightMapTexture::getHeight(){
	return m_size.y;
}
double* HeightMapTexture::getHeightData(){
	return &m_heightData[0];
}

video::ITexturePtr  HeightMapTexture::GenerateTexture(video::IVideoDevice*device)
{
	video::ITexturePtr  tex=device->createTexture2D(m_size,video::EPixel_LUMINANCE8,true);
	GCPtr<video::IHardwarePixelBuffer> buff=tex->getSurface(0);
	const video::LockedPixelBox&box =buff->lock(math::box3d(0,0,0,m_size.x,m_size.y,1),video::IHardwareBuffer::ELO_Discard);

	uchar*ptr=(uchar*)box.data;
	double*dataPtr=&m_heightData[0];
	for (int i=0;i<m_size.x*m_size.y;++i)
	{
		ptr[i]=(uchar)dataPtr[i]*255;
	}

	buff->unlock();

	return tex;
}


}
}


