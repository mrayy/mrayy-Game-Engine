

#include "stdafx.h"
#include "DVISwitcherController.h"

#include "OFSerialPort.h"


namespace mray
{
namespace VT
{

DVISwitcherController::DVISwitcherController()
{
	m_sp=new OS::OFSerialPort();
}


DVISwitcherController::~DVISwitcherController()
{
	delete m_sp;
}

EDVIState DVISwitcherController::readState()
{
	if(!m_sp->IsOpen())
		return Unkown;

	char reply[6];
	char *data="S";
	m_sp->Write(data,1);
	m_sp->Read(reply,5);

	reply[4]=0;
	int avaliable=m_sp->AvailableData();

	if(strcmp(reply,"SRC1")==0)
		return Channel1;
	if(strcmp(reply,"SRC2")==0)
		return Channel2;
	return Unkown;
}

bool DVISwitcherController::Open(const core::string& port)
{
	return m_sp->OpenByName(port,115200);
}

bool DVISwitcherController::SwitchToSource1()
{
	char *data="1\n";
	m_sp->Write(data,1);
	char reply[5];
	m_sp->Read(reply,5);
//	m_sp->Flush(true,false);
	return readState()==Channel1;
}

bool DVISwitcherController::SwitchToSource2()
{
	char *data="2\n";
	m_sp->Write(data,1);

	char reply[5];
	m_sp->Read(reply,5);

	return readState()==Channel2;
}

EDVIState DVISwitcherController::CurrentSource()
{
	return readState();
}

}
}

