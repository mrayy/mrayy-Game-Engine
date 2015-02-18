
/********************************************************************
	created:	2009/08/04
	created:	4:8:2009   11:17
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ISoundListener.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ISoundListener
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ISoundListener___
#define ___ISoundListener___

#include <quaternion.h>
#include "IObject.h"
#include "SoundEnums.h"

namespace mray{
namespace scene{
	class IMovable;
}
namespace sound{

	enum EListenerDistanceModel
	{
		InverseDistance,
		InverseDistanceClamped,
		LinearDistance,
		LinearDistanceClamped,
		ExponentDistance,
		ExponentDistanceClamped
	};

class MRAY_DLL ISoundListener:public IObject
{
	DECLARE_RTTI;
private:
protected:
public:

	virtual void setPosition(const math::vector3d& p)=0;
	virtual const math::vector3d& getPosition()=0;

	virtual void setVelocity(const math::vector3d& p)=0;
	virtual const math::vector3d& getVelocity()=0;

	virtual void setOreintation(const math::quaternion& d)=0;
	virtual const math::quaternion& getOreintation()=0;

	virtual void attachToListener(scene::IMovable*node)=0;

	virtual scene::IMovable* getListenerNode()=0;

	virtual void setDistanceModel(EListenerDistanceModel m)=0;
	virtual EListenerDistanceModel getDistanceModel()=0;

	virtual void GetSamples(float* samples, float count, int channel) = 0;
	virtual void GetSpectrum(float* samples, float count, int channel, EFFT_WindowType window) = 0;

	virtual void update(float dt)=0;
};

}
}


#endif //___ISoundListener___
