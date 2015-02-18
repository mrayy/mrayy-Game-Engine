

#include "stdafx.h"
#include "GeomDepthCalculator.h"
#include "OpenNIUtils.h"


namespace mray
{
namespace AugTel
{
	//http://www.azerdev.com/wp-content/uploads/2013/02/IGS_Report.pdf

	void GeomDepthCalculator::DepthFrame::copyData(const ushort* ptr, int w, int h)
	{
		memcpy(data, ptr, w*h*sizeof(ushort));
	}

#define NORM_DEVISION 1000.0f
#define NORM_OFFSET 0.0f

GeomDepthCalculator::GeomDepthCalculator()
{
	m_normalizedDepth = 0;
	m_width = m_height = 0;
	m_amplitude = 2.0f;
	m_normalData = 0;
	m_minDepthDistance = 850;
	m_maxDepthDistance = 4000;
	m_maxDepthDistanceOffset = 3150;
	m_preNormDepth = 0;

	m_smoothing = true;
	m_innerBandThreshold = 3;
	m_outerBandThreshold = 6;

	m_maxFrames = 1;
}

GeomDepthCalculator::~GeomDepthCalculator()
{
	_clean();
}
void GeomDepthCalculator::_clean()
{
	delete[] m_normalData;
	delete[]m_normalizedDepth;
	delete[]m_preNormDepth;
	delete[]m_bilteralTmp;

	std::list<DepthFrame*>::iterator it = m_frames.begin();
	for (; it != m_frames.end();++it)
	{
		delete *it;
	}
	m_frames.clear();
	
	m_smoothedDepthData.clean();
	m_depthData.clean();
	m_gussainData.clean();
	m_width = m_height = 0;
	m_normalData = 0;
	m_normalizedDepth = 0;
	m_preNormDepth = 0;
	m_bilteralTmp = 0;
}

GeomDepthCalculator::DepthFrame* GeomDepthCalculator::_CreateFrame()
{
	DepthFrame* ret = new DepthFrame();
	ret->data = new ushort[m_width*m_height];
	return ret;
}
void GeomDepthCalculator::_calculateHistogram(DepthFrame* frame)
{
	// Calculate the accumulative histogram (the yellow display...)
	float* histo = &m_depthHisto[0];
	memset(&histo[0], 0, m_depthHisto.size()*sizeof(float));
	int restOfRow = 0;// frame.getStrideInBytes() / sizeof(openni::DepthPixel) - m_width;
	ushort* pDepth = frame->data;
	unsigned int nNumberOfPoints = 0;
	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x, ++pDepth)
		{
			if (*pDepth != 0)
			{
				histo[*pDepth]++;
				nNumberOfPoints++;
			}
		}
		pDepth += restOfRow;
	}
	for (int nIndex = 1; nIndex < m_depthHisto.size(); nIndex++)
	{
		histo[nIndex] += histo[nIndex - 1];
	}
	if (nNumberOfPoints)
	{
		for (int nIndex = 1; nIndex < m_depthHisto.size(); nIndex++)
		{
			histo[nIndex] = ((1.0f - (histo[nIndex] / nNumberOfPoints)));
		}
	}
}
#define CHECK(v,m)(v>=m?m-1:(v<0? 0:v))

