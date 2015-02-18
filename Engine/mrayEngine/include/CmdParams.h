


/********************************************************************
	created:	2009/03/01
	created:	1:3:2009   23:48
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\CmdParams.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	CmdParams
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___CmdParams___
#define ___CmdParams___

#include "Param.h"

namespace mray{

class CmdParams
{
private:
	static const int MaxParams=16;
	Param m_params[MaxParams];
	int m_paramsCount;
public:
	CmdParams(){
		m_paramsCount=0;
	}
	virtual~CmdParams(){
	}

	CmdParams(const char*types){
		m_paramsCount=0;
		int len=strlen(types);
		if(len>MaxParams)
			len=MaxParams;
		for(int i=0;i<len;i++){
			addParam(types[i]);
		}
	}

	void setUsed(int cnt){
		m_paramsCount=cnt;
		if(m_paramsCount>MaxParams)
			m_paramsCount=MaxParams;
		for(int i=0;i<m_paramsCount;i++){
			m_params[i].setNull();
		}
	}

	Param* operator[] (int idx){return getParam(idx);}
	const Param* operator[] (int idx)const {return getParam(idx);}

	Param* addParam(char type){
		if(m_paramsCount>=MaxParams)
			return 0;
		m_params[m_paramsCount].setType(type);
		m_paramsCount++;
		return &m_params[m_paramsCount-1];
	}
	Param* addParam(const Param&p){
		if(m_paramsCount>=MaxParams)
			return 0;
		m_params[m_paramsCount]=p;
		m_paramsCount++;
		return &m_params[m_paramsCount-1];
	}
	void clear(){
		m_paramsCount=0;
	}

	int getNumberOfParams()const{
		return m_paramsCount;
	}
	Param*getParam(int index){
		if(index>=MaxParams)
			return 0;
		return &m_params[index];
	}
	const Param*getParam(int index)const {
		if(index>=MaxParams)
			return 0;
		return &m_params[index];
	}

};

}


#endif //___CmdParams___
