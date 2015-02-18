
/********************************************************************
	created:	2009/02/27
	created:	27:2:2009   23:14
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IApplication.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IApplication
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IApplication___
#define ___IApplication___

#include "OptionContainer.h"

namespace mray{

class IApplication
{
public:
	IApplication(){}
	virtual ~IApplication(){};

	virtual void init(const OptionContainer &extraOptions)=0;
	virtual void run()=0;
	virtual void update(float dt)=0;

	virtual void terminate()=0;

	virtual void onDone()=0;
};

}


#endif //___IApplication___
