#include "stdafx.h"

#include "NoiseTexture.h"
#include "ILogManager.h"
#include "mraySystem.h"


namespace mray{
namespace video{

NoiseTexture::NoiseTexture(int octaves,int freq,float ampl,float persistance){
	m_octaves=octaves;
	m_freq=freq;
	m_ampl=ampl;
	m_persistance=persistance;
}

NoiseTexture::~NoiseTexture(){
}



void NoiseTexture::createImage3D(uchar*data,int w,int h,int d,EPixelFormat format){

	if(format!=EPixel_LUMINANCE8 && format!=EPixel_R8G8B8 && format!=EPixel_R8G8B8A8 ){
		gLogManager.log(mT("NoiseTexture::createImage2D()-Image Format Not Supported!"),ELL_WARNING);
		return;
	}
	int freq=m_freq;
	float ampl=m_ampl;

	double incI,incJ,incK;
	math::vector3d sample;

	int value;
	int c;
	if(format==EPixel_LUMINANCE8)c=1;
	else if(format==EPixel_R8G8B8)c=3;
	else if(format==EPixel_R8G8B8A8)c=4;
	mraySystem::memSet(data,0,w*h*d*c);
	for(int o=0;o<m_octaves;o++,freq*=2,ampl*=m_persistance)
	{
		uchar*ptr=data;
		incI=(double)freq/(float)d;
		incJ=(double)freq/(float)h;
		incK=(double)freq/(float)w;
		sample.z=0;
		for(int i=0;i<d;i++,sample.z+=incI){
			sample.y=0;
			for(int j=0;j<h;j++,sample.y+=incJ){
				sample.x=0;
				for(int k=0;k<w;k++,sample.x+=incK){
					if((*ptr)==255)
						value=255;
					else
						value=math::Min<int>(((m_noise.Noise(sample,freq)+1)*ampl*128),255);
					if((*ptr)+value>255)
						value=255;
					else value+=(*ptr);
					switch(format){
						case EPixel_LUMINANCE8:
							*(ptr++)=value;
							break;
						case EPixel_R8G8B8:
							*(ptr++)=value;
							*(ptr++)=value;
							*(ptr++)=value;
							break;
						case EPixel_R8G8B8A8:
							*(ptr++)=value;
							*(ptr++)=value;
							*(ptr++)=value;
							*(ptr++)=value;
							break;
					}
				}
			}
		}
	}
}
void NoiseTexture::createImage3D(IHardwarePixelBuffer*surf){
	math::box3d b(0,0,0,surf->getWidth(),surf->getHeight(),surf->getDepth());
	LockedPixelBox pb= surf->lock(b,IHardwareBuffer::ELO_Discard);
	createImage3D((uchar*)pb.data,pb.box.getWidth(),pb.box.getHeight(),pb.box.getDepth(),surf->getFormat());
	surf->unlock();
}

void NoiseTexture::createImage2D(uchar*data,int w,int h,EPixelFormat format){

	createImage3D(data,w,h,1,format);
}
void NoiseTexture::createImage2D(IHardwarePixelBuffer*surf){
	math::box3d b(0,0,0,surf->getWidth(),surf->getHeight(),1);
	LockedPixelBox pb= surf->lock(b,IHardwareBuffer::ELO_Discard);
	createImage2D((uchar*)pb.data,pb.box.getWidth(),pb.box.getHeight(),surf->getFormat());
	surf->unlock();
}

void NoiseTexture::createImage1D(uchar*data,int w,EPixelFormat format){

	createImage3D(data,w,1,1,format);
}
void NoiseTexture::createImage1D(IHardwarePixelBuffer*surf){
	math::box3d b(0,0,0,surf->getWidth(),1,1);
	LockedPixelBox pb= surf->lock(b,IHardwareBuffer::ELO_Discard);
	createImage1D((uchar*)pb.data,pb.box.getWidth(),surf->getFormat());
	surf->unlock();
}

}
}
