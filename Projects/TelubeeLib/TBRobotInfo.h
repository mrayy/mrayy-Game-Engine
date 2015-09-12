

/********************************************************************
	created:	2013/01/28
	created:	28:1:2013   21:34
	filename: 	C:\Development\mrayEngine\Projects\TELUBee\TBRobotInfo.h
	file path:	C:\Development\mrayEngine\Projects\TELUBee
	file base:	TBRobotInfo
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __TBRobotInfo__
#define __TBRobotInfo__


#include "StreamWriter.h"
#include "StreamReader.h"

namespace mray
{
namespace TBee
{

class TBRobotInfo
{
protected:
public:
	TBRobotInfo(){}
	virtual~TBRobotInfo(){}

	long ID;
	core::string name;
	core::string IP;
	core::string Location;
	float lng;
	float lat;

	bool Connected;
	bool Avaliable;

	xml::XMLElement* WriteXML(xml::XMLElement* elem)
	{
		xml::XMLElement* e = new xml::XMLElement("RobotInfo");

		e->addAttribute("ID", core::StringConverter::toString(ID));
		e->addAttribute("Name", name);
		e->addAttribute("IP", IP);
		e->addAttribute("Location", Location);
		e->addAttribute("Longitude", core::StringConverter::toString(lng));
		e->addAttribute("Latitude", core::StringConverter::toString(lat));

		elem->addSubElement(e);
		return e;
	}

	int Write(OS::StreamWriter* w)
	{
		int len = 0;
		len += w->writeValue(ID);
		len += w->binWriteString(name);
		len += w->binWriteString(IP);
		len += w->binWriteString(Location);
		len += w->writeValue(lng);
		len += w->writeValue(lat);
		len += w->writeValue(Connected);
		len += w->writeValue(Avaliable);
		return len;
	}
	void Read(OS::StreamReader* w)
	{
		w->readValue(ID);
		name = w->readString();
		IP = w->readString();
		Location = w->readString();
		w->readValue(lng);
		w->readValue(lat);
		w->readValue(Connected);
		w->readValue(Avaliable);
	}
};

}
}


#endif