void GeomDepthCalculator::_createNormalMap()
{
#define DEPTH(x,y)m_normalizedDepth[ CHECK(y,m_height)*m_width+ CHECK(x,m_width)] 

	float hh = (float)1.0f / (float)m_width;
	float vh = (float)1.0 / m_height;// (float)m_width;

	math::vector3d* ptr = m_normalData;
#define SampleOffset 1
	for (int x = 0; x < m_width; x++){
		for (int y = 0; y < m_height; y++){
			int idx = y*m_width + x;
			
			math::vector3d l((x - SampleOffset)*hh, y*vh,DEPTH(x - SampleOffset, y)*m_amplitude);	//left
			math::vector3d r((x + SampleOffset)*hh, y*vh, DEPTH(x + SampleOffset, y)*m_amplitude);	//right

			math::vector3d lt((x - SampleOffset)*hh, (y - SampleOffset)*vh, DEPTH(x - SampleOffset, y - SampleOffset)*m_amplitude);	//left top
			math::vector3d rt((x + SampleOffset)*hh, (y - SampleOffset)*vh, DEPTH(x + SampleOffset, y - SampleOffset)*m_amplitude);	//right top


			math::vector3d t(x*hh, (y - SampleOffset)*vh, DEPTH(x, y - SampleOffset)*m_amplitude);	//top
			math::vector3d b(x*hh, (y + SampleOffset)*vh, DEPTH(x, y + SampleOffset)*m_amplitude);	//bottom

			math::vector3d lb((x - SampleOffset)*hh, (y + SampleOffset)*vh, DEPTH(x - SampleOffset, y + SampleOffset)*m_amplitude);	//left bottom
			math::vector3d rb((x + SampleOffset)*hh, (y + SampleOffset)*vh, DEPTH(x + SampleOffset, y + SampleOffset)*m_amplitude);	//right bottom


			math::vector3d V = t - b; V.Normalize();
			math::vector3d H = r - l; H.Normalize();

			math::vector3d D1 = lt - rb; D1.Normalize();
			math::vector3d D2 = rt - lb; D2.Normalize();
			/*
			
			math::vector3d g(x*hh, y*vh, DEPTH(x , y)*m_amplitude);
			math::vector3d h((x + 1)*hh, y*vh, DEPTH(x + 1, y)*m_amplitude);
			math::vector3d v(x*hh, DEPTH(x, y + 1)*m_amplitude, (y + 1)*vh);

			math::vector3d a = h - g; a.Normalize();
			math::vector3d b = v - g; b.Normalize();
			*/
			math::vector3d n = (V.crossProduct(H) + D1.crossProduct(D2));
		//	math::Swap(n.y,n.z);
			n.y = -n.y;
			n.Normalize();
			//n.Normalize();
// 				n *= 0.5;
// 				n += 0.5;
		//	n *= 255;

			ptr[idx]=n;
		}
	}
}

float GeomDepthCalculator::_calculateIntensityFromDistance(ushort dist)
{
	int newMax = dist;// -m_minDepthDistance;
	if (newMax > 0)
		return (1.0f - newMax / (float)m_maxDepthDistanceOffset);
	else
		return 1.0f;

}

void GeomDepthCalculator::SetDepthFrame(openni::VideoStream& stream)
{
	static openni::VideoFrameRef depthFrame;
	stream.readFrame(&depthFrame);
	int w = depthFrame.getWidth();
	int h = depthFrame.getHeight();

	
	if (w != m_width || h != m_height)
	{
		_clean();
		m_width = w;
		m_height = h;
		m_depthData.data = new ushort[m_width*m_height];
		m_gussainData.data = new ushort[m_width*m_height];
		m_preNormDepth = new float[m_width*m_height];
		m_normalizedDepth = new float[m_width*m_height];
		m_bilteralTmp = new float[m_width*m_height];
		m_normalData = new math::vector3d[m_width*m_height];
		if (m_smoothing)
			m_smoothedDepthData.data = new ushort[m_width*m_height];
	}

	if (m_depthHisto.size() != stream.getMaxPixelValue())
		m_depthHisto.resize(stream.getMaxPixelValue());
	const openni::DepthPixel* pDepthRow = (const openni::DepthPixel*)depthFrame.getData();
	ushort* dptr = m_depthData.data;

	int rowSize = depthFrame.getStrideInBytes();
	rowSize /= sizeof(openni::DepthPixel);

	DepthFrame* frame = 0;
	if (m_frames.size() < m_maxFrames)
	{
		frame = _CreateFrame();
	}
	else
	{
		frame = *m_frames.begin();
		m_frames.erase(m_frames.begin());
	}
	frame->copyData(pDepthRow, m_width, m_height);
	m_frames.push_back(frame);

	_averageFrames(&m_depthData);
#if 0
	for (int y = 0; y < h; ++y)
	{
		const openni::DepthPixel* pDepth = pDepthRow;

		for (int x = 0; x < w; ++x)
		{
			*dptr = *pDepth;// _calculateIntensityFromDistance(*pDepth);
			/*
			if (*pDepth != 0)
			{
				int nHistValue = *pDepth;
				*dptr = histo[math::Min<int>(m_depthHisto.size() - 1, nHistValue)];
			}
			else
			{
				*dptr = 0;
			}*/
			++pDepth;
			++dptr;
		}

		pDepthRow += rowSize;
	}
#endif
	DepthFrame* depthPtr = &m_depthData;
	if (m_smoothing)
	{
		_smoothDepthMap();
		depthPtr = &m_smoothedDepthData;
	}

	float* histo = &m_depthHisto[0];
	_calculateHistogram(depthPtr);
	float* nptr = m_normalizedDepth;
	dptr = depthPtr->data;
	for (int y = 0; y < h; ++y)
	{

		for (int x = 0; x < w; ++x)
		{
			int index = (h-y-1)*w + x;
			*nptr = ((float)dptr[index]/NORM_DEVISION);
			//openni::CoordinateConverter::convertDepthToWorld(stream, );
			//*nptr = _calculateIntensityFromDistance(dptr[index]);
			/*
			if (*dptr != 0)
			{
				int nHistValue = *dptr;
				*nptr = histo[math::Min<int>(m_depthHisto.size() - 1, nHistValue)];
			}
			else`
			{
				*nptr = 0;
			}*/
			++nptr;
		}
	}
	//_bilteralFilter(m_preNormDepth, m_normalizedDepth);

	_createNormalMap();
}


