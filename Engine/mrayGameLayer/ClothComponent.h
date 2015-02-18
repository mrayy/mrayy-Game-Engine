

/********************************************************************
	created:	2012/08/31
	created:	31:8:2012   14:15
	filename: 	d:\Development\mrayEngine\Engine\mrayGameLayer\ClothComponent.h
	file path:	d:\Development\mrayEngine\Engine\mrayGameLayer
	file base:	ClothComponent
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___ClothComponent___
#define ___ClothComponent___

#include "IGameComponent.h"
#include "IPhysicalClothDef.h"
#include "IMeshBuffer.h"

namespace mray
{
namespace physics
{
	class IPhysicManager;
	class IPhysicalCloth;
}
namespace game
{


class ClothComponent:public IGameComponent
{
	DECLARE_RTTI;
protected:

	struct VertexAttachmentInfo
	{
	public:
		VertexAttachmentInfo():id(-1),tearable(false),twoWays(false)
		{
		}
		int id;
		bool tearable;
		bool twoWays;
		math::vector3d localPos;
	};
	struct ShapeAttachmentInfo
	{
	public:
		core::string nodeName;
		core::string shapeName;
		std::vector<VertexAttachmentInfo> vertices;
	};
	struct CoreAttachmentInfo
	{
	public:
		CoreAttachmentInfo():impulseThreshold(0),penetrationDepth(0),maxDeformationDistance(0)
		{
		}
		core::string nodeName;
		float impulseThreshold;
		float penetrationDepth;
		float maxDeformationDistance;
	};

	core::string m_targetNode;// node to be converted into cloth
	core::string m_targetAttachmentNode; // attachment physical node
	core::string m_targetAttachmentShape; // attachment physical shape
	physics::PhysicalClothDesc m_desc;

	scene::IMeshBufferPtr m_oldBuffer;

	std::vector<ShapeAttachmentInfo> m_shapeAttachments;
	std::vector<CoreAttachmentInfo> m_coreAttachments;

	physics::IPhysicalCloth* m_cloth;

	physics::IPhysicManager* m_phyiscManage;
public:
	DECLARE_PROPERTY_TYPE(TargetNode,core::string,)
	DECLARE_SETGET_PROP(TargetNode,core::string);

public:
	ClothComponent(game::GameEntityManager* mngr);
	virtual~ClothComponent();

	virtual bool InitComponent();
	virtual void Update(float dt);

	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem);
};

DECLARE_GAMECOMPONENT_FACTORY(ClothComponent);

}
}

#endif
