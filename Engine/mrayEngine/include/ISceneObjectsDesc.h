



/********************************************************************
	created:	2012/02/27
	created:	27:2:2012   14:49
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\ISceneObjectsDesc.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	ISceneObjectsDesc
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __ISceneObjectsDesc__
#define __ISceneObjectsDesc__

#include "ICullable.h"

namespace mray
{
namespace scene
{

class MRAY_DLL ISceneNodeDesc
{
private:
protected:
public:
	ISceneNodeDesc()
	{
		SetDefault();
	}
	virtual~ISceneNodeDesc(){}

	virtual void SetDefault()
	{
		position=0;
		orintation=math::quaternion::Identity;
		scale=1;

		visible=true;
		cullType=SCT_BBOX;
		viewDist=-1;
	}

	core::string name;
	math::vector3d position;
	math::quaternion orintation;
	math::vector3d scale;

	bool visible;

	ENodeCullingType cullType;
	float viewDist;

	virtual void loadFromXML(xml::XMLElement* e);
	virtual void saveToXML(xml::XMLElement* e);
};

class MRAY_DLL CameraNodeDesc:public ISceneNodeDesc
{
public:
	CameraNodeDesc()
	{
		SetDefault();
	}
	virtual void SetDefault()
	{
		ISceneNodeDesc::SetDefault();
		fovY=math::PI32/2.5f;
		zNear=1;
		zFar=3000;
		aspect=1;
		autoUpdateAspect=true;
		isPerspective=true;
		target=0;
	}
	float fovY;
	float zNear;
	float zFar;
	float aspect;
	bool autoUpdateAspect;
	bool isPerspective;
	math::vector3d target;

	virtual void loadFromXML(xml::XMLElement* e);
	virtual void saveToXML(xml::XMLElement* e);
};

class MRAY_DLL LightNodeDesc:public ISceneNodeDesc
{
public:
	LightNodeDesc()
	{
		SetDefault();
	}
	virtual void SetDefault()
	{
		radius=100;
		ambient.Set(1,1,1,0);
		diffuse.Set(1,1,1,0);
		specular.Set(0,0,0,0);
		castShadows=false;
		enabled=true;
		target=0;
	}
	float radius;
	video::SColor ambient;
	video::SColor diffuse;
	video::SColor specular;
	bool castShadows;
	bool enabled;
	math::vector3d target;


	virtual void loadFromXML(xml::XMLElement* e);
	virtual void saveToXML(xml::XMLElement* e);
};
class MRAY_DLL IRenderableNodeDesc
{
public:

	IRenderableNodeDesc()
	{
		SetDefault();
	}
	virtual~IRenderableNodeDesc(){}

	void SetDefault()
	{
		shadowCaster=false;
		shadowReceiver=false;
		targetRenderGroup=0;
		customRenderGroup=false;
		useIdentityMatrix=false;
	}
	bool shadowCaster;
	bool shadowReceiver;
	int  targetRenderGroup;
	bool customRenderGroup;
	bool useIdentityMatrix;

	virtual void loadFromXML(xml::XMLElement* e);
	virtual void saveToXML(xml::XMLElement* e);
};


}
}

#endif
