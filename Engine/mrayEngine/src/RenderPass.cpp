

#include "stdafx.h"
#include "RenderPass.h"
#include "EnumManager.h"
#include "ShaderResourceManager.h"


namespace mray
{
namespace video
{


	RenderPass::RenderPass(RenderTechnique*t)
	{
		m_owner=t;
		//matRenderType=MRT_SOLID;

		m_enabled = PropertyTypeEnabled::instance.GetDefaultValue();
		m_ambientColor = PropertyTypeAmbient::instance.GetDefaultValue();
		m_diffuseColor=PropertyTypeDiffuse::instance.GetDefaultValue();
		m_emissiveColor=PropertyTypeEmissive::instance.GetDefaultValue();
		m_specularColor=PropertyTypeSpecular::instance.GetDefaultValue();
		m_shininess=PropertyTypeShininess::instance.GetDefaultValue();
		m_thickness=PropertyTypeThickness::instance.GetDefaultValue();
		m_Alpha=PropertyTypeAlpha::instance.GetDefaultValue();

		m_CullFaceDir=PropertyTypeCullFaceDir::instance.GetDefaultValue();

		m_SrcBlend=PropertyTypeSourceFactor::instance.GetDefaultValue();
		m_DstBlend=PropertyTypeDestinationFactor::instance.GetDefaultValue();

		m_AlphaFunction=PropertyTypeAlphaFunction::instance.GetDefaultValue();
		m_AlphaRef=PropertyTypeAlphaRef::instance.GetDefaultValue();

		m_states[RS_Wireframe]	=ES_DontUse;
		m_states[RS_Smooth]		=ES_Use;
		m_states[RS_Lighting]	=ES_Use;
		m_states[RS_ZTest]		=ES_Use;
		m_states[RS_ZWrite]		=ES_Use;
		m_states[RS_AlphaTest]	=ES_Use;
		m_states[RS_Blend]		=ES_DontUse;
		m_states[RS_Points]		=ES_DontUse;
		m_states[RS_CullFace]	=ES_Use;
		m_states[RS_Fog]		=ES_Unknown;

		if(ShaderResourceManager::isExist())
			setRenderShader(gShaderResourceManager.getDefaultShader());



		CPropertieDictionary *dic=0;
		if(CreateDictionary(&dic))
		{
			PropertyTypeSourceFactor::instance.SetEnum(EnumManager::getInstance().getEnumInfo(mT("EBlendFactor")));
			PropertyTypeDestinationFactor::instance.SetEnum(EnumManager::getInstance().getEnumInfo(mT("EBlendFactor")));
			PropertyTypeAlphaFunction::instance.SetEnum(EnumManager::getInstance().getEnumInfo(mT("EAlphaFunc")));
			PropertyTypeCullFaceDir::instance.SetEnum(EnumManager::getInstance().getEnumInfo(mT("ECullFaceMode")));


			dic->addPropertie(&PropertyTypeEnabled::instance);
			dic->addPropertie(&PropertyTypeName::instance);
			dic->addPropertie(&PropertyTypeAmbient::instance);
			dic->addPropertie(&PropertyTypeDiffuse::instance);
			dic->addPropertie(&PropertyTypeSpecular::instance);
			dic->addPropertie(&PropertyTypeEmissive::instance);
			dic->addPropertie(&PropertyTypeShininess::instance);
			dic->addPropertie(&PropertyTypeAlpha::instance);
			dic->addPropertie(&PropertyTypeThickness::instance);
			dic->addPropertie(&PropertyTypeSourceFactor::instance);
			dic->addPropertie(&PropertyTypeDestinationFactor::instance);
			dic->addPropertie(&PropertyTypeAlphaFunction::instance);
			dic->addPropertie(&PropertyTypeAlphaRef::instance);
			dic->addPropertie(&PropertyTypeCullFaceDir::instance);
		}
	}
	bool RenderPass::setMaterialRenderer(EMaterialRenderType type){
		switch(type){
		case MRT_SOLID:
			m_SrcBlend = EBF_ZERO;
			m_DstBlend = EBF_ONE;
		setRenderState(RS_Blend,ES_DontUse);
		setRenderState(RS_AlphaTest, ES_Use);
		setRenderState(RS_ZWrite, ES_Use);
		break;
	case MRT_TRANSPARENT:

		m_SrcBlend=EBF_SRC_ALPHA;
		m_DstBlend=EBF_ONE_MINUS_SRC_ALPHA;
		setRenderState(RS_Blend,ES_Use);
	//	setRenderState(RS_Lighting	,ES_DontUse);
		setRenderState(RS_Fog		,ES_DontUse);
		setRenderState(RS_AlphaTest	,ES_DontUse);
		setRenderState(RS_ZWrite	,ES_DontUse);
		break;
	case MRT_ADD:
		m_SrcBlend=EBF_ONE;
		m_DstBlend=EBF_ONE;
		setRenderState(RS_Blend		,ES_Use);
		setRenderState(RS_Lighting	,ES_DontUse);
		setRenderState(RS_Fog		,ES_DontUse);
		setRenderState(RS_AlphaTest	,ES_DontUse);
		setRenderState(RS_ZWrite	,ES_DontUse);
		m_AlphaFunction=EAF_Greater;
		m_AlphaRef=0.01;
		break;
	case MRT_ADDALPHA:
		m_SrcBlend=EBF_SRC_ALPHA;
		m_DstBlend=EBF_ONE;
		setRenderState(RS_Blend		,ES_Use);
		setRenderState(RS_AlphaTest	,ES_DontUse);
		setRenderState(RS_Fog		,ES_DontUse);
		setRenderState(RS_Lighting	,ES_DontUse);
		setRenderState(RS_ZWrite	,ES_DontUse);
		m_AlphaFunction=EAF_Greater;
		m_AlphaRef=0.01;
		break;
	case MRT_MUL:
		setRenderState(RS_Blend,ES_Use);
		setRenderState(RS_AlphaTest,ES_DontUse);
		m_SrcBlend=EBF_DST_COLOR;
		m_DstBlend=EBF_ZERO;
		break;
		}
		return true;
	}
	RenderPass::~RenderPass(){
		for(int i=0;i<m_textures.size();++i)
			if(m_textures[i])
				m_textures[i]=0;
		ValuesMap::iterator it=m_values.begin();
		for (;it!=m_values.end();++it)
		{
			delete it->second;
			it->second=0;
		}
		m_values.clear();
	}


