

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
};

}
}


#endif
