

/********************************************************************
	created:	2013/01/29
	created:	29:1:2013   12:43
	filename: 	C:\Development\mrayEngine\Projects\TELUBee\TBRobotUserInfo.h
	file path:	C:\Development\mrayEngine\Projects\TELUBee
	file base:	TBRobotUserInfo
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __TBRobotUserInfo__
#define __TBRobotUserInfo__




namespace mray
{
namespace TBee
{

class TBRobotUserInfo
{
protected:
public:
	TBRobotUserInfo(){}
	virtual~TBRobotUserInfo(){}

	long ID;
	long userID;
	long robotID;

};

}
}


#endif
