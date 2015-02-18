

/********************************************************************
	created:	2012/01/31
	created:	31:1:2012   15:56
	filename: 	d:\Development\mrayEngine\Tests\TestProfile\StereoRenderer.h
	file path:	d:\Development\mrayEngine\Tests\TestProfile
	file base:	StereoRenderer
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __StereoRenderer__
#define __StereoRenderer__

#include "compileConfig.h"
#include "GPUShader.h"

namespace mray
{
namespace video
{
	class RenderWindow;
}
namespace scene
{

	enum EStereoMode
	{
		EStereo_None,
		EStereo_SideBySide,
		EStereo_TopDown,
		EStereo_Anglyph,
		EStereo_Interlance,

		EStereo_ModesCount
	};

	class ViewPort;
	class CameraNode;
	class IMovable;

class MRAY_DLL StereoRenderer
{
private:
protected:
	EStereoMode m_mode;

	ViewPort* m_leftVP;
	ViewPort* m_rightVP;
	ViewPort* m_fullVP;

	CameraNode* m_leftCam;
	CameraNode* m_rightCam;
	
	IMovable* m_headMount;

	float m_offset;

	video::IGPUShaderProgramPtr m_interlanceProg;
	video::IGPUShaderProgramPtr m_anglyphProg;

	GCPtr<video::GPUShader> m_stereoPP;

	void _init();

public:
	StereoRenderer(video::RenderWindow* rw,CameraNode*leftCam,CameraNode* rightCam,
		IMovable* headMount);
	StereoRenderer(ViewPort* left,ViewPort*right,IMovable* headMount);
	virtual~StereoRenderer();

	IMovable* GetHeadMount()const{return m_headMount;}

	ViewPort* GetFullVP()const{return m_fullVP;}
	ViewPort* GetLeftVP()const{return m_leftVP;}
	ViewPort* GetRightVP()const{return m_rightVP;}

	void SetMode(EStereoMode mode);
	EStereoMode GetMode();

	CameraNode* GetLeftCamera()const{return m_leftCam;}
	CameraNode* GetRightCamera()const{return m_rightCam;}

	void SetLeftCamera(CameraNode* c);
	void SetRightCamera(CameraNode* c);

	void SetOffset(float offset);
	float GetOffset()const;

	void Render();

};

}
}

#endif

