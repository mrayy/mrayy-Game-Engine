
/********************************************************************
	created:	2009/03/29
	created:	29:3:2009   18:20
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ILogManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ILogManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ILogManager___
#define ___ILogManager___

#include "ISingleton.h"
#include "mstring.h"
#include "ILogDevice.h"

namespace mray{

	enum ELogLevel{
		ELL_INFO,
		ELL_SUCCESS,
		ELL_WARNING,
		ELL_ERROR
	};
	enum EVerbosLevel
	{
		EVL_None,
		EVL_Normal,
		EVL_Heavy
	};


class MRAY_CORE_DLL  ILogManager:public ISingleton<ILogManager>
{
private:
protected:
	virtual void Output(const core::string& v) = 0;
public:
	ILogManager(){}
	virtual~ILogManager(){}

	virtual void setVerbosLevel(EVerbosLevel vl)=0;
	virtual EVerbosLevel GetVerbosLevel()=0;

	virtual void log(const core::string &msg,const core::string &info,ELogLevel level,EVerbosLevel vl=EVL_Normal)=0;
	virtual void log(const core::string &msg,ELogLevel level,EVerbosLevel vl=EVL_Normal)=0;

	virtual void startSection(const core::string &name)=0;
	virtual void endSection(bool Success)=0;

	virtual void close()=0;

	virtual void addLogDevice(const ILogDevicePtr&logger)=0;
	virtual void removeLogDevice(const ILogDevicePtr&logger)=0;

	virtual void flush()=0;


	virtual ILogManager& StartLog(ELogLevel level)
	{
		return *this;
	}

	//make sure to call StartLog before stream log your output
	template <class  T>
	ILogManager& operator << (const T& v)
	{
		Output(core::StringConverter::toString(v));
		return *this;
	}

	static const mchar* getLogLevelStr(ELogLevel l);
};

}
#define gLogManager mray::ILogManager::getInstance()


#endif //___ILogManager___

