
/********************************************************************
	created:	2009/10/18
	created:	18:10:2009   22:46
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject\Segment.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject
	file base:	Segment
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___Segment___
#define ___Segment___

#include <list>
#include "SegmentDesc.h"

namespace mray{

class Segment;
typedef std::list<GCPtr<Segment>> SegmentList;

class Segment
{
private:
protected:

	core::string m_name;

	Segment*m_parent;
	GCPtr<physics::IDOF6Joint3D> m_joint;
	GCPtr<scene::ISceneNode> m_shape;

	GCPtr<physics::IPhysicalNode> m_physicalNode;

	SegmentList m_children;

public:
	Segment(Segment*parent);
	virtual~Segment();

	void setName(const mchar*n);
	const mchar* getName();

	void loadFromDesc(SegmentDesc*desc,scene::SceneManager*smngr,scene::ISceneNode*root);

	void setParent(Segment*p);
	Segment* getParent();

	physics::IDOF6Joint3D* getJoint();

	physics::IPhysicalNode* getPhysicalNode();


	void update(float dt);

};

}


#endif //___Segment___

