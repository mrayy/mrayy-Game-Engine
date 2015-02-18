
/********************************************************************
	created:	2013/01/22
	created:	22:1:2013   13:45
	filename: 	C:\Development\mrayEngine\Engine\mrayGameLayer\IGhostInterface.h
	file path:	C:\Development\mrayEngine\Engine\mrayGameLayer
	file base:	IGhostInterface
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	implement a component based ghost object to serialize it over network
*********************************************************************/

#ifndef __IGhostInterface__
#define __IGhostInterface__


#include "GUID.h"

namespace mray
{
	namespace OS
	{
		class StreamReader;
		class StreamWriter;
	}
namespace game
{

class IGhostInterface
{
protected:
	mray::GUID m_guid;
public:
	IGhostInterface(const core::string& type):m_guid(type){}
	virtual~IGhostInterface(){}

	virtual const mray::GUID& GetGUID(){return m_guid;}

	virtual void CreateWrite(OS::StreamWriter* stream);
	virtual void CreateRead(OS::StreamReader* stream)=0;

	virtual void UpdateWrite(OS::StreamWriter* stream);
	virtual void UpdateRead(OS::StreamReader* stream)=0;
};

}
}


#endif
