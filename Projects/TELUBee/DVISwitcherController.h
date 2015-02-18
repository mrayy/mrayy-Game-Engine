


/********************************************************************
	created:	2013/02/18
	created:	18:2:2013   14:35
	filename: 	C:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5\DVISwitcherController.h
	file path:	C:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5
	file base:	DVISwitcherController
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __DVISwitcherController__
#define __DVISwitcherController__




namespace mray
{
	namespace OS
	{
		class ISerialPort;
	}
namespace TBee
{

	enum EDVIState
	{
		Channel1,
		Channel2,
		Unkown
	};
class DVISwitcherController
{
protected:
	OS::ISerialPort* m_sp;

	EDVIState readState();
public:
	DVISwitcherController();
	virtual~DVISwitcherController();

	bool Open(const core::string& port);

	bool SwitchToSource1();
	bool SwitchToSource2();
	EDVIState CurrentSource();
};

}
}


#endif
