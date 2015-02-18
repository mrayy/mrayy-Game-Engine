

/********************************************************************
	created:	2010/07/17
	created:	17:7:2010   17:26
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\MorphAnimationMixer.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	MorphAnimationMixer
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef MorphAnimationMixer_h__
#define MorphAnimationMixer_h__

#include "AnimationMixer.h"
#include "MorphTrackGroup.h"

namespace mray
{
namespace animation{

class MRAY_DLL MorphAnimationMixer:public AnimationMixer
{
	DECLARE_RTTI;
private:
protected:
public:
	MorphAnimationMixer();
	virtual~MorphAnimationMixer();

	virtual void addTrackGroup(MorphTrackGroup*group);
	void apply(float time,float weight);

	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem);

protected:

};

MakeSharedPtrType(MorphAnimationMixer)

}
}
#endif // MorphAnimationMixer_h__
