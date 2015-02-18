

/********************************************************************
	created:	2012/12/24
	created:	24:12:2012   13:20
	filename: 	C:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\SoundWaveGenerator.h
	file path:	C:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	SoundWaveGenerator
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __SoundWaveGenerator__
#define __SoundWaveGenerator__




namespace mray
{
namespace VT
{
	class SoundWaveGeneratorImpl;

class SoundWaveGenerator
{
protected:
	SoundWaveGeneratorImpl* m_impl;
public:
	SoundWaveGenerator();
	virtual~SoundWaveGenerator();

	void Init(int freq);
	float Sample();
	void Samples(float* samples,int count);
};

}
}


#endif
