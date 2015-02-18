


/********************************************************************
	created:	2014/04/08
	created:	8:4:2014   17:47
	filename: 	C:\Development\mrayEngine\Projects\TEDxTokyo\TwitterVisualizer\PAVelocityOverTimeInterpolater.h
	file path:	C:\Development\mrayEngine\Projects\TEDxTokyo\TwitterVisualizer
	file base:	PAVelocityOverTimeInterpolater
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __PAVelocityOverTimeInterpolater__
#define __PAVelocityOverTimeInterpolater__


#include "IParticleAffector.h"


namespace mray
{
namespace scene
{

class PAVelocityOverTimeInterpolater :public IParticleAffector
{
protected:
	math::vector3d m_minVel;
	math::vector3d m_maxVel;
public:
	PAVelocityOverTimeInterpolater();
	virtual~PAVelocityOverTimeInterpolater();


	virtual bool affectsTransformations(){ return true; }

	virtual void applyOnParticle(IParticle*p, float dt);

	void SetMinMaxVel(const math::vector3d &minV, const math::vector3d &maxV)
	{
		m_minVel = minV;
		m_maxVel = maxV;
	}
};

}
}


#endif
