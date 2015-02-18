#include "stdafx.h"

#include "FunctionProfiler.h"
#include "ITimer.h"
#include "Engine.h"
#include "FunctionProfileManager.h"


namespace mray
{


	uint FunctionProfiler::IDCounter=0;

	FunctionProfiler* FunctionProfiler::currentFunction=0;


FunctionProfiler::FunctionProfiler(const std::string& name):m_name(name),m_executionTime(0),
m_enterTime(0),m_caller(0),m_subTime(0),m_level(0),m_totalExecutionTime(0)
{
	m_id=IDCounter++;
}

uint FunctionProfiler::GetID()const{return m_id;}
const std::string& FunctionProfiler::GetName()const {return m_name;}
void FunctionProfiler::OnEnter()
{
	m_subTime=0;
	m_enterTime=gEngine.getTimer()->getSeconds();
	m_caller=currentFunction;
	if(m_caller)
		m_level=m_caller->m_level+1;
	else
		m_level=0;
	currentFunction=this;

	FunctionProfileManager::getInstance().BeginProfiling(this);
}
void FunctionProfiler::OnExit()
{
	m_executionTime=gEngine.getTimer()->getSeconds()-m_enterTime;
	m_totalExecutionTime+=m_executionTime;
	if(m_caller)
		m_caller->m_subTime+=m_executionTime;
	currentFunction=m_caller;

	FunctionProfileManager::getInstance().EndProfiling(this);
/*
	for(int i=0;i<m_level;++i)
		printf("\t");
	printf("[%s]=%f/%f\n",m_name.c_str(),m_executionTime*0.001f,m_subTime*0.001f);*/
}
double FunctionProfiler::GetLastExecutionTime()const{return m_executionTime;}
double FunctionProfiler::GetTotalExecutionTime()const{return m_executionTime;}
double FunctionProfiler::GetSubExecutionTime()const{return m_subTime;}


}
