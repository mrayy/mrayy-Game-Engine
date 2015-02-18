

#include "StdAfx.h"
#include "IPrimitiveModelComponent.h"
#include "MaterialResourceManager.h"
#include "MeshBufferData.h"
#include "TangentCalculater.h"

namespace mray
{
namespace game
{

	IMPLEMENT_PROPERTY_TYPE_HEADER(Material,IPrimitiveModelComponent,core::string,mT("Material"),EPBT_Basic,mT("Material"),mT(""));
	IMPLEMENT_PROPERTY_TYPE_GENERIC(Material,IPrimitiveModelComponent,core::string,SetMaterialName,GetMaterialName,,,false);

	IMPLEMENT_PROPERTY_TYPE_HEADER(GenerateTangents,IPrimitiveModelComponent,bool,mT("GenerateTangents"),EPBT_Basic,mT("Generate Tangents"),false);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(GenerateTangents,IPrimitiveModelComponent,bool,SetGenerateTangents,GetGenerateTangents,core::StringConverter::toString,core::StringConverter::toBool,false);

IPrimitiveModelComponent::IPrimitiveModelComponent()
{
	m_generateTangents=false;

	CPropertieDictionary* dic;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeMaterial::instance);
		dic->addPropertie(&PropertyTypeGenerateTangents::instance);
	}
}

IPrimitiveModelComponent::~IPrimitiveModelComponent()
{

}

void IPrimitiveModelComponent::_postInit()
{
	video::RenderMaterialPtr mat=gMaterialResourceManager.getMaterial(m_materialName);
	scene::SMesh* mesh= m_model->getMesh();
	int cnt=mesh->getBuffersCount();
	scene::TangentCalculater calc(false);
	for(int i=0;i<cnt;++i)
	{
		scene::MeshBufferData*data= mesh->getBufferData(i);
		data->setMaterial(mat);
		if(m_generateTangents)
		{
			calc.calculateTangents(data->getMeshBuffer());
		}
	}
}


bool IPrimitiveModelComponent::InitComponent()
{
	if(!IModelComponent::InitComponent())
		return false;
	m_model=new scene::MeshRenderableNode(new scene::SMesh());
	return true;
}



IObject* IPrimitiveModelComponent::GetAttachedObject()
{
	return m_model;
}

}
}
