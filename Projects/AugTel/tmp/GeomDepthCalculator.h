

/********************************************************************
created:	2014/01/12
created:	12:1:2014   19:59
filename: 	C:\Development\mrayEngine\Projects\AugTel\GeomDepthCalculator.h
file path:	C:\Development\mrayEngine\Projects\AugTel
file base:	GeomDepthCalculator
file ext:	h
author:		MHD Yamen Saraiji
	
purpose:	
*********************************************************************/

#ifndef __GeomDepthCalculator__
#define __GeomDepthCalculator__


#include <NiTE.h>


namespace mray
{
namespace AugTel
{

class GeomDepthCalculator
{
protected:

	struct DepthFrame
	{
		ushort* data;

		DepthFrame()
		{
			data = 0;
		}
		~DepthFrame()
		{
			clean();
		}

		void clean()
		{
			delete[]data;
			data = 0;
		}

		void copyData(const ushort* ptr,int w,int h);
	};

	std::list<DepthFrame*> m_frames;

	int m_maxFrames;

	int m_width;
	int m_height;
	float m_amplitude;

	DepthFrame m_depthData;
	DepthFrame m_smoothedDepthData;
	DepthFrame m_gussainData;
	float* m_preNormDepth;
	float* m_normalizedDepth;
	float* m_bilteralTmp;
	math::vector3d *m_normalData;
	std::vector<float> m_depthHisto;

	ushort m_minDepthDistance;
	ushort m_maxDepthDistance;
	ushort m_maxDepthDistanceOffset;

	bool m_smoothing;
	int m_innerBandThreshold;//1->8
	int m_outerBandThreshold;//1->16

	void _calculateHistogram(DepthFrame* frame);
	void _createNormalMap();
	void _clean();

	DepthFrame* _CreateFrame();

	void _averageFrames(DepthFrame* f);
	void _bilteralFilter(float* f, float* ret);
	void _smoothDepthMap();
	float _calculateIntensityFromDistance(ushort dist);
public:
	GeomDepthCalculator();
	virtual~GeomDepthCalculator();

	void SetDepthFrame(openni::VideoStream& stream);

	float GetDepthAtPoint(int x, int y);
	math::vector3d GetNormalAtPoint(int x, int y);

	void SetAmplitude(float amp){ m_amplitude = amp; }
	float Amplitude(){ return m_amplitude; }
	int Width(){ return m_width; }
	int Height(){ return m_height; }

	float* GetDepthData(){ return m_normalizedDepth; }
	math::vector3d* GetNormalData(){ return m_normalData; }


	//returns the intersection distance, ret>0 intersection happened
	//p is defined in depth camera space coordinates
	float Intersect(const math::vector3d& p);

};

}
}


#endif
