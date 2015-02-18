
#include "stdafx.h"
#include "SoundWaveGenerator.h"
#include "SineWave.h"


namespace mray
{
namespace VT
{

class SoundWaveGeneratorImpl
{
protected:
	stk::SineWave m_sinWave;
public:
	SoundWaveGeneratorImpl()
	{
		Init(44100);
	}

	void Init(int freq)
	{
		m_sinWave.setFrequency(freq);
	}
	float Sample()
	{
		return m_sinWave.tick();
	}
	void Samples(float* samples,int count)
	{
		for(int i=0;i<count;++i,++samples)
			*samples=m_sinWave.tick();
	}

};

SoundWaveGenerator::SoundWaveGenerator()
{
	m_impl=new SoundWaveGeneratorImpl();
}
SoundWaveGenerator::~SoundWaveGenerator()
{
	delete m_impl;
}

void SoundWaveGenerator::Init(int freq)
{
	m_impl->Init(freq);
}
float SoundWaveGenerator::Sample()
{
	return m_impl->Sample();
}
void SoundWaveGenerator::Samples(float* samples,int count)
{
	m_impl->Samples(samples,count);
}

}
}


