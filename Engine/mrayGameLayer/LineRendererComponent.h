/********************************************************************
	created:	2013/07/19
	created:	19:7:2013   0:43
	filename: 	C:\Development\mrayEngine\Engine\mrayGameLayer\LineRendererComponent.h
	file path:	C:\Development\mrayEngine\Engine\mrayGameLayer
	file base:	LineRendererComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __LineRendererComponent__
#define __LineRendererComponent__


#include "IGameComponent.h"
#include "SBillboardChain.h"

namespace mray
{
namespace game
{

	
class LineRendererComponent:public IGameComponent
{
	DECLARE_RTTI
protected:
	GCPtr<scene::SBillboardChain> m_renderNode;
	core::string m_materialName;
public:
	DECLARE_PROPERTY_TYPE(MaterialName,core::string,)
	DECLARE_PROPERTY_TYPE(UseWorldSpace,bool,)
	DECLARE_PROPERTY_TYPE(Width,float,)
	DECLARE_PROPERTY_TYPE(Start,math::vector3d,)
	DECLARE_PROPERTY_TYPE(End,math::vector3d,)
public:
	LineRendererComponent(GameEntityManager*);
	virtual~LineRendererComponent();


	bool SetWidth(float width);
	float GetWidth();

	bool SetUseWorldSpace(bool u);
	bool GetUseWorldSpace();

	bool SetMaterialName(const core::string&n);
	const core::string& GetMaterialName(){return m_materialName;}
		
	bool SetStartPoint(const math::vector3d& start);
	const math::vector3d& GetStartPoint();

	bool SetEndPoint(const math::vector3d& end);
	const math::vector3d& GetEndPoint();

	virtual bool InitComponent();
};

DECLARE_GAMECOMPONENT_FACTORY(LineRendererComponent)

}
}


#endif