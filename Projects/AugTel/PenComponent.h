

/********************************************************************
	created:	2014/02/16
	created:	16:2:2014   4:36
	filename: 	C:\Development\mrayEngine\Projects\AugTel\PenComponent.h
	file path:	C:\Development\mrayEngine\Projects\AugTel
	file base:	PenComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __PenComponent__
#define __PenComponent__

#include "MountableComponent.h"
#include "IUDPClient.h"
#include "SurfaceWrapper.h"


namespace mray
{
namespace AugTel
{
	class MovingAverage;
class PenComponent:public VT::MountableComponent
{
	DECLARE_RTTI;
protected:

	enum class EDrawMessages
	{
		AddLine = 1,
		RemoveLastLine = 2,
		CurrentLine = 3,
		CurrentPos=4
	};
	struct LineInfo
	{
		LineInfo()
		{
			clr = video::DefaultColors::Black;
			width = 2;
		}
		std::vector<math::vector2d> points;
		video::SColor clr;
		float width;
	};

	bool m_writing;
	LineInfo m_points;

	math::vector3d m_lastPos;
	math::vector2d m_currentProjPos;

	float m_threshold;

	core::string m_drawIp;
	int m_drawPort;
	bool m_remove;

	bool m_calibrated;

	math::vector2d m_calibPoints[4];
	int m_currentCalibPoint;
	bool m_calibPointInsert;

	MovingAverage* m_movingAvg;

	network::IUDPClient*m_sender;
	network::NetAddress m_addr;

	core::string m_penTipName;
	scene::ISceneNode* m_penTip;

	video::RenderPass* m_penTipMtrl;

	SurfaceWrapper m_wrapper;

	DECLARE_PROPERTY_TYPE(Threshold, float, );
	DECLARE_PROPERTY_TYPE(DrawPort, int, );
	DECLARE_PROPERTY_TYPE(DrawIP, core::string, );
	DECLARE_PROPERTY_TYPE(PenTipName, core::string, );

	void _Send(EDrawMessages msg, const PenComponent::LineInfo* line);
	int _writeLine(OS::StreamWriter& wrtr, const PenComponent::LineInfo* ret);

public:
	PenComponent(game::GameEntityManager*m);
	virtual~PenComponent();
	DECLARE_SETGET_PROP(Threshold, float);
	DECLARE_SETGET_PROP(DrawPort, int);
	DECLARE_SETGET_PROP(DrawIP, core::string);
	DECLARE_SETGET_PROP(PenTipName, core::string);

	virtual bool InitComponent();
	virtual void Update(float dt);

	virtual void DebugRender(scene::IDebugDrawManager* renderer);
	virtual void OnGUIRender(GUI::IGUIRenderer* renderer, const math::rectf& vp);
};

DECLARE_GAMECOMPONENT_FACTORY(PenComponent);
}
}


#endif