	bool RenderPass::SetName(const core::string&name)
	{
		m_name=(name);
		return true;
	}
	core::string RenderPass::GetName()const
	{
		return m_name;
	}


	bool RenderPass::SetAmbient(const SColor& clr)
	{
		m_ambientColor=clr;
		return true;
	}
	const SColor& RenderPass::GetAmbient()const
	{
		return m_ambientColor;
	}

	bool RenderPass::SetDiffuse(const SColor& clr)
	{
		m_diffuseColor.Set(clr.R,clr.G,clr.B,m_Alpha);
		return true;
	}
	const SColor& RenderPass::GetDiffuse()const
	{
		return m_diffuseColor;
	}

	bool RenderPass::SetSpecular(const SColor& clr)
	{
		m_specularColor=clr;
		return true;
	}
	const SColor& RenderPass::GetSpecular()const
	{
		return m_specularColor;
	}

	bool RenderPass::SetEmissive(const SColor& clr)
	{
		m_emissiveColor=clr;
		return true;
	}
	const SColor& RenderPass::GetEmissive()const
	{
		return m_emissiveColor;
	}

	bool RenderPass::SetShininess(float s)
	{
		m_shininess=s;
		return true;
	}
	float RenderPass::GetShininess()const
	{
		return m_shininess;
	}

	bool RenderPass::SetAlpha(float s)
	{
		m_Alpha=s;
		m_diffuseColor.A=s;
		return true;
	}
	float RenderPass::GetAlpha()const
	{
		return m_Alpha;
	}

	bool RenderPass::SetThickness(float s)
	{
		m_thickness=s;
		return true;
	}
	float RenderPass::GetThickness()const
	{
		return m_thickness;
	}


	bool RenderPass::SetSourceBlendFactor(EBlendFactor f)
	{
		m_SrcBlend=f;
		return true;
	}
	EBlendFactor RenderPass::GetSourceBlendFactor()
	{
		return m_SrcBlend;
	}

