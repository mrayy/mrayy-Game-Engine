
/********************************************************************
	created:	2009/08/05
	created:	5:8:2009   7:14
	filename: 	i:\Programing\GameEngine\mrayEngine\mraySound\FSLListener.h
	file path:	i:\Programing\GameEngine\mrayEngine\mraySound
	file base:	FSLListener
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___FSLListener___
#define ___FSLListener___


#include <IMovable.h>
#include <ISoundListener.h>

#include "compileConfig.h"

namespace mray{
namespace sound{

class MRAY_SOUND_DLL FSLListener:public ISoundListener
{
private:
protected:
	math::vector3d m_velocity;
	math::vector3d m_lastPos;
	math::vector3d m_position;
	math::quaternion m_orient;
	scene::IMovable* m_node;

	EListenerDistanceModel m_distanceModel;

public:
	FSLListener();
	virtual~FSLListener();

	void setPosition(const math::vector3d& p);
	const math::vector3d& getPosition();

	void setVelocity(const math::vector3d& p);
	const math::vector3d& getVelocity();

	void setOreintation(const math::quaternion& d);
	const math::quaternion& getOreintation();

	void attachToListener(scene::IMovable*node);

	scene::IMovable* getListenerNode();

	void setDistanceModel(EListenerDistanceModel m);
	EListenerDistanceModel getDistanceModel();

	void GetSamples(float* samples, float count, int channel){};
	void GetSpectrum(float* samples, float count, int channel, EFFT_WindowType window){};
	void update(float dt);
};

}
}

#endif //___FSLListener___
