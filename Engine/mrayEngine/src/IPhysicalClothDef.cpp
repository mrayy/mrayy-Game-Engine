


#include "stdafx.h"
#include "IPhysicalClothDef.h"
#include "xmlElement.h"
#include "StringConverter.h"
#include "MeshResourceManager.h"
#include "SMesh.h"


namespace mray
{
namespace physics
{

	const core::string clothFlags_str[]=
	{

		mT("Pressure"),
		mT("Static"),
		mT("DisableCollision"),
		mT("SelfCollision"),
		mT("Gravity"),
		mT("Bending"),
		mT("BendingOrtho"),
		mT("Damping"),
		mT("CollisionTwoWay"),
		mT("TriangleCollision"),
		mT("Terrable"),
		mT("Hardware"),
		mT("COMDamping"),
		mT("ValidBounds"),
		mT("FluidCollision"),
		mT("DisableDynamicCCD"),
		mT("Adhere")
	};

#define PARSE_ATTRIBUTE(val,attrName) \
	{if((attr=e->getAttribute(mT(attrName)))!=0)\
	core::StringConverter::parse(attr->value,val);}
#define PARSE_ATTRIBUTE_str(val,attrName) \
	{if((attr=e->getAttribute(attrName))!=0)\
	core::StringConverter::parse(attr->value,val);}
#define WRITE_ATTRIBUTE(val,attrName)\
	{e->addAttribute(mT(attrName),core::StringConverter::toString(val));}
#define WRITE_ATTRIBUTE_str(val,attrName)\
	{e->addAttribute(attrName,core::StringConverter::toString(val));}

PhysicalClothDesc::PhysicalClothDesc()
{
	autoDelete=false;
	setToDefault();
}

void PhysicalClothDesc::setToDefault()
{
	for (int i=0;i<ECF_ClothFlags_Count;++i)
	{
		flags[i]=false;
	}
	meshData=0;
	thickness = 0.01f;
	density = 1.0f;
	bendingStiffness = 1.0f;
	stretchingStiffness = 1.0f;
	dampingCoefficient = 0.5f;
	friction = 0.5f;
	pressure = 1.0f;
	tearFactor = 1.5f;
	attachmentTearFactor = 1.5f;
	attachmentResponseCoefficient = 0.2f;
	collisionResponseCoefficient = 0.2f;
	toFluidResponseCoefficient = 1.0f;
	fromFluidResponseCoefficient = 1.0f;
	minAdhereVelocity = 1.0f;
	flags[ECF_Gravity]=true;;
	solverIterations = 5;
	wakeUpCounter = 1.0f/30.0f;
	sleepLinearVelocity = -1.0f;
	externalAcceleration.set(0.0f, 0.0f, 0.0f);
	windAcceleration.set(0.0f, 0.0f, 0.0f);
	relativeGridSpacing = 0.25f;
	userData = NULL;
	collisionGroup=0;
}
void PhysicalClothDesc::SaveToXML(mray::xml::XMLElement *e)
{
	WRITE_ATTRIBUTE(collisionGroup,"CollisionGroup");
	WRITE_ATTRIBUTE(thickness,"Thickness");
	WRITE_ATTRIBUTE(density,"Density");
	WRITE_ATTRIBUTE(bendingStiffness,"BendingStiffness");
	WRITE_ATTRIBUTE(stretchingStiffness,"StretchingStiffness");
	WRITE_ATTRIBUTE(dampingCoefficient,"DampingCoefficient");
	WRITE_ATTRIBUTE(friction,"Friction");
	WRITE_ATTRIBUTE(pressure,"Pressure");
	WRITE_ATTRIBUTE(tearFactor,"TearFactor");
	WRITE_ATTRIBUTE(collisionResponseCoefficient,"CollisionResponseCoefficient");
	WRITE_ATTRIBUTE(attachmentResponseCoefficient,"AttachmentResponseCoefficient");
	WRITE_ATTRIBUTE(attachmentTearFactor,"AttachmentTearFactor");
	WRITE_ATTRIBUTE(toFluidResponseCoefficient,"ToFluidResponseCoefficient");
	WRITE_ATTRIBUTE(fromFluidResponseCoefficient,"FromFluidResponseCoefficient");
	WRITE_ATTRIBUTE(minAdhereVelocity,"MinAdhereVelocity");
	WRITE_ATTRIBUTE(solverIterations,"SolverIterations");
	WRITE_ATTRIBUTE(externalAcceleration,"ExternalAcceleration");
	WRITE_ATTRIBUTE(windAcceleration,"WindAcceleration");
	WRITE_ATTRIBUTE(wakeUpCounter,"WakeUpCounter");
	WRITE_ATTRIBUTE(sleepLinearVelocity,"SleepLinearVelocity");
	WRITE_ATTRIBUTE(validBounds,"ValidBounds");
	WRITE_ATTRIBUTE(relativeGridSpacing,"RelativeGridSpacing");
	WRITE_ATTRIBUTE(sleepLinearVelocity,"SleepLinearVelocity");

	for(int i=0;i<ECF_ClothFlags_Count;++i)
		WRITE_ATTRIBUTE_str(flags[i],clothFlags_str[i]);


	math::quaternion q(globalPos);
	math::vector3d pos;
	math::vector3d angles;
	pos=globalPos.getTranslation();
	q.toEulerAngles(angles);


	WRITE_ATTRIBUTE(pos,"Position");
	WRITE_ATTRIBUTE(angles,"Rotation");
}
void PhysicalClothDesc::LoadFromXML(xml::XMLElement* e)
{

	xml::XMLAttribute*attr;
	PARSE_ATTRIBUTE(collisionGroup,"CollisionGroup");
	PARSE_ATTRIBUTE(thickness,"Thickness");
	PARSE_ATTRIBUTE(density,"Density");
	PARSE_ATTRIBUTE(bendingStiffness,"BendingStiffness");
	PARSE_ATTRIBUTE(stretchingStiffness,"StretchingStiffness");
	PARSE_ATTRIBUTE(dampingCoefficient,"DampingCoefficient");
	PARSE_ATTRIBUTE(friction,"Friction");
	PARSE_ATTRIBUTE(pressure,"PressureValue");
	PARSE_ATTRIBUTE(tearFactor,"TearFactor");
	PARSE_ATTRIBUTE(collisionResponseCoefficient,"CollisionResponseCoefficient");
	PARSE_ATTRIBUTE(attachmentResponseCoefficient,"AttachmentResponseCoefficient");
	PARSE_ATTRIBUTE(attachmentTearFactor,"AttachmentTearFactor");
	PARSE_ATTRIBUTE(toFluidResponseCoefficient,"ToFluidResponseCoefficient");
	PARSE_ATTRIBUTE(fromFluidResponseCoefficient,"FromFluidResponseCoefficient");
	PARSE_ATTRIBUTE(minAdhereVelocity,"MinAdhereVelocity");
	PARSE_ATTRIBUTE(solverIterations,"SolverIterations");
	PARSE_ATTRIBUTE(externalAcceleration,"ExternalAcceleration");
	PARSE_ATTRIBUTE(windAcceleration,"WindAcceleration");
	PARSE_ATTRIBUTE(wakeUpCounter,"WakeUpCounter");
	PARSE_ATTRIBUTE(sleepLinearVelocity,"SleepLinearVelocity");
	PARSE_ATTRIBUTE(validBounds,"ValidBounds");
	PARSE_ATTRIBUTE(relativeGridSpacing,"RelativeGridSpacing");
	PARSE_ATTRIBUTE(sleepLinearVelocity,"SleepLinearVelocity");

	for(int i=0;i<ECF_ClothFlags_Count;++i)
		PARSE_ATTRIBUTE_str(flags[i],clothFlags_str[i]);

	math::vector3d pos;
	math::vector3d angles;
	PARSE_ATTRIBUTE(pos,"Position");
	PARSE_ATTRIBUTE(angles,"Rotation");

	math::quaternion(angles).toMatrix(globalPos);
	globalPos.setTranslation(pos);

	if(meshPath!=mT(""))
	{
		GCPtr<scene::SMesh> m=gMeshResourceManager.loadMesh(meshPath,true);
		if(m)
		{
			meshData=m->getBuffer(0);
		}
	}
}

}
}