void GeomDepthCalculator::_averageFrames(DepthFrame* ret)
{
	memset(ret->data, 0, m_width*m_height*sizeof(ushort));
	std::list<DepthFrame*>::iterator it = m_frames.begin();
	for (; it != m_frames.end(); ++it)
	{
		DepthFrame* f = *it;
		ushort* ptrF = f->data;
		ushort* ptrR = ret->data;

		for (int y = 0; y < m_height; ++y)
		{
			for (int x = 0; x < m_width; ++x)
			{
				*ptrR += *ptrF;
				++ptrR;
				++ptrF;
			}
		}
	}
	float inv = 1.0f / m_frames.size();
	ushort* ptrR = ret->data;
	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			*ptrR = (ushort)(((float)*ptrR)* inv);
			++ptrR;
		}
	}
}

//http://en.wikipedia.org/wiki/Bilateral_filter
void GeomDepthCalculator::_bilteralFilter(float* f, float*ret)
{
	//usefull generator: http://www.embege.com/gauss/
	//float kernel[] = { 0.0044330481752440426, 0.05400558262241584, 0.24203622937611433, 0.3990502796524516, 0.24203622937611433, 0.05400558262241584, 0.0044330481752440426 };
	float kernel[] = { 0.05355906223876094, 0.12323811095021552, 0.20318529488452097, 0.24003506385300505, 0.20318529488452097, 0.12323811095021552, 0.05355906223876094 };// sigma 3
	int kerLength = 7;

	memset(m_bilteralTmp, 0, m_width*m_height*sizeof(float));
	for (int i = 0; i < m_width; ++i)
	{
		for (int j = 0; j < m_height; ++j)
		{
			int index = i + m_width*j;
			int start = math::Max(0, i - kerLength / 2);
			int end = math::Min(m_width - 1, i + kerLength / 2);
			int kidx = 0;
			float v = 0;
			float w = 0;
			for (int k = start; k < end; ++k)
			{
				int idx2 = k + m_width*j;
				float val = kernel[kidx] * powf(1 - fabs(f[idx2] - f[index]), 2.0f);
				v += ((float)f[idx2])*val;
				w += val;
				kidx++;
			}
			m_bilteralTmp[index] = v;// / (2);

		}
	}
	for (int i = 0; i < m_width; ++i)
	{
		for (int j = 0; j < m_height; ++j)
		{
			int index = i + m_width*j;
			int start = math::Max(0, j - kerLength / 2);
			int end = math::Min(m_height - 1, j + kerLength / 2);
			int kidx = 0;
			float v = 0;
			float w = 0;
			for (int k = start; k < end; ++k)
			{
				int idx2 = i + m_width*k;
				float val = kernel[kidx] * powf(1 - fabs(m_bilteralTmp[idx2] - m_bilteralTmp[index]), 2.0f);
				v += ((float)m_bilteralTmp[idx2])*val;
				w += val;
				kidx++;
			}
			ret[index] = v;
		}
	}


}