	bool RenderPass::SetDestinationBlendFactor(EBlendFactor f)
	{
		m_DstBlend=f;
		return true;
	}
	EBlendFactor RenderPass::GetDestinationBlendFactor()
	{
		return m_DstBlend;
	}

	bool RenderPass::SetAlphaFunction(EAlphaFunc f)
	{
		m_AlphaFunction=f;
		return true;
	}
	EAlphaFunc RenderPass::GetAlphaFunction()
	{
		return m_AlphaFunction;
	}

	bool RenderPass::SetAlphaReferenceValue(float r)
	{
		m_AlphaRef=r;
		return true;
	}
	float RenderPass::GetAlphaReferenceValue()
	{
		return m_AlphaRef;
	}

	bool RenderPass::SetCullFaceDirection(ECullFaceMode d)
	{
		m_CullFaceDir=d;
		return true;
	}
	ECullFaceMode RenderPass::GetCullFaceDirection()
	{
		return m_CullFaceDir;
	}


	bool RenderPass::setRenderShader(const GPUShaderPtr& shader)
	{
		if(!m_shader.isNull())
			_RemoveDependedResourceHolders(m_shader);
		m_shader=shader;
		if(!m_shader.isNull())
			_AddDependedResourceHolders(m_shader);
		return true;
	}

	const GPUShaderPtr& RenderPass::getRenderShader()
	{
		return m_shader;
	}

	bool RenderPass::setTexture(const ITexturePtr&tex,int stage)
	{
		for(int i=m_textures.size();i<=stage;++i)
		{
			m_textures.push_back(new TextureUnit());
		}/*
		 if(!m_textures[stage]){
		 m_textures[stage]=new TextureUnit();
		 }*/
		m_textures[stage]->SetTexture(tex);
		return true;
	}

	bool RenderPass::setTextureUnit(const TextureUnitPtr& unit,int stage)
	{
		for(int i=m_textures.size();i<stage;++i)
		{
			m_textures.push_back(new TextureUnit());
		}
		if(m_textures.size()==stage)
			m_textures.push_back(unit);
		else
			m_textures[stage]=unit;
		return true;
	}

	const ITexturePtr& RenderPass::getTexture(int stage){

		if(stage>=m_textures.size())
		{
			return ITexturePtr::Null;
		}
		if(!m_textures[stage])
			return ITexturePtr::Null;
		return m_textures[stage]->GetTexture();
	}
	const TextureUnitPtr& RenderPass::getTextureUnit(int stage){
		if(stage>=m_textures.size())
		{
			return TextureUnitPtr::Null;
		}
		return m_textures[stage];
	}

	int RenderPass::getTexturesCount(){
		return m_textures.size();
	}
	bool RenderPass::setTexturesCount(int cnt){
		m_textures.resize(cnt);
		return true;
	}


	bool RenderPass::setRenderState(RenderStates flag,EState set)
	{
		m_states[flag]=set;
		return true;
	}
	EState RenderPass::getRenderState(RenderStates flag)
	{
		return m_states[flag];
	}


	void RenderPass::addValue(IValue*value){
		m_values.insert(ValuesMap::value_type(value->getName(),value));
	}
	IValue* RenderPass::getValue(const core::string&name){
		ValuesMap::iterator it= m_values.find(name);
		if(it==m_values.end())
			return 0;
		return it->second;
	}
	void RenderPass::removeValue(const core::string&name){
		ValuesMap::iterator it= m_values.find(name);
		if(it==m_values.end())
			return;
		delete it->second;
		m_values.erase(it);
	}

	RenderPass* RenderPass::Duplicate(){
		RenderPass* m=new RenderPass(m_owner);
		m->SetName(m_name);

		m->SetAmbient(GetAmbient());
		m->SetDiffuse(GetDiffuse());
		m->SetEmissive(GetEmissive());
		m->SetSpecular(GetSpecular());
		m->SetShininess(GetShininess());
		m->SetAlpha(GetAlpha());
		m->SetThickness(GetThickness());
		m->SetSourceBlendFactor(GetSourceBlendFactor());
		m->SetDestinationBlendFactor(GetDestinationBlendFactor());

		m->SetAlphaFunction(GetAlphaFunction());
		m->SetAlphaReferenceValue(GetAlphaReferenceValue());
		m->SetCullFaceDirection(GetCullFaceDirection());

		for(int i=0;i<RS_RenderStates_Count;++i){
			m->m_states[i]=m_states[i];
		}
		ValuesMap::iterator it= m_values.begin();
		for (;it!=m_values.end();++it)
		{
			m->m_values[it->first]=it->second->duplicate();
		}
		m->setRenderShader(m_shader);

		m->m_textures.resize(m_textures.size());
		for(int i=0;i<m_textures.size();++i)
			m->m_textures[i]=new TextureUnit(m_textures[i]);

		return m;
	}

