

/********************************************************************
created:	2014/01/16
created:	16:1:2014   19:42
filename: 	C:\Development\mrayEngine\Projects\AugTel\OpenNIHandler.h
file path:	C:\Development\mrayEngine\Projects\AugTel
file base:	OpenNIHandler
file ext:	h
author:		MHD Yamen Saraiji
	
purpose:	
*********************************************************************/

#ifndef __OpenNIHandler__
#define __OpenNIHandler__


#include "OpenNICaptureDevice.h"
#include "GeomDepthCalculator.h"


namespace mray
{
namespace TBee
{

	class OpenNIHandlerThread;
class OpenNIHandler
{
protected:

	GCPtr<OpenNICaptureDevice> m_openNiCamera;

	std::vector<float> m_depthHisto;


	GCPtr<OS::IThread> m_thread;
	GCPtr<OS::IMutex> m_mutix;
	OpenNIHandlerThread* m_threadFunc;

	GeomDepthCalculator m_normalCalc;

	math::vector2d m_scale;
	math::vector2d m_center;


	math::vector2di m_size;

	bool m_started;

	friend class OpenNIHandlerThread;

	void _Update();
public:
	OpenNIHandler();
	virtual~OpenNIHandler();

	void Init();
	void Start(int w,int h);
	void Close();

	bool IsStarted(){ return m_started; }
	
	void CreateDepthFrame(int w, int h);
	const math::vector2di& GetSize(){ return m_size; }

	GeomDepthCalculator& GetNormalCalculator(){ return m_normalCalc; }
	void SetCalculateNormals(bool calc){ m_normalCalc.SetCalculateNormals(calc); }

	void Update(float dt);

	void SetScale(const math::vector2d& scale){ m_scale = scale; }
	const math::vector2d&  GetScale(){ return m_scale; }


	void SetCenter(const math::vector2d& center){ m_center = center; }
	const math::vector2d&  GetCenter(){ return m_center; }


	//pos is 0->1 range in screen space
	bool ConvertToImageSpace(const math::vector2d& pos, math::vector2di& ret);
};

}
}

#endif