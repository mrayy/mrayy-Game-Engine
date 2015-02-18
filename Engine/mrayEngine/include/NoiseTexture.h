
/********************************************************************
	created:	2009/03/10
	created:	10:3:2009   22:43
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\NoiseTexture.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	NoiseTexture
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___NoiseTexture___
#define ___NoiseTexture___

#include "ITexture.h"
#include "ITexture.h"
#include "ITexture.h"
#include <SeamlessNoise.h>

namespace mray{
namespace video{

class MRAY_DLL NoiseTexture
{
protected:
	int m_octaves;
	int m_freq;
	float m_ampl;
	float m_persistance;

	math::SeamlessNoise m_noise;
public:
	NoiseTexture(int octaves,int freq,float ampl,float persistance);
	virtual~NoiseTexture();

	void setOctaves(int o){m_octaves=o;}
	int getOctaves(){return m_octaves;}

	void setFreq(int o){m_freq=o;}
	int getFreq(){return m_freq;}

	void setAmplitude(float o){m_ampl=o;}
	float getAmplitude(){return m_ampl;}

	void setPersistancel(float o){m_persistance=o;}
	float getPersistancel(){return m_persistance;}


	virtual void createImage3D(uchar*data,int w,int h,int d,EPixelFormat format);
	virtual void createImage3D(IHardwarePixelBuffer*surf);
	virtual void createImage2D(uchar*data,int w,int h,EPixelFormat format);
	virtual void createImage2D(IHardwarePixelBuffer*surf);
	virtual void createImage1D(uchar*data,int w,EPixelFormat format);
	virtual void createImage1D(IHardwarePixelBuffer*surf);
};

}
}


#endif //___NoiseTexture___