	xml::XMLElement* RenderPass::loadXMLSettings(xml::XMLElement* elem){
		//xml::XMLElement* elem=e->getSubElement(mT("Material"));
		//if(!elem)
		//	return 0;

		IObject::loadFromXML(elem);

		xml::XMLAttribute*attr;

		/*
		attr=elem->getAttribute(mT("Name"));
		if(attr)
		SetName(attr->value);

		attr=elem->getAttribute(mT("Ambient"));
		if(attr)
		SetAmbient(core::StringConverter::toColor(attr->value));
		attr=elem->getAttribute(mT("Diffuse"));
		if(attr)
		SetDiffuse(core::StringConverter::toColor(attr->value));
		attr=elem->getAttribute(mT("Emissive"));
		if(attr)
		SetEmissive(core::StringConverter::toColor(attr->value));
		attr=elem->getAttribute(mT("Specular"));
		if(attr)
		core::StringConverter::parse(attr->value,specularColor);
		attr=elem->getAttribute(mT("Shininess"));
		if(attr)
		core::StringConverter::parse(attr->value,shininess);
		attr=elem->getAttribute(mT("Alpha"));
		if(attr)
		Alpha=core::StringConverter::toInt(attr->value);

		*/

		xml::XMLElement*shaderElem=elem->getSubElement(mT("RenderShader"));
		if(shaderElem)
		{
			GPUShaderPtr shader=new GPUShader();
			shader->loadXMLSettings(shaderElem);
			setRenderShader(shader);
		}

		for (int i=0;i<RS_RenderStates_Count;++i)
		{
			attr=elem->getAttribute(EnumManager::getInstance().getName(mT("RenderStates"),i));
			if(attr)
			{
				m_states[i]=core::StringConverter::toBool(attr->value)? ES_Use:ES_DontUse ;
			}
		}

		xml::XMLElement* vals=elem->getSubElement(mT("Values"));
		if(vals)
		{
			xml::xmlSubElementsMapIT it= vals->getElementsBegin();
			xml::xmlSubElementsMapIT end= vals->getElementsEnd();
			for(;it!=end;++it)
			{
				if((*it)->GetType()!=xml::ENT_Element)continue;
				xml::XMLElement*e=dynamic_cast<xml::XMLElement*>(*it);
				EValueType type=EVT_UNKOWN;
				attr=e->getAttribute(mT("Type"));
				if(attr)
				{
					int v=EnumManager::getInstance().getValue(mT("EValueType"),attr->value);
					if(v==-1)
						continue;
					type=(EValueType)v;
				}
				attr=e->getAttribute(mT("Name"));
				if(attr)
				{
					IValue*v=IValue::CreateValueByType(attr->value,type);
					v->loadXMLSettings(e);
					m_values[attr->value]=v;
				}
			}
		}
		xml::XMLElement* units=elem->getSubElement(mT("TexUnits"));
		if(units)
		{
			xml::XMLElement* e= units->getSubElement(mT("Unit"));
			int unit=0;
			while(e)
			{
				int u;
				attr=e->getAttribute(mT("ID"));
				if(!attr)
					u=unit;
				else core::StringConverter::parse(attr->value,u);
				GCPtr<TextureUnit> texU=new TextureUnit();
				texU->loadXMLSettings(e);
				setTextureUnit(texU,u);
				unit=math::Max(unit,u)+1;
				e=e->nextSiblingElement(mT("Unit"));
			}
		}
		return elem;
	}

