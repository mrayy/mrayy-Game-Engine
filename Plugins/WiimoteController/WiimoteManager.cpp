
#include "stdafx.h"

#include "WiimoteManager.h"
#include "wiimote.h"
#include "EventQueue.h"
#include "WiiEvent.h"
#include "EventMemoryManager.h"

#include "IThreadFunction.h"
#include "IThreadManager.h"
#include "IThread.h"
#include "IMutex.h"
#include "CWiimote.h"


namespace mray
{
namespace controllers
{


	class WiimoteManagerImpl
	{
		WiimoteManager* m_owner;
		OS::IMutex* m_eventMutex;
		bool m_eventHappened;
		OS::IThreadFunction* m_threadFunction;
		OS::IThread* m_thread;

		std::vector<CWiimote*> m_wiimotes;

		struct wiievent
		{

			EWiiEventType event;

			controllers::CWiimote* mote;
		};
		std::list<wiievent> m_events;
	public:
		WiimoteManagerImpl(WiimoteManager* owner);
		~WiimoteManagerImpl();
		void _PollEvents();

		void RefreshWiimotes();

		CWiimote* GetByID(int ID);
		std::vector<CWiimote*>& GetWiimotes();


		int Find(int timeout);
		std::vector<CWiimote*>& ConnectWithAllMotes();
		CWiimote* Connect(int i);

		void DisconnectWithAllMotes();
		void Disconnect(int i);

		void PollEvents();
};

	class WiimoteManagerPollThread :public OS::IThreadFunction
	{
		WiimoteManagerImpl* m_manager;
	public:
		WiimoteManagerPollThread(WiimoteManagerImpl* m)
		{
			m_manager = m;
		}
		virtual void setup()
		{
		}
		virtual void execute(OS::IThread*caller, void*arg)
		{
			while (caller->isActive())
			{
				m_manager->_PollEvents();
				OS::IThreadManager::getInstance().sleep(10);//sleep for 10 ms
			}
		}
	};

WiimoteManagerImpl::WiimoteManagerImpl(WiimoteManager* owner) :m_eventHappened(false), m_owner(owner)
{

	m_eventMutex=OS::IThreadManager::getInstance().createMutex();
	m_threadFunction=new WiimoteManagerPollThread(this);

	m_thread=OS::IThreadManager::getInstance().createThread(m_threadFunction);
	m_thread->start(0);
}
WiimoteManagerImpl::~WiimoteManagerImpl()
{
	OS::IThreadManager::getInstance().killThread(m_thread);
	delete m_thread;
	delete m_threadFunction;
	delete m_eventMutex;

	DisconnectWithAllMotes();
}


void WiimoteManagerImpl::RefreshWiimotes()
{
}

CWiimote* WiimoteManagerImpl::GetByID(int ID)
{
	if (ID < 0 || ID >= m_wiimotes.size())
		return 0;
	return m_wiimotes[ID];
}
std::vector<CWiimote*>& WiimoteManagerImpl::GetWiimotes()
{
	return m_wiimotes;
}

std::vector<CWiimote*>& WiimoteManagerImpl::ConnectWithAllMotes()
{
	DisconnectWithAllMotes();
	while (true)
	{
		CWiimote* mote = new CWiimote();
		if (!mote->Connect())
		{
			delete mote;
			break;;
		}
		m_wiimotes.push_back(mote);
	}
	return m_wiimotes;
}
CWiimote* WiimoteManagerImpl::Connect(int ID)
{
	if (ID < 0 || ID >= m_wiimotes.size())
		return 0;
	if (!m_wiimotes[ID]->IsConnected())
		m_wiimotes[ID]->Connect(ID);
	return m_wiimotes[ID];
}

void WiimoteManagerImpl::DisconnectWithAllMotes()
{
	for (int i = 0; i < m_wiimotes.size();++i)
	{
		m_wiimotes[i]->Disconnect();
	}
	m_wiimotes.clear();
}
void WiimoteManagerImpl::Disconnect(int ID)
{
	if (ID < 0 || ID >= m_wiimotes.size())
		return;
	m_wiimotes[ID]->Disconnect();
}

void WiimoteManagerImpl::PollEvents()
{
	if (m_eventMutex->tryLock()){
		std::list<wiievent>::iterator it = m_events.begin();
		for (; it != m_events.end();++it)
		{
			WiiEvent* evt = (WiiEvent*)EventMemoryManager::getInstance().createEvent(WiiEvent::EventID).pointer();
			if (!evt)
				evt = new WiiEvent();
			evt->event = it->event;
			evt->mote = it->mote;
			EventQueue::getInstance().pushEvent(evt);
		}
		m_events.clear();
		m_eventMutex->unlock();
	}

}

void WiimoteManagerImpl::_PollEvents()
{

	m_eventMutex->lock();
	for (int i = 0; i < m_wiimotes.size(); ++i)
	{
		CWiimote*w = m_wiimotes[i];
		if (!w)continue;

		state_change_flags flags = w->RefreshState();
		EWiiEventType type = EWiiEvent_Unkown;
		if (flags & CONNECTED)type = EWiiEvent_Connect;
		else if (flags & CONNECTION_LOST)type = EWiiEvent_Disconnect;
		else if (flags & BATTERY_CHANGED)type = EWiiEvent_BatteryChanged;
		else if (flags & BATTERY_DRAINED)type = EWiiEvent_BatteryDrained;
		else if (flags & BUTTONS_CHANGED)type = EWiiEvent_ButtonChanged;
		else if (flags & ACCEL_CHANGED)type = EWiiEvent_AccelChanged;
		else if (flags & ORIENTATION_CHANGED)type = EWiiEvent_OrintationChanged;
		else if (flags & IR_CHANGED)type = EWiiEvent_IRChanged;
		else if (flags & NUNCHUK_CONNECTED)type = EWiiEvent_NunchukInserted;
		else if (flags & BALANCE_CONNECTED)type = EWiiEvent_BalanceConnected;
		else if (flags & BALANCE_WEIGHT_CHANGED)type = EWiiEvent_BalanceWeightChanged;
		else
			continue;
		wiievent e;
		e.event = type;
		e.mote = w;
		m_events.push_back(e);
	}
	m_eventMutex->unlock();
}

WiimoteManager::WiimoteManager()
{
	m_impl = new WiimoteManagerImpl(this);
}
WiimoteManager::~WiimoteManager()
{
	delete m_impl;
}

void WiimoteManager::RefreshWiimotes()
{
	m_impl->RefreshWiimotes();
}

CWiimote* WiimoteManager::GetByID(int ID)
{
	return m_impl->GetByID(ID);
}
std::vector<CWiimote*>& WiimoteManager::GetWiimotes()
{
	return m_impl->GetWiimotes();
}


std::vector<CWiimote*>& WiimoteManager::ConnectWithAllMotes()
{
	return m_impl->ConnectWithAllMotes();
}
CWiimote* WiimoteManager::Connect(int i)
{
	return m_impl->Connect(i);
}

void WiimoteManager::DisconnectWithAllMotes()
{
	m_impl->DisconnectWithAllMotes();
}
void WiimoteManager::Disconnect(int i)
{
	m_impl->Disconnect(i);
}

void WiimoteManager::PollEvents()
{
	m_impl->PollEvents();
}



}
}