

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

#ifdef USE_OPENNI

#include <NiTE.h>
#include "DepthFrame.h"

namespace mray
{
namespace TBee
{
	class GeomDepthRect;

class GeomDepthCalculator
{
protected:

	typedef std::list<DepthFrame::FrameData<ushort>*> FrameList;
	FrameList m_frames;
	DepthFrame::FrameData<ushort> m_avgFrame;
	float* m_frameWeights;
	DepthFrame* m_frame;
	int m_maxFrames;

	bool m_calcNormals;

	void _averageFrames();
	void _clean();
public:
	GeomDepthCalculator();
	virtual~GeomDepthCalculator();

	void SetDepthFrame(openni::VideoFrameRef& stream);

	void SetThresholds(float min, float max);
	float GetMinThreshold();
	float GetMaxThreshold();

	void SetCalculateNormals(bool calc){ m_calcNormals = calc; }
	bool IsCalculateNormals(){ return m_calcNormals ; }

	float GetDepthAtPoint(int x, int y);
	math::vector3d GetNormalAtPoint(int x, int y);

	void SetAmplitude(float amp);
	float Amplitude();
	int Width();
	int Height();

	float* GetDepthData();
	math::vector3d* GetNormalData();

	DepthFrame* GetDepthFrame(){ return m_frame; }

	void CreateDepthFrame(int w, int h);
	void AddDepthRect(const GeomDepthRect* rc);

	//returns the intersection distance, ret>0 intersection happened
	//p is defined in depth camera space coordinates
	float Intersect(const math::vector3d& p);

};

}
}

#endif

#endif
