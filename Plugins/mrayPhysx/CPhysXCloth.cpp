#include "stdafx.h"


#include "CPhysXCloth.h"
#include "IHardwareIndexBuffer.h"
#include "IMeshBuffer.h"
#include "PhysXShape.h"
#include "CPhysXNode.h"
#include "IRenderable.h"
#include "IMeshBuffer.h"


#include <NxPhysics.h>
#include <NxMeshData.h>
#include "PhysXCommon.h"
#include "PhysXObjectConverter.h"

namespace mray
{
namespace physics
{

CPhysXCloth::CPhysXCloth(scene::IMeshBuffer*renderBuffer)
:m_renderBuffer(0),m_NumVertices(0),m_NumIndices(0),m_LastVertices(0),m_LastIndices(0),m_posStream(0),m_texStream(0),
m_normalStream(0),m_indexData(0),m_meshDirtyFlags(0),m_cloth(0),m_userData(0),m_teared(0),m_attachedNode(0),m_attachedMesh(0)
{
	m_ReceiveBuffers=new NxMeshData();
	m_renderBuffer=renderBuffer;
	_init();
}
CPhysXCloth::~CPhysXCloth()
{
	if(m_posStream)
		m_posStream->unlock();
	if(m_normalStream)
		m_normalStream->unlock();
	if(m_texStream)
		m_texStream->unlock();
	if(m_indexData)
		m_indexData->indexBuffer->unlock();
	delete m_ReceiveBuffers;
}
void CPhysXCloth::AttachNode(scene::IRenderable*node,scene::IMeshBuffer*mesh)
{
	m_attachedNode=node;
	m_attachedMesh=mesh;
}
scene::IRenderable* CPhysXCloth::GetAttachedNode()
{
	return m_attachedNode;
}
scene::IMeshBuffer* CPhysXCloth::GetAttachedMesh()
{
	return m_attachedMesh;
}
void CPhysXCloth::_init()
{
	m_posStream=m_renderBuffer->getStream(0,video::EMST_Position);
	m_normalStream=m_renderBuffer->getStream(0,video::EMST_Normal);
	m_texStream=m_renderBuffer->getStream(0,video::EMST_Texcoord);
	m_indexData=m_renderBuffer->getIndexData();

	int maxVertices=m_posStream->length();
	int maxIndices=m_indexData->indexCount;

	m_ReceiveBuffers->verticesPosBegin=m_posStream->lock(0,0,video::IHardwareBuffer::ELO_Normal);
	m_ReceiveBuffers->verticesPosByteStride=sizeof(math::vector3d);
	m_ReceiveBuffers->verticesNormalBegin=m_normalStream->lock(0,0,video::IHardwareBuffer::ELO_Normal);
	m_ReceiveBuffers->verticesNormalByteStride=sizeof(math::vector3d);
	m_ReceiveBuffers->maxVertices=maxVertices;
	m_ReceiveBuffers->numVerticesPtr  = &m_NumVertices;

	m_ReceiveBuffers->indicesBegin             = m_indexData->indexBuffer->lock(m_indexData->firstIndex,m_indexData->indexCount,video::IHardwareBuffer::ELO_Normal);
	m_ReceiveBuffers->indicesByteStride        = m_indexData->indexBuffer->getIndexSize();
	m_ReceiveBuffers->maxIndices               = maxIndices;
	m_ReceiveBuffers->numIndicesPtr            = &m_NumIndices;

	/*
	if(m_texStream)
	{
		m_ReceiveBuffers->
	}*/

	m_ReceiveBuffers->dirtyBufferFlagsPtr = &m_meshDirtyFlags;

}

void CPhysXCloth::SetCloth(NxCloth*c)
{
	m_cloth=c;
}
NxCloth* CPhysXCloth::GetCloth()
{
	return m_cloth;
}
NxMeshData& CPhysXCloth::getReceiveBuffers()
{
	return *m_ReceiveBuffers;
}

scene::IMeshBuffer* CPhysXCloth::GetRenderBuffer()
{
	return m_renderBuffer;
}
void CPhysXCloth::setFlag(EClothFlags flag,bool set)
{
}

bool CPhysXCloth::setBendingStiffness(float v)
{
	m_cloth->setBendingStiffness(v);
	return true;
}
float CPhysXCloth::getBendingStiffness()
{
	return m_cloth->getBendingStiffness();
}

bool CPhysXCloth::setStretchingStiffness(float v)
{
	m_cloth->setStretchingStiffness(v);
	return true;
}
float CPhysXCloth::getStretchingStiffness()
{
	return m_cloth->getStretchingStiffness();
}

bool CPhysXCloth::setDampingCoefficient(float v)
{
	m_cloth->setDampingCoefficient(v);
	return true;
}
float CPhysXCloth::getDampingCoefficient()
{
	return m_cloth->getDampingCoefficient();
}

bool CPhysXCloth::setFriction(float v)
{
	m_cloth->setFriction(v);
	return true;
}
float CPhysXCloth::getFriction()
{
	return m_cloth->getFriction();
}

bool CPhysXCloth::setPressure(float v)
{
	m_cloth->setPressure(v);
	return true;
}
float CPhysXCloth::getPressure()
{
	return m_cloth->getPressure();
}

bool CPhysXCloth::setTearFactor(float v)
{
	m_cloth->setTearFactor(v);
	return true;
}
float CPhysXCloth::getTearFactor()
{
	return m_cloth->getTearFactor();
}

bool CPhysXCloth::setAttachmentTearFactor(float v)
{
	m_cloth->setAttachmentTearFactor(v);
	return true;
}
float CPhysXCloth::getAttachmentTearFactor()
{
	return m_cloth->getAttachmentTearFactor();
}

bool CPhysXCloth::setThickness(float v)
{
	m_cloth->setThickness(v);
	return true;
}
float CPhysXCloth::getThickness()
{
	return m_cloth->getThickness();
}

float CPhysXCloth::getDensity()
{
	return m_cloth->getDensity();
}

bool CPhysXCloth::setSolverIterations(uint v)
{
	m_cloth->setSolverIterations(v);
	return true;
}
uint CPhysXCloth::getSolverIterations()
{
	return m_cloth->getSolverIterations();
}

void CPhysXCloth::getWorldBounds(math::box3d& AABB)
{
	NxBounds3 bounds;
	m_cloth->getWorldBounds(bounds);
	AABB.MinP.set(bounds.min.x,bounds.min.y,bounds.min.z);
	AABB.MaxP.set(bounds.max.x,bounds.max.y,bounds.max.z);
}

void CPhysXCloth::attachToShape(IPhysicalShape*shape,uint attachemtFlags)
{
	if(!shape)return;
	NxShape*s=(NxShape*) ((PhysXShape*)shape)->getNxShape();
	if(s==0)return;
	m_cloth->attachToShape(s,attachemtFlags);
}
void CPhysXCloth::attachToCollidingShapes(uint attachemtFlags)
{
	m_cloth->attachToCollidingShapes(attachemtFlags);
}
void CPhysXCloth::deattachFromShape(IPhysicalShape*shape)
{
	if(!shape)return;
	NxShape*s=(NxShape*) ((PhysXShape*)shape)->getNxShape();
	if(s==0)return;
	m_cloth->detachFromShape(s);
}
void CPhysXCloth::attachVertexToShape(uint verID,IPhysicalShape* node,const math::vector3d&localPos,uint attachemtFlags)
{
	if(!node)return;
	NxShape*s=(NxShape*) ((PhysXShape*)node)->getNxShape();
	if(s==0)return;
	m_cloth->attachVertexToShape(verID,s,ToNxVec3(localPos),attachemtFlags);
}
void CPhysXCloth::attachVertexToGlobalPos(uint verID,const math::vector3d&pos)
{
	m_cloth->attachVertexToGlobalPosition(verID,ToNxVec3(pos));
}
void CPhysXCloth::freeVertex(uint verID)
{
	m_cloth->freeVertex(verID);
}

void CPhysXCloth::dominateVertex(uint verID,float weight,float period)
{
	m_cloth->dominateVertex(verID,period,weight);
}
EClothVertexAttachmentStatus CPhysXCloth::getVertexAttachmentStatus(uint verID)
{
	NxClothVertexAttachmentStatus st= m_cloth->getVertexAttachmentStatus(verID);
	switch(st)
	{
	case NX_CLOTH_VERTEX_ATTACHMENT_GLOBAL:
		return ECVAS_Global;
	case NX_CLOTH_VERTEX_ATTACHMENT_SHAPE:
		return ECVAS_Shape;
	}
	return ECVAS_None;
}
IPhysicalShape* CPhysXCloth::getVertexAttachmentShape(uint verID)
{
	NxShape*sh= m_cloth->getVertexAttachmentShape(verID);
	if(!sh)return 0;
	return (IPhysicalShape*)sh->userData;
}
math::vector3d CPhysXCloth::getVertexAttachmentPosition(uint verID)
{
	NxVec3 p= m_cloth->getVertexAttachmentPosition(verID);
	return math::vector3d(p.x,p.y,p.z);
}

void CPhysXCloth::attachToCore(IPhysicalNode*node,float impulseThreshold,float penetrationDepth,
							   float maxDeformationDistance)
{
	if(!node)return;
	NxActor*s=(NxActor*) ((CPhysXNode*)node)->getActor();
	if(s==0)return;
	m_cloth->attachToCore(s,impulseThreshold,penetrationDepth,maxDeformationDistance);
}

void CPhysXCloth::tearVertex(uint verID,const math::vector3d&normal)
{
	m_cloth->tearVertex(verID,ToNxVec3(normal));
}

bool CPhysXCloth::rayCast(const math::Ray3d&ray,math::vector3d&hit,uint& verID)
{
	NxVec3 chit;
	bool ret=m_cloth->raycast(NxRay(ToNxVec3(ray.Start),ToNxVec3(ray.Dir)),chit,verID);
	hit.set(chit.x,chit.y,chit.z);
	return ret;
}

void CPhysXCloth::setCollisionGroup(GroupID gorup)
{
	m_cloth->setGroup(gorup);
}
GroupID CPhysXCloth::getCollisionGroup()
{
	return m_cloth->getGroup();
}

void CPhysXCloth::setMeshData(scene::IMeshBuffer*mesh)
{
}
scene::IMeshBuffer* CPhysXCloth::getMeshData()
{
	return m_renderBuffer;
}

void CPhysXCloth::setValidBounds(const math::box3d&bounds)
{
	NxBounds3 b;
	b.min=ToNxVec3(bounds.MinP);
	b.max=ToNxVec3(bounds.MaxP);
	m_cloth->setValidBounds(b);
}
void CPhysXCloth::getValidBounds(math::box3d&bounds)
{
	NxBounds3 b;
	m_cloth->getValidBounds(b);
	bounds.MinP.set(b.min.x,b.min.y,b.min.z);
	bounds.MaxP.set(b.max.x,b.max.y,b.max.z);
}

void CPhysXCloth::setVertexPosition(uint verID,const math::vector3d&pos)
{
	m_cloth->setPosition(ToNxVec3(pos),verID);
}
math::vector3d CPhysXCloth::getVertexPosition(uint verID)
{
	NxVec3 p= m_cloth->getPosition(verID);
	return math::vector3d(p.x,p.y,p.z);
}

void CPhysXCloth::setPositions(math::vector3d* buf)
{
	m_cloth->setPositions(buf,sizeof(math::vector3d));
}
void CPhysXCloth::getPositions(math::vector3d* buf)
{
	m_cloth->getPositions(buf,sizeof(math::vector3d));
}

void CPhysXCloth::setVertexVelocity(uint verID,const math::vector3d&vel)
{
	m_cloth->setVelocity(ToNxVec3(vel),verID);
}
math::vector3d CPhysXCloth::getVertexVelocity(uint verID)
{
	NxVec3 p= m_cloth->getVelocity(verID);
	return math::vector3d(p.x,p.y,p.z);
}

void CPhysXCloth::setVelocities(math::vector3d* buf)
{
	m_cloth->setVelocities(buf,sizeof(math::vector3d));
}
void CPhysXCloth::getVelocities(math::vector3d* buf)
{
	m_cloth->getVelocities(buf,sizeof(math::vector3d));
}

uint CPhysXCloth::getNumberOfParticles()
{
	return m_cloth->getNumberOfParticles();
}
void CPhysXCloth::setConstrainPositions(math::vector3d* positions)
{
	m_cloth->setConstrainPositions(positions,sizeof(math::vector3d));
}
void CPhysXCloth::setConstrainNormals(math::vector3d* normals)
{
	m_cloth->setConstrainNormals(normals,sizeof(math::vector3d));
}
void CPhysXCloth::setConstrainCoefficients(const PhysicalClothConstraintCoeff* coeffs)
{
	m_cloth->setConstrainCoefficients((NxClothConstrainCoefficients*)coeffs);
}

uint CPhysXCloth::queryShapePointers()
{
	return m_cloth->queryShapePointers();
}
void CPhysXCloth::getShapePointers(IPhysicalShape** shapePointers,uint *flags) 
{
}
void CPhysXCloth::setShapePointers(IPhysicalShape** shapePointers,uint *flags) 
{
}

bool CPhysXCloth::setCollisionResponseCoefficient(float v)
{
	m_cloth->setCollisionResponseCoefficient(v);
	return true;
}
float CPhysXCloth::getCollisionResponseCoefficient()
{
	return m_cloth->getCollisionResponseCoefficient();
}

bool CPhysXCloth::setAttachmentResponseCoefficient(float v)
{
	m_cloth->setAttachmentResponseCoefficient(v);
	return true;
}
float CPhysXCloth::getAttachmentResponseCoefficient()
{
	return m_cloth->getAttachmentResponseCoefficient();
}

bool CPhysXCloth::setExternalAcceleration(const math::vector3d& v)
{
	m_cloth->setExternalAcceleration(ToNxVec3(v));
	return true;
}
math::vector3d CPhysXCloth::getExternalAcceleration( )
{
	NxVec3 p= m_cloth->getExternalAcceleration();
	return math::vector3d(p.x,p.y,p.z);
}

bool CPhysXCloth::setWindAcceleration(const math::vector3d& v)
{
	m_cloth->setWindAcceleration(ToNxVec3(v));
	return true;
}
math::vector3d CPhysXCloth::getWindAcceleration()
{
	NxVec3 p= m_cloth->getWindAcceleration();
	return math::vector3d(p.x,p.y,p.z);
}

bool CPhysXCloth::isSleeping()
{
	return m_cloth->isSleeping();
}

bool CPhysXCloth::setSleepLinearVelocity(float v)
{
	m_cloth->setSleepLinearVelocity(v);
	return true;
}
float  CPhysXCloth::getSleepLinearVelocity()
{
	return m_cloth->getSleepLinearVelocity();
}

void CPhysXCloth::wakeUp(float wakeCounterValue)
{
	m_cloth->wakeUp(wakeCounterValue);
}
void CPhysXCloth::putToSleep()
{
	m_cloth->putToSleep();
}

void CPhysXCloth::addForceAtVertex(const math::vector3d& force, uint vertexId, 
								   EForceMode mode) 
{
	m_cloth->addForceAtVertex(ToNxVec3(force),vertexId,PhysXObjectConverter::convertForceMode(mode));
}
void CPhysXCloth::addForceAtPoint(const math::vector3d& pos, float magnitude,float radius, 
								  EForceMode mode ) 
{
	m_cloth->addForceAtPos(ToNxVec3(pos),magnitude,radius,PhysXObjectConverter::convertForceMode(mode));
}
void CPhysXCloth::addDirectedForceAtPoint(const math::vector3d& pos, const math::vector3d& force,float radius, 
										  EForceMode mode ) 
{
	m_cloth->addDirectedForceAtPos(ToNxVec3(pos),ToNxVec3(force),radius,PhysXObjectConverter::convertForceMode(mode));
}

void CPhysXCloth::setUserData(IObject*ud)
{
	m_userData=ud;
}
IObject*CPhysXCloth::getUserData()
{
	return m_userData;
}
void CPhysXCloth::update()
{
	m_LastVertices=m_NumVertices;
	m_LastIndices=m_NumIndices;
	if(m_renderBuffer )
	{
		math::box3d bb;
		getWorldBounds(bb);
		m_renderBuffer->setBoundingBox(bb);
		if(m_attachedNode)
			m_attachedNode->NotifyChangeBoundingBox();

		/*
		m_posStream->unlock();
		m_normalStream->unlock();
		m_indexData->indexBuffer->unlock();
		m_ReceiveBuffers->verticesPosBegin=m_posStream->lock(0,0,video::IHardwareBuffer::ELO_Normal);
		m_ReceiveBuffers->verticesNormalBegin=m_normalStream->lock(0,0,video::IHardwareBuffer::ELO_Normal);
		m_ReceiveBuffers->indicesBegin             = m_indexData->indexBuffer->lock(m_indexData->firstIndex,m_indexData->indexCount,video::IHardwareBuffer::ELO_Normal);*/
	}
}

}
}