	xml::XMLElement*  RenderPass::exportXMLSettings(xml::XMLElement* elem){

		xml::XMLElement* node=0;

		node=new xml::XMLElement(mT("Pass"));
		IObject::exportToXML(node);

		for (int i=0;i<RS_RenderStates_Count;++i)
		{
			node->addAttribute(EnumManager::getInstance().getName(mT("RenderStates"),i),core::StringConverter::toString(m_states[i]==ES_Use));
		}

		if(m_shader)
		{
			xml::XMLElement* e=new xml::XMLElement(mT("RenderShader"));
			node->addSubElement(e);
			m_shader->exportXMLSettings(e);
		}


		if(m_textures.size())
		{
			xml::XMLElement* units=new xml::XMLElement(mT("TexUnits"));
			for(int i=0;i<m_textures.size();++i){
				if(m_textures[i]){
					xml::XMLElement* t=new xml::XMLElement(mT("Unit"));
					m_textures[i]->exportXMLSettings(t);
					t->addAttribute(mT("ID"),core::StringConverter::toString(i));
					units->addSubElement(t);
				}
			}
			node->addSubElement(units);
		}

		if(m_values.size())
		{
			xml::XMLElement* vals=new xml::XMLElement(mT("Values"));
			ValuesMap::iterator it= m_values.begin();
			for (;it!=m_values.end();++it)
			{
				it->second->exportXMLSettings(vals);
			}
			node->addSubElement(vals);
		}
		elem->addSubElement(node);

		return node;

	}

	//////////////////////////////////////////////////////////////////////////

	IMPLEMENT_PROPERTY_TYPE_HEADER(Name,RenderPass,string,mT("Name"),EPBT_Basic,mT("Pass Name"),mT(""));
	IMPLEMENT_PROPERTY_TYPE_GENERIC(Name,RenderPass,core::string,SetName,GetName,,,false)

		//////////////////////////////////////////////////////////////////////////

		IMPLEMENT_PROPERTY_TYPE_HEADER(Enabled, RenderPass, bool, mT("Enabled"), EPBT_Basic, mT("Enable/Disable Pass"), true);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(Enabled, RenderPass, bool, SetEnabled, IsEnabled, core::StringConverter::toString, core::StringConverter::toBool, false)


		//////////////////////////////////////////////////////////////////////////

		IMPLEMENT_PROPERTY_TYPE_HEADER(Ambient,RenderPass,SColor,mT("Ambient"),EPBT_Struct,mT("Ambient color"),0.5);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(Ambient,RenderPass,video::SColor,SetAmbient,GetAmbient,core::StringConverter::toString,core::StringConverter::toColor,false)

		//////////////////////////////////////////////////////////////////////////

		IMPLEMENT_PROPERTY_TYPE_HEADER(Diffuse,RenderPass,SColor,mT("Diffuse"),EPBT_Struct,mT("Diffuse color"),1);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(Diffuse,RenderPass,video::SColor,SetDiffuse,GetDiffuse,core::StringConverter::toString,core::StringConverter::toColor,false)

		//////////////////////////////////////////////////////////////////////////

		IMPLEMENT_PROPERTY_TYPE_HEADER(Specular,RenderPass,SColor,mT("Specular"),EPBT_Struct,mT("Specular color"),0);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(Specular,RenderPass,video::SColor,SetSpecular,GetSpecular,core::StringConverter::toString,core::StringConverter::toColor,false)

		//////////////////////////////////////////////////////////////////////////

		IMPLEMENT_PROPERTY_TYPE_HEADER(Emissive,RenderPass,SColor,mT("Emissive"),EPBT_Struct,mT("Emissive color"),0);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(Emissive,RenderPass,video::SColor,SetEmissive,GetEmissive,core::StringConverter::toString,core::StringConverter::toColor,false)


		//////////////////////////////////////////////////////////////////////////

		IMPLEMENT_PROPERTY_TYPE_HEADER(Shininess,RenderPass,float,mT("Shininess"),EPBT_Basic,mT("Material Shininess"),10);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(Shininess,RenderPass,float,SetShininess,GetShininess,core::StringConverter::toString,core::StringConverter::toFloat,false)

		//////////////////////////////////////////////////////////////////////////

		IMPLEMENT_PROPERTY_TYPE_HEADER(Alpha,RenderPass,float,mT("Alpha"),EPBT_Basic,mT("Material Alpha"),1);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(Alpha,RenderPass,float,SetAlpha,GetAlpha,core::StringConverter::toString,core::StringConverter::toFloat,false)

