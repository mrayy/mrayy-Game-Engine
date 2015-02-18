


#include "stdafx.h"
#include "PhysicalShapes.h"
#include "XMLElement.h"
#include "StringConverter.h"
#include "HeightMapRaw16.h"
#include "IFileSystem.h"
#include "MeshResourceManager.h"
#include "SMesh.h"
#include "IPhysicMaterial.h"



namespace mray{
namespace physics{

#define PARSE_ATTRIBUTE(val,attrName) \
	{if((attr=e->getAttribute(mT(attrName)))!=0)\
		core::StringConverter::parse(attr->value,val);}
#define WRITE_ATTRIBUTE(val,attrName)\
	{e->addAttribute(mT(attrName),core::StringConverter::toString(val));}


IPhysicalShapeDesc::IPhysicalShapeDesc(){
	autoDelete=false;
	materialDesc=0;
	material=0;
	setDefault();
}
IPhysicalShapeDesc::~IPhysicalShapeDesc()
{
	if(materialDesc && materialDesc->autoDel)
		delete materialDesc;
}

void IPhysicalShapeDesc::setDefault(){
	density=1;
	localSpace.makeIdentity();
	mass=1;
	collisionGroup=0;
	//materialIndex=0;
	material=0;
	userData=0;
	if(materialDesc && materialDesc->autoDel)
		delete materialDesc;
	materialDesc=0;


	flags=ESF_ClothTwoWay | ESF_SoftbodyTwoWay;
}
void IPhysicalShapeDesc::SaveToXML(mray::xml::XMLElement *e)
{
	WRITE_ATTRIBUTE(density,"Density");
	WRITE_ATTRIBUTE(mass,"Mass");
	WRITE_ATTRIBUTE(collisionGroup,"CollisionGroup");
	WRITE_ATTRIBUTE(flags,"Flags");
	WRITE_ATTRIBUTE(name,"Name");
	if(materialName!=mT(""))
	{
		WRITE_ATTRIBUTE(materialName,"MaterialName");
	}
	else if(material)
	{
		PhysicMaterialDesc desc;
		material->SaveToDesc(&desc);
		xml::XMLElement* elem=new xml::XMLElement(mT("Material"));
		e->addSubElement(elem);
		desc.SaveToXML(elem);
	}else  if(materialDesc)
	{
		xml::XMLElement* elem=new xml::XMLElement(mT("Material"));
		e->addSubElement(elem);
		materialDesc->SaveToXML(elem);
	}

	math::quaternion q(localSpace);
	math::vector3d rot;
	q.toEulerAngles(rot);
	math::vector3d pos=localSpace.getTranslation();

	WRITE_ATTRIBUTE(rot,"Rotation");
	WRITE_ATTRIBUTE(pos,"Position");
}
void IPhysicalShapeDesc::LoadFromXML(xml::XMLElement* e)
{
	math::vector3d rot;
	math::quaternion q;
	math::vector3d pos;
	xml::XMLAttribute* attr;
	PARSE_ATTRIBUTE(density,"Density");
	PARSE_ATTRIBUTE(name,"Name");
	PARSE_ATTRIBUTE(materialName,"MaterialName");
	PARSE_ATTRIBUTE(mass,"Mass");
	PARSE_ATTRIBUTE(collisionGroup,"CollisionGroup");
	PARSE_ATTRIBUTE(flags,"Flags");
	PARSE_ATTRIBUTE(rot,"Rotation");
	PARSE_ATTRIBUTE(pos,"Position");
	q.fromEulerAngles(rot.x,rot.y,rot.z);
	q.toMatrix(localSpace);
	localSpace.setTranslation(pos);

	xml::XMLElement* elem=e->getSubElement(mT("Material"));
	if(elem)
	{
		materialDesc=new PhysicMaterialDesc();
		materialDesc->autoDel=true;
		materialDesc->LoadFromXML(elem);
	}

}
//////////////////////////////////////////////////////////////////////////

SphereShapeDesc::SphereShapeDesc(){
	setDefault();
}
SphereShapeDesc::SphereShapeDesc(float radius){
	setDefault();
	this->radius=radius;
}
void SphereShapeDesc::setDefault(){
	IPhysicalShapeDesc::setDefault();
	radius=1;
}
ESupportedShapes SphereShapeDesc::getType(){
	return ESS_Sphere;
}
void SphereShapeDesc::SaveToXML(mray::xml::XMLElement *e)
{
	IPhysicalShapeDesc::SaveToXML(e);
	WRITE_ATTRIBUTE("Sphere","Type");
	WRITE_ATTRIBUTE(radius,"Radius");

}
void SphereShapeDesc::LoadFromXML(xml::XMLElement* e)
{
	xml::XMLAttribute* attr;
	IPhysicalShapeDesc::LoadFromXML(e);
	PARSE_ATTRIBUTE(radius,"Radius");
}
//////////////////////////////////////////////////////////////////////////

BoxShapeDesc::BoxShapeDesc(){
	setDefault();
}
BoxShapeDesc::BoxShapeDesc(const math::vector3d& extent){
	setDefault();
	this->extent=extent;
}
void BoxShapeDesc::setDefault(){
	IPhysicalShapeDesc::setDefault();
	extent=1;
}
ESupportedShapes BoxShapeDesc::getType(){
	return ESS_Box;
}
void BoxShapeDesc::SaveToXML(mray::xml::XMLElement *e)
{
	IPhysicalShapeDesc::SaveToXML(e);
	WRITE_ATTRIBUTE("Box","Type");
	WRITE_ATTRIBUTE(extent,"Extent");
}
void BoxShapeDesc::LoadFromXML(xml::XMLElement* e)
{
	xml::XMLAttribute* attr;
	IPhysicalShapeDesc::LoadFromXML(e);
	PARSE_ATTRIBUTE(extent,"Extent");
}

//////////////////////////////////////////////////////////////////////////


PlaneShapeDesc::PlaneShapeDesc(){
	setDefault();
}
PlaneShapeDesc::PlaneShapeDesc(const math::Plane& p){
	setDefault();
	this->plane=p;
}
void PlaneShapeDesc::setDefault(){
	IPhysicalShapeDesc::setDefault();
	plane.Normal.set(0,1,0);
	plane.D=0;
}
ESupportedShapes PlaneShapeDesc::getType(){
	return ESS_Plane;
}
void PlaneShapeDesc::SaveToXML(mray::xml::XMLElement *e)
{
	IPhysicalShapeDesc::SaveToXML(e);
	WRITE_ATTRIBUTE("Plane","Type");
	WRITE_ATTRIBUTE(plane,"Plane");
}
void PlaneShapeDesc::LoadFromXML(xml::XMLElement* e)
{
	xml::XMLAttribute* attr;
	IPhysicalShapeDesc::LoadFromXML(e);
	PARSE_ATTRIBUTE(plane,"Plane");
}
//////////////////////////////////////////////////////////////////////////


TerrainShapeDesc::TerrainShapeDesc(){
	m_heightMapData=0;
	setDefault();
}
TerrainShapeDesc::TerrainShapeDesc(int cols,int rows,double*data,math::vector3d scale,int skip){
	m_heightMapData=0;
	setDefault();
	this->cols=cols;
	this->rows=rows;
	this->data=data;
	this->scale=scale;
	this->skip=skip;
}
TerrainShapeDesc::~TerrainShapeDesc()
{
	if(m_heightMapData)
		delete m_heightMapData;
	m_heightMapData=0;
}
void TerrainShapeDesc::setDefault(){
	IPhysicalShapeDesc::setDefault();
	this->cols=0;
	this->rows=0;
	this->data=0;
	if(m_heightMapData)
		delete m_heightMapData;
	m_heightMapData=0;
	skip=0;
	scale=1;
}
ESupportedShapes TerrainShapeDesc::getType(){
	return ESS_Terrain;
}
void TerrainShapeDesc::SaveToXML(mray::xml::XMLElement *e)
{
	IPhysicalShapeDesc::SaveToXML(e);
	WRITE_ATTRIBUTE("Plane","Type");
	WRITE_ATTRIBUTE(heightMapPath,"Path");
	WRITE_ATTRIBUTE(scale,"Scale");
	WRITE_ATTRIBUTE(skip,"Skip");
}
void TerrainShapeDesc::LoadFromXML(xml::XMLElement* e)
{
	xml::XMLAttribute* attr;
	IPhysicalShapeDesc::LoadFromXML(e);
	core::string path=mT("");
	PARSE_ATTRIBUTE(path,"Path");
	PARSE_ATTRIBUTE(scale,"Scale");
	PARSE_ATTRIBUTE(skip,"Skip");

	if(path!=mT(""))
	{
		heightMapPath=attr->value;
		m_heightMapData=new loaders::HeightMapRaw16();
		((loaders::HeightMapRaw16*)m_heightMapData)->loadHeightMap(gFileSystem.openFile(heightMapPath));
		cols=m_heightMapData->getWidth();
		rows=m_heightMapData->getHeight();
		data=m_heightMapData->getHeightData();
	}

}
//////////////////////////////////////////////////////////////////////////


CapsuleShapeDesc::CapsuleShapeDesc(){
	setDefault();
}
CapsuleShapeDesc::CapsuleShapeDesc(float radius,float height){
	setDefault();
	this->radius=radius;
	this->height=height;
}
void CapsuleShapeDesc::setDefault(){
	IPhysicalShapeDesc::setDefault();
	radius=1;
	height=1;
}
ESupportedShapes CapsuleShapeDesc::getType(){
	return ESS_Capsule;
}
void CapsuleShapeDesc::SaveToXML(mray::xml::XMLElement *e)
{
	IPhysicalShapeDesc::SaveToXML(e);
	WRITE_ATTRIBUTE("Capsule","Type");
	WRITE_ATTRIBUTE(radius,"Radius");
	WRITE_ATTRIBUTE(height,"Height");
}
void CapsuleShapeDesc::LoadFromXML(xml::XMLElement* e)
{
	xml::XMLAttribute* attr;
	IPhysicalShapeDesc::LoadFromXML(e);
	PARSE_ATTRIBUTE(radius,"Radius");
	PARSE_ATTRIBUTE(height,"Height");

}
//////////////////////////////////////////////////////////////////////////

ConvexShapeDesc::ConvexShapeDesc(){
	setDefault();
}
ConvexShapeDesc::ConvexShapeDesc(math::vector3d*points,int count){
	setDefault();
	this->points.resize(count);
	for(int i=0;i<count;++i){
		this->points[i]=points[i];
	}
}
void ConvexShapeDesc::setDefault(){
	IPhysicalShapeDesc::setDefault();
	points.clear();
	mesh=0;
	flipNormals=0;
	computeConvex=1;
	inflanteConvex=0;
}
ESupportedShapes ConvexShapeDesc::getType(){
	return ESS_Convex;
}
void ConvexShapeDesc::SaveToXML(mray::xml::XMLElement *e)
{
	IPhysicalShapeDesc::SaveToXML(e);
	WRITE_ATTRIBUTE("Convex","Type");
	WRITE_ATTRIBUTE(meshPath,"Path");
	WRITE_ATTRIBUTE(flipNormals,"FlipNormals");
	WRITE_ATTRIBUTE(computeConvex,"ComputeConvex");
	WRITE_ATTRIBUTE(inflanteConvex,"InflanteConvex");
}
void ConvexShapeDesc::LoadFromXML(xml::XMLElement* e)
{
	xml::XMLAttribute* attr;
	IPhysicalShapeDesc::LoadFromXML(e);
	PARSE_ATTRIBUTE(meshPath,"Path");
	PARSE_ATTRIBUTE(flipNormals,"FlipNormals");
	PARSE_ATTRIBUTE(computeConvex,"ComputeConvex");
	PARSE_ATTRIBUTE(inflanteConvex,"InflanteConvex");
	if(meshPath!=mT(""))
	{
		GCPtr<scene::SMesh> m=gMeshResourceManager.loadMesh(meshPath,true);
		if(m)
		{
			mesh=m->getBuffer(0);
		}
	}

}

TriangleShapeDesc::TriangleShapeDesc(){
	setDefault();
}
TriangleShapeDesc::TriangleShapeDesc(scene::IMeshBuffer* mesh){
	setDefault();
	this->mesh=mesh;
}
void TriangleShapeDesc::setDefault(){
	IPhysicalShapeDesc::setDefault();
	mesh=0;
	flipNormals=0;
	hardwareMesh=0;
}
ESupportedShapes TriangleShapeDesc::getType(){
	return ESS_TriangleMesh;
}
void TriangleShapeDesc::SaveToXML(mray::xml::XMLElement *e)
{
	IPhysicalShapeDesc::SaveToXML(e);
	WRITE_ATTRIBUTE("Triangle","Type");
	WRITE_ATTRIBUTE(meshPath,"Path");
	WRITE_ATTRIBUTE(flipNormals,"FlipNormals");
	WRITE_ATTRIBUTE(hardwareMesh,"HardwareMesh");
}
void TriangleShapeDesc::LoadFromXML(xml::XMLElement* e)
{
	xml::XMLAttribute* attr;
	IPhysicalShapeDesc::LoadFromXML(e);
	PARSE_ATTRIBUTE(meshPath,"Path");
	PARSE_ATTRIBUTE(flipNormals,"FlipNormals");
	PARSE_ATTRIBUTE(hardwareMesh,"HardwareMesh");
	if(meshPath!=mT(""))
	{
		GCPtr<scene::SMesh> m=gMeshResourceManager.loadMesh(meshPath,true);
		if(m)
		{
			mesh=m->getBuffer(0);
		}
	}
}



WheelShapeDesc::WheelShapeDesc(){
	setDefault();
}
void WheelShapeDesc::setDefault(){
	IPhysicalShapeDesc::setDefault();
	radius = 1.0f;				
	suspensionTravel = 1.0f;	
	//simulation constants:
	inverseWheelMass = 1.0f;
	//dynamic inputs:
	motorTorque = 0.0f;
	brakeTorque = 0.0f;
	steerAngle = 0.0f;
	suspension.setDefault();
}
ESupportedShapes WheelShapeDesc::getType(){
	return ESS_Wheel;
}

void WheelShapeDesc::SaveToXML(mray::xml::XMLElement *e)
{
	IPhysicalShapeDesc::SaveToXML(e);
	e->addAttribute(mT("Type"),mT("Wheel"));
}
void WheelShapeDesc::LoadFromXML(xml::XMLElement* e)
{
	IPhysicalShapeDesc::LoadFromXML(e);
}

}
}

