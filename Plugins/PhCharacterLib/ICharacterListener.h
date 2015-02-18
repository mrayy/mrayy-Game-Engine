

/********************************************************************
	created:	2012/02/14
	created:	14:2:2012   15:36
	filename: 	d:\Development\mrayEngine\Engine\PhCharacterLib\ICharacterListener.h
	file path:	d:\Development\mrayEngine\Engine\PhCharacterLib
	file base:	ICharacterListener
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __ICharacterListener__
#define __ICharacterListener__


namespace mray
{
namespace PhysicsBasedCharacter
{

	class Character;
class ICharacterListener
{
private:
protected:
public:
	ICharacterListener(){}
	virtual~ICharacterListener(){}

	virtual void OnCharacterContact(Character* ch,physics::IPhysicalNode* node){};
};


}
}

#endif

