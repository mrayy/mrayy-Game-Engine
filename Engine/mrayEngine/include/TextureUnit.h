
/********************************************************************
	created:	2009/08/11
	created:	11:8:2009   9:16
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\TextureUnit.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	TextureUnit
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___TextureUnit___
#define ___TextureUnit___

//#include "ITexture.h"

#include "GCPtr.h"
#include "IResourceHolder.h"
#include "ValueController.h"
#include "ITextureSource.h"



namespace mray{
namespace video{


class MRAY_DLL TextureUnit:public IResourceHolder
{
public:


	enum ETextureEffectType
	{
		ETA_UVScroll,
		ETA_Rotate,
		ETA_EnvironmentMap,
		ETA_Projection,
		ETA_Transform
	};

	enum EEnvMapType
	{
		EMNT_Normal,
		ENMT_Planner,
		EMNT_Curved,
		EMNT_Reflection
	};
	enum ETextureTransformType
	{
		ETTT_TranslateU,
		ETTT_TranslateV,
		ETTT_ScaleU,
		ETTT_ScaleV,
		ETTT_Rotate
	};

	enum EWavType
	{
		EWT_Sin,
		EWT_Triangle,
		EWT_Square,
		EWT_SawTooth,
		EWT_InvSawTooth
	};

	struct TextureEffector
	{
	public:
		ETextureEffectType type;
		ValueController<float>* controller;

		float frequency;
		float phase;
		float amplitude;
	};

	typedef std::multimap<ETextureEffectType,TextureEffector*> EffectsMap;

private:
protected:
	math::matrix4x4 transform;
	ETextureGenMode m_texGenMode[ETGT_Count];
	bool m_enableGenMode[ETGT_Count];

	ETextureClamp m_textureWrap[ETW_Count];

	int m_textureCoordSet;

	ITextureSourcePtr m_textureSource;
	ITexturePtr m_texture;
	SColor m_EdgeColor;

	EffectsMap m_effects;
public:



	math::vector3d trans;
	math::vector3d rotate;
	math::vector3d scale;

	TextureUnit();
	TextureUnit(TextureUnit*o);
	virtual~TextureUnit();

	void SetTexture(const ITexturePtr&t);
	const ITexturePtr& GetTexture();

	void SetTextureSource(ITextureSourceCRef t);
	ITextureSourceCRef GetTextureSource(){return m_textureSource;}

	const SColor& GetEdgeColor()const{return m_EdgeColor;}
	void SetEdgeColor(const SColor& c){m_EdgeColor=c;}

	virtual void setTextureGenMode(ETextureGenType type,ETextureGenMode mode);
	virtual ETextureGenMode getTextureGenMode(ETextureGenType type);

	virtual void enableGenMode(ETextureGenType type,bool e);
	virtual bool isEnableGenMode(ETextureGenType type);


	virtual void setTextureClamp(ETextureWrap wrap,ETextureClamp clamp){
		m_textureWrap[wrap]=clamp;
	}
	virtual ETextureClamp getTextureClamp(ETextureWrap wrap){
		return m_textureWrap[wrap];
	}

	math::matrix4x4& getTransformationMatrix(){return transform;}
	void updateMatrix();

	xml::XMLElement* loadXMLSettings(xml::XMLElement*elem);
	xml::XMLElement* exportXMLSettings(xml::XMLElement*elem);
};

MakeSharedPtrType(TextureUnit)

}
}


#endif //___TextureUnit___
