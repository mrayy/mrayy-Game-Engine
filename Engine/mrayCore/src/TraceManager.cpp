#include "stdafx.h"


#include "TraceManager.h"
#include "macros.h"
#include "ILogManager.h"
#include "ITimer.h"
#include "StringConverter.h"



namespace mray{


	
TraceManager::TraceManager(){
}
TraceManager::~TraceManager(){
	while(!m_functions.empty())
	{
		m_functions.pop();
	}
}

void TraceManager::pushFunction(TraceFunctionProfile*f){
	m_functions.push(f);

}
void TraceManager::popFunction(TraceFunctionProfile*f){
	FATAL_ERROR(m_functions.empty(),mT("No Function to pop!"));
//	FATAL_ERROR(m_functions.top()!=name,"function name not matched!");
	m_functions.pop();
	
}

void TraceManager::flushToLogger(){
	FuctionsList::container_type::const_iterator it= m_functions._Get_container().begin();
	gLogManager.log(mT("**************    Trace    **************"),ELL_INFO);
	int i=0;
	core::string str;
	for(;it!=m_functions._Get_container().end();++it,i++){
		str=core::StringConverter::toString(i);
		str+=mT(" [");
		str+=(*it)->m_catagory;
		str+=mT("] ");
		str+=mT(" -> ");
		str+=core::StringConverter::toString((*it)->m_name.c_str());
		gLogManager.log(mT("trace "),str.c_str(),ELL_INFO);
	}
	gLogManager.log(mT("************** End of Trace **************"),ELL_INFO);

}

TraceFunctionProfile::TraceFunctionProfile(const char*name,const mchar*catagory,bool timeStumb){
	m_name=name;
	m_catagory=catagory;
	m_timeStumb=timeStumb;
	m_startTime=0;
	m_endTime=0;
}
void TraceFunctionProfile::enterFunction(){
	if(m_timeStumb){
		m_startTime = 0;//gEngine.getTimer()->getSeconds();
	}
	TraceManager::getInstance().pushFunction(this);
}
void TraceFunctionProfile::exitFunction(){
	if(m_timeStumb){
		m_endTime = 0;// gEngine.getTimer()->getSeconds();
	}
	TraceManager::getInstance().popFunction(this);

}

TraceFunction::TraceFunction(TraceFunctionProfile*tf){
	m_tf=tf;
	m_exit=false;
	if(m_tf)
		m_tf->enterFunction();
}
TraceFunction::~TraceFunction(){
	if(m_tf && !m_exit)
		m_tf->exitFunction();

}

void TraceFunction::exit(){
	if( !m_exit && m_tf){
		m_tf->exitFunction();
		m_exit=true;
	}

}
}

