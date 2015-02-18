


/********************************************************************
	created:	2012/06/26
	created:	26:6:2012   13:02
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\RenderPass.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine
	file base:	RenderPass
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __RenderPass__
#define __RenderPass__

#include "CompileConfig.h"
#include "videoCommon.h"
#include "GPUShader.h"
#include "IValue.h"
#include "TextureUnit.h"
#include "TypedProperty.h"
#include "IResourceHolder.h"

namespace mray
{
namespace video
{

	class RenderTechnique;

class MRAY_DLL RenderPass:public IObject,public IResourceHolder
{
	DECLARE_RTTI;
private:
protected:
	core::string m_name;

	ValuesMap m_values;

	SColor m_ambientColor;
	SColor m_diffuseColor;
	SColor m_emissiveColor;
	SColor m_specularColor;
	float  m_shininess;


	EBlendFactor m_SrcBlend;
	EBlendFactor m_DstBlend;

	EAlphaFunc m_AlphaFunction;
	float m_AlphaRef;
	float m_thickness;//for lines and points
	float m_Alpha;

	ECullFaceMode m_CullFaceDir;

	EState m_states[RS_RenderStates_Count];

	GPUShaderPtr m_shader;

	std::vector<TextureUnitPtr> m_textures;

	RenderTechnique* m_owner;
public:
	DECLARE_PROPERTY_TYPE(Name,core::string,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(Ambient,SColor,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(Diffuse,SColor,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(Specular,SColor,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(Emissive,SColor,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(Shininess,float,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(Alpha,float,MRAY_DLL);

	DECLARE_PROPERTY_TYPE(Thickness,float,MRAY_DLL);

	DECLARE_PROPERTY_TYPE(SourceFactor,EBlendFactor,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(DestinationFactor,EBlendFactor,MRAY_DLL);

	DECLARE_PROPERTY_TYPE(AlphaFunction,EAlphaFunc,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(AlphaRef,float,MRAY_DLL);

	DECLARE_PROPERTY_TYPE(CullFaceDir,ECullFaceMode,MRAY_DLL);

public:
	RenderPass(RenderTechnique*t);
	virtual~RenderPass();

	void SetOwnerTechnique(RenderTechnique*t){m_owner=t;}
	RenderTechnique* GetOwnerTechnique(){return m_owner;}

	bool SetName(const core::string&name);
	core::string GetName()const;


	bool SetAmbient(const SColor& clr);
	const SColor& GetAmbient()const;

	bool SetDiffuse(const SColor& clr);
	const SColor& GetDiffuse()const;

	bool SetSpecular(const SColor& clr);
	const SColor& GetSpecular()const;

	bool SetEmissive(const SColor& clr);
	const SColor& GetEmissive()const;

	bool SetShininess(float s);
	float GetShininess()const;

	bool SetAlpha(float s);
	float GetAlpha()const;

	bool SetThickness(float s);
	float GetThickness()const;


	bool SetSourceBlendFactor(EBlendFactor f);
	EBlendFactor GetSourceBlendFactor();

	bool SetDestinationBlendFactor(EBlendFactor f);
	EBlendFactor GetDestinationBlendFactor();

	bool SetAlphaFunction(EAlphaFunc f);
	EAlphaFunc GetAlphaFunction();

	bool SetAlphaReferenceValue(float r);
	float GetAlphaReferenceValue();

	bool SetCullFaceDirection(ECullFaceMode d);
	ECullFaceMode GetCullFaceDirection();


	bool setRenderShader(const GPUShaderPtr& shader);
	const GPUShaderPtr& getRenderShader();

	bool setMaterialRenderer(EMaterialRenderType type);

	bool setTexture(const ITexturePtr&tex,int stage);
	bool setTextureUnit(const TextureUnitPtr& unit,int stage);

	const ITexturePtr& getTexture(int stage);
	const TextureUnitPtr& getTextureUnit(int stage);

	int getTexturesCount();
	bool setTexturesCount(int cnt);

	bool setRenderState(RenderStates flag,EState set);
	EState getRenderState(RenderStates flag);

	void addValue(IValue*value);
	IValue* getValue(const core::string&name);
	void removeValue(const core::string&name);

	RenderPass* Duplicate();

	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);

};


}
}

#endif
