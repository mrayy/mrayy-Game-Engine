

/********************************************************************
	created:	2012/06/26
	created:	26:6:2012   13:02
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\RenderTechnique.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine
	file base:	RenderTechnique
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __RenderTechnique__
#define __RenderTechnique__


#include "CompileConfig.h"
#include "videoCommon.h"
#include "GPUShader.h"
#include "TypedProperty.h"

namespace mray
{
namespace video
{

	class RenderMaterial;
	class RenderPass;
typedef std::vector<RenderPass*> RenderPassList;

class MRAY_DLL RenderTechnique:public IObject
{
	DECLARE_RTTI;
private:
protected:
	core::string m_name;
	RenderPassList m_passes;
	bool m_isSupported;

	core::string m_profiles;

	GPUShaderPtr m_depthShader;

	RenderMaterial* m_owner;
public:
	DECLARE_PROPERTY_TYPE(Name,core::string,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(RequiredProfiles,core::string,MRAY_DLL);
public:
	RenderTechnique(RenderMaterial*o);
	virtual~RenderTechnique();

	bool SetRequiredProfiles(const core::string& profiles);
	const core::string& GetRequiredProfiles();

	void SetOwnerMaterial(RenderMaterial*m){m_owner=m;}
	RenderMaterial* GetOwnerMaterial(){return m_owner;}

	bool SetName(const core::string& name);
	const core::string& GetName()const;

	RenderPass* CreatePass(const core::string&name);
	const RenderPassList& GetPasses()const;
	RenderPass* GetPassAt(int i);
	RenderPass* GetPassByName(const core::string& name);
	void AddPass(RenderPass*p);
	void RemovePass(RenderPass*p);
	void RemovePassAt(int index);
	void ClearPasses();

	void SetDepthShader(GPUShaderCRef shader){m_depthShader=shader;}
	GPUShaderCRef GetDepthShader(){return m_depthShader;}

	bool IsSupported(){return m_isSupported;}

	RenderTechnique*Duplicate();

	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};


}
}

#endif
