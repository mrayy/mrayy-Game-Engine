
/********************************************************************
	created:	2009/04/03
	created:	3:4:2009   22:33
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\src\IDManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject\src
	file base:	IDManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IDManager___
#define ___IDManager___

#include <ISingleton.h>

namespace mray{

class IDManager:public ISingleton<IDManager>
{
private:
protected:
	uint m_id;
public:
	IDManager();
	virtual~IDManager();

	uint getID();
	void freeID(uint id);
};

}


#endif //___IDManager___
