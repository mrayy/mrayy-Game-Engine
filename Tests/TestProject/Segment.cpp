
#include "stdafx.h"
#include "Segment.h"
#include "PhysXManager.h"


namespace mray{

Segment::Segment(Segment*parent)
{
	m_parent=parent;
}
Segment::~Segment()
{
	m_children.clear();
}


void Segment::setName(const mchar*n)
{
	m_name=n;
}
const mchar* Segment::getName()
{
	return m_name.c_str();
}

void Segment::loadFromDesc(SegmentDesc*desc,scene::SceneManager*smngr,scene::ISceneNode*root)
{

	m_name=desc->name;
	//1st , create physical node
	physics::PhysicalNodeDesc phNodeDesc;
	physics::PhysicalBodyDesc phBodyDesc;
	physics::CapsuleShapeDesc phCapsuleDesc;
	core::array<physics::ConvexShapeDesc> phTriangleDesc;

	math::quaternion q;
	math::matrix4x4 mat;
	
	//fill shape prop
	if (desc->shapeDesc.length > 0)
	{
		phCapsuleDesc.height = desc->shapeDesc.length;
		if (desc->shapeDesc.radius > 0)
			phCapsuleDesc.radius = desc->shapeDesc.radius;
		else
			phCapsuleDesc.radius = desc->shapeDesc.length * 0.2;
	}


	//create scene node 
	if(desc->model!=mT("") ){
		//load it from file
		m_shape=smngr->addAnimatedMesh(desc->model.c_str(),0,math::quaternion::Identity,1,mT(""),root);

		GCPtr<video::SMaterial>linkMat=gMaterialResourceManager.getMaterial(mT("LinkMaterial"));
		GCPtr<video::SMaterial>ballmat=gMaterialResourceManager.getMaterial(mT("BallMaterial"));
		if(m_shape){
			GCPtr<scene::SMesh> mesh=m_shape->getMesh();
			physics::ConvexShapeDesc tShape;
			for (int i=0;i<mesh->getBuffersCount();++i)
			{
				GCPtr<scene::MeshBufferData> bd=mesh->getBufferData(i);
				if(bd->getMeshBuffer()->getMeshName().find(mT("Link"))!=-1){
					bd->setMaterial(linkMat);
				}else if(bd->getMeshBuffer()->getMeshName().find(mT("Ball"))!=-1){
					bd->setMaterial(ballmat);
				}

				scene::SMeshManipulator::getInstance().scaleMesh(bd->getMeshBuffer(),phCapsuleDesc.height);

				tShape.mesh=bd->getMeshBuffer().pointer();
				tShape.flipNormals=false;
				//tShape.localSpace.setScale(phCapsuleDesc.height);
				phTriangleDesc.push_back(tShape);
			}
		//	m_shape->scale=phCapsuleDesc.height;

			for (int i=0;i<phTriangleDesc.size();++i)
			{
				phNodeDesc.shapes.push_back(&phTriangleDesc[i]);
			}
			m_shape->ShadowCaster=true;
			m_shape->ShadowReciver=true;
		}
	}
	if(!m_shape){
		GCPtr<scene::SMesh> mesh=new scene::SMesh();
		GCPtr<scene::MeshBufferData>bd=mesh->addNewBuffer();
		scene::MeshGenerator::getInstance().generateCapsule(phCapsuleDesc.radius,phCapsuleDesc.height,20,bd->getMeshBuffer());
		math::matrix4x4 m;
		m.rotateX(90).setTranslation(math::vector3d(0,0,phCapsuleDesc.height/2+phCapsuleDesc.radius));
		scene::SMeshManipulator::getInstance().applyTransformationOnMesh(bd->getMeshBuffer(),m);
		m_shape=new scene::MeshSceneNode(mesh,mT(""),root,0,math::quaternion::Identity,1,0);

		GCPtr<video::SMaterial> material=gMaterialResourceManager.getMaterial(mT("ShadowedMaterial"));
		m_shape->setMaterial(material);
		smngr->addSceneNode(m_shape);


		phNodeDesc.shapes.push_back(&phCapsuleDesc);
	}



	//fill body&node prop
	phNodeDesc.bodyDesc = &phBodyDesc;
	
	if ((desc->inertiaDesc.mass > 0) && (desc->inertiaDesc.moments.LengthSQ() > 0))
	{
		phNodeDesc.density = 0;		
		phBodyDesc.mass =  desc->inertiaDesc.mass;
		phBodyDesc.massSpaceInertia = desc->inertiaDesc.moments;
		q.fromEulerAngles(desc->inertiaDesc.rot.x,desc->inertiaDesc.rot.y,desc->inertiaDesc.rot.z);
		q.toMatrix(phBodyDesc.massLocalPos);
		phBodyDesc.massLocalPos.setTranslation(desc->inertiaDesc.trans);
	}
	else
	{
		//phNodeDesc.shapes.push_back(&phCapsuleDesc);
		q.fromAngle(90,math::vector3d(1,0,0));
		q.toMatrix(phCapsuleDesc.localSpace);
		phCapsuleDesc.localSpace.setTranslation(math::vector3d(0,0,phCapsuleDesc.height/2+phCapsuleDesc.radius));		
		phBodyDesc.mass = 0;
		phBodyDesc.massSpaceInertia = math::vector3d(0,0,0);
		phNodeDesc.density = desc->inertiaDesc.density;
	}
/**/

	q.fromEulerAngles(desc->rot.x,desc->rot.y,desc->rot.z);
	q.toMatrix(mat);
	mat.setTranslation(desc->trans);

	if (m_parent)
	{
		math::matrix4x4 mat2;
		m_parent->getPhysicalNode()->getGlobalOrintation().toMatrix(mat2);
		mat2.setTranslation(m_parent->getPhysicalNode()->getGlobalPosition());
		phNodeDesc.globalPos = mat2 * mat;
	}
	else
	{
		phNodeDesc.globalPos = mat;
	}

	m_physicalNode=physics::IPhysicManager::getInstance().createNode(&phNodeDesc);


	//2nd, create the joint

	physics::IPhysicalJointD6Desc jointDesc;
	if(!m_parent)
	{
		jointDesc.body1 = 0;
	}
	else
	{
		jointDesc.body1 = m_parent->getPhysicalNode();
	}
	jointDesc.body2 = getPhysicalNode();

	jointDesc.setFromGlobalAnchor(m_physicalNode->getGlobalPosition());

	q.fromEulerAngles(desc->jointDesc.frameRot.x,desc->jointDesc.frameRot.y,desc->jointDesc.frameRot.z);
	q.toMatrix(mat);

	jointDesc.localAxis[0].x = mat(2,0);jointDesc.localAxis[0].y = mat(2,1);jointDesc.localAxis[0].z = mat(2,2);
	jointDesc.localNormal[0].x = mat(1,0);jointDesc.localNormal[0].y = mat(1,1);jointDesc.localNormal[0].z = mat(1,2);

	jointDesc.flags[physics::EJFlag_CollisionEnable]=false;

	jointDesc.localAxis[1] = math::vector3d.ZAxis;
	jointDesc.localNormal[1] = math::vector3d.YAxis;


	jointDesc.xMotion=desc->jointDesc.xlimitType;
	jointDesc.yMotion=desc->jointDesc.ylimitType;
	jointDesc.zMotion=desc->jointDesc.zlimitType;
	jointDesc.linearLimit=desc->jointDesc.linearLimitDesc;

	jointDesc.swing1Motion=desc->jointDesc.swing1Type;
	jointDesc.swing2Motion=desc->jointDesc.swing2Type;
	jointDesc.twistMotion=desc->jointDesc.twistType;

	

	jointDesc.swing1Limit = desc->jointDesc.swing1Limit;
	jointDesc.swing2Limit = desc->jointDesc.swing2Limit;
	jointDesc.twistLimit = desc->jointDesc.twistLimit;


	//if(m_parent && false)
		m_joint = physics::IPhysicManager::getInstance().createJoint(&jointDesc);	

	
	


	m_shape->position= m_physicalNode->getGlobalPosition();
	m_shape->setOrintation(m_physicalNode->getGlobalOrintation());

	SegmentDesc::SegmentList::iterator it=desc->children.begin();
	for(;it!=desc->children.end();++it){
		SegmentDesc* c=*it;
		Segment*bone=new Segment(this);
		bone->loadFromDesc(c,smngr,root);
		m_children.push_back(bone);
	}
}

void Segment::setParent(Segment*p)
{
	m_parent=p;
}
Segment* Segment::getParent()
{
	return m_parent;
}

physics::IDOF6Joint3D* Segment::getJoint()
{
	return m_joint;
}



physics::IPhysicalNode* Segment::getPhysicalNode()
{
	return m_physicalNode;
}

void Segment::update(float dt)
{
	if(m_physicalNode && m_shape && !m_physicalNode->isSleeping()){
		m_shape->setPosition(m_physicalNode->getGlobalPosition());
		m_shape->setOrintation(m_physicalNode->getGlobalOrintation());
	}

	SegmentList::iterator it=m_children.begin();
	for(;it!=m_children.end();++it){
		GCPtr<Segment> c=*it;
		c->update(dt);
	}
}

}

