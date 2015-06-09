

#ifndef LatencyTestState_h__
#define LatencyTestState_h__

#include "IRenderingState.h"
#include "ICameraVideoSource.h"
#include "CRobotConnector.h"

#include "VideoGrabberTexture.h"

namespace mray
{
namespace AugTel
{
	
class LatencyTestState :public TBee::IRenderingState
{
protected:
	TBee::ICameraVideoSource* m_camVideoSrc;
	bool m_showColor;
	TBee::CRobotConnector* m_robotConnector;
	bool m_changed;

	float m_startTime;
	float m_autoTesterTimer;

	float m_minLatency, m_maxLatency;

	std::vector<float> m_latency;

	OS::IStreamPtr m_outValues;

	GUI::IGUIRenderer* m_guiRenderer;

public:
	LatencyTestState(const core::string& name, TBee::ICameraVideoSource* src);
	virtual ~LatencyTestState();

	virtual void InitState();
	virtual bool OnEvent(Event* e, const math::rectf& rc);
	virtual void OnEnter(IRenderingState*prev);
	virtual void OnExit();
	virtual video::IRenderTarget* Render(const math::rectf& rc, TBee::ETargetEye eye);
	virtual void Update(float dt);

	virtual bool CanSleep(){ return false; }

};

}
}

#endif // LatencyTestState_h__
