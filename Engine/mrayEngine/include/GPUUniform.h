
/********************************************************************
created:	2011/04/28
created:	28:4:2011   22:29
filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\GPUUniform.h
file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
file base:	GPUUniform
file ext:	h
author:		MHD Yamen Saraiji

purpose:	
*********************************************************************/



#ifndef __GPUUniform__
#define __GPUUniform__

#include "videoCommon.h"


namespace mray
{
namespace video
{
	class ITexture;
	class IVideoDevice;



	enum EUniformType
	{
		EUT_Int1,
		EUT_Int2,
		EUT_Int3,
		EUT_Int4,
		EUT_Float1,
		EUT_Float2,
		EUT_Float3,
		EUT_Float4,
		EUT_Sampler1D,
		EUT_Sampler2D,
		EUT_Sampler3D,
		EUT_SamplerCube,
		EUT_Matrix2x2,
		EUT_Matrix2x3,
		EUT_Matrix2x4,
		EUT_Matrix3x3,
		EUT_Matrix3x4,
		EUT_Matrix4x2,
		EUT_Matrix4x3,
		EUT_Matrix4x4,
		EUT_Array,
		EUT_Unkown
	};


class MRAY_DLL GPUUniform{
private:
	struct ShaderSemanticDef{
		core::stringc semantic;
		int id;
		bool perObject;
	};

	static std::vector<ShaderSemanticDef> m_userShaderSemantics;

	bool matchWithSemantic(const core::stringc&v,const core::stringc&semantic,int& outIdx);

public:
	EUniformType type;
	core::stringc name;
	int index;			//used for some attrs like an index (Tex0,Tex1,...)
	size_t elemSize;
	size_t arraySize;
	size_t startIndex;

	bool perObject;//this flag indicates that this uniform is special for each object (like: WorldMatrix)

	int semantic;

	static void registerUserSemantic(const core::stringc& semantic,int id,bool peerObj);

	GPUUniform(){
		semantic=0;
		perObject=true;
		index=-1;
	}
	virtual~GPUUniform(){}

	void setSemantic(int s){
		semantic=s;
	}
	void setSemanticByString(const core::stringc& s);

	bool operator <(const GPUUniform&o)const{
		return name<o.name;
	}

	bool isInt(){
		switch(type){
	case EUT_Int1:
	case EUT_Int2:
	case EUT_Int3:
	case EUT_Int4:
		return true;
		}
		return false;
	}
	bool isFloat(){
		switch(type){
	case EUT_Float1:
	case EUT_Float2:
	case EUT_Float3:
	case EUT_Float4:
		return true;
		}
		return false;
	}
	bool isSampler(){
		switch(type){
	case EUT_Sampler1D:
	case EUT_Sampler2D:
	case EUT_Sampler3D:
	case EUT_SamplerCube:
		return true;
		}
		return false;
	}
	bool isMatrix(){
		switch(type){
	case EUT_Matrix2x2:
	case EUT_Matrix3x3:
	case EUT_Matrix4x4:
		return true;
		}
		return false;
	}

};

typedef std::map<core::stringc,GPUUniform*> UniformList;
typedef UniformList::iterator UniformListIT;

}

}

#endif