		//////////////////////////////////////////////////////////////////////////

		IMPLEMENT_PROPERTY_TYPE_HEADER(Thickness,RenderPass,float,mT("Thickness"),EPBT_Basic,mT("Material Thickness"),1);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(Thickness,RenderPass,float,SetThickness,GetThickness,core::StringConverter::toString,core::StringConverter::toFloat,false)

		//////////////////////////////////////////////////////////////////////////

		IMPLEMENT_PROPERTY_TYPE_HEADER(SourceFactor,RenderPass,enum,mT("SourceFactor"),EPBT_Enum,mT("Material Source Blend Factor"),EBF_ZERO);

	RenderPass::PropertyTypeSourceFactor RenderPass::PropertyTypeSourceFactor::instance;
	EBlendFactor RenderPass::PropertyTypeSourceFactor::GetValue(CPropertieSet*object)const
	{
		RenderPass* o=dynamic_cast<RenderPass*>(object);
		if(!o)
			return m_default;
		return o->GetSourceBlendFactor();
	}
	bool RenderPass::PropertyTypeSourceFactor::SetValue(CPropertieSet*object,const EBlendFactor& v)
	{
		RenderPass* o=dynamic_cast<RenderPass*>(object);
		if(!o)return false;
		return o->SetSourceBlendFactor(v);
	}
	core::string RenderPass::PropertyTypeSourceFactor::toString(CPropertieSet*object)const
	{
		if(GetEnum())
			return GetEnum()->ToString(GetValue(object));
		return core::StringConverter::toString((int)GetValue(object),core::string(mT("BlendFactor")));
	}
	bool RenderPass::PropertyTypeSourceFactor::parse(CPropertieSet*reciver,const core::string&str)
	{
		if(GetEnum())
		{
			if(!CheckIsValidValue(str))
				return false;
		}
		int v=0;
		if(GetEnum())
			v= GetEnum()->Parse(str);
		else
			core::StringConverter::parse(str,v,mT("BlendFactor"));
		return SetValue(reciver,(EBlendFactor)v);
	}

	bool RenderPass::PropertyTypeSourceFactor::isDefault(CPropertieSet*reciver)const
	{
		if(GetValue(reciver)==GetDefaultValue())
			return true;
		return false;
	}

	//////////////////////////////////////////////////////////////////////////

	IMPLEMENT_PROPERTY_TYPE_HEADER(DestinationFactor,RenderPass,enum,mT("DestinationFactor"),EPBT_Enum,mT("Material Destination Blend Factor"),EBF_ONE);

	RenderPass::PropertyTypeDestinationFactor RenderPass::PropertyTypeDestinationFactor::instance;
	EBlendFactor RenderPass::PropertyTypeDestinationFactor::GetValue(CPropertieSet*object)const
	{
		RenderPass* o=dynamic_cast<RenderPass*>(object);
		if(!o)
			return m_default;
		return o->GetDestinationBlendFactor();
	}
	bool RenderPass::PropertyTypeDestinationFactor::SetValue(CPropertieSet*object,const EBlendFactor& v)
	{
		RenderPass* o=dynamic_cast<RenderPass*>(object);
		if(!o)return false;
		return o->SetDestinationBlendFactor(v);
	}
	core::string RenderPass::PropertyTypeDestinationFactor::toString(CPropertieSet*object)const
	{
		if(GetEnum())
			return GetEnum()->ToString(GetValue(object));
		return core::StringConverter::toString((int)GetValue(object),core::string(mT("BlendFactor")));
	}
	bool RenderPass::PropertyTypeDestinationFactor::parse(CPropertieSet*reciver,const core::string&str)
	{
		if(GetEnum())
		{
			if(!CheckIsValidValue(str))
				return false;
		}
		int v=0;
		if(GetEnum())
			v= GetEnum()->Parse(str);
		else
			core::StringConverter::parse(str,v,mT("BlendFactor"));
		return SetValue(reciver,(EBlendFactor)v);
	}

	bool RenderPass::PropertyTypeDestinationFactor::isDefault(CPropertieSet*reciver)const
	{
		if(GetValue(reciver)==GetDefaultValue())
			return true;
		return false;
	}
	//////////////////////////////////////////////////////////////////////////

