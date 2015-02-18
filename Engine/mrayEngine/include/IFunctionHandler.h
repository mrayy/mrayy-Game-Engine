
/********************************************************************
	created:	2009/06/26
	created:	26:6:2009   14:14
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IFunctionHandler.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IFunctionHandler
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IFunctionHandler___
#define ___IFunctionHandler___


#include "CmdParams.h"

namespace mray{
namespace script{

#define USER_DATA void*

class IFunctionHandler
{
private:
protected:
public:

	virtual int getParamsCount()=0;

	virtual void getParams(CmdParams&p,bool autoType=false)=0;

	virtual bool	toBool(int idx,bool def=0)=0;
	virtual int		toInteger(int idx,int def=0)=0;
	virtual float	toFloat(int idx,float def=0)=0;
	virtual const char* toString(int idx,const char* def="")=0;
	virtual USER_DATA	toUserData(int idx,USER_DATA def=0)=0;

	virtual bool getParam(int idx,bool&v)=0;
	virtual bool getParam(int idx,int&v)=0;
	virtual bool getParam(int idx,float&v)=0;
	virtual bool getParam(int idx,const char* &v)=0;
	virtual bool getParam(int idx,USER_DATA &v)=0;

	virtual void* getAttachedObject(const char*type)=0;
	virtual int getAttachedFunctionID()=0;

	virtual void pushParam(bool v)=0;
	virtual void pushParam(int v)=0;
	virtual void pushParam(float v)=0;
	virtual void pushParam(const char* v)=0;
	virtual void pushParam(USER_DATA v)=0;
	virtual void pushParam(void* obj,const char*type)=0;

	virtual Param::EParamType getParamType(int idx)=0;

	virtual void beginCall(const core::stringc&name)=0;
	virtual int endFunctionCall(int nArgs,int nResults=0)=0;
	virtual int endCoroutineCall(int nArgs)=0;

};

}
}

#endif //___IFunctionHandler___
