

/********************************************************************
	created:	2009/04/13
	created:	13:4:2009   0:27
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IResLoadListener.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IResLoadListener
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IResLoadListener___
#define ___IResLoadListener___

#include "mstring.h"

namespace mray{

class IResLoadListener
{
private:
protected:
public:
	IResLoadListener(){}
	virtual~IResLoadListener(){}

	virtual void beginLoadResources(const core::string& category,int count)=0;

	virtual void onResourceLoadStart(const core::string&name)=0;
	virtual void onResourceLoadDone(const core::string&name)=0;

	// called when there is an info need to be sayed
	virtual void resourceInfo(const core::string&info)=0;

	virtual void loadDone()=0;
};

}


#endif //___IResLoadListener___