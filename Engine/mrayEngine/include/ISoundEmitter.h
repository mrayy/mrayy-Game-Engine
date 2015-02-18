


/********************************************************************
	created:	2011/02/11
	created:	11:2:2011   23:38
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ISoundEmitter.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ISoundEmitter
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ISoundEmitter_h__
#define ISoundEmitter_h__


namespace mray
{
namespace sound
{

	class ISoundListener;
	class ISound;

class ISoundEmitter
{
private:
protected:
public:
	ISoundEmitter(){}
	virtual ~ISoundEmitter(){}

	virtual void AttachSound(ISound* s)=0;
	virtual ISound* GetAttachedSound()=0;

	virtual void Update(ISoundListener* l)=0;
};

}
}


#endif // ISoundEmitter_h__