//http://www.codeproject.com/Articles/317974/KinectDepthSmoothing
void GeomDepthCalculator::_smoothDepthMap()
{
	ushort filterCollection[24][2];
	for (int col = 0; col < m_width; ++col)
	{
		for (int row = 0; row < m_height; ++row)
		{
			int depthIndex = row*m_width + col;
			if (m_depthData.data[depthIndex] == 0 && false)
			{
				int x = col;// index%m_width;
				int y = row;// (index - x) / m_width;

				int innerBandCount = 0;
				int outerBandCount = 0;

				for (int i = 0; i < 2; ++i)
				for (int j = 0; j < 24; ++j)
					filterCollection[j][i] = 0;

				// The following loops will loop through a 5 X 5 matrix of pixels surrounding the 
				// candidate pixel.  This defines 2 distinct 'bands' around the candidate pixel.
				// If any of the pixels in this matrix are non-0, we will accumulate them and count
				// how many non-0 pixels are in each band.  If the number of non-0 pixels breaks the
				// threshold in either band, then the average of all non-0 pixels in the matrix is applied
				// to the candidate pixel.
				for (int yi = -2; yi < 3; ++yi)
				{
					for (int xi = -2; xi < 3; ++xi)
					{
						// yi and xi are modifiers that will be subtracted from and added to the
						// candidate pixel's x and y coordinates that we calculated earlier.  From the
						// resulting coordinates, we can calculate the index to be addressed for processing.

						// We do not want to consider the candidate pixel (xi = 0, yi = 0) in our process at this point.
						// We already know that it's 0
						// 
						if (xi != 0 || yi != 0)
						{
							int xSearch = x + xi;
							int ySearch = y + yi;
							if (xSearch >= 0 && xSearch < m_width &&
								ySearch >= 0 && ySearch < m_height)
							{
								int index = xSearch + ySearch*m_width;
								if (m_depthData.data[index] != 0)
								{
									for (int i = 0; i < 24; ++i)
									{
										if (filterCollection[i][0] == m_depthData.data[index])
										{
											// When the depth is already in the filter collection
											// we will just increment the frequency.
											filterCollection[i][1]++;
											break;;
										}else if (filterCollection[i][0] == 0)
										{
											// When we encounter a 0 depth in the filter collection
											// this means we have reached the end of values already counted.
											// We will then add the new depth and start it's frequency at 1.
											filterCollection[i][0] = m_depthData.data[index];
											filterCollection[i][1]++;
											break;;
										}
									}

									// We will then determine which band the non-0 pixel
									// was found in, and increment the band counters.
									if (yi != 2 && yi != -2 &&
										xi != 2 && xi != -2)
										innerBandCount++;
									else
										outerBandCount++;
								}
							}
						}
					}
				}

				// Once we have determined our inner and outer band non-zero counts, and accumulated all of those values,
				// we can compare it against the threshold to determine if our candidate pixel will be changed to the
				// statistical mode of the non-zero surrounding pixels.
				// 
				if (innerBandCount >= m_innerBandThreshold || outerBandCount >= m_outerBandThreshold)
				{
					ushort freq = 0, depth = 0;

					// This loop will determine the statistical mode
					// of the surrounding pixels for assignment to
					// the candidate.
					for (int i = 0; i < 24; ++i)
					{
						// This means we have reached the end of our
						// frequency distribution and can break out of the
						// loop to save time.
						if (filterCollection[i][0]==0)
							break;
						if (filterCollection[i][1]>freq)
						{
							depth = filterCollection[i][0];
							freq = filterCollection[i][1];
						}
					}
					m_smoothedDepthData.data[depthIndex] = depth;
				}
			}else
				m_smoothedDepthData.data[depthIndex] = m_depthData.data[depthIndex];
		}
	}
}
float GeomDepthCalculator::GetDepthAtPoint(int x, int y)
{
	if (x >= m_width || x < 0 || y >= m_height || y < 0)
		return 0;
	return m_normalizedDepth[y*m_width + x]+NORM_OFFSET;
}
math::vector3d GeomDepthCalculator::GetNormalAtPoint(int x, int y)
{
	if (x >= m_width || x < 0 || y >= m_height || y < 0)
		return 0;
	return m_normalData[y*m_width + x];
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

}
}

