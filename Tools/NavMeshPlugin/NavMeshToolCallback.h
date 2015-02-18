
/********************************************************************
	created:	2010/03/04
	created:	4:3:2010   14:16
	filename: 	i:\Programing\GameEngine\mrayEngine\Tools\NavMeshPlugin\NavMeshToolCallback.h
	file path:	i:\Programing\GameEngine\mrayEngine\Tools\NavMeshPlugin
	file base:	NavMeshToolCallback
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___NavMeshToolCallback___
#define ___NavMeshToolCallback___

#include <IEventReciver.h>

namespace mray{

class NavMeshToolCallback:public IEventReciver
{
private:
protected:
	int m_NavMeshBtnId;
public:
	NavMeshToolCallback();
	virtual~NavMeshToolCallback();

	virtual void onEvent(GCPtr<Event> event);

	void setNavMeshBtnID(int id){m_NavMeshBtnId=id;}
};

}


#endif //___NavMeshToolCallback___
