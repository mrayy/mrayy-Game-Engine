

#include "stdafx.h"
#include "DepthFrame.h"


//references :
// http://www.azerdev.com/wp-content/uploads/2013/02/IGS_Report.pdf
// http://www.codeproject.com/Articles/317974/KinectDepthSmoothing
// http://en.wikipedia.org/wiki/Bilateral_filter


namespace mray
{
namespace TBee
{
#define NORM_DEVISION 1000.0f
class DepthFrameImpl
{
public:




	math::vector2di m_size;
	bool m_depthDirty;
	bool m_normalDirty;

	float m_amplitude;

	bool m_thresholdEnable;
	bool m_smoothing;

	float m_minThreshold;
	float m_maxThreshold;

	bool m_gapFill;
	int m_innerBandThreshold;//1->8
	int m_outerBandThreshold;//1->16


	DepthFrame::FrameData<math::vector3d> m_normals;
	DepthFrame::FrameData<ushort> m_depthData;
	DepthFrame::FrameData<ushort> m_smoothedDepthData;
	DepthFrame::FrameData<ushort> m_gussainData;
	DepthFrame::FrameData<float> m_preNormDepth;
	DepthFrame::FrameData<float> m_normalizedDepth;
	DepthFrame::FrameData<float> m_bilteralTmp;

	std::vector<double> m_gaussKernel;
	float *m_gaussSim;

	float gauss(double sigma, int x, int y) {
		return exp(-((x * x + y * y) / (2 * sigma * sigma)));
	}
public:
	DepthFrameImpl()
	{
		m_depthDirty = 0;
		m_normalDirty = 0;
		m_amplitude = 1;

		m_thresholdEnable = true;
		m_gapFill = true;
		m_smoothing = true;
		m_innerBandThreshold = 3;
		m_outerBandThreshold = 6;

		m_minThreshold = 0;
		m_maxThreshold = 10;

		int kernelSize = 7;
		float sigmaD = 2;
		m_gaussKernel.resize(kernelSize);
		int center = (kernelSize - 1) / 2;
		float w = 0;
		for (int i = 0; i < kernelSize; ++i)
		{
			m_gaussKernel[i] = gauss(2, 0, i - center);
			w += m_gaussKernel[i];
		}
		w = 1.0f / w;
		for (int i = 0; i < kernelSize; ++i)
			m_gaussKernel[i] *= w;

		float sigmaR = 6;
		double twoSigmaRSquared = 2 * sigmaR * sigmaR;
		m_gaussSim = new float[256];
		for (int i = 0; i < 256; ++i)
		{
			m_gaussSim[i] = exp(-((double)i / twoSigmaRSquared));
		}
 	}
	virtual~DepthFrameImpl()
	{
		Clean();
		delete[] m_gaussSim;
	}

	void Clean()
	{
		
		m_size = 0;
		m_normals.clean();
		m_smoothedDepthData.clean();
		m_depthData.clean();
		m_gussainData.clean();
		m_normalizedDepth.clean();
		m_preNormDepth.clean();
		m_bilteralTmp.clean();

		m_depthDirty = false;
		m_normalDirty = false;
	}
	void SetThresholds(float min, float max)
	{
		m_minThreshold = min;
		m_maxThreshold = max;
	}

	float GetMinThreshold()
	{
		return m_minThreshold;
	}
	float GetMaxThreshold()
	{
		return m_maxThreshold;
	}
	void SetNormalAmplitude(float amp)
	{
		m_amplitude = amp;
	}
	float GetNormalAmplitude()
	{
		return m_amplitude;
	}
	const math::vector2di& GetSize()
	{
		return m_size;
	}
	float* GetDepth()
	{
		return m_normalizedDepth.Data();
	}
	math::vector3d* GetNormals()
	{
		return m_normals.Data();
	}

