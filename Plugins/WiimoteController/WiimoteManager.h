


/********************************************************************
	created:	2011/01/28
	created:	28:1:2011   22:22
	filename: 	i:\Programing\GameEngine\mrayEngine\Controllers\WiimoteController\WiimoteManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\Controllers\WiimoteController
	file base:	WiimoteManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef WiimoteManager_h__
#define WiimoteManager_h__

#include "ISingleton.h"
#include <vector>
#include "CMutexVar.h"


namespace mray
{
	namespace OS
	{
		class IThread;
		class IThreadFunction;
		class IMutex;
	}
namespace controllers
{
	class CWiimote;
	class WiimoteManagerImpl;
class WiimoteManager:public ISingleton<WiimoteManager>
{
private:
protected:
	WiimoteManagerImpl* m_impl;
public:
	WiimoteManager();
	virtual ~WiimoteManager();

	void RefreshWiimotes();

	CWiimote* GetByID(int ID);
	std::vector<CWiimote*>& GetWiimotes();


	std::vector<CWiimote*>& ConnectWithAllMotes();
	CWiimote* Connect(int i);

	void DisconnectWithAllMotes();
	void Disconnect(int i);

	void PollEvents();

};

}
}
#endif // WiimoteManager_h__
