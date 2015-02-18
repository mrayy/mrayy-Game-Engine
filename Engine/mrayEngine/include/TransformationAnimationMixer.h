
/********************************************************************
	created:	2009/06/28
	created:	28:6:2009   22:22
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\AnimationMixer.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	AnimationMixer
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___AnimationMixer___
#define ___AnimationMixer___

#include "AnimationMixer.h"
#include "TransAnimationGroup.h"

namespace mray{
namespace animation{

class MRAY_DLL TransformationAnimationMixer:public AnimationMixer
{
	DECLARE_RTTI;
private:
protected:
public:
	TransformationAnimationMixer();
	virtual~TransformationAnimationMixer();

	virtual AnimationController* addTrackGroup(TransAnimationGroup* group);
	//virtual void applyOnNodes();

	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem);

protected:

};

}
}


#endif //___AnimationMixer___
