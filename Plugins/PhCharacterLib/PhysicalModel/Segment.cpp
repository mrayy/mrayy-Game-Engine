#include "stdafx.h"
//#include <NxPhysics.h>
#include "Segment.h"
//#include "PhysXManager.h"
//#include "physXCommon.h"
//#include "CPhysXNode.h"
#include "MeshBufferData.h"
#include "BoneNode.h"
#include <cmath>
#include "IDebugDrawManager.h"
#include "MeshResourceManager.h"
#include "Character.h"


namespace mray{
namespace PhysicsBasedCharacter
{

Segment::Segment(Character* creator,Segment*parent)
{
	m_creator=creator;
	m_physicalNode=0;
	m_parent=parent;
	m_joint=0;
}
Segment::~Segment()
{
	m_children.clear();
}

void Segment::loadFromDesc(SegmentDesc*desc,physics::IPhysicManager*phManager, scene::Skeleton* skeleton,float scale)
{
/*
	math::vector3d xAxis = desc->back.Normalize();
	math::vector3d zAxis = desc->down.Normalize();
	math::vector3d yAxis = zAxis.crossProduct(xAxis);
	m_boneToSegment.setComponent(xAxis,yAxis,zAxis);
	m_boneToSegment.getTransposeMatrix(m_SegmentToBone);*/

 
	m_name=desc->name;
	//1st , create physical node
	physics::PhysicalNodeDesc phNodeDesc;
	physics::PhysicalBodyDesc phBodyDesc;
	physics::PhysicMaterialDesc phMatDesc;
	std::vector<physics::ConvexShapeDesc> phTriangleDesc;
	physics::SphereShapeDesc phShpereDesc;
	physics::CapsuleShapeDesc phCapsuleDesc;		
	physics::BoxShapeDesc phBoxDesc;
	math::quaternion q;
	math::matrix4x4 mat, swing1Mat,swing2Mat,twistMat;


	//Create Physical shapes 
	if(desc->shapeDesc.type==mT("model") )
	{
		scene::SMesh* mesh=gMeshResourceManager.loadMesh(desc->shapeDesc.model,true);
		if(mesh){
			//GCPtr<scene::SMesh> mesh=m_shape->getMesh();
			physics::ConvexShapeDesc tShape;
			for (int i=0;i<mesh->getBuffersCount();++i)
			{
				scene::MeshBufferData* bd=mesh->getBufferData(i);
				scene::SMeshManipulator::getInstance().scaleMesh(bd->getMeshBuffer(),desc->shapeDesc.length);

				tShape.mesh=bd->getMeshBuffer().pointer();
				tShape.flipNormals=false;
				phTriangleDesc.push_back(tShape);
			}

			for (int i=0;i<phTriangleDesc.size();++i)
			{
				phNodeDesc.shapes.push_back(&phTriangleDesc[i]);
			}
		}
	}
	else if(desc->shapeDesc.type==mT("sphere") )
	{
		phShpereDesc.radius = desc->shapeDesc.radius*scale;
		if (desc->shapeDesc.attach==mT("top"))
		{
			desc->shapeDesc.trans.set(0,0,phShpereDesc.radius);
		}else if(desc->shapeDesc.attach==mT("bottom"))
		{
			desc->shapeDesc.trans.set(0,0,-phShpereDesc.radius);
		}
		mat.setTranslation(desc->shapeDesc.trans*scale);
		phShpereDesc.localSpace = mat;
		phNodeDesc.shapes.push_back(&phShpereDesc);

	}
	else if(desc->shapeDesc.type==mT("capsule"))
	{
		phCapsuleDesc.height = desc->shapeDesc.length-2*desc->shapeDesc.radius;
		phCapsuleDesc.radius = desc->shapeDesc.radius;
		phCapsuleDesc.height*=scale;
		phCapsuleDesc.radius*=scale;
		if (desc->shapeDesc.attach==mT("top"))
		{
			desc->shapeDesc.trans.set(0,0,(phCapsuleDesc.height/2)+phCapsuleDesc.radius);
		}else if(desc->shapeDesc.attach==mT("bottom"))
		{
			desc->shapeDesc.trans.set(0,0,-(phCapsuleDesc.height/2)-phCapsuleDesc.radius);
		}
		mat.rotateX(90).setTranslation(desc->shapeDesc.trans*scale);
		phCapsuleDesc.localSpace = mat;
		phNodeDesc.shapes.push_back(&phCapsuleDesc);

	}
	else if(desc->shapeDesc.type==mT("box"))
	{
		phBoxDesc.extent = desc->shapeDesc.extent*scale/2;
		if (desc->shapeDesc.attach==mT("top"))
		{
			desc->shapeDesc.trans.set(0,0,phBoxDesc.extent.z);
		}else if(desc->shapeDesc.attach==mT("topback"))
		{
			desc->shapeDesc.trans.set(-0.7*phBoxDesc.extent.x,0,phBoxDesc.extent.z);
		}
		mat.setTranslation(desc->shapeDesc.trans*scale);

		phBoxDesc.localSpace = mat;
		phNodeDesc.shapes.push_back(&phBoxDesc);
	}
	else if (desc->shapeDesc.type==mT("objects"))
	{

		ObjectDesc* objDesc;
		physics::IPhysicalShapeDesc* physicalShapeDesc;

		for (std::list<ObjectDesc*>::iterator it = desc->shapeDesc.objects.begin();it!=desc->shapeDesc.objects.end();++it)
		{
			objDesc = (*it);
			math::vector3d pos=objDesc->translation*scale;

			if (objDesc->type == mT("cylinder"))
			{
				physics::CapsuleShapeDesc* capsuleDesc = new physics::CapsuleShapeDesc();
				capsuleDesc->height = objDesc->height*scale;
				capsuleDesc->radius = objDesc->radius*scale;
				physicalShapeDesc = capsuleDesc;
				//pos=0;
				//pos.x=objDesc->radius+0.5f*objDesc->height;
			}
			else if (objDesc->type == mT("cube"))
			{
				physics::BoxShapeDesc* boxDesc = new physics::BoxShapeDesc();
				boxDesc->extent.set(objDesc->width/2,objDesc->height/2,objDesc->depth/2);
				boxDesc->extent*=scale;
				physicalShapeDesc = boxDesc;
			}
			else if(objDesc->type == mT("sphere"))
			{
				physics::SphereShapeDesc* sphereDesc = new physics::SphereShapeDesc();
				sphereDesc->radius = objDesc->radius*scale;
				physicalShapeDesc = sphereDesc;
			}
			math::matrix4x4 transfom;

			transfom = transfom.setRotationDegrees(objDesc->rotation);
			//transfom=xRot*yRot*zRot;
			//transfom = transfom.setTranslation(objDesc->translation);
			transfom = transfom.setTranslation(pos);
			//transfom = transfom.setScale(objDesc->scale);
			physicalShapeDesc->localSpace = /*m_boneToSegment * */transfom;
			phNodeDesc.shapes.push_back(physicalShapeDesc);				

		}

	}
	phNodeDesc.name=m_name;
	//create physic material
	phMatDesc.dynamicFriction = 0.8;
	phMatDesc.staticFriction = 1;
	phMatDesc.restitution = 0;
	if(phNodeDesc.shapes.size()>0)
	{
		physics::IPhysicMaterial* shapeMat = phManager->createMaterial(&phMatDesc);
		for (int i=0;i<phNodeDesc.shapes.size();i++)
		{			
			phNodeDesc.shapes[i]->name=m_name;
			phNodeDesc.shapes[i]->material = shapeMat;
		}
	}
	//Fill Body Node Info
	phNodeDesc.bodyDesc = &phBodyDesc;		

	phBodyDesc.solverIterationCount = 4;//15;
	//Fill Inertial Info
	if (desc->inertiaDesc.shape)
	{
		//std::list<physics::IPhysicalShapeDesc*>::iterator it = phNodeDesc.shapes.begin();
		//for (;it!=phNodeDesc.shapes.end();it++)
		//{			
		//	(*it)->density = desc->inertiaDesc.density;
		//	(*it)->mass = desc->inertiaDesc.mass;
		//}


		phNodeDesc.density = 0;
		phBodyDesc.mass =  desc->inertiaDesc.mass;
		phBodyDesc.massSpaceInertia.set(0,0,0);
	}
	else
	{
		phBodyDesc.massSpaceInertia = desc->inertiaDesc.moments;
		twistMat.makeIdentity();twistMat.rotateZ(desc->inertiaDesc.rot.z);
		swing1Mat.makeIdentity();swing1Mat.rotateY(desc->inertiaDesc.rot.y);
		swing2Mat.makeIdentity();swing2Mat.rotateX(desc->inertiaDesc.rot.x);
		phBodyDesc.massLocalPos = swing1Mat * swing2Mat * twistMat;
		phBodyDesc.massLocalPos.setTranslation(desc->inertiaDesc.trans*scale);
	}



	//Fill Pose Info

	m_bone =0;
	if (desc->bone!=mT("") && skeleton!=0)
	{
		m_bone = skeleton->getBone(desc->bone);
		if(!m_bone)
			return;
		if (m_parent!=0)
		{
			math::matrix4x4 childBoneToWorld;
			math::matrix4x4 parentBoneToWorld;
			math::matrix4x4 worldToParentBone;

			m_bone->getAbsoluteBaseOrintation().toMatrix(childBoneToWorld);
			childBoneToWorld.setTranslation(m_bone->getAbsoluteBasePosition()*scale);

			m_parent->m_bone->getAbsoluteBaseOrintation().toMatrix(parentBoneToWorld);
			parentBoneToWorld.setTranslation(m_parent->m_bone->getAbsoluteBasePosition()*scale);
			parentBoneToWorld.getInverse(worldToParentBone);

			//m_bone->getBaseOrintation().toMatrix(childBoneToParentBone);
			//childBoneToParentBone.setTranslation(m_bone->getBasePosition());
			//mat =  m_parent->boneToSegment * childBoneToParentBone * this->SegmentToBone;
			math::matrix4x4 childBoneToParentBone = worldToParentBone * childBoneToWorld ;
			//mat =  m_parent->m_boneToSegment * childBoneToParentBone * this->m_SegmentToBone;
 			
 			math::matrix4x4 mat2;
 			m_parent->m_physicalNode->getGlobalOrintation().toMatrix(mat2);
 			mat2.setTranslation(m_parent->m_physicalNode->getGlobalPosition());
// 			
			//m_bone->getBaseOrintation().toMatrix(mat);
			//mat.setTranslation(m_bone->getBasePosition());
			phNodeDesc.globalPos = mat2 * childBoneToParentBone;
		}
		else
		{
			math::matrix4x4 mat2;
			/*
			math::matrix4x4 boneToWorld;
			m_bone->getAbsoluteBaseOrintation().toMatrix(boneToWorld);
			boneToWorld.setTranslation(m_bone->getAbsoluteBasePosition());
			mat =  boneToWorld * this->m_SegmentToBone;*/
			
			mat.makeIdentity();
			mat.setRotationDegrees(desc->rot);
			mat.setTranslation(desc->trans);

			mat2=m_bone->getAbsoluteTransformation();

			mat2.setTranslation(mat2.getTranslation()*scale);

			phNodeDesc.globalPos =mat * mat2;
		}
	}
	else
	{
		twistMat.makeIdentity();twistMat.rotateZ(desc->rot.z);
		swing1Mat.makeIdentity();swing1Mat.rotateY(desc->rot.y);
		swing2Mat.makeIdentity();swing2Mat.rotateX(desc->rot.x);
		mat = swing1Mat * swing2Mat * twistMat;
		mat.setTranslation(desc->trans);

		if (m_parent)
		{
			math::matrix4x4 mat2;
			m_parent->m_physicalNode->getGlobalOrintation().toMatrix(mat2);
			mat2.setTranslation(m_parent->m_physicalNode->getGlobalPosition());
			phNodeDesc.globalPos = mat2 * mat;
		}
		else
		{
			phNodeDesc.globalPos = mat;
		}	
	}

	startOrient.fromMatrix(phNodeDesc.globalPos);
	//Other Properties
	phNodeDesc.bodyDesc->sleepDamping = 10;
	phNodeDesc.userData=m_creator;

	m_physicalNode=phManager->createNode(&phNodeDesc);

	for (int i=0;i<phNodeDesc.shapes.size();i++)
	{
		delete phNodeDesc.shapes[i];
	}

	//Create The Joint

	physics::IPhysicalJointD6Desc jointDesc;
	if(!m_parent)
	{
		jointDesc.body1 = 0;
	}
	else
	{
		jointDesc.body1 = m_parent->m_physicalNode;
	}
	jointDesc.body2 = m_physicalNode;


	jointDesc.setFromGlobalAnchor(m_physicalNode->getGlobalPosition());
	//jointDesc.setFromGlobalAxis(-math::vector3d::ZAxis);

	float swing1Value = (desc->jointDesc.swing1Limit.high - desc->jointDesc.swing1Limit.low)/2;
	float swing2Value = (desc->jointDesc.swing2Limit.high - desc->jointDesc.swing2Limit.low)/2;
	float parentS1 =	-swing1Value - desc->jointDesc.swing1Limit.low;
	float parentS2 =	-swing2Value - desc->jointDesc.swing2Limit.low;

	if(1)
	{

		swing1Mat.makeIdentity();swing1Mat.rotateY(-parentS1);
		swing2Mat.makeIdentity();swing2Mat.rotateX(-parentS2);


		mat =   (swing1Mat * swing2Mat);

		jointDesc.flags[physics::EJFlag_CollisionEnable]=desc->jointDesc.collision;

		jointDesc.localAxis[0] = mat.getAxisVector(2); // parentFrame's ZAxis
		jointDesc.localNormal[0] = mat.getAxisVector(1);// parentFrame's YAxis

		jointDesc.localAxis[1] = math::vector3d.ZAxis;
		jointDesc.localNormal[1] = math::vector3d.YAxis;
		m_swing1Offset = parentS1;
		m_swing2Offset = -parentS2;

	}


	if(0)
	{
		jointDesc.slerpDrive.drivePosition=true;
		jointDesc.slerpDrive.spring=250;
		jointDesc.slerpDriveFlag=true;
	}

	jointDesc.xMotion=desc->jointDesc.xlimitType;
	jointDesc.yMotion=desc->jointDesc.ylimitType;
	jointDesc.zMotion=desc->jointDesc.zlimitType;
	jointDesc.linearLimit=desc->jointDesc.linearLimitDesc;

	if(1)
	{
		jointDesc.swing1Motion=desc->jointDesc.swing1Type;
		jointDesc.swing2Motion=desc->jointDesc.swing2Type;
		jointDesc.twistMotion=desc->jointDesc.twistType;


		jointDesc.swing1Limit.value = math::toRad(swing1Value);
		jointDesc.swing1Limit.damping = desc->jointDesc.swing1Limit.damping;
		jointDesc.swing1Limit.restitution = desc->jointDesc.swing1Limit.restitution;
		jointDesc.swing1Limit.spring = desc->jointDesc.swing1Limit.spring;

		jointDesc.swing2Limit.value = math::toRad(swing2Value);
		jointDesc.swing2Limit.damping = desc->jointDesc.swing2Limit.damping;
		jointDesc.swing2Limit.restitution = desc->jointDesc.swing2Limit.restitution;
		jointDesc.swing2Limit.spring = desc->jointDesc.swing2Limit.spring;

		jointDesc.twistLimit = desc->jointDesc.twistLimit;
		jointDesc.twistLimit.low.value = math::toRad(desc->jointDesc.twistLimit.low.value);
		jointDesc.twistLimit.high.value = math::toRad(desc->jointDesc.twistLimit.high.value);
	}

	//	jointDesc.projectionMode = physics::EJointProjectionMode::EJPM_PointMindist;
	//jointDesc.projectionDistance = 0.0005;
	//jointDesc.projectionDistance = 0.001;
	m_joint =(physics::IDOF6Joint3D*) phManager->createJoint(&jointDesc);	


	//Pose Shape
/*			if(m_shape)
	{
		m_shape->position= m_physicalNode->getGlobalPosition();
		m_shape->setOrintation(m_physicalNode->getGlobalOrintation());
	}*/


	//Load Children
	SegmentDesc::SegmentList::iterator it=desc->children.begin();
	for(;it!=desc->children.end();++it){
		SegmentDesc* c=*it;
		Segment*bone=new Segment(m_creator,this);
		bone->loadFromDesc(c,phManager, skeleton,scale/*,smngr,root*/);
		m_children.push_back(bone);
	}
}



void Segment::setName(const core::string&n)
{
	m_name=n;
}
const core::string& Segment::getName()
{
	return m_name;
}

void Segment::setParent(Segment*p)
{
	m_parent=p;
}
Segment* Segment::getParent()
{
	return m_parent;
}

SegmentList* Segment::getDirectChildren()
{
	return &m_children;
}

void Segment::getAllChildren(SegmentList&children)
{
	children.push_back(this);
	for (SegmentList::iterator it=m_children.begin(); it!=m_children.end();++it)
	{				
		(*it)->getAllChildren(children);
	}

}

Segment* Segment::getChild( const core::string &name )
{
	if (name.equals_ignore_case(m_name))
	{
		return this;
	}
	Segment* temp;
	for (SegmentList::iterator it=m_children.begin(); it!=m_children.end();++it)
	{				
		temp = (*it)->getChild(name);
		if (temp != NULL)
		{
			return temp;
		}
	}
	return NULL;
}

physics::IPhysicalNode* Segment::getActor()
{
	return m_physicalNode;

}

physics::IDOF6Joint3D* Segment::getJoint()
{
	return m_joint;

}

void Segment::setTargetOrientation(const math::vector3d& orient)
{
	math::matrix4x4 s1,s2,twistMat,swing1Mat,swing2Mat;
	math::quaternion targetOrient,temp;
	twistMat.rotateX(orient.z);
	swing1Mat.rotateY(orient.y);
	swing2Mat.rotateZ((-1 * orient.x));

	s1.rotateY(m_swing1Offset);
	s2.rotateZ(m_swing2Offset);

	targetOrient.fromMatrix( s2 * s1 * swing1Mat * swing2Mat  *  twistMat);
	m_joint->setDriveOrientation(targetOrient);
}

void Segment::setTargetOrientation(const math::quaternion& orient)
{
	math::vector3d e;
	orient.toEulerAngles(e);
	setTargetOrientation(e);
	return;
	math::matrix4x4 mat;
	orient.toMatrix(mat);
	setTargetOrientation(matrixToEularYXZ(mat));
}	
void Segment::setTargetAngularVelocity(const math::vector3d &vel )
{
	math::matrix4x4 s1,s2,tempMat;
	math::vector3d tempVec1,tempVec2;

	tempVec1.x = vel.z;
	tempVec1.y = vel.y;
	tempVec1.z = -1 * vel.x;
	tempMat = s2 * s1;
	tempVec2=tempMat*tempVec1;

	m_joint->setDriveAngularVelocity(tempVec2);
}




void Segment::setAngularDriveProperties( float stifness, float damping,float torquelimit, bool driveOrientation, bool driveVelocity )
{

	physics::PhysicalJointDriveDesc twistDesc;
	twistDesc.spring = stifness;
	twistDesc.damping = damping;
	twistDesc.forceLimit = torquelimit;
	twistDesc.drivePosition = driveOrientation;
	twistDesc.driveVelocity = driveVelocity;

	physics::PhysicalJointDriveDesc swingDesc;
	swingDesc.spring = stifness;
	swingDesc.damping = damping;
	swingDesc.forceLimit = torquelimit;
	swingDesc.drivePosition = driveOrientation;
	swingDesc.driveVelocity = driveVelocity;

	m_joint->setAngularDriveProperties(twistDesc,swingDesc);
}


void Segment::setGlobalTranslation(const math::vector3d& pos)
{
	m_physicalNode->setGlobalPosition(pos);
}

void Segment::setGlobalOrientation(const math::quaternion& orient)
{
	m_physicalNode->setGlobalOrintation(orient);
}

math::vector3d Segment::getGlobalTranslation()
{
	if(!m_physicalNode)
		return math::vector3d();
	return m_physicalNode->getGlobalPosition();
}
math::quaternion Segment::getGlobalOrientation()
{
	if(!m_physicalNode)
		return math::quaternion();
	return m_physicalNode->getGlobalOrintation();
}
math::vector3d Segment::getGlobalLinearVelocity()
{
	if(!m_physicalNode)
		return math::vector3d();
	return m_physicalNode->getLinearVelocity();
}
math::vector3d Segment::getGlobalAngularVelocity()
{
	if(!m_physicalNode)
		return math::vector3d();
	return m_physicalNode->getAngularVelocity();
}


math::vector3d Segment::getRelativeTranslation(const math::quaternion& frame)
{
	if(!m_physicalNode)
		return math::vector3d();
	return frame.inverse() * m_physicalNode->getGlobalPosition();
}
math::quaternion Segment::getRelativeOrientation(const math::quaternion& frame)
{
	if(!m_physicalNode)
		return math::quaternion();
	return frame.inverse() * m_physicalNode->getGlobalOrintation();
}
math::vector3d Segment::getRelativeLinearVelocity(const math::quaternion& frame)
{
	if(!m_physicalNode)
		return math::vector3d();
	return frame.inverse() * m_physicalNode->getLinearVelocity();
}
math::vector3d Segment::getRelativeAngularVelocity(const math::quaternion& frame)
{
	if(!m_physicalNode)
		return math::vector3d();
	return  frame.inverse() * m_physicalNode->getAngularVelocity();
}


void Segment::applyGlobalTorque( const math::vector3d &torque )
{
	if(!m_physicalNode)
		return;
	m_physicalNode->addTorque(torque,physics::EFM_Force,true);
}

void Segment::applyRelativeTorque(const math::quaternion& frame, const math::vector3d &torque)
{
	if(!m_physicalNode)
		return;
	m_physicalNode->addTorque(frame * torque,physics::EFM_Force,true);
}


math::vector3d Segment::getPD_Torque(const math::quaternion& frame, float ks, float kd)
{
	math::vector3d torque,currentVel,error,Rel,RelD;
	float torqueX,torqueY,torqueZ;

	math::quaternion qRel = getRelativeOrientation(frame);
	qRel.toEulerAngles(Rel);
	RelD = target;

	Rel.x = math::toRad(Rel.x); Rel.y = math::toRad(Rel.y);Rel.z = math::toRad(Rel.z);
	RelD.x = math::toRad(RelD.x); RelD.y = math::toRad(RelD.y); RelD.z = math::toRad(RelD.z);
	error  = (RelD - Rel);
	currentVel = qRel.inverse() * getRelativeAngularVelocity(frame);

	torqueX=(ks*error.x)-(kd*currentVel.x);
	torqueY=(ks*error.y)-(kd*currentVel.y);
	torqueZ=(ks*error.z)-(kd*currentVel.z);
	torque = qRel * (math::vector3d(torqueX,torqueY,torqueZ));

	return torque;
}

math::quaternion Segment::getSpecialOrientation()
{
	if(!m_physicalNode)
		return math::quaternion();
	return startOrient.inverse() * m_physicalNode->getGlobalOrintation();
}

math::vector3d Segment::getSpecialAngularVelocity()
{
	if(!m_physicalNode)
		return math::vector3d();
	return  startOrient.inverse() * m_physicalNode->getAngularVelocity();
}

void Segment::applySpecialTorque( const math::vector3d &torque )
{
	if(!m_physicalNode)
		return;
	m_physicalNode->addTorque(startOrient * torque,physics::EFM_Force,true);
}

math::vector3d Segment::getSpecialTorque(const math::vector3d& ks, const math::vector3d& kd)
{
	math::vector3d torque,currentVel,error,Rel,RelD,torqueX,torqueY,torqueZ;

	math::quaternion qRel = getSpecialOrientation();
	qRel.toEulerAngles(Rel);
	RelD = target;

	Rel.x = math::toRad(Rel.x); Rel.y = math::toRad(Rel.y);Rel.z = math::toRad(Rel.z);
	RelD.x = math::toRad(RelD.x); RelD.y = math::toRad(RelD.y); RelD.z = math::toRad(RelD.z);
	error  = (RelD - Rel);
	currentVel = qRel.inverse() * getSpecialAngularVelocity();

	torqueX.set((ks.x*error.x)-(kd.x*currentVel.x),0,0);
	torqueY.set(0,(ks.y*error.y)-(kd.y*currentVel.y),0);
	torqueZ.set(0,0,(ks.z*error.z)-(kd.z*currentVel.z));

	torque = qRel * (torqueX+torqueY+torqueZ);

	return torque;
}

void Segment::applyGlobalForce( const math::vector3d &force )
{
	if(!m_physicalNode)
		return;
	m_physicalNode->addForce(force, physics::EFM_Force, true);
}

void Segment::applyGlobalImpact( const math::vector3d &force )
{
	if(!m_physicalNode)
		return;
	m_physicalNode->addForce(force, physics::EFM_Impulse, true);

}

math::vector3d Segment::matrixToEularYXZ(const math::matrix4x4& MInput)
{
	math::vector3d r;
	math::matrix4x4 M=MInput;

	for (int i=0; i<4; i++)
	{
		for (int j=0; j<4 ;j++)
		{
			if(math::fEqual(M(i,j),0,0.00001))
				M(i,j) = 0;
			if(math::fEqual(M(i,j),1,0.00001))
				M(i,j) = 1;
			if(math::fEqual(M(i,j),-1,0.00001))
				M(i,j) = -1;
		}
	}

	if (M(2,1)<1)
	{
		if (M(2,1)>-1)
		{
			r.x = math::toDeg(asin(-1 * M(2,1)));
			r.y = math::toDeg(atan2(M(2,0), M(2,2)));
			r.z = math::toDeg(atan2(M(0,1),M(1,1)));
		}
		else
		{
			r.x = 90;
			r.y = -1 * math::toDeg(atan2(-1 * M(1,0),M(0,0)));
			r.z = 0;
		}
	}
	else
	{
		r.x = -90;
		r.y = math::toDeg(atan2(-1 * M(1,0),M(0,0)));
		r.z = 0;
	}

	return r;
}

math::vector3d Segment::quaternionToVector3D( const math::quaternion& q )
{
	math::vector3d ret;
	q.toEulerAngles(ret);
	return ret;
	math::matrix4x4 mat;
	q.toMatrix(mat);
	return matrixToEularYXZ(mat);
}

float Segment::getMass()
{
	if(!m_physicalNode)
		return 0;
	return m_physicalNode->getMass();
}

void Segment::setGlobalLinearVelocity( const math::vector3d& po )
{
	if(!m_physicalNode)
		return;
	m_physicalNode->setLinearVelocity(po);
}

void Segment::setGlobalAngularVelocity( const math::vector3d& po )
{
	if(!m_physicalNode)
		return;
	m_physicalNode->setAngularVelocity(po);
}

void Segment::moveGlobalTranslation( const math::vector3d& pos )
{
	getActor()->moveGlobalPosition(pos);
}

void Segment::moveGlobalOrientation(const  math::quaternion& orient )
{
	getActor()->moveGlobalOrientation(orient);
}
void Segment::update(float dt)
{/*
 if(m_physicalNode && m_shape && !m_physicalNode->isSleeping()){
 m_shape->setPosition(m_physicalNode->getGlobalPosition());
 m_shape->setOrintation(m_physicalNode->getGlobalOrintation());
 }*/

	if (m_bone!=0)
	{
		if(m_physicalNode->GetBodyFlag(physics::EBF_Kinematic))
		{
			m_physicalNode->setGlobalOrintation(m_bone->getAbsoluteOrintation());
		}else
		{
			//math::matrix4x4 segmentToWorld;
			//math::matrix4x4 worldToParentBone;
			math::quaternion qWorld=getGlobalOrientation();
			math::quaternion qParentInv;
			//getGlobalOrientation().toMatrix(segmentToWorld);
			if(m_bone->getParentBone())
				qParentInv=m_bone->getParentBone()->getAbsoluteOrintation().inverse();//.toMatrix(worldToParentBone);
			//m_bone->setOrintation(worldToParentBone  * segmentToWorld);// * this->m_boneToSegment);
			m_bone->setOrintation(qParentInv*qWorld);
			m_bone->update(dt);
		}
	}
	//if(m_joint )
	//	setAngularDriveProperties(0,0,0,false,false);

	SegmentList::iterator it=m_children.begin();
	for(;it!=m_children.end();++it){
		Segment* c=*it;
		c->update(dt);
	}
}
void Segment::Visualize(scene::IDebugDrawManager* renderer)
{
	if(!m_physicalNode)
		return;
	math::vector3d pos=getGlobalTranslation();
	//math::vector3d vel=m_physicalNode->getLinearVelocity();
	renderer->AddCross(pos,1,video::DefaultColors::Red,0);
	//renderer->AddArrow(pos,pos+vel,video::DefaultColors::Yellow,1,0);
	SegmentList::iterator it=m_children.begin();
	for(;it!=m_children.end();++it)
	{
		Segment* c=*it;
		math::vector3d endP=c->getGlobalTranslation();
		renderer->AddLine(pos,endP,video::DefaultColors::Orange,0);
		c->Visualize(renderer);
	}
	if(m_children.size()==0)
	{
		math::vector3d endP=pos+getGlobalOrientation()*math::vector3d(0,0,2);
		renderer->AddLine(pos,endP,video::DefaultColors::Orange,0);
	}
}


}
}

