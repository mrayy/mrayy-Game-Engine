#include "stdafx.h"




#include "CMultiPassPP.h"
#include "IRenderTarget.h"
#include "IVideoDevice.h"


namespace mray{
namespace video{

CMultiPassPP::CMultiPassPP(video::IVideoDevice*dev)
	:m_output(0),device(dev)
{

}
CMultiPassPP::~CMultiPassPP(){
	clear();
}

void CMultiPassPP::Setup(const math::rectf& targetVP)
{
	std::list<IPostProcessingPtr>::iterator it=m_postProcessors.begin();
	for(;it!=m_postProcessors.end();++it){
		(*it)->Setup(targetVP);
	}

}
IRenderArea* CMultiPassPP::render(IRenderArea* input){
	if(!isEnabled())
		return input;
	std::list<IPostProcessingPtr>::iterator it=m_postProcessors.begin();
	m_output=input;
	for(;it!=m_postProcessors.end();++it){
		m_output=(*it)->render(m_output);
	}

	if(!m_postProcessors.size())
		device->setRenderTarget(0,0,0);

	return m_output;

}
IRenderArea* CMultiPassPP::getOutput(){
	return m_output;
}

void CMultiPassPP::addPostProcessor(const IPostProcessingPtr& pp){
	m_postProcessors.push_back(pp);
}

bool CMultiPassPP::removePostProcessor(const IPostProcessingPtr& pp){
	std::list<IPostProcessingPtr>::iterator it=m_postProcessors.begin();
	for(;it!=m_postProcessors.end();++it){
		if((*it)==pp){
			m_postProcessors.erase(it);
			return 1;
		}
	}
	return 0;
}

int CMultiPassPP::getPostProcessorCount(){
	return m_postProcessors.size();
}

void CMultiPassPP::clear(){
	m_postProcessors.clear();
}

}
}
