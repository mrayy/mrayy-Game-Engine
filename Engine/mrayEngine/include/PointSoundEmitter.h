

/********************************************************************
	created:	2011/02/11
	created:	11:2:2011   23:43
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\PointSoundEmitter.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	PointSoundEmitter
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef PointSoundEmitter_h__
#define PointSoundEmitter_h__

#include "CompileConfig.h"
#include "ISoundEmitter.h"


namespace mray
{
namespace sound
{


class MRAY_DLL PointSoundEmitter:public ISoundEmitter
{
private:
protected:
	math::vector3d m_origin;
	ISound* m_sound;
public:
	PointSoundEmitter();
	virtual ~PointSoundEmitter();

	void SetPoint(const math::vector3d&b);
	const math::vector3d& GetPoint();

	virtual void AttachSound(ISound* s);
	virtual ISound* GetAttachedSound();

	virtual void Update(ISoundListener* l);
};

}
}
#endif // PointSoundEmitter_h__
