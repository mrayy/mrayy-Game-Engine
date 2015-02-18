#include "stdafx.h"

#include "ParsedShaderPP.h"
#include "ILogManager.h"

#include "IStream.h"
#include "XMLTree.h"
#include "TextureResourceManager.h"
#include "IVideoDevice.h"
#include "ShaderResourceManager.h"
#include "ShaderSemanticTable.h"

#include "IShaderConstantsCallback.h"
#include "DefaultColors.h"
#include "Engine.h"
#include "IFileSystem.h"

#include "StringUtil.h"

namespace mray
{
namespace video
{


	void ParsedShaderPP::MappedParams::SetValue(float v)
	{
		for(int i=0;i<mappedVals.size();++i)
		{
			mappedVals[i].value->floatParam[0]=v;
		}
	}
	void ParsedShaderPP::MappedParams::SetValue(const math::vector2d& v)
	{
		for(int i=0;i<mappedVals.size();++i)
		{
			mappedVals[i].value->floatParam[0]=v.x;
			mappedVals[i].value->floatParam[1]=v.y;
		}
	}
	void ParsedShaderPP::MappedParams::SetValue(const math::vector3d& v)
	{
		for(int i=0;i<mappedVals.size();++i)
		{
			mappedVals[i].value->floatParam[0]=v.x;
			mappedVals[i].value->floatParam[1]=v.y;
			mappedVals[i].value->floatParam[2]=v.z;
		}
	}
	void ParsedShaderPP::MappedParams::SetValue(const math::vector4d& v)
	{
		for(int i=0;i<mappedVals.size();++i)
		{
			mappedVals[i].value->floatParam[0]=v.x;
			mappedVals[i].value->floatParam[1]=v.y;
			mappedVals[i].value->floatParam[2]=v.z;
			mappedVals[i].value->floatParam[3]=v.w;
		}
	}
	void ParsedShaderPP::MappedParams::SetTexture(ITexture* t)
	{
		for(int i=0;i<mappedVals.size();++i)
		{
			if(mappedVals[i].value->texParam)
				mappedVals[i].value->texParam->SetTexture(t);
		}
	}

ParsedShaderPP::ProcessingPass::ProcessingPass(ParsedShaderPP* owner)
{
	m_owner=owner;
	m_sizeMultiplier=1;
	//m_targetTexture=new TextureUnit();
	m_isChain=false;
}
ParsedShaderPP::ProcessingPass::~ProcessingPass()
{
	for(int i=0;i<m_valus.size();++i)
		delete m_valus[i].texParam;
	m_valus.clear();
	m_targetTexture.clear();
	m_rtTextures.clear();
	m_rtDesc.clear();
	m_targetRenderTarget.clear();
	m_lastTexture=0;
	m_lastRenderTarget=0;
	m_storedTexture=0;
	m_storedRenderTarget=0;
}

ParsedShaderPP::PassValue* ParsedShaderPP::ProcessingPass::GetValue(const core::string& name)
{
	for(int i=0;i<m_valus.size();++i)
	{
		if(m_valus[i].name==name)
			return &m_valus[i];
	}
	return 0;
}
void ParsedShaderPP::ProcessingPass::Setup(const math::rectf& rc)
{
	if(m_sizeType==EViewport)
	{
		CreateRenderTargets(rc.getSize()*m_sizeMultiplier);
	}
}

IRenderArea* ParsedShaderPP::ProcessingPass::render(IRenderArea* input)
{
	IVideoDevice*device=m_owner->getDevice();
	IShaderConstantsCallback callBack;

	callBack.setConstants(m_shader);

	TextureUnit* texUnit;
	ITexture*  lastUnit;

	TextureUnit inputUnit,mainInputUnit,prevInputUnit;
	inputUnit.setTextureClamp(video::ETW_WrapR,ETC_CLAMP_TO_EDGE);
	inputUnit.setTextureClamp(video::ETW_WrapS,ETC_CLAMP_TO_EDGE);
	inputUnit.setTextureClamp(video::ETW_WrapT,ETC_CLAMP_TO_EDGE);

	prevInputUnit.setTextureClamp(video::ETW_WrapR,ETC_CLAMP_TO_EDGE);
	prevInputUnit.setTextureClamp(video::ETW_WrapS,ETC_CLAMP_TO_EDGE);
	prevInputUnit.setTextureClamp(video::ETW_WrapT,ETC_CLAMP_TO_EDGE);

	mainInputUnit.setTextureClamp(video::ETW_WrapR,ETC_CLAMP_TO_EDGE);
	mainInputUnit.setTextureClamp(video::ETW_WrapS,ETC_CLAMP_TO_EDGE);
	mainInputUnit.setTextureClamp(video::ETW_WrapT,ETC_CLAMP_TO_EDGE);

	prevInputUnit.SetTexture(input->GetColorTexture());

	video::IGPUShaderProgram* vshader= m_shader->GetVertexShader();
	video::IGPUShaderProgram* fshader=m_shader->GetFragmentShader();

	lastUnit = input->GetColorTexture();

	for (int tex=0;tex<m_targetRenderTarget.size();++tex)
	{
		device->setRenderTarget(m_targetRenderTarget[tex], 1, 1, 0);
		device->useShader(m_shader);
		
		texUnit=m_targetTexture[tex];
		for(int i=0;i<m_valus.size();++i)
		{
			PassValue& v=m_valus[i];
			switch(v.type)
			{
			case EV_Texture:
				{
					if(fshader)
						fshader->setTexture(v.name,v.texParam);
// 					if(v.name.equals_ignore_case(mT("tex0")))
// 						texUnit=&tmpUnit;
				}break;
			case EV_Output:
				{
					ProcessingPass*pass=m_owner->GetPass(v.intParam);
					if(!pass)
						break;
					//tmpUnit.SetTexture(pass->getOutput()->getColorTexture());
					if(fshader)
					{
						video::TextureUnit t;
						t.SetTexture(pass->getOutput()->GetColorTexture(v.texIndex));
						//not a good way 
						//pass->m_lastTexture->SetTexture(pass->m_lastRenderTarget->getColorTexture(v.texIndex));
						fshader->setTexture(v.name,&t);//pass->m_lastTexture);
					}
 				//	if(v.name.equals_ignore_case(mT("tex0")))
 				//		texUnit=pass->m_lastTexture.pointer();
				}break;
			case EV_Input:
				{
					if(fshader)
					{
						inputUnit.SetTexture(input->GetColorTexture(v.intParam));
						fshader->setTexture(v.name,&inputUnit);
					}
// 					if(v.name.equals_ignore_case(mT("tex0")))
// 						texUnit=&tmpUnit;
				}break;
			case EV_PrevInput:
				{
					if(fshader)
						fshader->setTexture(v.name,&prevInputUnit);
// 					if(v.name.equals_ignore_case(mT("tex0")))
// 						texUnit=&tmpUnit;
				}break;
			case EV_MainInput:
				{
				mainInputUnit.SetTexture(m_owner->getInput()->GetColorTexture(v.texIndex));
					if(fshader)
						fshader->setTexture(v.name,&mainInputUnit);
// 					if(v.name.equals_ignore_case(mT("tex0")))
// 						texUnit=&tmpUnit;
				}break;
			case EV_TexSize:
				{
					if(v.strParam.equals_ignore_case(mT("input")) ){
						math::vector2d sz;
						sz.x = input->GetColorTexture()->getSize().x;
						sz.y = input->GetColorTexture()->getSize().y;
// 						core::stringc vv;
// 						core::string_to_char(v.name,vv);
						GPUUniform* u=0;
						if( vshader)
							u=vshader->getUniform(v.name);
						if(u)
							vshader->setConstant(u,&sz.x,2);
						else
						{
							if(fshader){
								u=fshader->getUniform(v.name);
								if(u)
									fshader->setConstant(u,&sz.x,2);
							}
						}
					}
				}break;
			case EV_PrevTexSize:
				{
					math::vector2d sz;
					sz.x=prevInputUnit.GetTexture()->getSize().x;
					sz.y=prevInputUnit.GetTexture()->getSize().y;
					GPUUniform* u=0;
					if( vshader)
						u=vshader->getUniform(v.name);
					if(u)
						vshader->setConstant(u,&sz.x,2);
					else
					{
						if(fshader){
							u=fshader->getUniform(v.name);
							if(u)
								fshader->setConstant(u,&sz.x,2);
						}
					}
				}break;
			case EV_LastFrame:
				{
					if(fshader)
						fshader->setTexture(v.name,m_storedTexture);
// 					if(v.name.equals_ignore_case(mT("tex0")))
// 						texUnit=m_storedTexture;
				}break;
			case EV_Float:
				{
					if(vshader)
						vshader->setConstant(v.name,v.floatParam,1);
					if(fshader)
						fshader->setConstant(v.name,v.floatParam,1);
				}break;
			case EV_Float2:
				{
					if(vshader)
						vshader->setConstant(v.name,v.floatParam,2);
					if(fshader)
						fshader->setConstant(v.name,v.floatParam,2);
				}break;
			case EV_Float3:
				{
					if(vshader)
						vshader->setConstant(v.name,v.floatParam,3);
					if(fshader)
						fshader->setConstant(v.name,v.floatParam,3);
				}break;
			case EV_Float4:
				{
					if(vshader)
						vshader->setConstant(v.name,v.floatParam,4);
					if(fshader)
						fshader->setConstant(v.name,v.floatParam,4);
				}break;
				/*
			case EV_Depth:
				{
					m_shader->setTexture(v.name,owner->getInput()->getDepthBuffer());
					if(v.name.equals_ignore_case(mT("tex0")))
						texUnit=owner->getInput()->getDepthBuffer();
				}break;*/
			}
		}
	
		if(m_isChain && fshader)
		{
			fshader->setTexture(mT("ChainTex"),texUnit);
		}

		texUnit=ShaderSemanticTable::getInstance().getTextureStage(0);

		device->draw2DImage(math::rectf(0,1),/*texUnit,*/video::DefaultColors::White);
		prevInputUnit.SetTexture(m_targetTexture[tex]->GetTexture());
	}

	device->unuseShader();
	device->setRenderTarget(0,0,0,0);

	IRenderTarget* ret=m_lastRenderTarget;
	if(m_storedTexture)
	{
		int lastIdx=m_targetRenderTarget.size()-1;
		math::Swap(m_targetRenderTarget[lastIdx],m_storedRenderTarget);
		math::Swap(m_targetTexture[lastIdx],m_storedTexture);
		//m_lastRenderTarget=m_targetRenderTarget[lastIdx];
	}

	return ret;
}

IRenderArea* ParsedShaderPP::ProcessingPass::getOutput()
{
	return m_lastRenderTarget;
}

void ParsedShaderPP::ProcessingPass::CreateRenderTargets(const math::vector2di&texSize)
{
	IVideoDevice* dev=Engine::getInstance().getDevice();
	int count=1;
	math::vector2di size=1;
	if(m_isChain)
	{
		int currSize=1;
		int startSize=math::Min(texSize.x,texSize.y);
		for (;currSize<startSize;currSize*=3)
			++count;
		count=math::Max(1,count);
		size=currSize;
	}else
	{
		size=texSize;
	}
	m_rtTextures.resize(count);
	m_targetTexture.resize(count);
	m_targetRenderTarget.resize(count);
	for (int i=0;i<count;++i)
	{
		if(m_targetTexture[i].isNull())
		{
			m_targetTexture[i]=new TextureUnit();

			m_targetTexture[i]->setTextureClamp(video::ETW_WrapR,ETC_CLAMP_TO_EDGE);
			m_targetTexture[i]->setTextureClamp(video::ETW_WrapS,ETC_CLAMP_TO_EDGE);
			m_targetTexture[i]->setTextureClamp(video::ETW_WrapT,ETC_CLAMP_TO_EDGE);
		}

		for(int j=0;j<m_rtDesc.size();++j)
		{
			video::ITexturePtr tex=0;
			if(m_rtTextures[i].size()<=j)
			{
				tex=dev->createTexture2D(size,m_rtDesc[j],true);
				m_rtTextures[i].push_back(tex);
				if(j==0)
				{
					m_targetTexture[i]->SetTexture(tex);

					m_targetRenderTarget[i]=dev->createRenderTarget(name+mT("_")+core::StringConverter::toString(i),m_targetTexture[i]->GetTexture(),0,0,m_depthOnly);
				}else
					m_targetRenderTarget[i]->attachRenderTarget(tex,j);
			}else
			{
				tex=m_rtTextures[i][j];
				tex->createTexture(math::vector3d(size.x,size.y,1),tex->getImageFormat());
			}
		}
		size/=3;
	}

	m_lastTexture=m_targetTexture[count-1];
	m_lastRenderTarget=m_targetRenderTarget[count-1];

}

void ParsedShaderPP::ProcessingPass::Parse(xml::XMLElement*elem)
{
	xml::XMLElement* node;
	xml::XMLAttribute*attr;
	math::vector2d texSize;
	EPixelFormat texFormat=EPixel_R8G8B8A8;
	core::string shaderVP;
	core::string shaderFP;
	core::stringc mainVP=("main_vp");
	core::stringc mainFP=("main_fp");
	m_depthOnly=false;

	attr=elem->getAttribute(mT("name"));
	if(attr)
		name=attr->value;
	attr=elem->getAttribute(mT("targetSize"));
	if(!attr)
	{
		attr=elem->getAttribute(mT("defSize"));
		if(!attr)
		{
			gLogManager.log(mT("ProcessingPass::Parse() - Target texture size not set."),ELL_WARNING);
			texSize=8;
		}else
		{
			if(attr->value.equals_ignore_case(mT("viewport")))
			{
				m_sizeType=EViewport;
	//			texSize=owner->getDevice()->getViewportRect().getSize();
			}
			attr=elem->getAttribute(mT("sizeMultiplayer"));
			if(attr)
			{
				m_sizeMultiplier=core::StringConverter::toFloat(attr->value);
				texSize*=m_sizeMultiplier;
			}
		}
	}else
	{
		m_sizeType=ECustom;
		texSize=core::StringConverter::toVector2d(attr->value);
	}

	attr=elem->getAttribute(mT("depthOnly"));
	if(attr)
	{
		m_depthOnly=core::StringConverter::toBool(attr->value);
	}
	attr=elem->getAttribute(mT("isChain"));
	if(attr)
	{
		m_isChain=core::StringConverter::toBool(attr->value);
	}

	attr=elem->getAttribute(mT("shaderVP"));
	if(!attr)
	{
		gLogManager.log(mT("ProcessingPass::Parse() - Shader Vertex Program (ShaderVP) not set."),ELL_WARNING);
	}else
	{
		shaderVP=attr->value;
	}
	attr=elem->getAttribute(mT("shaderFP"));
	if(!attr)
	{
		gLogManager.log(mT("ProcessingPass::Parse() - Shader Fragment Program (ShaderFP) not set."),ELL_WARNING);
	}else
	{
		shaderFP=attr->value;
	}
	attr=elem->getAttribute(mT("mainVP"));
	if(attr)
	{
		core::string_to_char(attr->value,mainVP);
	}
		attr=elem->getAttribute(mT("mainFP"));
	if(attr)
	{
		core::string_to_char(attr->value,mainFP);
	}
	attr=elem->getAttribute(mT("targetType"));
	if(attr)
	{
		int v=core::StringConverter::toEnum(mT("EPixelFormat"),attr->value);
		if(v!=-1)
			texFormat=(EPixelFormat)v;
	}

	m_rtDesc.clear();

	node=elem->getSubElement(mT("RenderTarget"));
	while(node)
	{
		attr=node->getAttribute(mT("Type"));
		if(attr)
		{
			int v=core::StringConverter::toEnum(mT("EPixelFormat"),attr->value);
			if(v!=-1)
				m_rtDesc.push_back((EPixelFormat)v);
		}

		node=node->nextSiblingElement(mT("RenderTarget"));
	}
	if(m_rtDesc.size()==0)
	{
		m_rtDesc.push_back(texFormat);
	}
	if(texSize==0)
		texSize=1;

	CreateRenderTargets(texSize);
	m_shader=new GPUShader();
	m_shader->SetVertexShader(gShaderResourceManager.loadShaderFromFile(shaderVP, video::EShader_VertexProgram, mainVP.c_str(), video::ShaderPredefList(), mT("cg")));
	m_shader->SetFragmentShader(gShaderResourceManager.loadShaderFromFile(shaderFP, video::EShader_FragmentProgram, mainFP.c_str(), video::ShaderPredefList(), mT("cg")));


	node=elem->getSubElement(mT("Value"));
	while(node)
	{
		PassValue v;
		bool add=true;
		attr=node->getAttribute(mT("name"));
		if(attr)
			v.name=attr->value;
		attr=node->getAttribute(mT("type"));
		xml::XMLAttribute* valAttr=node->getAttribute(mT("value"));
		if(attr)
		{
			xml::XMLAttribute*valueAttr=node->getAttribute(mT("value"));
			if(attr->value.equals_ignore_case(mT("Texture")))
			{
				v.texParam=new TextureUnit();
				v.type=EV_Texture;
				if(valueAttr)
				{
					attr=node->getAttribute(mT("texType"));
					if(attr){
						if(attr->value.equals_ignore_case(mT("1D")))
							v.texParam->SetTexture(gTextureResourceManager.loadTexture1D(valueAttr->value));
						else  if(attr->value.equals_ignore_case(mT("2D")))
							v.texParam->SetTexture(gTextureResourceManager.loadTexture2D(valueAttr->value));
						else  if(attr->value.equals_ignore_case(mT("3D")))
							v.texParam->SetTexture(gTextureResourceManager.loadTexture3D(valueAttr->value));
						else  if(attr->value.equals_ignore_case(mT("Cube")))
						{
							{
								core::string base,ext;
								core::StringUtil::SplitPathExt(valueAttr->value,base,ext);
								std::vector<core::string> paths;
								paths.push_back(base+"+x."+ext);
								paths.push_back(base+"-x."+ext);
								paths.push_back(base+"+y."+ext);
								paths.push_back(base+"-y."+ext);
								paths.push_back(base+"+z."+ext);
								paths.push_back(base+"-z."+ext);
								v.texParam->SetTexture(gTextureResourceManager.loadTextureCube(valueAttr->value,&paths[0],true));
							}
						}
						else
							v.texParam->SetTexture(gTextureResourceManager.loadTexture2D(valueAttr->value));

						if(v.texParam->GetTexture())
						{
							v.texParam->setTextureClamp(video::ETW_WrapR,ETC_CLAMP_TO_EDGE);
							v.texParam->setTextureClamp(video::ETW_WrapS,ETC_CLAMP_TO_EDGE);
							v.texParam->setTextureClamp(video::ETW_WrapT,ETC_CLAMP_TO_EDGE);
							v.texParam->GetTexture()->setMipmapsFilter(false);
							v.texParam->GetTexture()->load(false);
						}
					}
				}
			}else if(attr->value.equals_ignore_case(mT("Output")))
			{
				v.type=EV_Output;
				if(valueAttr)
				{
					v.intParam=m_owner->GetPassID(valueAttr->value);
				}
				attr=node->getAttribute(mT("Index"));
				if(attr)
				{
					v.texIndex=core::StringConverter::toInt(attr->value);
				}
			}else if(attr->value.equals_ignore_case(mT("Output")))
			{
				v.type=EV_Output;
				if(valueAttr)
				{
					v.intParam=m_owner->GetPassID(valueAttr->value);
				}
				attr=node->getAttribute(mT("Index"));
				if(attr)
				{
					v.texIndex=core::StringConverter::toInt(attr->value);
				}
			}else if(attr->value.equals_ignore_case(mT("Input")))
			{
				v.type=EV_Input;
				attr=node->getAttribute(mT("Index"));
				if(attr)
				{
					v.texIndex=core::StringConverter::toInt(attr->value);
				}
			}else if(attr->value.equals_ignore_case(mT("PrevInput")))
			{
				v.type=EV_PrevInput;
				attr=node->getAttribute(mT("Index"));
				if(attr)
				{
					v.texIndex=core::StringConverter::toInt(attr->value);
				}
			}else if(attr->value.equals_ignore_case(mT("MainInput")))
			{
				v.type=EV_MainInput;
				attr=node->getAttribute(mT("Index"));
				if(attr)
				{
					v.texIndex=core::StringConverter::toInt(attr->value);
				}
			}
			else if(attr->value.equals_ignore_case(mT("Depth")))
			{
				v.type=EV_Depth;
			}else if(attr->value.equals_ignore_case(mT("TexSize")))
			{
				v.type=EV_TexSize;
				attr=node->getAttribute(mT("TexName"));
				if(attr)
					v.strParam=attr->value;
				else
					v.strParam=mT("input");
			}else if(attr->value.equals_ignore_case(mT("PrevTexSize")))
			{
				v.type=EV_PrevTexSize;
			}else if (attr->value.equals_ignore_case(mT("Last")))
			{
				v.type=EV_LastFrame;
				if(!m_storedTexture)
				{
					math::vector2di size;
					size.x=m_lastTexture->GetTexture()->getSize().x;
					size.y=m_lastTexture->GetTexture()->getSize().y;
					m_storedTexture=new TextureUnit();
					m_storedTexture->SetTexture(m_owner->getDevice()->createTexture2D(size,texFormat,true));
					m_storedRenderTarget=m_owner->getDevice()->createRenderTarget(mT(""),m_storedTexture->GetTexture(),0,0,m_depthOnly);
				}
			}
			else if (attr->value.equals_ignore_case(mT("Float")))
			{
				if(!valAttr)
					continue;
				v.type=EV_Float;
				v.floatParam[0]=core::StringConverter::toFloat(valAttr->value);
			}
			else if (attr->value.equals_ignore_case(mT("Float2")))
			{
				if(!valAttr)
					continue;
				v.type=EV_Float2;
				math::vector2d vec=core::StringConverter::toVector2d(valAttr->value);
				v.floatParam[0]=vec.x;
				v.floatParam[1]=vec.y;
			}
			else if (attr->value.equals_ignore_case(mT("Float3")))
			{
				if(!valAttr)
					continue;
				v.type=EV_Float3;
				math::vector3d vec=core::StringConverter::toVector3d(valAttr->value);
				v.floatParam[0]=vec.x;
				v.floatParam[1]=vec.y;
				v.floatParam[2]=vec.z;
			}
			else if (attr->value.equals_ignore_case(mT("Float4")))
			{
				if(!valAttr)
					continue;
				v.type=EV_Float4;
				math::vector4d vec=core::StringConverter::toVector4d(valAttr->value);
				v.floatParam[0]=vec.x;
				v.floatParam[1]=vec.y;
				v.floatParam[2]=vec.z;
				v.floatParam[3]=vec.w;
			}else
				add=false;

			if(add)
				m_valus.push_back(v);
		}
		node=node->nextSiblingElement(mT("Value"));
	}
}


//////////////////////////////////////////////////////////////////////////

ParsedShaderPP::ForwardPass::ForwardPass(ParsedShaderPP* owner):ProcessingPass(owner)
{
	name=mT("ForwardPass");
}
ParsedShaderPP::ForwardPass::~ForwardPass()
{
}
void ParsedShaderPP::ForwardPass::Setup(const math::rectf& rc)
{
}

IRenderArea* ParsedShaderPP::ForwardPass::render(IRenderArea* input)
{
	for(int i=0;i<m_targets.size();++i)
	{
		const RenderTargetInfo& v=m_targets[i];
		video::ITexturePtr tex;
		switch(v.type)
		{
		case EV_Output:
			{
				ProcessingPass*pass=m_owner->GetPass(v.passID);
				if(!pass)
					break;
				tex = pass->getOutput()->GetColorTexture(v.texIndex);
			}
			break;
		case EV_Input:
			tex = input->GetColorTexture(v.texIndex);
			break;
		case EV_MainInput:
			tex = m_owner->getInput()->GetColorTexture(v.texIndex);
			break;
		}
		m_targetTexture[i]->SetTexture(tex);
		m_lastRenderTarget->attachRenderTarget(tex,i);
	}
	return m_lastRenderTarget;
}
IRenderArea* ParsedShaderPP::ForwardPass::getOutput()
{
	return m_lastRenderTarget;
}

void ParsedShaderPP::ForwardPass::Parse(xml::XMLElement*elem)
{
	xml::XMLElement* node;
	xml::XMLAttribute* attr;
	attr=elem->getAttribute(mT("name"));
	if(attr)
		name=attr->value;
	node=elem->getSubElement(mT("Target"));
	while(node)
	{
		RenderTargetInfo v;
		xml::XMLAttribute*typeAttr=node->getAttribute(mT("Type"));
		if(!typeAttr)
		{
			node=node->nextSiblingElement(mT("Target"));
			continue;
		}
		xml::XMLAttribute*valueAttr=node->getAttribute(mT("Value"));
		if(typeAttr->value.equals_ignore_case(mT("Output")))
		{
			v.type=EV_Output;
			if(valueAttr)
			{
				v.passID=m_owner->GetPassID(valueAttr->value);
			}
			attr=node->getAttribute(mT("Index"));
			if(attr)
			{
				v.texIndex=core::StringConverter::toInt(attr->value);
			}
		}else if(typeAttr->value.equals_ignore_case(mT("Input")))
		{
			v.type=EV_Input;
			attr=node->getAttribute(mT("Index"));
			if(attr)
			{
				v.texIndex=core::StringConverter::toInt(attr->value);
			}
		}else if(typeAttr->value.equals_ignore_case(mT("MainInput")))
		{
			v.type=EV_MainInput;
			attr=node->getAttribute(mT("Index"));
			if(attr)
			{
				v.texIndex=core::StringConverter::toInt(attr->value);
			}
		}
		m_targets.push_back(v);
		node=node->nextSiblingElement(mT("Target"));
	}
	m_targetTexture.resize(m_targets.size());
	m_targetRenderTarget.resize(1);
	for(int i=0;i<m_targets.size();++i)
	{
		m_targetTexture[i]=new TextureUnit();

		m_targetTexture[i]->setTextureClamp(video::ETW_WrapR,ETC_CLAMP_TO_EDGE);
		m_targetTexture[i]->setTextureClamp(video::ETW_WrapS,ETC_CLAMP_TO_EDGE);
		m_targetTexture[i]->setTextureClamp(video::ETW_WrapT,ETC_CLAMP_TO_EDGE);
	}

	m_targetRenderTarget[0]=m_owner->getDevice()->
		createRenderTarget(name+mT("_main"),0,0,0,0);

	m_lastRenderTarget=m_targetRenderTarget[0];
	m_lastTexture=m_targetTexture[m_targetTexture.size()-1];
}

//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

ParsedShaderPP::SubPostEffect::SubPostEffect(ParsedShaderPP* owner):ProcessingPass(owner)
{
	name=mT("SubPostEffect");
}
ParsedShaderPP::SubPostEffect::~SubPostEffect()
{
}
void ParsedShaderPP::SubPostEffect::Setup(const math::rectf& rc)
{
	if(m_ppEffect)
		m_ppEffect->Setup(rc);
}

void ParsedShaderPP::SubPostEffect::ConstructRT(IRenderArea* input)
{
	for(int i=0;i<m_targets.size();++i)
	{
		const RenderTargetInfo& v=m_targets[i];
		video::ITexturePtr tex;
		switch(v.type)
		{
		case EV_Output:
			{
				ProcessingPass*pass=m_owner->GetPass(v.passID);
				if(!pass)
					break;
				tex = pass->getOutput()->GetColorTexture(v.texIndex);
			}
			break;
		case EV_Input:
			tex = input->GetColorTexture(v.texIndex);
			break;
		case EV_MainInput:
			tex = m_owner->getInput()->GetColorTexture(v.texIndex);
			break;
		}
		m_targetTexture[i]->SetTexture(tex);
		m_lastRenderTarget->attachRenderTarget(tex,i);
	}
}
IRenderArea* ParsedShaderPP::SubPostEffect::render(IRenderArea* input)
{
	if(!m_ppEffect)
		return input;
	ConstructRT(input);
	return m_ppEffect->render(m_lastRenderTarget);
}
IRenderArea* ParsedShaderPP::SubPostEffect::getOutput()
{
	if(!m_ppEffect)
		return 0;
	return m_ppEffect->getOutput();
}

void ParsedShaderPP::SubPostEffect::Parse(xml::XMLElement*elem)
{
	xml::XMLElement* node;
	xml::XMLAttribute* attr;
	attr=elem->getAttribute(mT("name"));
	if(attr)
		name=attr->value;
	attr= elem->getAttribute(mT("EffectName"));
	if(attr)
	{
		m_effectName=attr->value;
		m_ppEffect=new ParsedShaderPP(m_owner->getDevice());
		OS::IStreamPtr stream=gFileSystem.openFile(m_effectName);
		if(stream)
		{
			m_ppEffect->LoadXML(stream);
			stream->close();
		}
	}
	node=elem->getSubElement(mT("Input"));
	while(node)
	{
		RenderTargetInfo v;
		xml::XMLAttribute*typeAttr=node->getAttribute(mT("Type"));
		if(!typeAttr)
		{
			node=node->nextSiblingElement(mT("Input"));
			continue;
		}
		xml::XMLAttribute*valueAttr=node->getAttribute(mT("Value"));
		if(typeAttr->value.equals_ignore_case(mT("Output")))
		{
			v.type=EV_Output;
			if(valueAttr)
			{
				v.passID=m_owner->GetPassID(valueAttr->value);
			}
			attr=node->getAttribute(mT("Index"));
			if(attr)
			{
				v.texIndex=core::StringConverter::toInt(attr->value);
			}
		}else if(typeAttr->value.equals_ignore_case(mT("Input")))
		{
			v.type=EV_Input;
			attr=node->getAttribute(mT("Index"));
			if(attr)
			{
				v.texIndex=core::StringConverter::toInt(attr->value);
			}
		}else if(typeAttr->value.equals_ignore_case(mT("MainInput")))
		{
			v.type=EV_MainInput;
			attr=node->getAttribute(mT("Index"));
			if(attr)
			{
				v.texIndex=core::StringConverter::toInt(attr->value);
			}
		}
		m_targets.push_back(v);
		node=node->nextSiblingElement(mT("Input"));
	}
	m_targetTexture.resize(m_targets.size());
	m_targetRenderTarget.resize(1);
	for(int i=0;i<m_targets.size();++i)
	{
		m_targetTexture[i]=new TextureUnit();

		m_targetTexture[i]->setTextureClamp(video::ETW_WrapR,ETC_CLAMP_TO_EDGE);
		m_targetTexture[i]->setTextureClamp(video::ETW_WrapS,ETC_CLAMP_TO_EDGE);
		m_targetTexture[i]->setTextureClamp(video::ETW_WrapT,ETC_CLAMP_TO_EDGE);
	}

	m_targetRenderTarget[0]=m_owner->getDevice()->
		createRenderTarget(name+mT("_main"),0,0,0,0);

	m_lastRenderTarget=m_targetRenderTarget[0];
	m_lastTexture=m_targetTexture[m_targetTexture.size()-1];
}


//////////////////////////////////////////////////////////////////////////

ParsedShaderPP::ParsedShaderPP(IVideoDevice*device)
{
	m_device=device;
	m_result=0;
	m_input=0;
}
ParsedShaderPP::~ParsedShaderPP()
{
}

bool ParsedShaderPP::LoadXML(OS::IStream* stream)
{
	if(!stream)
		return false;
	xml::XMLTree tree;
	if(!tree.load(stream))
	{
		gLogManager.log(core::string(mT("Cann't load PostProcessing file: "))+stream->getStreamName(),ELL_WARNING);
		return 0;
	}

	m_passes.clear();
	m_passID.clear();

	xml::XMLAttribute* attr;
	xml::XMLElement*elem=tree.getSubElement(mT("PostProcessing"));
	if(!elem)
	{
		gLogManager.log(core::string(mT("Cann't find main tag \"PostProcessing\" in: "))+stream->getStreamName(),ELL_WARNING);
		return 0;
	}
	attr=elem->getAttribute(mT("name"));
	if(attr)
		m_name=attr->value;

	xml::xmlSubElementsMapIT it =elem->getElementsBegin();
	xml::xmlSubElementsMapIT end=elem->getElementsEnd();
	for (;it!=end;++it)
	{
		if((*it)->GetType()!=xml::ENT_Element)continue;
		xml::XMLElement*e=dynamic_cast<xml::XMLElement*>(*it);
		if(e->getName().equals_ignore_case(mT("Pass")))
		{
			m_result=new ProcessingPass(this);
			m_result->Parse(e);
			m_passes.push_back(m_result);
			m_passID[m_result->name]=m_passes.size()-1;
		}else if(e->getName().equals_ignore_case(mT("ForwardPass")))
		{
			m_result=new ForwardPass(this);
			m_result->Parse(e);
			m_passes.push_back(m_result);
			m_passID[m_result->name]=m_passes.size()-1;
		
		}else if(e->getName().equals_ignore_case(mT("EffectPass")))
		{
			SubPostEffect*pp=new SubPostEffect(this);;
			m_result=pp;
			m_result->Parse(e);
			m_passes.push_back(m_result);
			m_passID[m_result->name]=m_passes.size()-1;
			if(pp->GetEffect())
			{
				m_params.insert(m_params.end(),pp->GetEffect()->GetParams().begin(),pp->GetEffect()->GetParams().end());
			}
		
		}
	}

	xml::XMLElement* node=elem->getSubElement(mT("Params"));
	if(node)
	{
		node=node->getSubElement(mT("Param"));
		while(node)
		{
			MappedParams p;
			xml::XMLAttribute* attr=node->getAttribute(mT("Name"));
			if(attr)
				p.ParamName=attr->value;

			xml::XMLElement* subNode=node->getSubElement(mT("MappedValue"));
			while(subNode)
			{
				MappedParams::PassVals v;
				attr=subNode->getAttribute(mT("Pass"));
				if(attr)
					v.pass=GetPass(GetPassID(attr->value));

				attr=subNode->getAttribute(mT("Value"));
				if(attr && v.pass)
				{
					v.value=v.pass->GetValue(attr->value);
				}
				if(v.value)
					p.mappedVals.push_back(v);
				subNode=subNode->nextSiblingElement(mT("MappedValue"));
			}
			if(p.mappedVals.size()>0)
			{
				m_params.push_back(p);
				m_paramsIdx[p.ParamName]=m_params.size()-1;
			}
			node=node->nextSiblingElement(mT("Param"));
		}
	}

	return 1;
}

IVideoDevice* ParsedShaderPP::getDevice()
{
	return m_device;
}
ParsedShaderPP::MappedParams* ParsedShaderPP::GetParam(const core::string&name)
{
	std::vector<core::string> splits=core::StringUtil::Split(name,".");
	if(splits.size()>1)
	{
		ParsedShaderPP* pass=this;
		for (int i=0;i<splits.size()-1;++i)
		{
			int id=pass->GetPassID(splits[i]);
			if(id==-1)
				return 0;

			SubPostEffect* p = dynamic_cast<SubPostEffect*>(pass->GetPass(id));

			if (!p)
			{
				break;
			}
			pass=p->GetEffect();
		}

		if(!pass)return 0;
		return pass->GetParam(splits[splits.size()-1]);
	}else
	{
		ParamsMap::iterator it= m_paramsIdx.find(name);
		if(it==m_paramsIdx.end())return 0;
		return &m_params[it->second];
	}
}


ParsedShaderPP::PassValue* ParsedShaderPP::GetValue(const core::string&valuePath)
{
	std::vector<core::string> splits=core::StringUtil::Split(valuePath,".");
	if(splits.size()>1)
	{
		ParsedShaderPP* pass=this;
		SubPostEffect*sub=0;
		for (int i=0;i<splits.size()-1;++i)
		{
			int id=pass->GetPassID(splits[i]);
			if(id==-1)
				return 0;
			sub=((SubPostEffect*)pass->GetPass(id));
			if(!sub)
				break;
			pass=sub->GetEffect();
		}
		if(sub)
			return sub->GetValue(splits[splits.size()-1]);
	}
		return 0;
}

void ParsedShaderPP::Setup(const math::rectf& targetVP)
{
	if(m_lastVP==targetVP)
		return;
	m_lastVP=targetVP;
	for (int i=0;i<m_passes.size();++i)
	{
		m_passes[i]->Setup(targetVP);
	}
}
IRenderArea* ParsedShaderPP::render(IRenderArea* input)
{
	if(!isEnabled())return input;
	//getDevice()->set2DMode();
	m_input=input;
	IRenderArea* inTex = input;
	for (int i=0;i<m_passes.size();++i)
	{
		inTex=m_passes[i]->render(inTex);
	}
	m_input=0;
	return inTex;
}
IRenderArea* ParsedShaderPP::getOutput()
{
	if(!m_result)
		return 0;
	return m_result->getOutput();
}

 int ParsedShaderPP::GetPassID(const core::string&name)
 {
	 std::vector<core::string> splits=core::StringUtil::Split(name,".");
	 if(splits.size()>1)
	 {
		 ParsedShaderPP* pass=this;
		 for (int i=0;i<splits.size()-1;++i)
		 {
			 int id=pass->GetPassID(splits[i]);
			 if(id==-1)
				 return 0;
			 pass=((SubPostEffect*)pass->GetPass(id))->GetEffect();
		 }

		 if(!pass)return 0;
		 return pass->GetPassID(splits[splits.size()-1]);
	 }else
	 {
		 PassIDMap::iterator it= m_passID.find(name);
		 if(it==m_passID.end())
			 return -1;
		 return it->second;
	 }
 }
 ParsedShaderPP::ProcessingPass *ParsedShaderPP::GetPass(int id)
 {
	 if(id>=0 && id<m_passes.size())
		 return m_passes[id];
	 return 0;
 }


 IRenderArea* ParsedShaderPP::getInput()
 {
	 return m_input;
 }

}
}
