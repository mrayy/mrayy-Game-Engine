

#include "stdafx.h"
#include "WinFileMonitor.h"
#include "IThreadFunction.h"
#include "IThreadManager.h"
#include "WinMonitorFileInformation.h"
#include <windows.h>

namespace mray
{
namespace OS
{
class FileMonitorThreadFunction :public IThreadFunction
{
	WinFileMonitor* m_owner;
public:
	FileMonitorThreadFunction(WinFileMonitor* o)
	{
		m_owner = o;
	}
	virtual void execute(IThread*caller, void*arg)
	{
		while (caller->isActive())
		{
			bool             bStop = FALSE;
			void*           hDir = NULL;

			hDir = FindFirstChangeNotification(m_owner->GetDirectory().c_str(), true,
				FILE_NOTIFY_CHANGE_FILE_NAME |
				FILE_NOTIFY_CHANGE_DIR_NAME |
				FILE_NOTIFY_CHANGE_SIZE |
				FILE_NOTIFY_CHANGE_LAST_WRITE |
				FILE_NOTIFY_CHANGE_ATTRIBUTES);

			if (hDir == INVALID_HANDLE_VALUE)
			{
				m_owner->_FailedCreateThread();
				return;
			}
			MonitorFileList oldFL,newFL;
			while (m_owner->IsMonitoring() && caller->isActive())
			{
				WinMonitorFileInformation::ClearFiles(oldFL);
				WinMonitorFileInformation::EnumFiles(m_owner->GetDirectory(), oldFL);

				bStop = false;
				while (WaitForSingleObject(hDir, WAIT_TIMEOUT) != WAIT_OBJECT_0)
				{
					if (!m_owner->IsMonitoring() || !caller->isActive())
					{
						bStop = true;
						break;;
					}
				}
				if (bStop)
					break;

				WinMonitorFileInformation::ClearFiles(newFL);
				WinMonitorFileInformation::EnumFiles(m_owner->GetDirectory(), newFL);
				Sleep(WAIT_TIMEOUT);

				IMonitorFileInformation* f=0;
				EMonitorAction action = WinMonitorFileInformation::Compare(oldFL, newFL, f);
				if (action != EMonitorAction::None)
				{
					m_owner->__FIRE_OnMonitorFiles(m_owner, f, action);
				}

				if (FindNextChangeNotification(hDir) == 0)
				{
					break;
				}
			}
		}
	}
};

WinFileMonitor::WinFileMonitor()
{
	m_thread = 0;
	m_running = false;
}

WinFileMonitor::~WinFileMonitor()
{
	StopMonitoring();
}


bool WinFileMonitor::StartMonitoring()
{
	if (IsMonitoring() || m_thread || m_directory=="")
		return false;
	m_running = true;

	FileMonitorThreadFunction* f = new FileMonitorThreadFunction(this);
	m_thread = IThreadManager::getInstance().createThread(f);
	m_thread->start(0);

	return true;
}

void WinFileMonitor::StopMonitoring()
{
	if (!IsMonitoring() || !m_thread)
		return;

	m_running = false;
	m_thread->terminate();
	IThreadManager::getInstance().killThread(m_thread);
	m_thread = 0;
}

bool WinFileMonitor::IsMonitoring()
{
	return m_running;
}


void WinFileMonitor::SetDirectory(const core::string& path)
{
	m_directory = path;
}

const core::string& WinFileMonitor::GetDirectory()
{
	return m_directory;
}

void WinFileMonitor::_FailedCreateThread()
{
	StopMonitoring();
}

}
}

