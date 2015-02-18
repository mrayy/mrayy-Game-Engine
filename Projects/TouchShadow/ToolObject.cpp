

#include "stdafx.h"
#include "ToolObject.h"





namespace mray
{


ToolObject::ToolObject()
{

}
ToolObject::~ToolObject()
{
	m_serial.Close();
}

void ToolObject::Run()
{
	if(!m_serial.IsOpen())
		return;
	m_serial.Write("n\r\n");
}
void ToolObject::Stop()
{
	if(!m_serial.IsOpen())
		return;
	m_serial.Write("f");
}
void ToolObject::Close()
{
	m_serial.Close();
}
bool ToolObject::isOpen()
{
	return m_serial.IsOpen();
}
void ToolObject::Open()
{
	m_serial.Setup(CSerial::EBaud9600);
	if(m_serial.Open(m_port.c_str())==ERROR_SUCCESS)
		printf("Serial port was opened\n");
	else
	{
		printf("Failed to open Serial Port\n");

	}
}

void ToolObject::LoadFromXML(xml::XMLElement* e)
{
	m_port=e->getValueString("Port");
}


}


