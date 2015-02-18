

/********************************************************************
	created:	2014/01/20
	created:	20:1:2014   21:26
	filename: 	C:\Development\mrayEngine\Projects\AugTel\CompositeBoneComponent.h
	file path:	C:\Development\mrayEngine\Projects\AugTel
	file base:	CompositeBoneComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __CompositeBoneComponent__
#define __CompositeBoneComponent__


#include "IGameComponent.h"

namespace mray
{
namespace game
{

	class IMovableComponent;
class CompositeBoneComponent:public IGameComponent
{
	DECLARE_RTTI;
protected:
	struct SubNodes
	{
		IMovableComponent* node;
		core::string name;
	};

	std::vector<SubNodes> m_subNodes;
public:
	CompositeBoneComponent(game::GameEntityManager*m);
	virtual~CompositeBoneComponent();

	virtual bool InitComponent();

	void SetPosition(const math::vector3d& pos, bool local);
	void SetOrientation(const math::quaternion& ori, bool local);
	
	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem);
};
DECLARE_GAMECOMPONENT_FACTORY(CompositeBoneComponent);

}
}

#endif
