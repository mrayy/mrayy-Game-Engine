

/********************************************************************
	created:	2012/02/19
	created:	19:2:2012   12:07
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\animWriter.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	animWriter
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __animWriter__
#define __animWriter__

#include "CompileConfig.h"
#include "IAnimWriter.h"
#include "TransformationAnimationMixer.h"

namespace mray
{
namespace loaders
{

class MRAY_DLL AnimWriter:public IAnimWriter
{
private:
protected:
	void writeTrack(animation::TransformationAnimationTrack* t,OS::IStream* file);
	void writeAnimationTrack(animation::TransAnimationGroup*g,OS::IStream* file);
public:
	AnimWriter(){}
	virtual~AnimWriter(){}

	virtual bool write(animation::TransformationAnimationMixer* mixer,const  core::string& name);
	virtual bool write(animation::TransformationAnimationMixer* mixer,OS::IStream* file);
	virtual bool canWrite(const core::string&name);
};


}
}

#endif

