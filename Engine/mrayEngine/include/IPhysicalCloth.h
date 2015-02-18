
/********************************************************************
	created:	2009/11/27
	created:	27:11:2009   14:41
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IPhysicalCloth.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IPhysicalCloth
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IPhysicalCloth___
#define ___IPhysicalCloth___

#include "IPhysicalClothDef.h"
#include "IObject.h"
#include "TypedProperty.h"

namespace mray
{
namespace scene
{
	class IRenderable;
	class IMeshBuffer;

}
namespace physics
{

	class IPhysicalShape;
	class IPhysicalNode;

class MRAY_DLL IPhysicalCloth:public IObject
{
	DECLARE_RTTI;
private:
protected:
	void fillProperties();
public:
	DECLARE_PROPERTY_TYPE(BendingStiffness,float,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(StretchingStiffness,float,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(DampingCoeff,float,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(Friction,float,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(Pressure,float,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(TearFactor,float,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(SolverIterations,uint,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(CollisionResponseCoeff,float,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(AttachmentResponseCoeff,float,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(ExternalAcceleration,math::vector3d,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(WindAcceleration,math::vector3d,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(SleepLinearVelocity,float,MRAY_DLL);

public:

	IPhysicalCloth()
	{
		fillProperties();
	}
	virtual~IPhysicalCloth(){}

	virtual scene::IMeshBuffer* GetRenderBuffer()=0;

	virtual void AttachNode(scene::IRenderable*node,scene::IMeshBuffer*mesh)=0;
	virtual scene::IRenderable* GetAttachedNode()=0;
	virtual scene::IMeshBuffer* GetAttachedMesh()=0;

	virtual void setFlag(EClothFlags flag,bool set)=0;

	virtual bool setBendingStiffness(float v)=0;
	virtual float getBendingStiffness()=0;

	virtual bool setStretchingStiffness(float v)=0;
	virtual float getStretchingStiffness()=0;

	virtual bool setDampingCoefficient(float v)=0;
	virtual float getDampingCoefficient()=0;

	virtual bool setFriction(float v)=0;
	virtual float getFriction()=0;

	virtual bool setPressure(float v)=0;
	virtual float getPressure()=0;

	virtual bool setTearFactor(float v)=0;
	virtual float getTearFactor()=0;

	virtual bool setAttachmentTearFactor(float v)=0;
	virtual float getAttachmentTearFactor()=0;

	virtual bool setThickness(float v)=0;
	virtual float getThickness()=0;

	virtual float getDensity()=0;

	virtual bool setSolverIterations(uint v)=0;
	virtual uint getSolverIterations()=0;

	virtual void getWorldBounds(math::box3d& AABB)=0;

	virtual void attachToShape(IPhysicalShape*shape,uint attachemtFlags)=0;
	virtual void attachToCollidingShapes(uint attachemtFlags)=0;
	virtual void deattachFromShape(IPhysicalShape*shape)=0;
	virtual void attachVertexToShape(uint verID,IPhysicalShape* node,const math::vector3d&localPos,uint attachemtFlags)=0;
	virtual void attachVertexToGlobalPos(uint verID,const math::vector3d&pos)=0;
	virtual void freeVertex(uint verID)=0;

	virtual void dominateVertex(uint verID,float weight,float period)=0;
	virtual EClothVertexAttachmentStatus getVertexAttachmentStatus(uint verID)=0;
	virtual IPhysicalShape* getVertexAttachmentShape(uint verID)=0;
	virtual math::vector3d getVertexAttachmentPosition(uint verID)=0;

	virtual void attachToCore(IPhysicalNode*node,float impulseThreshold,float penetrationDepth=0,
		float maxDeformationDistance=0)=0;

	virtual void tearVertex(uint verID,const math::vector3d&normal)=0;

	virtual bool rayCast(const math::Ray3d&ray,math::vector3d&hit,uint& verID)=0;
	
	virtual void setCollisionGroup(GroupID gorup)=0;
	virtual GroupID getCollisionGroup()=0;

	virtual void setMeshData(scene::IMeshBuffer*mesh)=0;
	virtual scene::IMeshBuffer* getMeshData()=0;

	virtual void setValidBounds(const math::box3d&bounds)=0;
	virtual void getValidBounds(math::box3d&bounds)=0;

	virtual void setVertexPosition(uint verID,const math::vector3d&pos)=0;
	virtual math::vector3d getVertexPosition(uint verID)=0;

	virtual void setPositions(math::vector3d* buf)=0;
	virtual void getPositions(math::vector3d* buf)=0;

	virtual void setVertexVelocity(uint verID,const math::vector3d&vel)=0;
	virtual math::vector3d getVertexVelocity(uint verID)=0;

	virtual void setVelocities(math::vector3d* buf)=0;
	virtual void getVelocities(math::vector3d* buf)=0;

	virtual uint getNumberOfParticles()=0;

	virtual void setConstrainPositions(math::vector3d* positions)=0;
	virtual void setConstrainNormals(math::vector3d* normals)=0;
	virtual void setConstrainCoefficients(const PhysicalClothConstraintCoeff* coeffs)=0;

	virtual uint queryShapePointers()=0;
	virtual void getShapePointers(IPhysicalShape** shapePointers,uint *flags=0) = 0;
	virtual void setShapePointers(IPhysicalShape** shapePointers,uint *flags=0) = 0;

	virtual bool setCollisionResponseCoefficient(float  v)=0;
	virtual float getCollisionResponseCoefficient()=0;

	virtual bool setAttachmentResponseCoefficient(float v)=0;
	virtual float getAttachmentResponseCoefficient()=0;

	virtual bool setExternalAcceleration(const math::vector3d& v)=0;
	virtual math::vector3d getExternalAcceleration( )=0;

	virtual bool setWindAcceleration(const math::vector3d& v)=0;
	virtual math::vector3d getWindAcceleration()=0;

	virtual bool isSleeping()=0;

	virtual bool setSleepLinearVelocity(float v)=0;
	virtual float  getSleepLinearVelocity()=0;

	virtual void wakeUp(float wakeCounterValue=PHYICS_SLEEP_INTERVAL)=0;
	virtual void putToSleep()=0;

	virtual	void addForceAtVertex(const math::vector3d& force, uint vertexId, 
		EForceMode mode = EFM_Force) = 0;
	virtual	void addForceAtPoint(const math::vector3d& pos, float magnitude,float radius, 
		EForceMode mode = EFM_Force) = 0;
	virtual	void addDirectedForceAtPoint(const math::vector3d& pos, const math::vector3d& force,float radius, 
		EForceMode mode = EFM_Force) = 0;

	virtual void setUserData(IObject*ud)=0;
	virtual IObject*getUserData()=0;

	virtual void update()=0;

};

}
}


#endif //___IPhysicalCloth___
