

#ifndef ___SBILLBOARD_NODE___
#define ___SBILLBOARD_NODE___

#include "MeshRenderableNode.h"

namespace mray{
namespace scene{

class MRAY_DLL SBillboardNode:public MeshRenderableNode
{
protected:
	math::vector2d nodeSize;

	void StartUp();
	bool enableYRotation,enableXRotation;
	float m_angleZ;

public:
	
	//rwProperty<mray::scene::SBillboardNode,float> AngleZ;

	SBillboardNode(const math::vector2d& size);
	virtual ~SBillboardNode();

	virtual bool preRender(IRenderPass*pass);
	virtual bool render(IRenderPass*pass);
	virtual void update(float dt);


	void setupMesh();

	void setSize(math::vector2d size);
	math::vector2d getSize(){return nodeSize;}

	void setTexture(video::ITexturePtr  tex);

	void setXRotation(bool enable){enableXRotation=enable;}
	void setYRotation(bool enable){enableYRotation=enable;}
	bool hasXRotation(){return enableXRotation;}
	bool hasYRotation(){return enableYRotation;}


	const float& getAngleZ() { return m_angleZ; }
	void setAngleZ(const float &val) { m_angleZ = val; }

	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement* exportXMLSettings(xml::XMLElement*elem);
};


}
}


#endif








