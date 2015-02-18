
/********************************************************************
	created:	2009/05/30
	created:	30:5:2009   17:19
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IObjectFactory.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IObjectFactory
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ObjectFactory___
#define ___ObjectFactory___

#include "mstring.h"

namespace mray{

template <class T>
class ObjectFactory
{
private:
protected:
public:
	ObjectFactory(){}
	virtual~ObjectFactory(){}

	virtual const core::string& GetType()=0;
	virtual T* CreateObject()=0;

};

}


#endif //___IObjectFactory___
