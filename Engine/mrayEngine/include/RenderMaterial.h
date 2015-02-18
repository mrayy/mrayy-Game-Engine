
#ifndef ___RenderMaterial___
#define ___RenderMaterial___

#include "IResourceHolder.h"
#include "SColor.h"
#include "TextureUnit.h"

#include "CompileConfig.h"
#include "ISerializable.h"
#include "GCPtr.h"
#include "IValue.h"
#include "GPUShader.h"

#include "TypedProperty.h"

namespace mray{
namespace video{

	class RenderTechnique;
	typedef std::vector<RenderTechnique*> RenderTechniqueList;


class MRAY_DLL RenderMaterial:public IResource
{
	DECLARE_RTTI;
protected:
public:
	
	DECLARE_PROPERTY_TYPE(Name,core::string,MRAY_DLL);

protected:
	RenderTechniqueList m_techniques;
	RenderTechnique* m_activeTechnique;

	uint calcSizeInternal();

public:

	RenderMaterial();
	virtual~RenderMaterial();

	bool SetName(const core::string&name);
	core::string GetName()const;

	bool SetActiveTechniqueByIndex(int idx);
	bool SetActiveTechniqueByName(const core::string& name);
	RenderTechnique* GetActiveTechnique();

	RenderTechnique* CreateTechnique(const core::string& name);
	const RenderTechniqueList& GetTechniques()const;
	RenderTechnique* GetTechniqueAt(int i);
	RenderTechnique* GetTechniqueByName(const core::string& name);
	void AddTechnique(RenderTechnique*p);
	void RemoveTechnique(RenderTechnique*p);
	void RemoveTechniqueAt(int index);
	void ClearTechniques();

	RenderMaterial* Duplicate();
	
	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};

MakeSharedPtrType(RenderMaterial);

};//video
};//mray


#endif







