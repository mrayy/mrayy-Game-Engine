#include "stdafx.h"

#include "GPUUniform.h"
#include "StringConverter.h"
#include "ShaderUniformTypes.h"


namespace mray{
namespace video{

std::vector<GPUUniform::ShaderSemanticDef> GPUUniform::m_userShaderSemantics;


bool GPUUniform::matchWithSemantic(const core::stringc&v,const core::stringc&semantic,int& outIdx)
{
	outIdx=-1;
	int l=semantic.length();
	if(v.length()<l)
		return false;
	int diffLen=v.length()-l;
	core::stringc str=v.substr(0,l);
	core::stringc idxStr=v.substr(l,diffLen);

	core::string idxStrW;
	core::char_to_string(idxStr.c_str(),idxStrW);
	if(str.equals_ignore_case(semantic)){
		//check if "idxStr" is number
		if(idxStr.length()>0){
			int idx=core::StringConverter::toInt(idxStrW);
			if(core::StringConverter::toString(idx)!=idxStrW)
				return false;
			outIdx=idx;
		}
		return true;
	}
	return false;
}
void GPUUniform::setSemanticByString(const core::stringc& s){
	int idx;
	for(int i=0;i<ESS_Count;++i)
	{
		const UniformSettings* us=UniformSemanticToString((EShaderSemantic)i);
		if(matchWithSemantic(s,us->name,idx))
		{
			index=idx;
			semantic=i;
			perObject=us->perObject;
			return;
		}
	}
	//then try to search in user semantics
	for (int i=0;i<m_userShaderSemantics.size();++i)
	{
		if(matchWithSemantic(s,m_userShaderSemantics[i].semantic,idx))
		{
			index=idx;
			semantic=m_userShaderSemantics[i].id;
			perObject=m_userShaderSemantics[i].perObject;
			return;
		}
	}
	semantic=ESS_Unkown;
	name=s;
}

void GPUUniform::registerUserSemantic(const core::stringc& semantic,int id,bool perObj){
	ShaderSemanticDef s;
	s.id=id;
	s.semantic=semantic;
	s.perObject=perObj;
	m_userShaderSemantics.push_back(s);
}

}
}

