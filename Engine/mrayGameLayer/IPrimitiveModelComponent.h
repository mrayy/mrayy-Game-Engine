

/********************************************************************
	created:	2012/09/19
	created:	19:9:2012   12:39
	filename: 	d:\Development\mrayEngine\Engine\mrayGameLayer\IPrimitiveModelComponent.h
	file path:	d:\Development\mrayEngine\Engine\mrayGameLayer
	file base:	PrimitiveModelComponent
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___IPrimitiveModelComponent___
#define ___IPrimitiveModelComponent___

#include "IModelComponent.h"

namespace mray
{
namespace game
{

class IPrimitiveModelComponent:public IModelComponent
{
	DECLARE_RTTI;
protected:
	bool m_generateTangents;
	core::string m_materialName;
	GCPtr<scene::MeshRenderableNode> m_model;

	void _postInit();
public:
	DECLARE_PROPERTY_TYPE(Material,core::string,);
	DECLARE_PROPERTY_TYPE(GenerateTangents,bool,);
public:
	IPrimitiveModelComponent();
	virtual~IPrimitiveModelComponent();

	bool SetMaterialName(const core::string&name){m_materialName=name;return true;}
	const core::string& GetMaterialName()const{return m_materialName;}

	bool SetGenerateTangents(bool v){m_generateTangents=v;return true;}
	bool GetGenerateTangents()const{return m_generateTangents;}

	virtual bool InitComponent();

	const GCPtr<scene::MeshRenderableNode>& GetModel(){return m_model;}

	virtual IObject* GetAttachedObject();

};

}
}

#endif

