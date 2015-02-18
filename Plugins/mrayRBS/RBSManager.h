

/********************************************************************
	created:	2010/04/07
	created:	7:4:2010   15:45
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayRBS\RBSManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayRBS
	file base:	RBSManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef RBSManager_h__
#define RBSManager_h__

#include "ISingleton.h"
#include <map>

namespace mray
{
namespace AI
{
class RuleBasedSystem;

class RBSManager:public ISingleton<RBSManager>
{
private:
protected:
	typedef std::map<core::string,RuleBasedSystem*> RBSMap;
	RBSMap m_rbs;
public:
	RBSManager();
	virtual ~RBSManager();
	
	void AddRBS(RuleBasedSystem*rbs);
	RuleBasedSystem* GetRBS(const core::string&name);
	void RemoveRBS(const core::string&name);

	void ClearRBSs();
};

}
}

#endif // RBSManager_h__
