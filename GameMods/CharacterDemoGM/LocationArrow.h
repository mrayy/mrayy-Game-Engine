

/********************************************************************
	created:	2010/07/06
	created:	6:7:2010   15:45
	filename: 	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM\LocationArrow.h
	file path:	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM
	file base:	LocationArrow
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef LocationArrow_h__
#define LocationArrow_h__

#include "ISceneNode.h"

namespace mray
{
namespace GameMod
{

class LocationArrow
{
private:
protected:
	float m_timeSinceShown;
	float m_lifeDuration;
	math::vector3d m_position;
	scene::ISceneNode* m_node;
public:
	LocationArrow();
	virtual ~LocationArrow();
	
	void SetShape(scene::ISceneNode*shape);
	scene::ISceneNode* GetShape();

	void SetLocation(const math::vector3d&pos);

	void Show();
	void Hide();

	void Update(float dt);
};

}
}


#endif // LocationArrow_h__
