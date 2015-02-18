#include "stdafx.h"




#include "CShaderFactory.h"
#include "GPUShader.h"
#include "IShaderConstantsCallback.h"





namespace mray{
namespace video{


CShaderFactory::CShaderFactory()
{
}


CShaderFactory::~CShaderFactory(){
	clear();
}

void CShaderFactory::addFactory(IGPUShaderFactory*factory){
	m_factories[factory->getType()]=factory;
}

IGPUShaderFactory*CShaderFactory::getFactory(const core::string&type){
	std::map<core::string,IGPUShaderFactory*>::iterator it=m_factories.find(type);
	if(it==m_factories.end())return 0;
	return it->second;
}

IGPUShaderProgram* CShaderFactory::createShader(const core::string&type,EShaderProgramType programType)
{
	IGPUShaderFactory*f=getFactory(type);
	if(!f)return 0;
	return f->createShader(programType);
}


void CShaderFactory::clear(){
	std::map<core::string,IGPUShaderFactory*>::iterator it=m_factories.begin();
	for(;it!=m_factories.end();++it)
		delete it->second;
	m_factories.clear();
}

	
}
}



