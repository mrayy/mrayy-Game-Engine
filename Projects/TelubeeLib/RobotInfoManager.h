

/********************************************************************
	created:	2013/01/29
	created:	29:1:2013   14:32
	filename: 	C:\Development\mrayEngine\Projects\TELUBee\RobotInfoManager.h
	file path:	C:\Development\mrayEngine\Projects\TELUBee
	file base:	RobotInfoManager
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __RobotInfoManager__
#define __RobotInfoManager__


#include "TBRobotInfo.h"
#include "ListenerContainer.h"


namespace mray
{
namespace TBee
{
	class RobotInfoManager;

	class IRobotInfoManagerListener
	{
	public:
		virtual void OnRobotAdded(RobotInfoManager* mngr,const TBRobotInfo& ifo){};
		virtual void OnRobotStatusUpdated(RobotInfoManager* mngr,const TBRobotInfo& ifo){};
	};

	class RobotInfoManagerImpl;

class RobotInfoManager:public ListenerContainer<IRobotInfoManagerListener*>
{
protected:
	RobotInfoManagerImpl* m_impl;
protected:
	DECLARE_FIRE_METHOD(OnRobotAdded,(RobotInfoManager* mngr,const TBRobotInfo& ifo),(mngr,ifo));
	DECLARE_FIRE_METHOD(OnRobotStatusUpdated,(RobotInfoManager* mngr,const TBRobotInfo& ifo),(mngr,ifo));

public:

	RobotInfoManager();
	virtual~RobotInfoManager();

	void AddRobotInfo(const TBRobotInfo& ifo);
	TBRobotInfo* GetRobotInfo(int index);
	TBRobotInfo* GetRobotInfoByID(int id);
	const std::vector<TBRobotInfo>& GetRobots();
	void ClearRobots();

	void LoadRobots(const core::string& path);
};

}
}


#endif
