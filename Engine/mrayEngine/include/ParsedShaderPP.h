

/********************************************************************
	created:	2010/06/15
	created:	15:6:2010   19:40
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ParsedShaderPP.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ParsedShaderPP
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ParsedShaderPP_h__
#define ParsedShaderPP_h__

#include "IPostProcessing.h"
#include "GPUShader.h"
#include "TextureUnit.h"
#include "IRenderTarget.h"




namespace mray
{
namespace OS
{
	class IStream;
}

namespace video
{

class MRAY_DLL ParsedShaderPP:public IPostProcessing
{
private:
public:
	enum EValueType
	{
		EV_NotSet,
		EV_Texture,		//general texture
		EV_Output,		//output texture of a pass
		EV_PrevInput,	//in case of chained passes,use last rendered pass
		EV_Input,		//input texture of the pass
		EV_Depth,		//main texture's depth buffer
		EV_MainInput,	//main texture been passed
		EV_LastFrame,	//this will make use of the last rendered frame
		EV_Float,
		EV_Float2,
		EV_Float3,
		EV_Float4,

		EV_TexSize,
		EV_PrevTexSize	//in case of chained passes,use last rendered pass's size
	};
	struct PassValue
	{
		PassValue(){
			type=EV_NotSet;
			texParam=0;
			intParam=0;
			texParam=0;
			texIndex=0;
		}
		~PassValue()
		{
			//				delete texParam;
		}
		core::string name;
		EValueType type;

		core::string strParam;
		TextureUnit* texParam;
		int intParam;
		float floatParam[4];
		int texIndex;
	};
	class ProcessingPass;
	class MRAY_DLL MappedParams
	{
	public:
		struct PassVals
		{
			PassVals():pass(0),value(0)
			{}
			PassVals(ProcessingPass* p,PassValue* v):pass(p),value(v)
			{}
			ProcessingPass* pass;
			PassValue* value;	
		};
		MappedParams()
		{}
		core::string ParamName;
		std::vector<PassVals> mappedVals;

		void SetValue(float v);
		void SetValue(const math::vector2d& v);
		void SetValue(const math::vector3d& v);
		void SetValue(const math::vector4d& v);
		void SetTexture(ITexture* t);
	};
	class MRAY_DLL ProcessingPass:public IPostProcessing
	{
	public:
	protected:
		enum ESizeType
		{
			EViewport,
			ECustom
		};
		std::vector<PassValue> m_valus;

		ParsedShaderPP* m_owner;

		GPUShaderPtr  m_shader;

		std::vector<TextureUnitPtr> m_targetTexture;
		std::vector<IRenderTargetPtr> m_targetRenderTarget;
		std::vector<std::vector<video::ITexturePtr>> m_rtTextures;
		std::vector<EPixelFormat> m_rtDesc;

		TextureUnitPtr m_lastTexture;
		IRenderTargetPtr m_lastRenderTarget;

		TextureUnitPtr m_storedTexture;
		IRenderTargetPtr m_storedRenderTarget;

		float m_sizeMultiplier;
		ESizeType m_sizeType;
		
		bool m_depthOnly;

		bool m_isChain;	// if this pass is chain, then a series of passes will be created and devided by 2 until they reach 1x1

		void CreateRenderTargets(const math::vector2di&texSize);

	public:
		core::string name;

	public:
		ProcessingPass(ParsedShaderPP* owner);
		virtual~ProcessingPass();

		PassValue* GetValue(const core::string& name);

		GPUShader* GetShader(){return m_shader;}

		virtual void Setup(const math::rectf& rc);
		virtual IRenderArea* render(IRenderArea* input);
		virtual IRenderArea* getOutput();

		virtual void Parse(xml::XMLElement*elem);
	};
	class MRAY_DLL SubPostEffect:public ProcessingPass
	{
	protected:
		core::string m_effectName;
		ParsedShaderPP* m_ppEffect;

		class RenderTargetInfo
		{
		public:
			RenderTargetInfo():texIndex(0),passID(0)
			{
			}
			EValueType type;
			int passID;
			int texIndex;
		};
		std::vector<RenderTargetInfo> m_targets;

		void ConstructRT(IRenderArea* input);
	public:
		SubPostEffect(ParsedShaderPP* owner);
		virtual~SubPostEffect();

		ParsedShaderPP* GetEffect(){return m_ppEffect;}

		virtual void Setup(const math::rectf& rc);
		virtual IRenderArea* render(IRenderArea* input);
		virtual IRenderArea* getOutput();

		virtual void Parse(xml::XMLElement*elem);
	};

	class MRAY_DLL ForwardPass:public ProcessingPass
	{
	protected:
		class RenderTargetInfo
		{
		public:
			RenderTargetInfo():texIndex(0),passID(0)
			{
			}
			EValueType type;
			int passID;
			int texIndex;
		};
		std::vector<RenderTargetInfo> m_targets;
	public:
		ForwardPass(ParsedShaderPP* owner);
		virtual~ForwardPass();

		virtual void Setup(const math::rectf& rc);
		virtual IRenderArea* render(IRenderArea* input);
		virtual IRenderArea* getOutput();

		virtual void Parse(xml::XMLElement*elem);
	};

protected:


	typedef std::map<core::string,int> ParamsMap;
	typedef std::map<core::string,int> PassIDMap;
	typedef std::vector<GCPtr<ProcessingPass>> PassList;


	std::vector<MappedParams> m_params;
	ParamsMap m_paramsIdx;
	PassIDMap m_passID;
	PassList m_passes;

	core::string m_name;

	IVideoDevice* m_device;

	ProcessingPass* m_result;

	IRenderArea* m_input;

	math::rectf m_lastVP;

public:
	ParsedShaderPP(IVideoDevice*device);
	virtual ~ParsedShaderPP();
	
	bool LoadXML(OS::IStream* stream);

	int GetPassID(const core::string&name);
	ProcessingPass *GetPass(int id);

	const std::vector<MappedParams>& GetParams()const{return m_params;}
	//retrieve mapped value (aka param)
	MappedParams* GetParam(const core::string&name);

	//retrive unmapped value from sub pass
	//value path represents the location of the value inside sub passe(s)
	PassValue* GetValue(const core::string&valuePath);

	IVideoDevice* getDevice();

	IRenderArea* getInput();

	virtual void Setup(const math::rectf& targetVP);
	virtual IRenderArea* render(IRenderArea* input);
	virtual IRenderArea* getOutput();

};

}
}


#endif // ParsedShaderPP_h__
