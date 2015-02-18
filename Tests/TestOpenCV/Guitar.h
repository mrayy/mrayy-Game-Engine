
/********************************************************************
	created:	2012/11/06
	created:	6:11:2012   17:45
	filename: 	d:\Development\mrayEngine\Tests\TestOpenCV\Guitar.h
	file path:	d:\Development\mrayEngine\Tests\TestOpenCV
	file base:	Guitar
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __Guitar__
#define __Guitar__




namespace mray
{


class RingBuffer
{
protected:
	int m_capacity;
	int m_first;
	int m_last;
	float *m_data;


	int getIndex(int i)
	{
		while(i<0)
		{
			i+=m_capacity;
		}
		return i%m_capacity;
	
	}
public:
	RingBuffer(int n)
	{
		m_capacity=n;
		m_data=new float[n];
		m_first=0;
		m_last=0;
		for(int i=0;i<n;++i)
			m_data[i]=0;
	}

	~RingBuffer()
	{
		delete [] m_data;
	}

	int size(){return m_capacity;}
	bool empty(){return m_first==m_last;}
	bool isFull()
	{
		return getIndex(m_first-1)==m_last;
	}
	void add(float v)
	{
		if(isFull())
			pop();
		m_data[m_last]=v;
		m_last=getIndex(m_last+1);
	}
	float pop()
	{
		if(empty())
			return 0;
		float v=m_data[m_first];
		m_first=getIndex(m_first+1);
		return v;
	}

	float peek()
	{
		if(empty())
			return 0;
		return m_data[m_first];
	}
};


class GuitarString
{
protected:
	double m_freq;
	RingBuffer* m_buffer;

	int m_ticks;
public:
	GuitarString(double freq);
	~GuitarString();
	void pulk(float t,float strength);
	void tic();
	double sample();
	int time();
};

class Guitar
{
protected:
	std::vector<GuitarString*> m_strings;
public:
	Guitar(){}
	virtual~Guitar(){Clear();}

	void AddString(float freq);

	int GetStringsCount(){return m_strings.size();}

	void Pulk(int i,float t,float strength);

	void Tick();

	void Clear();

	float Sample();
};

}


#endif