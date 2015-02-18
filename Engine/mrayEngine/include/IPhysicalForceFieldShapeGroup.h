


/********************************************************************
	created:	2012/02/15
	created:	15:2:2012   11:58
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IPhysicalForceFieldShapeGroup.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IPhysicalForceFieldShapeGroup
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IPhysicalForceFieldShapeGroup__
#define __IPhysicalForceFieldShapeGroup__


namespace mray
{
namespace physics
{
	class IPhysicalForceFieldShape;
	class IPhysicalForceFieldShapeDesc;
	class IPhysicalForceField;

class IPhysicalForceFieldShapeGroup
{
private:
protected:
public:
	IPhysicalForceFieldShapeGroup(){}
	virtual~IPhysicalForceFieldShapeGroup(){}

	virtual void setName(const core::string& name)=0;
	virtual const core::string& getName()const=0;

	virtual void setUserData(IObject* d)=0;
	virtual IObject* getUserData()const=0;

	virtual IPhysicalForceFieldShape* createShape(IPhysicalForceFieldShapeDesc* desc)=0;
	virtual void releaseShape(IPhysicalForceFieldShape*shape)=0;

	virtual IPhysicalForceField* getForceField()const=0;

	virtual const std::list<IPhysicalForceFieldShape*>& getShapes()const=0;
};

}
}

#endif
