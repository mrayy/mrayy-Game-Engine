

/********************************************************************
created:	2011/02/11
created:	11:2:2011   23:44
filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\SphereSoundEmitter.h
file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
file base:	SphereSoundEmitter
file ext:	h
author:		Mohamad Yamen Saraiji

purpose:	
*********************************************************************/

#ifndef SphereSoundEmitter_h__
#define SphereSoundEmitter_h__


#include "CompileConfig.h"
#include "ISoundEmitter.h"


namespace mray
{
namespace sound
{


class MRAY_DLL SphereSoundEmitter:public ISoundEmitter
{
private:
	math::Sphere m_sphere;
	ISound* m_sound;
public:
	SphereSoundEmitter();
	virtual ~SphereSoundEmitter();

	void SetSphere(const math::Sphere&s);
	const math::Sphere& GetSphere();

	virtual void AttachSound(ISound* s);
	virtual ISound* GetAttachedSound();

	virtual void Update(ISoundListener* l);
};

}
}
#endif // SphereSoundEmitter_h__
