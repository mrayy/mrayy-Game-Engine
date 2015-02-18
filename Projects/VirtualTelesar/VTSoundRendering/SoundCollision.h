

/********************************************************************
	created:	2012/12/24
	created:	24:12:2012   15:42
	filename: 	C:\Development\mrayEngine\Projects\VirtualTelesar\VTSoundRendering\SoundCollision.h
	file path:	C:\Development\mrayEngine\Projects\VirtualTelesar\VTSoundRendering
	file base:	SoundCollision
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __SoundCollision__
#define __SoundCollision__




namespace mray
{
namespace VT
{

struct ObjectSoundProperties
{
public:
};

class SoundCollisionImpl;

class SoundCollision
{
protected:
	SoundCollisionImpl* m_impl;
public:
	SoundCollision();
	virtual~SoundCollision();

	void Init(float force,const math::vector3d& normal,const math::vector3d& pos);

	void GenerateSamples(float* samples,int count);
};

}


#endif