	IMPLEMENT_PROPERTY_TYPE_HEADER(AlphaFunction,RenderPass,enum,mT("AlphaFunction"),EPBT_Enum,mT("Material Alpha Function"),EAF_LessEqual);

	RenderPass::PropertyTypeAlphaFunction RenderPass::PropertyTypeAlphaFunction::instance;
	EAlphaFunc RenderPass::PropertyTypeAlphaFunction::GetValue(CPropertieSet*object)const
	{
		RenderPass* o=dynamic_cast<RenderPass*>(object);
		if(!o)
			return m_default;
		return o->GetAlphaFunction();
	}
	bool RenderPass::PropertyTypeAlphaFunction::SetValue(CPropertieSet*object,const EAlphaFunc& v)
	{
		RenderPass* o=dynamic_cast<RenderPass*>(object);
		if(!o)return false;
		return o->SetAlphaFunction(v);
	}
	core::string RenderPass::PropertyTypeAlphaFunction::toString(CPropertieSet*object)const
	{
		if(GetEnum())
			return GetEnum()->ToString(GetValue(object));
		return core::StringConverter::toString((int)GetValue(object),core::string(mT("AlphaFunc")));
	}
	bool RenderPass::PropertyTypeAlphaFunction::parse(CPropertieSet*reciver,const core::string&str)
	{
		if(GetEnum())
		{
			if(!CheckIsValidValue(str))
				return false;
		}
		int v=0;
		if(GetEnum())
			v= GetEnum()->Parse(str);
		else
			core::StringConverter::parse(str,v,mT("AlphaFunc"));
		return SetValue(reciver,(EAlphaFunc)v);
	}
	bool RenderPass::PropertyTypeAlphaFunction::isDefault(CPropertieSet*reciver)const
	{
		if(GetValue(reciver)==GetDefaultValue())
			return true;
		return false;
	}
	//////////////////////////////////////////////////////////////////////////

	IMPLEMENT_PROPERTY_TYPE_HEADER(AlphaRef,RenderPass,float,mT("AlphaRef"),EPBT_Basic,mT("Material Alpha Reference"),1);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(AlphaRef,RenderPass,float,SetAlphaReferenceValue,GetAlphaReferenceValue,core::StringConverter::toString,core::StringConverter::toFloat,false)


		//////////////////////////////////////////////////////////////////////////

		IMPLEMENT_PROPERTY_TYPE_HEADER(CullFaceDir,RenderPass,enum,mT("CullFaceDir"),EPBT_Enum,mT("Cull face direction"),ECFM_CCW);

	RenderPass::PropertyTypeCullFaceDir RenderPass::PropertyTypeCullFaceDir::instance;
	ECullFaceMode RenderPass::PropertyTypeCullFaceDir::GetValue(CPropertieSet*object)const
	{
		RenderPass* o=dynamic_cast<RenderPass*>(object);
		if(!o)
			return m_default;
		return o->GetCullFaceDirection();
	}
	bool RenderPass::PropertyTypeCullFaceDir::SetValue(CPropertieSet*object,const ECullFaceMode& v)
	{
		RenderPass* o=dynamic_cast<RenderPass*>(object);
		if(!o)return false;
		return o->SetCullFaceDirection(v);
	}
	core::string RenderPass::PropertyTypeCullFaceDir::toString(CPropertieSet*object)const
	{
		if(GetEnum())
			return GetEnum()->ToString(GetValue(object));
		return core::StringConverter::toString((int)GetValue(object),core::string(mT("ECullFaceMode")));
	}
	bool RenderPass::PropertyTypeCullFaceDir::parse(CPropertieSet*reciver,const core::string&str)
	{
		if(GetEnum())
		{
			if(!CheckIsValidValue(str))
				return false;
		}
		int v=0;
		if(GetEnum())
			v= GetEnum()->Parse(str);
		else
			core::StringConverter::parse(str,v,mT("ECullFaceMode"));
		return SetValue(reciver,(ECullFaceMode)v);
	}
	bool RenderPass::PropertyTypeCullFaceDir::isDefault(CPropertieSet*reciver)const
	{
		if(GetValue(reciver)==GetDefaultValue())
			return true;
		return false;
	}

}
}