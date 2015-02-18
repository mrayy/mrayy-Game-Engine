


/********************************************************************
	created:	2011/02/11
	created:	11:2:2011   23:42
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\BoxSoundEmitter.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	BoxSoundEmitter
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef BoxSoundEmitter_h__
#define BoxSoundEmitter_h__

#include "CompileConfig.h"
#include "ISoundEmitter.h"


namespace mray
{
namespace sound
{

class MRAY_DLL BoxSoundEmitter:public ISoundEmitter
{
private:
protected:
	math::box3d m_box;
	ISound* m_sound;
public:
	BoxSoundEmitter();
	virtual ~BoxSoundEmitter();

	void SetBox(const math::box3d&b);
	const math::box3d& GetBox();

	virtual void AttachSound(ISound* s);
	virtual ISound* GetAttachedSound();

	virtual void Update(ISoundListener* l);
};

}
}

#endif // BoxSoundEmitter_h__
