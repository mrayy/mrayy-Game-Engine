

/********************************************************************
	created:	2011/08/21
	created:	21:8:2011   14:03
	filename: 	d:\Development\mrayEngine\Engine\mrayPhysx\CPhysXCloth.h
	file path:	d:\Development\mrayEngine\Engine\mrayPhysx
	file base:	CPhysXCloth
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __CPhysXCloth__
#define __CPhysXCloth__

#include "compileConfig.h"
#include "IPhysicalCloth.h"

class NxCloth;
class NxMeshData;

namespace mray
{
namespace scene
{
	class IMeshBuffer;
}
namespace video
{
	class IHardwareStreamBuffer;
	class IndexData;
}
namespace physics
{

class MRAY_PHYSX_DLL CPhysXCloth:public IPhysicalCloth
{
private:
protected:
	NxMeshData *m_ReceiveBuffers;
	GCPtr<scene::IMeshBuffer> m_renderBuffer;
	uint m_NumVertices;
	uint m_NumIndices;
	uint m_LastVertices;
	uint m_LastIndices;
	uint m_meshDirtyFlags;
	video::IHardwareStreamBuffer*	m_posStream;
	video::IHardwareStreamBuffer*	m_normalStream;
	video::IHardwareStreamBuffer*	m_texStream;
	video::IndexData*				m_indexData;

	NxCloth* m_cloth;

	bool m_teared;

	IObject* m_userData;

	scene::IRenderable* m_attachedNode;
	scene::IMeshBuffer* m_attachedMesh;

	void _init();
public:
	CPhysXCloth(scene::IMeshBuffer*renderBuffer);
	virtual~CPhysXCloth();

	void AttachNode(scene::IRenderable*node,scene::IMeshBuffer*mesh);
	scene::IRenderable* GetAttachedNode();
	scene::IMeshBuffer* GetAttachedMesh();

	void SetCloth(NxCloth*c);
	NxCloth* GetCloth();
	NxMeshData& getReceiveBuffers();

	scene::IMeshBuffer* GetRenderBuffer();

	void setFlag(EClothFlags flag,bool set);

	bool setBendingStiffness(float v);
	float getBendingStiffness();

	bool setStretchingStiffness(float v);
	float getStretchingStiffness();

	bool setDampingCoefficient(float v);
	float getDampingCoefficient();

	bool setFriction(float v);
	float getFriction();

	bool setPressure(float v);
	float getPressure();

	bool setTearFactor(float v);
	float getTearFactor();

	bool setAttachmentTearFactor(float v);
	float getAttachmentTearFactor();

	bool setThickness(float v);
	float getThickness();

	float getDensity();

	bool setSolverIterations(uint v);
	uint getSolverIterations();

	void getWorldBounds(math::box3d& AABB);

	void attachToShape(IPhysicalShape*shape,uint attachemtFlags);
	void attachToCollidingShapes(uint attachemtFlags);
	void deattachFromShape(IPhysicalShape*shape);
	void attachVertexToShape(uint verID,IPhysicalShape* node,const math::vector3d&localPos,uint attachemtFlags);
	void attachVertexToGlobalPos(uint verID,const math::vector3d&pos);
	void freeVertex(uint verID);

	void dominateVertex(uint verID,float weight,float period);
	EClothVertexAttachmentStatus getVertexAttachmentStatus(uint verID);
	IPhysicalShape* getVertexAttachmentShape(uint verID);
	math::vector3d getVertexAttachmentPosition(uint verID);

	void attachToCore(IPhysicalNode*node,float impulseThreshold,float penetrationDepth,
		float maxDeformationDistance);

	void tearVertex(uint verID,const math::vector3d&normal);

	bool rayCast(const math::Ray3d&ray,math::vector3d&hit,uint& verID);

	void setCollisionGroup(GroupID gorup);
	GroupID getCollisionGroup();

	void setMeshData(scene::IMeshBuffer*mesh);
	scene::IMeshBuffer* getMeshData();

	void setValidBounds(const math::box3d&bounds);
	void getValidBounds(math::box3d&bounds);

	void setVertexPosition(uint verID,const math::vector3d&pos);
	math::vector3d getVertexPosition(uint verID);

	void setPositions(math::vector3d* buf);
	void getPositions(math::vector3d* buf);

	void setVertexVelocity(uint verID,const math::vector3d&vel);
	math::vector3d getVertexVelocity(uint verID);

	void setVelocities(math::vector3d* buf);
	void getVelocities(math::vector3d* buf);

	uint getNumberOfParticles();

	void setConstrainPositions(math::vector3d* positions);
	void setConstrainNormals(math::vector3d* normals);
	void setConstrainCoefficients(const PhysicalClothConstraintCoeff* coeffs);

	uint queryShapePointers();
	void getShapePointers(IPhysicalShape** shapePointers,uint *flags) ;
	void setShapePointers(IPhysicalShape** shapePointers,uint *flags) ;

	bool setCollisionResponseCoefficient(float v);
	float getCollisionResponseCoefficient();

	bool setAttachmentResponseCoefficient(float v);
	float getAttachmentResponseCoefficient();

	bool setExternalAcceleration(const math::vector3d& v);
	math::vector3d getExternalAcceleration( );

	bool setWindAcceleration(const math::vector3d& v);
	math::vector3d getWindAcceleration();

	bool isSleeping();

	bool setSleepLinearVelocity(float v);
	float  getSleepLinearVelocity();

	void wakeUp(float wakeCounterValue=PHYICS_SLEEP_INTERVAL);
	void putToSleep();

	void addForceAtVertex(const math::vector3d& force, uint vertexId, 
		EForceMode mode = EFM_Force) ;
	void addForceAtPoint(const math::vector3d& pos, float magnitude,float radius, 
		EForceMode mode = EFM_Force) ;
	void addDirectedForceAtPoint(const math::vector3d& pos, const math::vector3d& force,float radius, 
		EForceMode mode = EFM_Force) ;

	void setUserData(IObject*ud);
	IObject*getUserData();

	void update();
};

}
}

#endif
