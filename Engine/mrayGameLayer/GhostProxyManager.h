

/********************************************************************
	created:	2013/01/21
	created:	21:1:2013   13:40
	filename: 	C:\Development\mrayEngine\Engine\mrayGameLayer\GhostProxyManager.h
	file path:	C:\Development\mrayEngine\Engine\mrayGameLayer
	file base:	GhostProxyManager
	file ext:	cpp
	author:		MHD Yamen Saraiji
	
	purpose:	Ghost Manager is used to isolate physics processing from rendering. Thus it would be possible to let physics run on dedicated pc, and share results with others
*********************************************************************/

#ifndef __GhostProxyManager__
#define __GhostProxyManager__



#include "compileConfig.h"
#include "ListenerContainer.h"
#include "mstring.h"

namespace mray
{
	namespace physics
	{
		class IPhysicalNode;
	}
	namespace network
	{
		class NetAddress;
	}
	namespace scene
	{
		class IMovable;
	}
namespace game
{

	class GhostProxyManager;
	class IGhostProxyListener
	{
	public:

		virtual void OnConnected(GhostProxyManager* mngr, const network::NetAddress& addr){}
		virtual void OnDisconnected(GhostProxyManager* mngr){}

		virtual void OnObjectAttached(GhostProxyManager* mngr, uint ID,scene::IMovable* obj){};
		virtual void OnObjectRemoved(GhostProxyManager* mngr, uint ID,scene::IMovable* obj){};

		virtual void OnMasterDataArrive(GhostProxyManager* mngr,uint ID, const math::vector3d& pos,const math::quaternion& ori){}
	};

	class GhostProxyManagerImpl;
class MRAY_GAME_DLL GhostProxyManager:public ListenerContainer<IGhostProxyListener*>
{
protected:
	DECLARE_FIRE_METHOD(OnConnected,(GhostProxyManager* mngr, const network::NetAddress& addr),(mngr,addr));
	DECLARE_FIRE_METHOD(OnDisconnected,(GhostProxyManager* mngr),(mngr));
	DECLARE_FIRE_METHOD(OnObjectAttached,(GhostProxyManager* mngr, uint ID,scene::IMovable* obj),(mngr,ID,obj));
	DECLARE_FIRE_METHOD(OnObjectRemoved,(GhostProxyManager* mngr, uint ID,scene::IMovable* obj),(mngr,ID,obj));
	DECLARE_FIRE_METHOD(OnMasterDataArrive,(GhostProxyManager* mngr, uint ID,const math::vector3d& pos,const math::quaternion& ori),(mngr,ID,pos,ori));

	GhostProxyManagerImpl* m_impl;

public:
	GhostProxyManager();
	virtual~GhostProxyManager();

	void OpenPort(int port);
	void Connect(const network::NetAddress& addr);

	void Connect(const core::string& ip,int port);
	void Close();

	bool IsConnected();

	bool IsMaster();

	void AddMasterObject(uint ID,physics::IPhysicalNode* node);
	void RemoveMasterObject(uint ID);

	void AttachGhostObject(uint ID,scene::IMovable* node);
	void RemoveGhostObject(scene::IMovable* node);

	void Clear();

	void Update();
};

}
}


#endif

