

#include "stdafx.h"
#include "GeomDepthCalculator.h"
#include "GeomDepthRect.h"
#include "OpenNIUtils.h"


namespace mray
{
namespace TBee
{
	//http://www.azerdev.com/wp-content/uploads/2013/02/IGS_Report.pdf
#define NORM_OFFSET -0.2f

GeomDepthCalculator::GeomDepthCalculator()
{
	m_frame = new DepthFrame;
	m_maxFrames = 1;
	m_frame->SetThresholds(0.1, 2.0f);

	m_frameWeights = new float[m_maxFrames];
	float s = 0;
	for (int i = 0; i < m_maxFrames; ++i)
	{
		float w = powf(i + 1, 5);
		m_frameWeights[i] = w;
		s += w;
	}
	for (int i = 0; i < m_maxFrames; ++i)
	{
		m_frameWeights[i] /= s;
	}

	m_calcNormals = true;
}

GeomDepthCalculator::~GeomDepthCalculator()
{
	_clean();
	delete m_frame;
	delete m_frameWeights;
}
void GeomDepthCalculator::_clean()
{

	std::list<DepthFrame::FrameData<ushort>*>::iterator it = m_frames.begin();
	for (; it != m_frames.end();++it)
	{
		delete *it;
	}
	m_frames.clear();
}

void GeomDepthCalculator::SetThresholds(float min, float max)
{
	m_frame->SetThresholds(min,max);
}

float GeomDepthCalculator::GetMinThreshold()
{
	return m_frame->GetMinThreshold();
}
float GeomDepthCalculator::GetMaxThreshold()
{
	return m_frame->GetMaxThreshold();
}

void GeomDepthCalculator::SetDepthFrame(openni::VideoFrameRef& depthFrame)
{
	int w = depthFrame.getWidth();
	int h = depthFrame.getHeight();

	

	const openni::DepthPixel* pDepthRow = (const openni::DepthPixel*)depthFrame.getData();

	int rowSize = depthFrame.getStrideInBytes();
	rowSize /= sizeof(openni::DepthPixel);

	DepthFrame::FrameData<ushort>* frame = 0;
	if (m_frames.size() < m_maxFrames)
	{
		frame = new DepthFrame::FrameData<ushort>();
	}
	else
	{
		frame = *m_frames.begin();
		m_frames.erase(m_frames.begin());
	}
	frame->copyData(pDepthRow, w, h);
	m_frames.push_back(frame);

	m_avgFrame.createData(w,h);

	_averageFrames();

	m_frame->SetRawData(m_avgFrame.Data(), w, h);
	m_frame->CalculateDepth();
	if (m_calcNormals)
		m_frame->CalculateNormals();
	
}


void GeomDepthCalculator::_averageFrames()
{
	int w = m_avgFrame.Size().x;
	int h = m_avgFrame.Size().y;
	ushort* ptr = m_avgFrame.Data();
	memset(ptr, 0, w*h*sizeof(ushort));

	FrameList::iterator it = m_frames.begin();
	for (int i=0; it != m_frames.end(); ++it,++i)
	{
		DepthFrame::FrameData<ushort>* f = *it;
		ushort* ptrF = f->Data();
		ptr = m_avgFrame.Data();
		for (int y = 0; y < h; ++y)
		{
			for (int x = 0; x < w; ++x)
			{
				*ptr += ((float)*ptrF) * m_frameWeights[i];
				++ptr;
				++ptrF;
			}
		}
	}
	/*
	float inv = 1.0f / m_frames.size();
	ptr = m_avgFrame.Data();
	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			*ptr = (ushort)(((float)*ptr)* inv);
			++ptr;
		}
	}*/
}

float GeomDepthCalculator::GetDepthAtPoint(int x, int y)
{
	if (x >= m_frame->GetSize().x || x < 0 || y >= m_frame->GetSize().y || y < 0)
		return 0;
	if (m_frame->GetDepth() == 0)
		return 0;
	return m_frame->GetDepth()[y*m_frame->GetSize().x + x] + NORM_OFFSET;
}
math::vector3d GeomDepthCalculator::GetNormalAtPoint(int x, int y)
{
	if (x >= m_frame->GetSize().x || x < 0 || y >= m_frame->GetSize().y || y < 0)
		return 0;
	if (m_frame->GetNormals() == 0)
		return math::vector3d::ZAxis;
	return m_frame->GetNormals()[y*m_frame->GetSize().x + x];
}

float GeomDepthCalculator::Intersect(const math::vector3d& p)
{
	float d=GetDepthAtPoint(p.x, p.y);
	if (d> p.z)
	{
		return d - p.z;
	}
	return 0;
}


void GeomDepthCalculator::CreateDepthFrame(int w, int h)
{
	m_frame->CreateRawData(w, h);
}
void GeomDepthCalculator::AddDepthRect(const GeomDepthRect* rc)
{
	m_frame->AddFrame(rc->GetRect(), rc->GetFrame());
	m_frame->CalculateDepth();
	if (m_calcNormals)
		m_frame->CalculateNormals();
}

void GeomDepthCalculator::SetAmplitude(float amp)
{
	m_frame->SetNormalAmplitude(amp);
}
float GeomDepthCalculator::Amplitude()
{
	return m_frame->GetNormalAmplitude();
}
int GeomDepthCalculator::Width()
{
	return m_frame->GetSize().x;
}
int GeomDepthCalculator::Height()
{
	return m_frame->GetSize().y;
}
float* GeomDepthCalculator::GetDepthData()
{
	return m_frame->GetDepth();
}

math::vector3d* GeomDepthCalculator::GetNormalData()
{
	return m_frame->GetNormals();
}


}
}