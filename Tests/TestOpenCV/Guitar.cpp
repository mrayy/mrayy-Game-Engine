

#include "stdafx.h"
#include "Guitar.h"

namespace mray
{

GuitarString::GuitarString(double freq)
{
#define sample_rate 44100.0f
	if(freq==0)
		freq=1;
	int N=ceil(sample_rate/freq);
	
	m_freq=freq;

	m_buffer=new RingBuffer(N);
	m_ticks=0;
}
GuitarString::~GuitarString()
{
	delete m_buffer;
}
void GuitarString::pulk(float t,float strength)
{
	t=math::clamp<float>(t,0,1);
	int index=t*(m_buffer->size()-1);
	math::Randomizer rd;
	for(int i=0;i<m_buffer->size();++i)
	{
		float r=1;//-(float)abs(i-index)/(float)m_buffer->size();
		m_buffer->add((rd.rand01()-0.5f)*r*strength);
	}
}

float function1(float v1,float v2)
{
	return (v1+v2)*0.5*0.994;
}
float function2(float v1,float v2)
{
	return (v1+v2)*0.5;
}
void GuitarString::tic()
{
	m_ticks++;
	float v1=m_buffer->pop();
	float v2=m_buffer->peek();


	m_buffer->add(function1(v1,v2));

}
double GuitarString::sample()
{
	return m_buffer->peek();
}
int GuitarString::time()
{
	return m_ticks;
}



void Guitar::AddString(float freq)
{
	GuitarString* s=new GuitarString(freq);
	m_strings.push_back(s);
}

void Guitar::Pulk(int i,float t,float strength)
{
	if(i>=m_strings.size())
		return;
	m_strings[i]->pulk(t,strength);
}
void Guitar::Tick()
{
	for(int i=0;i<m_strings.size();++i)
	{
		m_strings[i]->tic();
	}
}

void Guitar::Clear()
{
	for(int i=0;i<m_strings.size();++i)
	{
		delete m_strings[i];
	}
	m_strings.clear();
}

float Guitar::Sample()
{
	float v=0;
	for(int i=0;i<m_strings.size();++i)
	{
		v+=m_strings[i]->sample();
	}

	//if(m_strings.size()>0)
	//	v/=(float)m_strings.size();

	return v;
}

}

