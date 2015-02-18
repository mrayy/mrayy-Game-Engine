

/********************************************************************
	created:	2013/09/17
	created:	17:9:2013   5:51
	filename: 	C:\Development\mrayEngine\Projects\TouchShadow\ToolObject.h
	file path:	C:\Development\mrayEngine\Projects\TouchShadow
	file base:	ToolObject
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __ToolObject__
#define __ToolObject__



#include "SerialPort.h"


namespace mray
{

class ToolObject
{
protected:
	core::string m_port;
	CSerial m_serial;
public:
	ToolObject();
	virtual~ToolObject();

	void Run();
	void Stop();

	void Open();
	void Close();
	bool isOpen();

	void LoadFromXML(xml::XMLElement* e);
};

}


#endif
