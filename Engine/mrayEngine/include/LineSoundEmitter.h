

/********************************************************************
	created:	2011/02/11
	created:	11:2:2011   23:44
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\LineSoundEmitter.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	LineSoundEmitter
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef LineSoundEmitter_h__
#define LineSoundEmitter_h__


#include "CompileConfig.h"
#include "ISoundEmitter.h"


namespace mray
{
namespace sound
{


class MRAY_DLL LineSoundEmitter:public ISoundEmitter
{
private:
	math::line3d m_line;
	ISound* m_sound;
public:
	LineSoundEmitter();
	virtual ~LineSoundEmitter();

	void SetLine(const math::line3d&b);
	const math::line3d& GetLine();

	virtual void AttachSound(ISound* s);
	virtual ISound* GetAttachedSound();

	virtual void Update(ISoundListener* l);
};

}
}
#endif // LineSoundEmitter_h__
