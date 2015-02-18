/********************************************************************
	created:	2010/07/15
	created:	15:7:2010   9:02
	filename: 	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\CharacterTracking.h
	file path:	d:\Character Animation\Solution\Project\PhysicsBasedCharacter
	file base:	CharacterTracking
	file ext:	h
	author:		Khaldoon Ghanem
	
	purpose:	
*********************************************************************/

#ifndef CharacterTracking_h__
#define CharacterTracking_h__
namespace mray
{
namespace xml
{
	class XMLElement;
}

namespace secne
{
	class Skeleton;
}

namespace PhysicsBasedCharacter
{
class Character;
class CharacterTracking
{
public:
	CharacterTracking(xml::XMLElement*elem, Character* character);
	virtual ~CharacterTracking();
	scene::Skeleton* getSkeleton();
	void startTracking();
	void stopTracking();
	void pauseTracking();
	void resumeTracking();
	void update(float dt);
protected:
	core::string m_controller;
	Character* m_character;
	int m_slot;
	bool isTracking;
private:
};
}
}

#endif // CharacterTracking_h__