	void _gapFillDepthMap()
	{
		m_smoothedDepthData.createData(m_size.x, m_size.y);
		ushort filterCollection[24][2];
		ushort* dptr = m_depthData.Data();
		ushort* sptr = m_smoothedDepthData.Data();
		for (int col = 0; col < m_size.x; ++col)
		{
			for (int row = 0; row < m_size.y; ++row)
			{
				int depthIndex = row*m_size.x + col;
				if (dptr[depthIndex] == 0 )
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
								if (xSearch >= 0 && xSearch < m_size.x &&
									ySearch >= 0 && ySearch < m_size.y)
								{
									int index = xSearch + ySearch*m_size.x;
									if (dptr[index] != 0)
									{
										for (int i = 0; i < 24; ++i)
										{
											if (filterCollection[i][0] == dptr[index])
											{
												// When the depth is already in the filter collection
												// we will just increment the frequency.
												filterCollection[i][1]++;
												break;;
											}
											else if (filterCollection[i][0] == 0)
											{
												// When we encounter a 0 depth in the filter collection
												// this means we have reached the end of values already counted.
												// We will then add the new depth and start it's frequency at 1.
												filterCollection[i][0] = dptr[index];
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
							if (filterCollection[i][0] == 0)
								break;
							if (filterCollection[i][1]>freq)
							{
								depth = filterCollection[i][0];
								freq = filterCollection[i][1];
							}
						}
						sptr[depthIndex] = depth;
					}
				}
				else
					sptr[depthIndex] = dptr[depthIndex];
			}
		}
	}

	float _sim(float a, float b)
	{
		int dist = fabs(a - b) * 256;
		if (dist < 0)dist = 0;
		if (dist>255)dist = 255;
		return m_gaussSim[dist];
	}

	void _bilteralFilter(float* f, float*ret)
	{
		//usefull generator: http://www.embege.com/gauss/
		//float kernel[] = { 0.0044330481752440426, 0.05400558262241584, 0.24203622937611433, 0.3990502796524516, 0.24203622937611433, 0.05400558262241584, 0.0044330481752440426 };
		//float kernel[] = { 0.05355906223876094, 0.12323811095021552, 0.20318529488452097, 0.24003506385300505, 0.20318529488452097, 0.12323811095021552, 0.05355906223876094 };// sigma 3
		int kerLength = m_gaussKernel.size();

		int kidx = 0;
		float v = 0;
		float w = 0;
		float val;


		m_bilteralTmp.createData(m_size.x, m_size.y);
		float* ptr = m_bilteralTmp.Data();
		memset(ptr, 0, m_size.x*m_size.y*sizeof(float));
		for (int i = 0; i < m_size.x; ++i)
		{
			for (int j = 0; j < m_size.y; ++j)
			{
				int index = i + m_size.x*j;
				int start = math::Max(0, i - kerLength / 2);
				int end = math::Min(m_size.x - 1, i + kerLength / 2);
				kidx = 0;
				v = w = 0;
				for (int k = start; k < end; ++k)
				{
					int idx2 = k + m_size.x*j;
					float dist = _sim(f[idx2], f[index]);
					val = m_gaussKernel[kidx] * dist;
					v += ((float)f[idx2])*val;
					w += val;
					kidx++;
				}
				if (w>0)
					ptr[index] = v / w;// / (2);
				else ptr[index] = 0;

			}
		}

		for (int i = 0; i < m_size.x; ++i)
		{
			for (int j = 0; j < m_size.y; ++j)
			{
				int index = i + m_size.x*j;
				int start = math::Max(0, j - kerLength / 2);
				int end = math::Min(m_size.y - 1, j + kerLength / 2);
				kidx = 0;
				v = w = 0;
				for (int k = start; k < end; ++k)
				{
					int idx2 = i + m_size.x*k;
					float dist = _sim(f[idx2], f[index]);
					val = m_gaussKernel[kidx] * dist;
					v += ((float)ptr[idx2])*val;
					w += val;
					kidx++;
				}
				if (w>0)
					ret[index] = v / w;// / (2);
				else ret[index] = 0;
			}
		}


	}
	void CalculateDepth()
	{
		if (!m_depthDirty)
			return;

		m_depthData.createData(m_size.x, m_size.y);
		m_preNormDepth.createData(m_size.x, m_size.y);
		m_normalizedDepth.createData(m_size.x, m_size.y);
		m_depthDirty = false;
		DepthFrame::FrameData<ushort>* depthPtr = &m_depthData;

		if (m_gapFill)
		{
			_gapFillDepthMap();
			depthPtr = &m_smoothedDepthData;
		}

		float* nptr = m_preNormDepth.Data();
		if (!m_smoothing)
			nptr = m_normalizedDepth.Data();
		ushort*dptr = depthPtr->Data();
		for (int y = 0; y < m_size.y; ++y)
		{

			for (int x = 0; x < m_size.x; ++x)
			{
				int index = (m_size.y - y - 1)*m_size.x + x;
				*nptr = ((float)dptr[index] / NORM_DEVISION);
				if (m_thresholdEnable){
					if (*nptr < m_minThreshold || *nptr>m_maxThreshold)
					{
						*nptr = 99;
					}
				}
				++nptr;
			}
		}
		if (m_smoothing)
			_bilteralFilter(m_preNormDepth.Data(), m_normalizedDepth.Data());
	}

	void CalculateNormals()
	{
		if (!m_normalDirty)
			return;
		if (m_depthDirty)
			CalculateDepth();

		m_normalizedDepth.createData(m_size.x, m_size.y);
		float* dptr = m_normalizedDepth.Data();

#define CHECK(v,m)(v>=m?m-1:(v<0? 0:v))
#define DEPTH(a,b)dptr[ CHECK((b),m_size.y)*m_size.x+ CHECK(a,m_size.x)] 
#define NORMALIZE(v){if(v.x==0 && v.y==0 && v.z==0) v.z=1;else v.Normalize();}
		float hh = (float)1.0f / (float)m_size.x;
		float vh = (float)1.0 / (float)m_size.y;// (float)m_width;

		m_normals.createData(m_size.x, m_size.y);
		math::vector3d* ptr = m_normals.Data();
#define SampleOffset 1
		for (int x = 0; x < m_size.x; x++){
			for (int y = 0; y < m_size.y; y++){
				int idx = y*m_size.x + x;

				math::vector3d l((x - SampleOffset)*hh, y*vh, DEPTH((x - SampleOffset), y)*m_amplitude);	//left
				math::vector3d r((x + SampleOffset)*hh, y*vh, DEPTH((x + SampleOffset), y)*m_amplitude);	//right

				math::vector3d lt((x - SampleOffset)*hh, (y - SampleOffset)*vh, DEPTH((x - SampleOffset), y - SampleOffset)*m_amplitude);	//left top
				math::vector3d rt((x + SampleOffset)*hh, (y - SampleOffset)*vh, DEPTH((x + SampleOffset), y - SampleOffset)*m_amplitude);	//right top


				math::vector3d t(x*hh, (y - SampleOffset)*vh, DEPTH(x, y - SampleOffset)*m_amplitude);	//top
				math::vector3d b(x*hh, (y + SampleOffset)*vh, DEPTH(x, y + SampleOffset)*m_amplitude);	//bottom

				math::vector3d lb((x - SampleOffset)*hh, (y + SampleOffset)*vh, DEPTH(x - SampleOffset, y + SampleOffset)*m_amplitude);	//left bottom
				math::vector3d rb((x + SampleOffset)*hh, (y + SampleOffset)*vh, DEPTH(x + SampleOffset, y + SampleOffset)*m_amplitude);	//right bottom


				math::vector3d V = t - b; NORMALIZE(V);
				math::vector3d H = r - l; NORMALIZE(H);

				math::vector3d D1 = lt - rb; NORMALIZE(D1);
				math::vector3d D2 = rt - lb; NORMALIZE(D2);
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

				ptr[idx] = n;
			}
		}
		m_normalDirty = false;
	}

	void CreateRawData(int w, int h)
	{
		m_depthData.createData(w, h);
		memset(m_depthData.Data(), 0, w*h*sizeof(ushort));
		m_size.set(w, h);
		m_depthDirty = true;
		m_normalDirty = true;
	}

	void SetRawData(ushort* d, int w, int h)
	{
		m_depthData.copyData(d, w, h);
		m_size.set(w, h);
		m_depthDirty = true;
		m_normalDirty = true;
	}
	ushort* GetRawData()
	{
		return m_depthData.Data();
	}

	math::recti  GetCroppedFrame(const math::recti& src, const DepthFrame* f)
	{
		math::recti rc = src;
		rc.ULPoint.x = math::clamp<int>(rc.ULPoint.x, 0, m_size.x);
		rc.ULPoint.y = math::clamp<int>(rc.ULPoint.y, 0, m_size.y);
		rc.BRPoint.x = math::clamp<int>(rc.BRPoint.x, 0, m_size.x);
		rc.BRPoint.y = math::clamp<int>(rc.BRPoint.y, 0, m_size.y);
		
		int w = rc.getWidth();
		int h = rc.getHeight();
		DepthFrameImpl* o= f->m_impl;
		o->m_depthData.createData(w, h);
		o->m_size.set(w, h);
		o->m_depthDirty = true;
		o->m_normalDirty = true;

		ushort *dstPtr = o->m_depthData.Data();
		ushort *srcPtr = m_depthData.Data();
		for (int x = 0; x < w; ++x)
		{
			for (int y = 0; y < h; ++y)
			{
				int idx = (y + rc.ULPoint.y)*w+(x + rc.ULPoint.x);
				dstPtr[y*w + x] = srcPtr[idx];
			}
		}
		return rc;
	}
	void  AddFrame(const math::recti& src, const DepthFrame* f)
	{
		math::recti rc = src;
		rc.ULPoint.x = math::clamp<int>(rc.ULPoint.x, 0, m_size.x);
		rc.ULPoint.y = math::clamp<int>(rc.ULPoint.y, 0, m_size.y);
		rc.BRPoint.x = math::clamp<int>(rc.BRPoint.x, 0, m_size.x);
		rc.BRPoint.y = math::clamp<int>(rc.BRPoint.y, 0, m_size.y);

		int w = rc.getWidth();
		int h = rc.getHeight();
		DepthFrameImpl* o = f->m_impl;
		m_depthDirty = true;
		m_normalDirty = true;

		ushort *srcPtr = o->m_depthData.Data();
		ushort *dstPtr = m_depthData.Data();
		for (int x = 0; x < w; ++x)
		{
			for (int y = 0; y < h; ++y)
			{
				int idx = (y + rc.ULPoint.y)*GetSize().x + (x + rc.ULPoint.x);
				int idx2 = (y)*w + (x );
				dstPtr[idx] = srcPtr[idx2];
			}
		}
	}
};




DepthFrame::DepthFrame()
{
	m_impl = new DepthFrameImpl;
}

DepthFrame::~DepthFrame()
{
	delete m_impl;
}

void DepthFrame::EnableThreshold(bool e)
{
	m_impl->m_thresholdEnable = e;
}
bool DepthFrame::IsThresholdEnabled()
{
	return m_impl->m_thresholdEnable;
}
void DepthFrame::SetThresholds(float min, float max)
{
	m_impl->SetThresholds(min, max);
}

float DepthFrame::GetMinThreshold()
{
	return m_impl->GetMinThreshold();
}
float DepthFrame::GetMaxThreshold()
{
	return m_impl->GetMaxThreshold();
}
void DepthFrame::Clean()
{
	m_impl->Clean();
}


void DepthFrame::EnableSmoothing(bool e)
{
	m_impl->m_smoothing = e;
}
bool DepthFrame::IsSmoothingEnabled()
{
	return m_impl->m_smoothing;
}

void DepthFrame::EnableGapFill(bool e)
{
	m_impl->m_gapFill = e;
}
bool DepthFrame::IsGapFillEnabled()
{
	return m_impl->m_gapFill;
}
void DepthFrame::SetBandThreshold(int inner, int outer)
{
	m_impl->m_innerBandThreshold = inner;
	m_impl->m_outerBandThreshold = outer;
}
int DepthFrame::GetInnerBandThreshold()
{
	return m_impl->m_innerBandThreshold;
}
int DepthFrame::GetOuterBandThreshold()
{
	return m_impl->m_outerBandThreshold;
}

const math::vector2di& DepthFrame::GetSize()
{
	return m_impl->GetSize();
}

float* DepthFrame::GetDepth()
{
	return m_impl->GetDepth();
}

math::vector3d* DepthFrame::GetNormals()
{
	return m_impl->GetNormals();
}


void DepthFrame::CalculateDepth()
{
	m_impl->CalculateDepth();
}

void DepthFrame::CalculateNormals()
{
	m_impl->CalculateNormals();
}

void DepthFrame::SetNormalAmplitude(float amp)
{
	m_impl->SetNormalAmplitude(amp);
}

float DepthFrame::GetNormalAmplitude()
{
	return m_impl->GetNormalAmplitude();
}

void DepthFrame::CreateRawData(int w, int h)
{
	m_impl->CreateRawData(w,h);
}
void DepthFrame::SetRawData(ushort* d, int w, int h)
{
	m_impl->SetRawData(d,w,h);
}

ushort* DepthFrame::GetRawData()
{
	return m_impl->GetRawData();
}

math::recti DepthFrame::GetCroppedFrame(const math::recti& src, const DepthFrame* f)
{
	return m_impl->GetCroppedFrame(src, f);
}
void DepthFrame::AddFrame(const math::recti& src, const DepthFrame* f)
{
	 m_impl->AddFrame(src, f);

}


}
}
